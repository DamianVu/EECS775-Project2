#version 410 core

// Grid_Lines.gsh - a geometry shader that replaces incoming triangles
//                  with 0 or 1 contour line running through it.

layout ( triangles ) in;
layout ( line_strip, max_vertices = 4 ) out;

uniform float currentContourLevel = 10.0;
uniform float missingDataValue = -1.0;

in PVA
{
	float vertexValue;
} pvaIn[]; // This will have length 3

// Also inoput will be gl_in[] of length 3. You can use
// gl_in[i].gl_Position, where 0 <= i < 3.

void main()
{
	// This dummy geometry shader just pretends to use its
	// various inputs so that the linker will actually create
	// storage for them. All this dummy code is doing right
	// now is outputting the edges of the triangle:

	// BEGIN: Totally dummy code...
	if (pvaIn[0].vertexValue != missingDataValue)
		gl_Position = gl_in[0].gl_Position;
	else
		gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	for (int e=0 ; e<3 ; e++)
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
