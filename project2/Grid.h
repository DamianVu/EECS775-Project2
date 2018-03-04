// Grid.h - subclass of ModelView that represents a Grid

#ifndef GRID_H
#define GRID_H

#include "ShaderIF.h"

#include <string>

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "ModelView.h"

typedef float vec3[3];

class Grid : public ModelView
{
public:
	Grid(ShaderIF* sIF[],
		int nRowsOfVertices, int nColsOfVertices, float* vertexValues);
	virtual ~Grid();

	// You can declare and implement methods here to be called from
	// ExtendedController to handle various events.

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
	bool handleCommand(int aFunctionKey, double ldsX, double ldsY, int mods);
	bool handleCommand(Controller::SpecialKey aSpecialKey,
					   double ldsX, double ldsY, int mods);
	void render();
private:
	ShaderIF* shaderIF[3];
	int nRowsOfVertices, nColsOfVertices;
	float missingDataValue;
	float* level; // the array of contour levels
	int numContourLevels;
	vec3* color; // the array of rgb colors; color[i] is for level[i]

	// structures to convey geometry to OpenGL/GLSL:
	GLuint vao[1];
	GLuint vbo[2];
	GLuint* ebo;

	void defineModel(float* gridValues);
	void drawGrid();
	void render(int whichShader);
};

#endif
