
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include "Car.h"

Car::Car(Texture *tex, Model *model, bool cpuPlayer)
{
  carTex = tex;
  carModel = model;
  automatic = cpuPlayer;

  size = (carModel->getMax() - carModel->getMin());
  size.x = size.x * 0.2;
  size.y = size.y * 0.2;
  size.z = size.z * 0.2;
}

bool Car::checkTrackCollision(std::list<TrackTile*> allTracks) {
  bool collided = false;
  int i = 0;

  std::list<TrackTile*>::const_iterator iterator;
  for (iterator = allTracks.begin(); iterator != allTracks.end(); ++iterator) {
      if ((*iterator)->getY() == this->yPosition) {
        collided = true;
        currentTile = i;
      }
      i++;
  }
  return collided;
}

void Car::setPosition(int x, int y, int z) {
  this->xPosition = x;
  this->yPosition = y;
  this->zPosition = z;
}

glm::vec3 Car::movementGain(std::list<TrackTile*> allTracks) {
  int i;
  std::list<TrackTile*>::const_iterator auxTile = allTracks.begin();
  for (i = 0; i < currentTile; i++) {
    ++auxTile;
  }

  glm::vec3 auxPos = glm::vec3(xPosition, yPosition, zPosition);
  glm::vec3 distance = (*auxTile)->getPosition() - auxPos;
  glm::vec3 direction = glm::normalize(distance);

  xPosition += direction.x * 0.01;
  yPosition += direction.y;
  zPosition += direction.z * 0.01;
  std::cout << "Dentro: " << xPosition << " " << zPosition << "\n";
}
