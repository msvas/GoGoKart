#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"

#include "common/objloader.hpp"
#include "Model.h"

using namespace glm;
using namespace std;

Model::Model(const char *objName) {
  bool res = loadOBJ(objName, vertices, uvs, normals);

  findMinMax();

  glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
  glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
}

bool Model::findMinMax() {
  minPos.x = 0;
  minPos.y = 0;
  minPos.z = 0;
  maxPos.x = 0;
  maxPos.y = 0;
  maxPos.z = 0;

  std::vector<glm::vec3>::iterator it;
  for(it = vertices.begin(); it != vertices.end(); it++) {
    if((*it).x > maxPos.x)
      maxPos.x = (*it).x;
    if((*it).y > maxPos.y)
      maxPos.y = (*it).y;
    if((*it).z > maxPos.z)
      maxPos.z = (*it).z;
    if((*it).x < minPos.x)
      minPos.x = (*it).x;
    if((*it).y < minPos.y)
      minPos.y = (*it).y;
    if((*it).z < minPos.z)
      minPos.z = (*it).z;
  }
  //cout << minPos.x << " " << maxPos.x << "\n";
}

glm::vec3 Model::getMax() {
  return maxPos;
}

glm::vec3 Model::getMin() {
  return minPos;
}
