
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include "Car.h"

Car::Car(Texture *tex, Model *model)
{
  carTex = tex;
  carModel = model;

  size = (carModel->getMax() - carModel->getMin());
  size.x = size.x * 0.2;
  size.y = size.y * 0.2;
  size.z = size.z * 0.2;
}

bool Car::checkTrackCollision(std::list<TrackTile*> allTracks) {
  bool collided = false;

  std::list<TrackTile*>::const_iterator iterator;
  for (iterator = allTracks.begin(); iterator != allTracks.end(); ++iterator) {
      if ((*iterator)->getY() == this->yPosition)
        collided = true;
  }
  return collided;
}

void Car::setPosition(int x, int y, int z) {
  this->xPosition = x;
  this->yPosition = y;
  this->zPosition = z;
}
