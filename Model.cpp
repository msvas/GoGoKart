#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GL/freeglut.h"

#include "common/objloader.hpp"
#include "Model.h"

Model::Model(const char *objName) {
    bool res = loadOBJ(objName, vertices, uvs, normals);

    glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
}
