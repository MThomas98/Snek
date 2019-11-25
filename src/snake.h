#pragma once

#include <GL/glut.h>

#include "stdio.h"

class Snake {
private:
    int* segPos[2];
    int numSegs = 1;

public:
    void process();
    void process(unsigned char key);
};
