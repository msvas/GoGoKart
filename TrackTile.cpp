#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"

#include "TrackTile.h"

TrackTile::TrackTile(Texture *tex, Model *model, int x, int y, int z)
{
    trackTex = tex;
    trackModel = model;
    xPosition = x;
    yPosition = y;
    zPosition = z;

    size = (model->getMax() - model->getMin());
    size.x = size.x * 0.2;
    size.y = size.y * 0.2;
    size.z = size.z * 0.2;

    //cout << size.x << " " << size.y << " " << size.z << "\n";
}

void TrackTile::drawTile(GLuint matrixID, glm::mat4 MVP) {
  glm::mat4 MVPaux;

  //glm::mat4 rot = glm::rotate(mat4(1.0f), 180.0f, vec3(0, 1.0f, 0));
  glm::mat4 sca = glm::scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f));
  glm::mat4 tra = glm::translate(mat4(1.0f), vec3(xPosition, yPosition, zPosition));

  MVPaux = MVP * sca * tra;

  glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVPaux[0][0]);
  drawMesh(0, trackModel->vertexBuffer, 1, trackModel->uvBuffer, trackTex->id, 0, trackModel->vertices.size());
}

int TrackTile::getY() {
  return yPosition;
}

void TrackTile::drawMesh(int vAttri, GLuint vBuffer, int tAttri, GLuint tBuffer, GLuint text, GLfloat uniform, int vSize) {

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(vAttri);
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    //binding texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, text);
    glUniform1i(text, uniform);


    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(tAttri);
    glBindBuffer(GL_ARRAY_BUFFER, tBuffer);
    glVertexAttribPointer(
        1,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );
    //printf("%i %i\n", textureID1, vSize);
     glDrawArrays(GL_TRIANGLES, 0, vSize );
 }
