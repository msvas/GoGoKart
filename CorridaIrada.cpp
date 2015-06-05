#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#define GLEW_STATIC

#include <stdio.h>
#include <stdlib.h>
#include <list>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "CorridaIrada.h"

#include "common/shader.cpp"
#include "common/objloader.cpp"

#include "textures/sky.c"
#include "textures/car1.c"
#include "textures/planeTexture.c"

bool keystates[256];

GLuint programID;
GLuint matrixID;        // Get a handle for our "MVP" uniform

//plane
GLuint vertexBuffer;
GLuint uvBuffer;
GLuint textureID1;                  // Get a handle for our "myTextureSampler" uniform
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;

//hands
GLuint vertexID2;
GLuint vertexBuffer2;
GLuint uvBuffer2;
GLuint textureID2;                  // Get a handle for our "myTextureSampler" uniform
std::vector<glm::vec3> handVertices;
std::vector<glm::vec2> handUV;
std::vector<glm::vec3> handNormals;

//brick texture
GLuint textureID3;

//player position
float posx = 0;
float posy = 2;
float posz = -3;
float angle = 0;

CorridaIrada *game;

CorridaIrada::CorridaIrada() {

}

void CorridaIrada::insertTex(Texture *tex) {
    this->textures.push_back(tex);
}

void CorridaIrada::insertTrack(TrackTile *track) {
    this->allTracks.push_back(track);
}

void CorridaIrada::insertModel(Model *model) {
    this->models.push_back(model);
}

int CorridaIrada::init_resources() {

    int i;

    for (i = 0; i < 256; i++) {
        keystates[i] = false;
    }
	  glClearColor(0.0f, 0.5f, 0.0f, 0.0f);
	  glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &(this->vertexID));
    glBindVertexArray(this->vertexID);

    mainCar = new Car();

    programID = LoadShaders( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );
    matrixID = glGetUniformLocation(programID, "MVP");

    mainCar->carTex = new Texture(programID, car1.width, car1.height, car1.pixel_data);
    insertTex(mainCar->carTex);
    mainCar->carModel = new Model("objects/firstCar.obj");
    insertModel(mainCar->carModel);

    skyTex = new Texture(programID, sky.width, sky.height, sky.pixel_data);
    this->insertTex(skyTex);
    skyModel = new Model("objects/sphere.obj");
    this->insertModel(skyModel);

    trackTex = new Texture(programID, planeTexture.width, planeTexture.height, planeTexture.pixel_data);
    this->insertTex(trackTex);
    trackModel = new Model("objects/plane.obj");
    this->insertModel(trackModel);

    createTrack();

    return programID;
}

void CorridaIrada::keyboardDown(unsigned char key, int x, int y) {

    keystates[key] = true;
}


void CorridaIrada::keyboardUp(unsigned char key, int x, int y) {

    keystates[key] = false;

}

void CorridaIrada::idle() {

    if (keystates['w'] && posz < 7)
        posz += 0.05;
    if (keystates['s'] && posz > -7)
        posz -= 0.05;
    if (keystates['a'])
        angle -= 0.3;
    if (keystates['d'])
        angle += 0.3;
}

void CorridaIrada::drawMesh(int vAttri, GLuint vBuffer, int tAttri, GLuint tBuffer, GLuint text, GLfloat uniform, int vSize) {

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

void CorridaIrada::onDisplay() {

   	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUseProgram(programID);

    glm::mat4 Projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);

    glm::mat4 View       = glm::lookAt(
								glm::vec3(0, 15, 0),
								glm::vec3(0, 1, 3),
								glm::vec3(0, 1, 0)
						   );

    glm::mat4 Model      = glm::mat4(1.0f);

    glm::mat4 MVP;

    glm::mat4 rotMao = glm::rotate(mat4(1.0f), 180.0f, vec3(0, 1.0f, 0));
    glm::mat4 escMao = glm::scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
    glm::mat4 trMao = glm::translate(mat4(1.0f), vec3(0, 0, posz));
    MVP = Projection * View * Model * trMao * escMao * rotMao;
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    drawMesh(0, mainCar->carModel->vertexBuffer, 1, mainCar->carModel->uvBuffer, mainCar->carTex->id, 0, mainCar->carModel->vertices.size());

    /*MVP = Projection * View * Model;
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    drawMesh(0, trackModel->vertexBuffer, 1, trackModel->uvBuffer, trackTex->id, 0, trackModel->vertices.size());*/

    MVP = Projection * View * Model;
    std::list<TrackTile*>::const_iterator iterator;
    for (iterator = allTracks.begin(); iterator != allTracks.end(); ++iterator) {
        (*iterator)->drawTile(matrixID, MVP);
    }

    /*
    MVP = Projection * View * Model;
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    std::list<TrackTile*>::const_iterator iterator;
    TrackTile *auxTrack;
    for (iterator = allTracks.begin(); iterator != allTracks.end(); ++iterator) {
        auxTrack = (*iterator);
        drawMesh(0, auxTrack->trackModel->vertexBuffer, 1, auxTrack->trackModel->uvBuffer, auxTrack->trackTex->id, 0, auxTrack->trackModel->vertices.size());
    }*/

    glm::mat4 viewChar = glm::translate(mat4(1.0f), vec3(0, 0, posz)) * glm::translate(mat4(1.0f), vec3(0, 0, -posz)) * glm::scale(mat4(1.0f), vec3(60, 60, 60));

	  MVP = Projection * View * Model * viewChar;
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    drawMesh(0, skyModel->vertexBuffer, 1, skyModel->uvBuffer, skyTex->id, 0, skyModel->vertices.size());

    //disable
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

	  glutSwapBuffers();
    glutPostRedisplay();

}

//libera recursos
void CorridaIrada::free_resources() {

    glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexID);
    glDeleteProgram(programID);

}

int main(int argc, char* argv[]) {
    CorridaIrada preGame;
    game = &preGame;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Corrida Irada");

    glewExperimental = GL_TRUE;
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return 1;
    }

    if (game->staticInit() != 0) {

        glutDisplayFunc(game->staticOnDisplay);

        glutIgnoreKeyRepeat(1);
        glutKeyboardFunc(game->kDown);
        glutKeyboardUpFunc(game->kUp);
        glutIdleFunc(game->staticIdle);

        glutMainLoop();
    }

    game->staticFree_resources();
    return 0;
}

void CorridaIrada::staticIdle() {
    game->idle();
}

void CorridaIrada::staticOnDisplay() {
    game->onDisplay();
}

void CorridaIrada::staticFree_resources() {
    game->free_resources();
}

int CorridaIrada::staticInit() {
    return game->init_resources();
}

void CorridaIrada::kDown(unsigned char key, int x, int y) {
    game->keyboardDown(key,x, y);
}

void CorridaIrada::kUp(unsigned char key, int x, int y) {
    game->keyboardUp(key,x, y);
}

void CorridaIrada::createTrack() {
  TrackTile *tiles[10];
  int i;

  for(i = 0; i < 10; i++) {
    tiles[i] = new TrackTile(trackTex, trackModel, i*10, i*3);
    this->insertTrack(tiles[i]);
  }

}
