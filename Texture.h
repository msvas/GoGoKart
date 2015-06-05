#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;
using namespace std;

#define GLEW_STATIC

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"


class Texture {
    public:
        GLuint id;
        GLuint uniformId;
        Texture(GLuint programID, unsigned int width, unsigned int height, const unsigned char * data);
    private:
        void loadTexture(unsigned int width, unsigned int height, const unsigned char * data);
};

#endif // TEXTURE_H
