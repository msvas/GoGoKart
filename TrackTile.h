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
    glm::vec3 size;
    glm::vec3 begin;
    glm::vec3 end;
    float scale;
    int xPosition;
    int yPosition;
    int zPosition;

    TrackTile(Texture *tex, Model *model, float scale, int x, int y, int z);
    void drawTile(GLuint matrixID, glm::mat4 MVP);
    int getY();
    glm::vec3 getPosition();
    float getBottom();
    float getFront();
    float getLeft();
    float getRight();
  protected:
    void drawMesh(int vAttri, GLuint vBuffer, int tAttri, GLuint tBuffer, GLuint text, GLfloat uniform, int vSize);
  private:
};
