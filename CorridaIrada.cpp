#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#define GLEW_STATIC
#define pi 3.14159265

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
#include "textures/track.c"
#include "textures/car2.c"
#include "textures/sign.c"

int oldTimeSinceStart = 0;
float deltaTime;

bool keystates[256];

GLuint programID;
GLuint matrixID;        // Get a handle for our "MVP" uniform

//player position
float posx = 0;
float posy = 0;
float posz = 0;
float angle = 0;

glm::vec3 cameraPos   = glm::vec3(0, 2.5f, -1.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, 3.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

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
	  glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	  glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &(this->vertexID));
    glBindVertexArray(this->vertexID);

    programID = LoadShaders( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );
    matrixID = glGetUniformLocation(programID, "MVP");

    mainCar = new Car(new Texture(programID, car1.width, car1.height, car1.pixel_data), new Model("objects/firstCar.obj"), false, 0.2);
    mainCar->setPosition(0, -0.7, 0);
    mainCar->angle = -180;
    insertTex(mainCar->carTex);
    insertModel(mainCar->carModel);

    computerCars[0] = new Car(new Texture(programID, car2.width, car2.height, car2.pixel_data), new Model("objects/secondCar.obj"), true, 4);
    computerCars[0]->setPosition(5, 0.6, 0);
    insertTex(computerCars[0]->carTex);
    insertModel(computerCars[0]->carModel);

    computerCars[1] = new Car(new Texture(programID, car2.width, car2.height, car2.pixel_data), new Model("objects/secondCar.obj"), true, 4);
    computerCars[1]->setPosition(0, 0.6, 5);

    /*computerCars[1] = new Car(new Texture(programID, car1.width, car1.height, car1.pixel_data), new Model("objects/firstCar.obj"));
    insertTex(mainCar->carTex);
    insertModel(mainCar->carModel);*/

    skyTex = new Texture(programID, sky.width, sky.height, sky.pixel_data);
    this->insertTex(skyTex);
    skyModel = new Model("objects/sphere.obj");
    this->insertModel(skyModel);

    signTex = new Texture(programID, finish.width, finish.height, finish.pixel_data);
    this->insertTex(signTex);
    signModel = new Model("objects/sign.obj");
    this->insertModel(signModel);

    trackTex = new Texture(programID, track.width, track.height, track.pixel_data);
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
  int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
  deltaTime = (timeSinceStart - oldTimeSinceStart) * 0.001f;
  oldTimeSinceStart = timeSinceStart;

  if(!mainCar->checkTrackCollision(allTracks)) {
    mainCar->yPosition -= 0.01f;
  }
  mainCar->handleCarCollision(computerCars[0]);
  mainCar->handleCarCollision(computerCars[1]);
  computerCars[0]->handleCarCollision(mainCar);
  //computerCars[1]->handleCarCollision(mainCar, computerCars[0]);
  if(!computerCars[0]->checkTrackCollision(allTracks)) {
    //mainCar->yPosition -= 0.01f;
  }
  computerCars[0]->movementGain(allTracks, deltaTime);

  if (keystates['w']) {   //-9 < z|x < 9
      mainCar->acceleration.z = -cos(pi*mainCar->angle/180);   //cos() e sin() usam radianos, ent鉶 deve-se multiplicar o
      mainCar->acceleration.x = -sin(pi*mainCar->angle/180);   //angulo por pi e dividir por 180 para ter o valor certo
  }
  else if (keystates['s']) {
      mainCar->acceleration.z = cos(pi*mainCar->angle/180);
      mainCar->acceleration.x = sin(pi*mainCar->angle/180);
  }
  else {
    mainCar->acceleration.z = 0;
    mainCar->acceleration.x = 0;
  }
  mainCar->updateMovement(deltaTime);

  if (keystates['a'])
      mainCar->angle += 1.0f;
  if (keystates['d'])
      mainCar->angle -= 1.0f;

  if (mainCar->angle == 360)
      mainCar->angle = 0;
  if (mainCar->angle == -10)
      mainCar->angle = 350;
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

    //cameraFront = vec3(0, 0, posz);
    float xCam = mainCar->xPosition - 2*cos(pi*(-90-mainCar->angle)/180);
    float zCam = mainCar->zPosition - 2*sin(pi*(-90-mainCar->angle)/180);
    float xFront = mainCar->xPosition + 3*cos(pi*(-90-mainCar->angle)/180);
    float zFront = mainCar->zPosition + 3*sin(pi*(-90-mainCar->angle)/180);
    cameraPos = glm::vec3(xCam, 2.5f + mainCar->yPosition, zCam);
		cameraFront = glm::vec3(xFront, 1.0f + mainCar->yPosition, zFront);

    glm::mat4 View = glm::lookAt(
                     cameraPos,
                     cameraFront,
                     cameraUp
            				);

    glm::mat4 Model = glm::mat4(1.0f);

    glm::mat4 MVP;

    glm::mat4 rotMao = glm::rotate(mat4(1.0f), 180 + mainCar->angle, vec3(0, 1.0f, 0));
    glm::mat4 escMao = glm::scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
    glm::mat4 trMao = glm::translate(mat4(1.0f), vec3(mainCar->xPosition, mainCar->yPosition, mainCar->zPosition));
    //mainCar->setPosition(posx, posy, posz);
    MVP = Projection * View * Model * trMao * escMao * rotMao;
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    drawMesh(0, mainCar->carModel->vertexBuffer, 1, mainCar->carModel->uvBuffer, mainCar->carTex->id, 0, mainCar->carModel->vertices.size());

    rotMao = glm::rotate(mat4(1.0f), 180.0f + computerCars[0]->angle, vec3(0, 1.0f, 0));
    escMao = glm::scale(mat4(1.0f), vec3(4.0f, 4.0f, 4.0f));
    trMao = glm::translate(mat4(1.0f), vec3(computerCars[0]->xPosition, computerCars[0]->yPosition, computerCars[0]->zPosition));
    MVP = Projection * View * Model * trMao * escMao * rotMao;
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    drawMesh(0, computerCars[0]->carModel->vertexBuffer, 1, computerCars[0]->carModel->uvBuffer, computerCars[0]->carTex->id, 0, computerCars[0]->carModel->vertices.size());

    /*MVP = Projection * View * Model;
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    drawMesh(0, trackModel->vertexBuffer, 1, trackModel->uvBuffer, trackTex->id, 0, trackModel->vertices.size());*/

    MVP = Projection * View * Model;
    std::list<TrackTile*>::const_iterator iterator;
    for (iterator = allTracks.begin(); iterator != allTracks.end(); ++iterator) {
        (*iterator)->drawTile(matrixID, MVP);
    }

    /*glm::mat4 viewChar = glm::scale(mat4(1.0f), vec3(30, 30, 30));

	  MVP = Projection * View * Model * viewChar;
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    drawMesh(0, skyModel->vertexBuffer, 1, skyModel->uvBuffer, skyTex->id, 0, skyModel->vertices.size());*/

    glm::mat4 viewChar = glm::translate(mat4(1.0f), vec3(-12, 0, 12)) * glm::scale(mat4(1.0f), vec3(0.15, 0.05, 0.15));

	  MVP = Projection * View * Model * viewChar;
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    drawMesh(0, signModel->vertexBuffer, 1, signModel->uvBuffer, signTex->id, 0, signModel->vertices.size());

    //disable
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

	  glutSwapBuffers();
    glutPostRedisplay();

}

//libera recursos
void CorridaIrada::free_resources() {

    //glDeleteBuffers(1, &vertexBuffer);
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
  TrackTile *tiles[40];
  int i;

  for(i = 0; i < 10; i++) {
    tiles[i] = new TrackTile(trackTex, trackModel, 1, 0, 0, i*20);
    this->insertTrack(tiles[i]);
  }

  for(i = 10; i < 20; i++) {
    tiles[i] = new TrackTile(trackTex, trackModel, 1, ((i-10)+1)*20, 0, 180);
    this->insertTrack(tiles[i]);
  }
  int j = 29;
  for(i = 20; i < 30; i++) {
    tiles[j] = new TrackTile(trackTex, trackModel, 1, 220, 0, ((j-20))*20);
    this->insertTrack(tiles[j]);
    j--;
  }
  int k = 39;
  for(i = 30; i < 40; i++) {
    tiles[k] = new TrackTile(trackTex, trackModel, 1, ((k-30)+1)*20, 0, 0);
    this->insertTrack(tiles[k]);
    k--;
  }

}
