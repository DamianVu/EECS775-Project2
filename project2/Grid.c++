// Grid.c++ - subclass of ModelView that represents a Grid of values

#include <iostream>
#include <cmath>

#include "Grid.h"

Grid::Grid(ShaderIF* sIF[],
	int nRowsOfVerticesIn, int nColsOfVerticesIn, float* vertexValuesIn) :
		nRowsOfVertices(nRowsOfVerticesIn),
		nColsOfVertices(nColsOfVerticesIn),
		missingDataValue(-1.0), level(nullptr), numContourLevels(0), color(nullptr)
{
	for (int i=0 ; i<3 ; i++)
		shaderIF[i] = sIF[i];
	defineModel(vertexValuesIn);

	// TEMPORARY: Either modify the constructor to accept this data or define
	//            and implement a method to set this data, or do some
	//            combination of both.
	// BUT delete this code.  The expectations are that "color" has the same
	//     length as "level". Both are assumed to be "numContourLevels" long.
	numContourLevels = 1;
	level = new float[numContourLevels];
	color = new vec3[numContourLevels];
	// A "vec3" represents an RGB color in a 0..1 space:
	//    RED               GREEN               BLUE
	color[0][0] = 0.0; color[0][1] = 1.0; color[0][2] = 0.0;
	// END: TEMPORARY TO BE DELETED
}

Grid::~Grid()
{
	glDeleteBuffers(nRowsOfVertices-1, ebo);
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, vao);
	if (level != nullptr)
		delete [] level;
	if (color != nullptr)
		delete [] color;
}

void Grid::defineModel(float* vertexValues)
{
	// send data to GPU:
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(2, vbo);

	typedef float vec2[2];
	vec2* gridXY = new vec2[nRowsOfVertices * nColsOfVertices];
	for (int r=0 ; r<nRowsOfVertices ; r++)
		for (int c=0 ; c<nColsOfVertices ; c++)
		{
			gridXY[r*nColsOfVertices + c][0] = c; // x coord is column
			gridXY[r*nColsOfVertices + c][1] = r; // y coord is row
		}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	int nBytesXY = nRowsOfVertices * nColsOfVertices * sizeof(vec2);
	glBufferData(GL_ARRAY_BUFFER, nBytesXY, gridXY, GL_STATIC_DRAW);

	delete [] gridXY;

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	int nBytesValues = nRowsOfVertices * nColsOfVertices * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, nBytesValues, vertexValues, GL_STATIC_DRAW);

	// Element buffer to draw the rows of triangles in the grid.
	ebo = new GLuint[nRowsOfVertices - 1];
	glGenBuffers(nRowsOfVertices-1, ebo);
	GLuint vertexIndexStart = 0;
	GLuint* indexList = new GLuint[2*nColsOfVertices];
	for (int i=0 ; i<nRowsOfVertices-1 ; i++)
	{
		for (int j=0 ; j<nColsOfVertices ; j++)
		{
			indexList[2*j  ] = vertexIndexStart + j;
			indexList[2*j+1] = vertexIndexStart + j + nColsOfVertices;
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2*nColsOfVertices*sizeof(GLuint),
			indexList, GL_STATIC_DRAW);
		vertexIndexStart += nColsOfVertices;
	}
	delete [] indexList;
}

void Grid::drawGrid()
{
	for (int r=0 ; r<nRowsOfVertices-1 ; r++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[r]);
		glDrawElements(GL_TRIANGLE_STRIP, 2*nColsOfVertices, GL_UNSIGNED_INT, nullptr);
	}
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Grid::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = 0.0;
	xyzLimits[1] = nColsOfVertices - 1.0;
	xyzLimits[2] = 0.0;
	xyzLimits[3] = nRowsOfVertices - 1.0;
	xyzLimits[4] = -1.0; xyzLimits[5] = 1.0; // (zmin, zmax) (really 0..0)
}

// In each of these three handleCommand methods, return true if you
// handle the event. Execute the 'return' statement that is there now
// if you do NOT handle the event.

bool Grid::handleCommand(unsigned char anASCIIChar,
							double ldsX, double ldsY)
{
	return this->ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
}

bool Grid::handleCommand(int aFunctionKey,
						double ldsX, double ldsY, int mods)
{
	return this->ModelView::handleCommand(aFunctionKey, ldsX, ldsY, mods);
}

bool Grid::handleCommand(Controller::SpecialKey aSpecialKey,
						double ldsX, double ldsY, int mods)
{
	return this->ModelView::handleCommand(aSpecialKey, ldsX, ldsY, mods);
}

void Grid::render(int whichShader)
{
	// save the current GLSL program in use
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	// draw the triangles using our vertex and fragment shaders
	ShaderIF* sIF = shaderIF[whichShader];
	glUseProgram(sIF->getShaderPgmID());

	// define the mapping from MC to -1..+1 Logical Device Space:
	float scaleTrans[4];
	compute2DScaleTrans(scaleTrans);
	glUniform4fv(sIF->ppuLoc("scaleTrans"), 1, scaleTrans);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(sIF->pvaLoc("mcPosition"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(sIF->pvaLoc("mcPosition"));

	if (whichShader == 0) // Just drawing the grid
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawGrid();
	}
	else // Running the marching squares algorithm in one of the two geometry shaders
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// vertexValue is only actually used in shader programs with a geometry shader
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glVertexAttribPointer(sIF->pvaLoc("vertexValue"), 1, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(sIF->pvaLoc("vertexValue"));

		for (int cl=0 ; cl<numContourLevels ; cl++)
		{
			// Following are used only with shader programs that have a geometry shader
	    	glUniform1f(sIF->ppuLoc("missingDataValue"), missingDataValue);
	    	glUniform1f(sIF->ppuLoc("currentContourLevel"), level[cl]);
	    	glUniform3fv(sIF->ppuLoc("currentContourColor"), 1, color[cl]);

			drawGrid();
		}
	}

	// restore the previous program
	glUseProgram(pgm);
}

void Grid::render()
{
	render(0);
}
