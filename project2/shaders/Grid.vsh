#version 330 core

layout (location = 0) in vec2 mcPosition;
in float vertexValue;
out PVA
{
	float vertexValue;
} pvaOut;

uniform vec4 scaleTrans;

void main()
{
	pvaOut.vertexValue = vertexValue;

	float ldsX = scaleTrans[0]*mcPosition.x + scaleTrans[1];
	float ldsY = scaleTrans[2]*mcPosition.y + scaleTrans[3];
	gl_Position = vec4(ldsX, ldsY, 0, 1);
}
