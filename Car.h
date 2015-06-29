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
    glm::vec3 acceleration;
    glm::vec3 speed;
    glm::vec3 maxSpeed;
    float xPosition;
    float yPosition;
    float zPosition;
    float angle;
    float scale;
    int currentTile;
    int lastTile;
    int lap;
    bool automatic;

    Car(Texture *tex, Model *model, bool cpuPlayer, float scale);
    bool checkTrackCollision(std::list<TrackTile*> allTracks);
    bool handleCarCollision(Car* otherCar);
    void setPosition(float x, float y, float z);
    glm::vec3 movementGain(std::list<TrackTile*> allTracks, float delta);
    bool updateMovement(float delta);
  protected:
    bool enteredFinish;
    std::list<int> checkpoints;
  private:
    float getBottom();
    float getFront();
    float getLeft();
    float getRight();
    void checkLap(TrackTile* tile, int totalTiles);
};

#endif // CAR_H
