
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
  xPosition = 0;
  yPosition = 0;
  zPosition = 0;


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
      //cout << "Tile " << (*iterator)->getLeft() << " " << (*iterator)->getRight() << " " << (*iterator)->getBottom() << " " << (*iterator)->getFront() << "\n";
      //cout << "Car " << getLeft() << " " << getRight() << " " << getBottom() << " " << getFront() << "\n";
      if(getLeft() < (*iterator)->getRight()) {
        //cout << "1\n";
        if(getRight() > (*iterator)->getLeft()) {
          //cout << "2\n";
          if(getFront() > (*iterator)->getBottom()) {
            //cout << "3\n";
            if(getBottom() < (*iterator)->getFront()) {
              collided = true;
              currentTile = i;
              //cout << "Atual " << currentTile << "\n";
            }
          }
        }
      }
    }
    i++;
  }
  return collided;
}

void Car::setPosition(float x, float y, float z) {
  this->xPosition = x;
  this->yPosition = y;
  this->zPosition = z;
  //cout << xPosition << " " << zPosition << "\n";
}

float Car::getBottom() {
  float bottom;
  bottom = xPosition + (carModel->getMin().x * scale);
  return bottom;
}

float Car::getFront() {
  float front;
  front = xPosition + (carModel->getMax().x * scale);
  return front;
}

float Car::getLeft() {
  float left;
  left = zPosition + (carModel->getMin().z * scale);
  return left;
}

float Car::getRight() {
  float right;
  right = zPosition + (carModel->getMax().z * scale);
  return right;
}

glm::vec3 Car::movementGain(std::list<TrackTile*> allTracks, float delta) {
  int i;
  //cout << "Current: " << currentTile << "\n";
  std::list<TrackTile*>::const_iterator auxTile = allTracks.begin();
  for (i = 0; i < ((currentTile +1) % 40) ; i++) {
    ++auxTile;
  }

  glm::vec3 auxPos = glm::vec3(xPosition, yPosition, zPosition);
  glm::vec3 distance = (*auxTile)->getPosition() - auxPos;
  glm::vec3 direction = glm::normalize(distance);

  xPosition += direction.x * delta * 3;
  yPosition += direction.y;
  zPosition += direction.z * delta * 3;
  //std::cout << "Dentro: " << xPosition << " " << zPosition << " " << delta << "\n";
}
