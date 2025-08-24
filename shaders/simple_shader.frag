#version 450

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Push{
	vec2 offset;
	vec3 color;
} push;

void main() {
	outColor = vec4(push.color, 1.0);
}

/*
The Fragment Shader takes vertex shader output and applies color on a per pixel basis.
It does so by breaking up geometry in to fragments for each pixel that a triangle/polygon overlaps.
For each fragment, the rasterization stage automatically interpolates declared variables as outputs 
from the vertex to fragment shader using barycentric coordinates.
Each fragment has different coordinates based on its position within the triangle/polygon.
This means that the input color to the fragment shader is slightly different each time the fragment shader is run.
*/