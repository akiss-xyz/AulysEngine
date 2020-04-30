// A shader to create gradients between vertices of a triangle.

#type vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

out vec3 vPosition;
out vec4 vColor;

void main(){
	vPosition = aPosition;
	vColor = aColor;
	gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 vPosition;
in vec4 vColor;

void main(){
	color = vColor;
}
