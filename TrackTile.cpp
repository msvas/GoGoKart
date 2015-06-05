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

TrackTile::TrackTile(Texture *tex, Model *model, int x, int z)
{
    trackTex = tex;
    trackModel = model;
    xPosition = x;
    zPosition = z;
}

void TrackTile::drawTile(GLuint matrixID, glm::mat4 MVP) {
  glm::mat4 MVPaux;

  //glm::mat4 rot = glm::rotate(mat4(1.0f), 180.0f, vec3(0, 1.0f, 0));
  glm::mat4 sca = glm::scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f));
  glm::mat4 tra = glm::translate(mat4(1.0f), vec3(xPosition, 0, zPosition));

  MVPaux = MVP * sca * tra;

  glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVPaux[0][0]);
  drawMesh(0, trackModel->vertexBuffer, 1, trackModel->uvBuffer, trackTex->id, 0, trackModel->vertices.size());
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
