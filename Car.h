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
        float xPosition;
        float yPosition;
        float zPosition;
        int currentTile;
        bool automatic;

        Car(Texture *tex, Model *model, bool cpuPlayer);
        bool checkTrackCollision(std::list<TrackTile*> allTracks);
        void setPosition(int x, int y, int z);
        glm::vec3 movementGain(std::list<TrackTile*> allTracks);
    protected:
    private:
};

#endif // CAR_H
