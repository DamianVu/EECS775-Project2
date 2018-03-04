// main.c++: A simple shell for 2D OpenGL programs

#include <iostream>
#include <fstream>
#include <string>

#include "ExtendedController.h"
#include "Grid.h"

void createGrid(Controller& c, ShaderIF* sIF[], const char* descFileName)
{
	// INSTEAD of the following code, you will read and parse the description
	// file to determine the number of contour levels, the levels, and the
	// actual array of vertex values. Delete the following code and replace it
	// with that code. (You will probably want to pass the name of the description
	// file as a parameter to this function.)
	int nRowsOfVerts = 10;
	int nColsOfVerts = 20;
	float* vertexValues = new float[nRowsOfVerts * nColsOfVerts];
	for (int i=0 ; i<nRowsOfVerts*nColsOfVerts ; i++)
		vertexValues[i] = static_cast<float>(drand48());
	// END: Code to be thrown away when replacing with "real" code

	Grid* g = new Grid(sIF, nRowsOfVerts, nColsOfVerts, vertexValues);
	c.addModel(g);
	// Don't forget to pass color ramps and levels at which to contour to
	// the Grid instance, g.  You will need to add appropriate methods and
	// instance variables to the Grid class to accomplish this.
}

static ShaderIF::ShaderSpec vShader = {"shaders/Grid.vsh", GL_VERTEX_SHADER};
static ShaderIF::ShaderSpec gShader_lines = {"shaders/Grid_Lines.gsh", GL_GEOMETRY_SHADER};
static ShaderIF::ShaderSpec gShader_solid = {"shaders/Grid_Solid.gsh", GL_GEOMETRY_SHADER};
static ShaderIF::ShaderSpec fShader = {"shaders/Grid.fsh", GL_FRAGMENT_SHADER};

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " descriptionFileName\n";
		exit(1);
	}
	ExtendedController c("WEIRD TITLE");

	ShaderIF::ShaderSpec source_grid[] = { vShader, fShader };
	ShaderIF::ShaderSpec source_lines[] = { vShader, gShader_lines, fShader };
	ShaderIF::ShaderSpec source_solid[] = { vShader, gShader_solid, fShader };

	ShaderIF* sIF[3] = { new ShaderIF(source_grid, 2), new ShaderIF(source_lines, 3),
		new ShaderIF(source_solid, 3) };

	createGrid(c, sIF, argv[1]);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	ModelView::setMCRegionOfInterest(xyz);

	c.run();

	delete sIF[0];
	delete sIF[1];
	delete sIF[2];

	return 0;
}
