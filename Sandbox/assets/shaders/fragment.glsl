#type fragment

#version 330 core

layout(location = 0) out vec4 out_color;

#tag includes "Sandbox/assets/shaders/includes.glsl"

#include "Sandbox/assets/shaders/general.glsl"

uniform bool debugInfo = false; 
uniform vec2 debugVector2;

#include "Sandbox/assets/shaders/hyperbolic.glsl"

#include "Sandbox/assets/shaders/edgeTubes.glsl"

float globalSceneSDF(vec4 samplePoint, mat4 globalTransMatrix, bool collideWithLights){
  float distance = maxDist;
  if(collideWithLights){
    //Light Objects
    for(int i=0; i<4; i++){
      float objDist;
      if(lightIntensities[i].w == 0.0) { objDist = maxDist; }
      else{
        objDist = sphereSDF(samplePoint, lightPositions[i]*globalTransMatrix, 
										1.0/(10.0*lightIntensities[i].w));
        distance = min(distance, objDist);
        if(distance < EPSILON){
          hitWhich = 1;
          globalLightColor = lightIntensities[i];
          return distance;
        }
      }
    }
    //Global Objects
    for(int i=0; i<NUM_OBJECTS; i++) {
        float objDist;
        if(length(globalObjectRadii[i]) == 0.0){ objDist = maxDist;}
        else{
        objDist = sphereSDF(samplePoint, globalObjectBoosts[i][3] * globalTransMatrix, globalObjectRadii[i].x);
        distance = min(distance, objDist);
        if(distance < EPSILON){
            hitWhich = 2;
        }
        }
    }
    return distance;
    }
};

//NORMAL FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++
vec4 estimateNormal(vec4 p){ // normal vector is in tangent hyperplane to hyperboloid at p
    // float denom = sqrt(1.0 + p.x*p.x + p.y*p.y + p.z*p.z);  // first, find basis for that tangent hyperplane
    float newEp = EPSILON * 10.0;
    vec4 basis_x = geometryNormalize(vec4(p.w,0.0,0.0,p.x), true);  // dw/dx = x/w on hyperboloid
    vec4 basis_y = vec4(0.0,p.w,0.0,p.y);  // dw/dy = y/denom
    vec4 basis_z = vec4(0.0,0.0,p.w,p.z);  // dw/dz = z/denom  /// note that these are not orthonormal!
    basis_y = geometryNormalize(basis_y - geometryDot(basis_y, basis_x)*basis_x, true); // need to Gram Schmidt
    basis_z = geometryNormalize(basis_z - geometryDot(basis_z, basis_x)*basis_x - geometryDot(basis_z, basis_y)*basis_y, true);
    if(hitWhich == 1 || hitWhich == 2){ //global light scene
      return geometryNormalize( //p+EPSILON*basis_x should be lorentz normalized however it is close enough to be good enough
          basis_x * (globalSceneSDF(p + newEp*basis_x, invCellBoost, true) - globalSceneSDF(p - newEp*basis_x, invCellBoost, true)) +
          basis_y * (globalSceneSDF(p + newEp*basis_y, invCellBoost, true) - globalSceneSDF(p - newEp*basis_y, invCellBoost, true)) +
          basis_z * (globalSceneSDF(p + newEp*basis_z, invCellBoost, true) - globalSceneSDF(p - newEp*basis_z, invCellBoost, true)),
          true
      );
    }
    else{ //local scene
      return geometryNormalize(
          basis_x * (localSceneSDF(p + newEp*basis_x) - localSceneSDF(p - newEp*basis_x)) +
          basis_y * (localSceneSDF(p + newEp*basis_y) - localSceneSDF(p - newEp*basis_y)) +
          basis_z * (localSceneSDF(p + newEp*basis_z) - localSceneSDF(p - newEp*basis_z)),
          true
      );
    }
}

vec4 getRayPoint(vec2 resolution, vec2 fragCoord, bool isRight){ //creates a point that our ray will go through
    vec2 xy = 0.2*((fragCoord - 0.5*resolution)/resolution.x);
    float z = 0.1/tan(radians(fov*0.5));
    vec4 p =  geometryNormalize(vec4(xy,-z,1.0), false);
    return p;
}

