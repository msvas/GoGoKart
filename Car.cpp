
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include "Car.h"

Car::Car(Texture *tex, Model *model, bool cpuPlayer, float scale)
{
  carTex = tex;
  carModel = model;
  automatic = cpuPlayer;
  this->scale = scale;

  //std::cout << "Dentro: " << carModel->getMax().z << " " << carModel->getMin().z << "\n";
  size = (carModel->getMax() - carModel->getMin());
  size.x = size.x * scale;
  size.y = size.y * scale;
  size.z = size.z * scale;

  //cout << getLeft() << " " << getRight() << "\n";
}

bool Car::checkTrackCollision(std::list<TrackTile*> allTracks) {
  bool collided = false;
  int i = 0;

  std::list<TrackTile*>::const_iterator iterator;
  for (iterator = allTracks.begin(); iterator != allTracks.end(); ++iterator) {
      if ((*iterator)->getY() == this->yPosition) {
        cout << getLeft() << " " << (*iterator)->getLeft() << " " << getRight() << " " << (*iterator)->getRight() << "\n";
        if(getLeft() > (*iterator)->getLeft()) {
          if(getRight() < (*iterator)->getRight()) {
            if(getBottom() > (*iterator)->getBottom()) {
              if(getFront() < (*iterator)->getFront()) {
                collided = true;
                currentTile = i;
              }
            }
          }
        }
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

int Car::getBottom() {
  int bottom;
  bottom = xPosition + (carModel->getMin().x * scale);
  return bottom;
}

int Car::getFront() {
  int front;
  front = xPosition + (carModel->getMax().x * scale);
  return front;
}

int Car::getLeft() {
  int left;
  left = zPosition + (carModel->getMin().z * scale);
  return left;
}

int Car::getRight() {
  int right;
  right = zPosition + (carModel->getMax().z * scale);
  return right;
}

glm::vec3 Car::movementGain(std::list<TrackTile*> allTracks, float delta) {
  int i;
  std::list<TrackTile*>::const_iterator auxTile = allTracks.begin();
  for (i = 0; i < currentTile; i++) {
    ++auxTile;
  }

  glm::vec3 auxPos = glm::vec3(xPosition, yPosition, zPosition);
  glm::vec3 distance = (*auxTile)->getPosition() - auxPos;
  glm::vec3 direction = glm::normalize(distance);

  xPosition += direction.x * delta;
  yPosition += direction.y;
  zPosition += direction.z * delta;
  //std::cout << "Dentro: " << xPosition << " " << zPosition << " " << delta << "\n";
}
