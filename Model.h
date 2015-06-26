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

class Model {
    public:
      std::vector<glm::vec3> vertices;
      std::vector<glm::vec2> uvs;
      std::vector<glm::vec3> normals;
      glm::vec3 maxPos;
      glm::vec3 minPos;
      GLuint vertexBuffer;
      GLuint uvBuffer;

      Model(const char *objName);
      glm::vec3 getMax();
      glm::vec3 getMin();
    private:
      bool findMinMax();
};