bool isOutsideSimplex(vec4 samplePoint, out mat4 fixMatrix){
  vec4 kleinSamplePoint = projectToKlein(samplePoint);
  for(int i=0; i<4; i++){
    vec3 normal = simplexMirrorsKlein[i].xyz;
    vec3 offsetSample = kleinSamplePoint.xyz - normal * simplexMirrorsKlein[i].w;  // Deal with any offset.
    if( dot(offsetSample, normal) > 1e-7 ) {
      fixMatrix = invGenerators[i];
      return true;
    }
  }
  return false;
}

// This function is intended to be geometry-agnostic.
bool isOutsideCell(vec4 samplePoint, out mat4 fixMatrix){
  if( useSimplex ) {
    return isOutsideSimplex( samplePoint, fixMatrix );
  }

  vec4 kleinSamplePoint = projectToKlein(samplePoint);
  if(kleinSamplePoint.x > halfCubeWidthKlein){
    fixMatrix = invGenerators[0];
    return true;
  }
  if(kleinSamplePoint.x < -halfCubeWidthKlein){
    fixMatrix = invGenerators[1];
    return true;
  }
  if(kleinSamplePoint.y > halfCubeWidthKlein){
    fixMatrix = invGenerators[2];
    return true;
  }
  if(kleinSamplePoint.y < -halfCubeWidthKlein){
    fixMatrix = invGenerators[3];
    return true;
  }
  if(kleinSamplePoint.z > halfCubeWidthKlein){
    fixMatrix = invGenerators[4];
    return true;
  }
  if(kleinSamplePoint.z < -halfCubeWidthKlein){
    fixMatrix = invGenerators[5];
    return true;
  }
  return false;
}

void raymarch(vec4 rO, vec4 rD, out mat4 totalFixMatrix){
  float globalDepth = MIN_DIST; float localDepth = globalDepth;
  vec4 localrO = rO; vec4 localrD = rD;
  totalFixMatrix = mat4(1.0);
  mat4 fixMatrix = mat4(1.0);
  int fakeI = 0;
  vec3 seriesRecord = vec3(MIN_DIST, MIN_DIST, MIN_DIST);
  
  // Trace the local scene, then the global scene:
  for(int i = 0; i< MAX_MARCHING_STEPS; i++){
    if(fakeI >= maxSteps || globalDepth >= maxDist){
      //when we break it's as if we reached our max marching steps
      break;
    }
    fakeI++; // [x]
    vec4 localEndPoint = pointOnGeodesic(localrO, localrD, localDepth);
    if(isOutsideCell(localEndPoint, fixMatrix)){
      totalFixMatrix *= fixMatrix;
      localrO = geometryNormalize(localEndPoint*fixMatrix, false);
      localrD = geometryFixDirection(localrO, localrD, fixMatrix); 
      localDepth = MIN_DIST;
    }
    else{
      float localDist = min(0.5,localSceneSDF(localEndPoint));
      AddToSeriesRecord(seriesRecord, localDist);
      localDist = GetSeriesDistance(seriesRecord);
      if(localDist < EPSILON){
        hitWhich = 3;
        sampleEndPoint = localEndPoint;
        sampleTangentVector = tangentVectorOnGeodesic(localrO, localrD, localDepth);
        break;
      }
      localDepth += localDist;
      globalDepth += localDist;
    }
  }
  
  // Set localDepth to our new max tracing distance:
  localDepth = min(globalDepth, maxDist);
  globalDepth = MIN_DIST;
  seriesRecord = vec3(MIN_DIST, MIN_DIST, MIN_DIST);
  fakeI = 0;
  for(int i = 0; i< MAX_MARCHING_STEPS; i++){
    if(fakeI >= maxSteps){
      break;
    }
    fakeI++;
    vec4 globalEndPoint = pointOnGeodesic(rO, rD, globalDepth);
    float globalDist = globalSceneSDF(globalEndPoint, invCellBoost, true);
    AddToSeriesRecord(seriesRecord, globalDist);
    globalDist = GetSeriesDistance(seriesRecord);
    if(globalDist < EPSILON){
      totalFixMatrix = mat4(1.0);
      sampleEndPoint = globalEndPoint;
      sampleTangentVector = tangentVectorOnGeodesic(rO, rD, globalDepth);
      return;
    }
    globalDepth += globalDist;
    if(globalDepth >= localDepth){
      break;
    }
  }
}

vec4 col(float r) {
	return vec4(mod(r, 255.0)/255.0, 0.0/255.0, 0.0/255.0, 1.0);
}

