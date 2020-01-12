#pragma once

#include <GL/glew.h>

#include <cstdio>

#include "Texture.h"

class Model {
public:
	Model(Texture modelTex, const char* modelFile);

	void draw();

protected:
	Model(Texture modelTex, const float verts[], unsigned int sizeVerts);

	unsigned int vao;
	unsigned int numVerts;
	Texture tex;
};

/*---------------------*
*    Preset models	   *
*----------------------*/

// Model preset for a basic plane
class PlaneModel : public Model {
public:
	PlaneModel(Texture modelTex) :
		Model(modelTex, modelVerts, sizeVerts) {}

private:
	static const float modelVerts[];
	static const unsigned int sizeVerts;
};

// Model preset for a basic cube
class CubeModel : public Model {
public:
	CubeModel(Texture modelTex) :
		Model(modelTex, modelVerts, sizeVerts) {}

private:
	static const float modelVerts[];
	static const unsigned int sizeVerts;
};
