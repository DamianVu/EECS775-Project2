// main.c++: A simple shell for 2D OpenGL programs

#include <iostream>
#include <fstream>
#include <string>

#include "ExtendedController.h"
#include "Grid.h"

void createGrid(Controller& c, ShaderIF* sIF[], const char* descFileName)
{
	/*
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
	*/

	std::ifstream inFile;
	std::string contourLvlTxt;
	std::string fileName = descFileName;
	inFile.open(fileName);

	int numContourLevels;

	inFile >> numContourLevels;

	float* contourLevels = new float[numContourLevels];

	for (int i = 0; i < numContourLevels; i++) {
		inFile >> contourLevels[i];
	}

	int gridRows, gridCols;
	float missingValue;
	inFile >> gridRows >> gridCols >> missingValue;

	std::string dataFile;
	inFile >> dataFile;

	inFile.close();

	std::ifstream dataRead(dataFile, std::ifstream::binary);
	float* data = new float[gridRows * gridCols];

	if (dataRead) {
		dataRead.seekg(0,dataRead.end);
		int length = dataRead.tellg();
		dataRead.seekg(0,dataRead.beg);

		char* buffer = new char[length];

		dataRead.read(buffer, length);

		float* dataTemp = (float*)buffer;

		for (int i = 0; i < gridRows * gridCols; i++) {
			data[i] = dataTemp[i];
		}

		// Debug for outputting the grid we are working with
		// Definitely dont use for any Desc*.txt files...
		// int count = 1;
		// for (int i = 1; i <= gridRows; i++) {
		// 	int pos = (gridCols * (gridRows - i));
		// 	for (int j = 0; j < gridCols; j++) {
		// 		std::cout << data[pos];
		// 		if (pos != gridCols - 1) std::cout << ",";
		// 		pos++;
		// 	}
		// 	std::cout << std::endl;
		// }
	}



	Grid* g = new Grid(sIF, gridRows, gridCols, data, numContourLevels, contourLevels);
	c.addModel(g);

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

	// Check if file given exists.
	std::ifstream existsTest;
	existsTest.open(argv[1]);
	if (!existsTest.good()) {
		std::cerr << "Cannot find file '" << argv[1] << "'. Please try again.\n";
		exit(1);
	}
	existsTest.close();


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
