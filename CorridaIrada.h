#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#define GLEW_STATIC

#include <stdio.h>
#include <stdlib.h>
#include <list>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "Car.h"
#include "TrackTile.h"

class CorridaIrada {
    public:
        GLuint id;
        GLuint vertexID;
        std::list<TrackTile*> allTracks;
        std::list<Texture*> textures;
        std::list<Model*> models;
        Car *mainCar;
        Car *computerCars[2];
        Texture *skyTex;
        Model *skyModel;
        Texture *signTex;
        Model *signModel;
        Texture *trackTex;
        Model *trackModel;

        CorridaIrada();
        static int staticInit();
        static void kDown(unsigned char key, int x, int y);
        static void kUp(unsigned char key, int x, int y);
        static void staticIdle();
        static void staticOnDisplay();
        static void staticFree_resources();
        int init_resources();
        void keyboardDown(unsigned char key, int x, int y);
        void keyboardUp(unsigned char key, int x, int y);
        void idle();
        void drawMesh(int vAttri, GLuint vBuffer, int tAttri, GLuint tBuffer, GLuint text, GLfloat uniform, int vSize);
        void onDisplay();
        void free_resources();
    private:
        void insertTex(Texture *tex);
        void insertModel(Model *model);
        void insertTrack(TrackTile *track);
        void createTrack();
};