vec4 col(float r, float g) {
	return vec4(mod(r, 255.0)/255.0, mod(g, 255.0)/255.0, 0.0/255.0, 1.0);
}

vec4 col(float r, float g, float b) {
	return vec4(mod(r, 255.0)/255.0, mod(g, 255.0)/255.0, mod(b, 255.0)/255.0, 1.0);
}

vec4 col(float r, float g, float b, float a) {
	return vec4(mod(r, 255.0)/255.0, mod(g, 255.0)/255.0, mod(b, 255.0)/255.0, mod(a, 255.0)/255.0);
}

vec4 col(vec2 v) {
	return col(v.x, v.y);
}

vec4 col(vec3 v) {
	return col(v.x, v.y, v.z);
}

vec4 col(vec4 v) {
	return col(v.x, v.y, v.z, 0.5);
}

vec4 texcube(vec4 samplePoint, mat4 toOrigin){
    float k = 4.0;
    vec4 newSP = samplePoint * toOrigin;
    vec3 p = mod(newSP.xyz,1.0);
    vec3 n = geometryNormalize(N*toOrigin, true).xyz; //Very hacky you are warned
    vec3 m = pow(abs(n), vec3(k));
    vec4 x = texture2D(uTexture, p.yz);
    vec4 y = texture2D(uTexture, p.zx);
    vec4 z = texture2D(uTexture, p.xy);
    return (x*m.x + y*m.y + z*m.z) / (m.x+m.y+m.z);
}

vec4 shadeSurfaceLocal(mat4 invObjectBoost, mat4 globalTransMatrix) {
  //--------------------------------------------
  //Setup Variables
  //--------------------------------------------
  vec3 baseColor = vec3(0.0,1.0,1.0);
  vec4 SP = sampleEndPoint;
  vec4 TLP;
  vec4 V = -sampleTangentVector;

  baseColor = texcube(SP, mat4(1.0)).xyz;

  //Setup up color with ambient component
  return vec4(ambient * baseColor, 1.0f);
    // IMPROVEMENT: GOAL: Add back the lighting calculations. However, for simplicity and performance, lighting has been removed.
}

vec4 shadeSurfaceGlobal(mat4 invObjectBoost, mat4 globalTransMatrix) {
  //--------------------------------------------
  //Setup Variables
  //--------------------------------------------
  vec3 baseColor = vec3(0.0,1.0,1.0);
  vec4 SP = sampleEndPoint;
  vec4 TLP;
  vec4 V = -sampleTangentVector;

  baseColor = texcube(SP, cellBoost * invObjectBoost).xyz; 

  //Setup up color with ambient component
  return vec4(ambient * baseColor, 1.0f);
    // IMPROVEMENT: GOAL: Add back the lighting calculations. However, for simplicity and performance, lighting has been removed.
}

void main(){
	vec4 rayOrigin = ORIGIN;

	//stereo translations
	bool isRight = gl_FragCoord.x/screenResolution.x > 0.5;
	vec4 rayDirV = getRayPoint(screenResolution, gl_FragCoord.xy, isRight);

	rayOrigin *= currentBoost;
	rayDirV *= currentBoost;
	//generate direction then transform to hyperboloid ------------------------
	vec4 rayDirVPrime = geometryDirection(rayOrigin, rayDirV); // [x]
	//get our raymarched distance back ------------------------
	mat4 totalFixMatrix = mat4(1.0);
	raymarch(rayOrigin, rayDirVPrime, totalFixMatrix);

	//Based on hitWhich decide whether we hit a global object, local object, or nothing
	if(hitWhich == 0) { //Didn't hit anything ------------------------
		out_color = vec4(0.0, 0.0, 0.0, 1.0);
		return;
	}
	else if(hitWhich == 1) { // global lights
		out_color = vec4(1.0, 1.0, 1.0, 1.0);
		return;
	}
	else { // objects

		N = estimateNormal(sampleEndPoint);
		vec3 color;
		mat4 globalTransMatrix = invCellBoost * totalFixMatrix;
		if(hitWhich == 2){ // global objects
			out_color = shadeSurfaceGlobal(invGlobalObjectBoosts[0], globalTransMatrix);
		}
		else
		{ // local objects
			out_color = shadeSurfaceLocal(mat4(1.0), globalTransMatrix);
		}
  }
	return;

}
