#pragma once

#include <GL/glew.h>

#include <cstdio>
#include <string>

class Texture {
public:
	Texture(const char* texFile);

	void bind();

	// TODO: Texture option setting functions
	// TODO: Texture color
private:
	unsigned int texture;

};
