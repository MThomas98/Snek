#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cstdio>

class Light {
public:
	Light(glm::vec3 lColor, glm::vec3 lPos);



private:
	glm::vec3 color;
	glm::vec3 pos;
}
