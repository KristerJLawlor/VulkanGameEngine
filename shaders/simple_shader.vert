#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

layout(push_constant) uniform Push{
	vec2 offset;
	vec3 color;
} push;

void main()	{
	gl_Position = vec4(position + push.offset, 0.0, 1.0);
}

/*
 * This shader takes a 2D position and a color as input.
 * It applies an offset to the position and outputs the final vertex position.
 * The color is passed as a push constant but not used in this vertex shader.

 * In the rasterization stage, we take the color from the push constant and send it to the fragment shader as input
 * for coloring the fragment.
 */