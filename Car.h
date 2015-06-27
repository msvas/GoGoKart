#ifndef CAR_H
#define CAR_H

#pragma once

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Texture.h"
#include "Model.h"
#include "TrackTile.h"

class Car
{
    public:
        Texture *carTex;
        Model *carModel;
        glm::vec3 size;
        glm::vec3 begin;
        glm::vec3 end;
        int xPosition;
        int yPosition;
        int zPosition;

        Car(Texture *tex, Model *model);
        bool checkTrackCollision(std::list<TrackTile*> allTracks);
        void setPosition(int x, int y, int z);
    protected:
    private:
};

#endif // CAR_H
