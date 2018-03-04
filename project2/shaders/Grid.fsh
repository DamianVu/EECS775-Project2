#version 410 core

uniform vec3 currentContourColor = vec3(1.0, 0.0, 0.0);

out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(currentContourColor, 1.0);
}
