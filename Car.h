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
    float scale;
    int currentTile;
    bool automatic;

    Car(Texture *tex, Model *model, bool cpuPlayer, float scale);
    bool checkTrackCollision(std::list<TrackTile*> allTracks);
    void setPosition(int x, int y, int z);
    glm::vec3 movementGain(std::list<TrackTile*> allTracks, float delta);
  protected:
  private:
    int getBottom();
    int getFront();
    int getLeft();
    int getRight();
};

#endif // CAR_H
