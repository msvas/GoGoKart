#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GL/freeglut.h"

#include "Texture.h"
#include "Model.h"

class TrackTile
{
    public:
        Texture *trackTex;
        Model *trackModel;
        int xPosition;
        int zPosition;

        TrackTile(Texture *tex, Model *model, int x, int z);
        void drawTile(GLuint matrixID, glm::mat4 MVP);
    protected:
        void drawMesh(int vAttri, GLuint vBuffer, int tAttri, GLuint tBuffer, GLuint text, GLfloat uniform, int vSize);
    private:
};
