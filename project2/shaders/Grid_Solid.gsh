#version 330 core

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

	if (pvaIn[0].vertexValue != missingDataValue && pvaIn[1].vertexValue != missingDataValue && pvaIn[2].vertexValue != missingDataValue) 
	{
		bool first = false;
		bool second = false;
		bool third = false;

		// Since we are using right triangles, either 2 of the following IF statements must be true, or none are true. The only case where this is untrue is if all 3 points are the same as the contour level...

		if ((pvaIn[0].vertexValue < currentContourLevel && pvaIn[1].vertexValue > currentContourLevel) || (pvaIn[0].vertexValue > currentContourLevel && pvaIn[1].vertexValue < currentContourLevel)) 
		{

			// True if v0 < v1, false if v0 >= v1
			bool lesserVal = pvaIn[0].vertexValue <= pvaIn[1].vertexValue;

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
			gl_Position.zw = vec2(1.0,1.0);

			EmitVertex();

			first = true;

		}

		if ((pvaIn[1].vertexValue < currentContourLevel && pvaIn[2].vertexValue > currentContourLevel) || (pvaIn[1].vertexValue > currentContourLevel && pvaIn[2].vertexValue < currentContourLevel)) 
		{

			// True if v1 < v2, false if v1 >= v2
			bool lesserVal = pvaIn[1].vertexValue <= pvaIn[2].vertexValue;

			 // On a scale of 0 to 1, how far in between the two points is the contour level.
			float fracDifference;
			vec4 lesserLoc;
			vec4 greaterLoc;
			if (lesserVal) 
			{
				fracDifference = abs((currentContourLevel - pvaIn[2].vertexValue) / (pvaIn[1].vertexValue - pvaIn[2].vertexValue));
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
			gl_Position.zw = vec2(1.0,1.0);

			EmitVertex();

			second = true;


		}

		if ((pvaIn[0].vertexValue < currentContourLevel && pvaIn[2].vertexValue > currentContourLevel) || (pvaIn[0].vertexValue > currentContourLevel && pvaIn[2].vertexValue < currentContourLevel)) 
		{

			// True if v0 < v2, false if v0 >= v2
			bool lesserVal = pvaIn[0].vertexValue <= pvaIn[2].vertexValue;

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
			gl_Position.zw = vec2(1.0,1.0);

			EmitVertex();

			third = true;


		}

		// For each IF statement below, we have 2 cases.
		// First case: Emit 1 more vertex
		// Second Case: Emit 2 more vertices
		if (first && second)
		{
			if (pvaIn[0].vertexValue >= currentContourLevel && pvaIn[2].vertexValue >= currentContourLevel)
			{
				gl_Position = gl_in[0].gl_Position;
				EmitVertex();
				gl_Position = gl_in[2].gl_Position;
				EmitVertex();
			}
			else
			{
				gl_Position = gl_in[1].gl_Position;
				EmitVertex();
			}
		}
		else if (second && third)
		{
			if (pvaIn[0].vertexValue >= currentContourLevel && pvaIn[1].vertexValue >= currentContourLevel)
			{
				gl_Position = gl_in[0].gl_Position;
				EmitVertex();
				gl_Position = gl_in[1].gl_Position;
				EmitVertex();
			}
			else
			{
				gl_Position = gl_in[2].gl_Position;
				EmitVertex();
			}
		}
		else if (first && third)
		{
			if (pvaIn[1].vertexValue >= currentContourLevel && pvaIn[2].vertexValue >= currentContourLevel)
			{
				gl_Position = gl_in[1].gl_Position;
				EmitVertex();
				gl_Position = gl_in[2].gl_Position;
				EmitVertex();
			}
			else
			{
				gl_Position = gl_in[0].gl_Position;
				EmitVertex();
			}
		}
		else if (pvaIn[0].vertexValue >= currentContourLevel && pvaIn[1].vertexValue >= currentContourLevel && pvaIn[2].vertexValue >= currentContourLevel)
		{
			gl_Position = gl_in[0].gl_Position;
			EmitVertex();
			gl_Position = gl_in[1].gl_Position;
			EmitVertex();
			gl_Position = gl_in[2].gl_Position;
			EmitVertex();
		} 
		
	}

	EndPrimitive();
}
