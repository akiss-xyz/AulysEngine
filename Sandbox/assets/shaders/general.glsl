/* general.glsl */
//Quaternion Math
vec3 qtransform( vec4 q, vec3 v ){
  return v + 2.0*cross(cross(v, -q.xyz ) + q.w*v, -q.xyz);
}

//Raymarch Functions
float unionSDF(float d1, float d2){
  return min(d1, d2);
}

//--------------------------------------------------------------------
// Hyperbolic Functions
//--------------------------------------------------------------------
float cosh(float x){
  float eX = exp(x);
  return (0.5 * (eX + 1.0/eX));
}

float acosh(float x){ //must be more than 1
  return log(x + sqrt(x*x-1.0));
}

//--------------------------------------------------------------------
// Generalized Functions
//--------------------------------------------------------------------

vec4 geometryNormalize(vec4 v, bool toTangent);
vec4 geometryDirection(vec4 u, vec4 v);
vec4 geometryFixDirection(vec4 u, vec4 v, mat4 fixMatrix);
float geometryDot(vec4 u, vec4 v);
float geometryDistance(vec4 u, vec4 v);
float geometryNorm(vec4 v){
  return sqrt(abs(geometryDot(v,v)));
}

vec4 pointOnGeodesic(vec4 u, vec4 vPrime, float dist);
bool isOutsideCell(vec4 samplePoint, out mat4 fixMatrix);

//--------------------------------------------------------------------
// Generalized SDFs
//--------------------------------------------------------------------

float globalSceneSDF(vec4 samplePoint, mat4 globalTransMatrix, bool collideWithLights);
float localSceneSDF(vec4 samplePoint);

float sphereSDF(vec4 samplePoint, vec4 center, float radius){
  return geometryDistance(samplePoint, center) - radius;
}

//--------------------------------------------------------------------
// Raymarch Helpers
//--------------------------------------------------------------------

//Series Distance
//recorded distances per step
void AddToSeriesRecord(inout vec3 d, float newDist){
  d.x = d.y; 
  d.y = d.z;
  d.z = newDist;
}

float GetSeriesDistance(inout vec3 d){
  float delta = 0.04;
  float deltaPrime = 0.02;
  //Check out issue 68 for further explanation
  if((d.z < delta) && (d.x > d.y && d.y > d.z) && (abs(d.y*d.y - d.x*d.z) < d.y*d.y*deltaPrime)){
    d.z = (d.y*d.z)/(d.y-d.z);
  }
  return d.z;
}