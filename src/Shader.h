#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Shader {
public:
	void use();

	void setBool(string name, bool value) const;
	void setInt(string name, int value) const;
	void setFloat(string name, float value) const;
	void setVec4f(string name, float v1, float v2, float v3, float v4) const;
	void setMat4f(string name, glm::mat4 value) const;

	Shader(const char* vPath, const char* fPath);
private:
	unsigned int shaderProg;

};
