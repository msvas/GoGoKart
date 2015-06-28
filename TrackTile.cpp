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

TrackTile::TrackTile(Texture *tex, Model *model, float scale, int x, int y, int z)
{
    this->trackTex = tex;
    this->trackModel = model;
    this->xPosition = x;
    this->yPosition = y;
    this->zPosition = z;
    this->scale = scale;

    //std::cout << "Criando: " << xPosition << " " << zPosition << "\n";

    size = (model->getMax() - model->getMin());
    size.x = size.x * scale;
    size.y = size.y * scale;
    size.z = size.z * scale;

    //cout << size.x << " " << size.y << " " << size.z << "\n";
    cout << xPosition << " " << yPosition << " " << zPosition << "\n";
    //cout << getLeft() << " " << getRight() << "\n";
}

void TrackTile::drawTile(GLuint matrixID, glm::mat4 MVP) {
  glm::mat4 MVPaux;

  glm::mat4 sca = glm::scale(mat4(1.0f), vec3(scale, scale, scale));
  glm::mat4 tra = glm::translate(mat4(1.0f), vec3(xPosition, yPosition, zPosition));

  MVPaux = MVP * tra * sca;

  glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVPaux[0][0]);
  drawMesh(0, trackModel->vertexBuffer, 1, trackModel->uvBuffer, trackTex->id, 0, trackModel->vertices.size());
  //std::cout << "Dentro: " << xPosition << " " << zPosition << "\n";
}

int TrackTile::getY() {
  return yPosition;
}

glm::vec3 TrackTile::getPosition() {
  glm::vec3 pos = glm::vec3(xPosition, yPosition, zPosition);

  //std::cout << "Dentro: " << pos.x << " " << pos.y << "\n";
  //std::cout << "Dentro: " << xPosition << " " << zPosition << "\n";

  return pos;
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

 float TrackTile::getBottom() {
   float bottom;
   bottom = xPosition + (trackModel->getMin().x * scale);
   return bottom;
 }

 float TrackTile::getFront() {
   float front;
   front = xPosition + (trackModel->getMax().x * scale);
   return front;
 }

 float TrackTile::getLeft() {
   float left;
   left = zPosition + (trackModel->getMin().z * scale);
   return left;
 }

 float TrackTile::getRight() {
   float right;
   right = zPosition + (trackModel->getMax().z * scale);
   return right;
 }
