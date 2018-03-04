#version 410 core

// Grid_Solid.gsh - a geometry shader that replaces incoming triangles
//                  with 0, 1, or 2 triangles that are above the current
//                  contour level.

layout ( triangles ) in;
layout ( triangle_strip, max_vertices = 4 ) out;

uniform float currentContourLevel = 10.0;
uniform float missingDataValue = -1.0;

in PVA
{
	float vertexValue;
} pvaIn[]; // This will have length 3

// Also input will be gl_in[] of length 3. You can use
// gl_in[i].gl_Position, where 0 <= i < 3.

void main()
{
	// This dummy geometry shader just pretends to use its
	// various inputs so that the linker will actually create
	// storage for them. All this dummy code is doing right
	// now is outputting the input triangle:

	// BEGIN: Totally dummy code...
	if (pvaIn[0].vertexValue != missingDataValue)
		gl_Position = gl_in[0].gl_Position;
	else
		gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	for (int e=0 ; e<2 ; e++)
	{
		int i = (e == 2) ? 0 : e+1;
		int j = (i == 2) ? 0 : i+1;
		if (pvaIn[i].vertexValue != currentContourLevel)
			gl_Position = gl_in[i].gl_Position;
		else
			gl_Position = gl_in[j].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
	// END: Totally dummy code
}
