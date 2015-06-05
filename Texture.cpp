#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GL/freeglut.h"

#include "common/objloader.hpp"

#include "Texture.h"

Texture::Texture(GLuint programID, unsigned int width, unsigned int height, const unsigned char * data) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    loadTexture(width, height, data);
	this->uniformId = glGetUniformLocation(programID, "myTextureSampler");
}

void Texture::loadTexture(unsigned int width, unsigned int height, const unsigned char * data) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D, // target
        0, // level, 0 = base, no minimap,
        GL_RGB, // internalformat
        width, // width
        height, // height
        0, // border, always 0 in OpenGL ES
        GL_RGB, // format
        GL_UNSIGNED_BYTE, // type
        data // data
    );
}
