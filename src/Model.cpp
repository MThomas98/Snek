#include "Model.h"

// Constructor for predefined objects
Model::Model(Texture modelTex, const float verts[], unsigned int sizeVerts) : tex(modelTex) {
	// Set number of verts
	numVerts = sizeVerts / sizeof(float) / 5;

	// Generate and bind the VAO
	glGenVertexArrays(0, &vao);
	glBindVertexArray(vao);

	// Bind the vertex data (VBO) to the VAO
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts, verts, GL_STATIC_DRAW);

	// Define how the vertex data is stored
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);						// Vertex coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));		// Tex coords
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Model::draw() {
	tex.bind();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, numVerts);
}



const float PlaneModel::modelVerts[] = {
	0.5f,  0.5f,  0.0f,	 	1.0f,  1.0f,
	0.5f, -0.5f,  0.0f,	 	1.0f, -1.0f,
	-0.5f, -0.5f,  0.0f,	-1.0f, -1.0f,

	0.5f,  0.5f,  0.0f,	 	1.0f,  1.0f,
	-0.5f, 0.5f,  0.0f,	 	-1.0f, 1.0f,
	-0.5f, -0.5f,  0.0f,	-1.0f, -1.0f,
};
const unsigned int PlaneModel::sizeVerts = sizeof(PlaneModel::modelVerts);

const float CubeModel::modelVerts[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
const unsigned int CubeModel::sizeVerts = sizeof(CubeModel::modelVerts);
