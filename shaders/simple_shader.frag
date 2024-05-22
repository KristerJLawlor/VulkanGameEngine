#version 450


//layout qualifier takes location value
//out is an output qualifier that will output the proceeding variable
layout(location = 0) out vec4 outColor;

void main() {
	//red, green, blue, alpha channels
	outColor = vec4(1.0, 1.0, 0.0, 1.0);
}