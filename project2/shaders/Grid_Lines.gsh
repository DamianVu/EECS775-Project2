#version 330 core

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

// Also input will be gl_in[]of length 3. You can use
// gl_in[i].gl_Position, where 0 <= i < 3.

void main()
{

	if (pvaIn[0].vertexValue != missingDataValue && pvaIn[1].vertexValue != missingDataValue && pvaIn[2].vertexValue != missingDataValue) 
	{

		// Since we are using right triangles, either 2 of the following IF statements must be true, or none are true. The only case where this is untrue is if all 3 points are the same as the contour level...

		if ((pvaIn[0].vertexValue <= currentContourLevel && pvaIn[1].vertexValue >= currentContourLevel) || (pvaIn[0].vertexValue >= currentContourLevel && pvaIn[1].vertexValue <= currentContourLevel)) 
		{

			// True if v0 < v1, false if v0 >= v1
			bool lesserVal = pvaIn[0].vertexValue < pvaIn[1].vertexValue;

			 // On a scale of 0 to 1, how far in between the two points is the contour level.
			float fracDifference;
			vec4 lesserLoc;
			vec4 greaterLoc;
			if (lesserVal) 
			{
				fracDifference = abs((currentContourLevel - pvaIn[0].vertexValue) / (pvaIn[1].vertexValue - pvaIn[0].vertexValue));
				lesserLoc = gl_in[0].gl_Position;
				greaterLoc = gl_in[1].gl_Position;
			}
			else
			{
				fracDifference = abs((currentContourLevel - pvaIn[1].vertexValue) / (pvaIn[0].vertexValue - pvaIn[1].vertexValue));
				lesserLoc = gl_in[1].gl_Position;
				greaterLoc = gl_in[0].gl_Position;
			}

			gl_Position = ((1 - fracDifference) * lesserLoc) + (fracDifference * greaterLoc);

			EmitVertex();

		}

		if ((pvaIn[2].vertexValue <= currentContourLevel && pvaIn[1].vertexValue >= currentContourLevel) || (pvaIn[2].vertexValue >= currentContourLevel && pvaIn[1].vertexValue <= currentContourLevel)) 
		{

			// True if v1 < v2, false if v1 >= v2
			bool lesserVal = pvaIn[1].vertexValue < pvaIn[2].vertexValue;

			 // On a scale of 0 to 1, how far in between the two points is the contour level.
			float fracDifference;
			vec4 lesserLoc;
			vec4 greaterLoc;
			if (lesserVal) 
			{
				fracDifference = abs((currentContourLevel - pvaIn[1].vertexValue) / (pvaIn[2].vertexValue - pvaIn[1].vertexValue));
				lesserLoc = gl_in[1].gl_Position;
				greaterLoc = gl_in[2].gl_Position;
			}
			else
			{
				fracDifference = abs((currentContourLevel - pvaIn[2].vertexValue) / (pvaIn[1].vertexValue - pvaIn[2].vertexValue));
				lesserLoc = gl_in[2].gl_Position;
				greaterLoc = gl_in[1].gl_Position;
			}

			gl_Position = ((1 - fracDifference) * lesserLoc) + (fracDifference * greaterLoc);

			EmitVertex();

		}

		if ((pvaIn[0].vertexValue <= currentContourLevel && pvaIn[2].vertexValue >= currentContourLevel) || (pvaIn[0].vertexValue >= currentContourLevel && pvaIn[2].vertexValue <= currentContourLevel)) 
		{

			// True if v0 < v2, false if v0 >= v2
			bool lesserVal = pvaIn[1].vertexValue < pvaIn[2].vertexValue;

			 // On a scale of 0 to 1, how far in between the two points is the contour level.
			float fracDifference;
			vec4 lesserLoc;
			vec4 greaterLoc;
			if (lesserVal) 
			{
				fracDifference = abs((currentContourLevel - pvaIn[0].vertexValue) / (pvaIn[2].vertexValue - pvaIn[0].vertexValue));
				lesserLoc = gl_in[0].gl_Position;
				greaterLoc = gl_in[2].gl_Position;
			}
			else
			{
				fracDifference = abs((currentContourLevel - pvaIn[2].vertexValue) / (pvaIn[0].vertexValue - pvaIn[2].vertexValue));
				lesserLoc = gl_in[2].gl_Position;
				greaterLoc = gl_in[0].gl_Position;
			}

			gl_Position = ((1 - fracDifference) * lesserLoc) + (fracDifference * greaterLoc);

			EmitVertex();

		}

		//gl_Position = gl_in[1].gl_Position + gl_in[2].gl_Position;
		//gl_Position.xy = gl_Position.xy * 0.5;
		//gl_Position.zw = vec2(1.0,1.0);
		//EmitVertex();

		//gl_Position = vec4(-1.0,0.666,1.0,1.0);
		//EmitVertex();

	}

	EndPrimitive();
}
