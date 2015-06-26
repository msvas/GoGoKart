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
        int xPosition;
        int yPosition;
        int zPosition;

        Car();
        bool checkTrackCollision(std::list<TrackTile*> allTracks);
    protected:
    private:
};

#endif // CAR_H
