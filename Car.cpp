
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <list>
#include "Car.h"

#define pi 3.14159265

Car::Car(Texture *tex, Model *model, bool cpuPlayer, float scale)
{
  carTex = tex;
  carModel = model;
  automatic = cpuPlayer;
  this->scale = scale;
  xPosition = 0;
  yPosition = 0;
  zPosition = 0;
  acceleration = glm::vec3(0, 0, 0);
  speed = glm::vec3(0, 0, 0);
  maxSpeed = glm::vec3(0.1, 0.1, 0.1);
  lap = 0;
  angle = 0;
  enteredFinish = false;
  checkpoints.push_back(0);

  //std::cout << "Dentro: " << carModel->getMax().z << " " << carModel->getMin().z << "\n";
  size = (carModel->getMax() - carModel->getMin());
  size.x = size.x * scale;
  size.y = size.y * scale;
  size.z = size.z * scale;

  //cout << getLeft() << " " << getRight() << "\n";
}

bool Car::handleCarCollision(Car* otherCar) {
  bool collided = false;

  if(otherCar->getRight() > getLeft()) {
    if(otherCar->getLeft() < getLeft()) {
      if(otherCar->getBottom() < getFront()) {
        if(otherCar->getFront() > getBottom()) {
          collided = true;
          otherCar->xPosition = getLeft() - (carModel->getMax().x * scale);
        }
      }
    }
  }
  if(otherCar->getLeft() < getRight()) {
    if(otherCar->getRight() > getRight()) {
      if(otherCar->getBottom() < getFront()) {
        if(otherCar->getFront() > getBottom()) {
          collided = true;
          otherCar->xPosition = getRight() - (carModel->getMin().x * scale);
        }
      }
    }
  }

}

bool Car::checkTrackCollision(std::list<TrackTile*> allTracks) {
  bool collided = false;
  int i = 0;

  std::list<TrackTile*>::const_iterator iterator;
  for (iterator = allTracks.begin(); iterator != allTracks.end(); ++iterator) {
    if ((*iterator)->getY() == (this->yPosition - yPosition)) {
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
              lastTile = currentTile;
              currentTile = i;
              checkLap(*iterator, allTracks.size());
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

void Car::checkLap(TrackTile* tile, int totalTiles) {
  if(lastTile != currentTile) {
    std::list<int>::iterator iter = checkpoints.end();
    --iter;
    if(*iter < currentTile) {
      checkpoints.push_back(currentTile);
      //cout << currentTile << "\n";
    }
    if(currentTile == 0) {
      if(checkpoints.size() == (totalTiles)) {
        lap++;
        cout << "volta " << lap << "\n";
        checkpoints.clear();
        checkpoints.push_back(0);
        //cout << checkpoints.size() << " " << totalTiles << "\n";
      }
    }
  }
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
/*
glm::vec3 Car::getBottom() {
  glm::vec3 bottom;
  bottom = xPosition + (carModel->getMin().x * scale);
  return bottom;
}

glm::vec3 Car::getFront() {
  glm::vec3 front;
  front = xPosition + (carModel->getMax().x * scale);
  return front;
}

glm::vec3 Car::getLeft() {
  glm::vec3 left;
  left = zPosition + (carModel->getMin().z * scale);
  return left;
}

glm::vec3 Car::getRight() {
  glm::vec3 right;
  right = zPosition + (carModel->getMax().z * scale);
  return right;
}*/

glm::vec3 Car::movementGain(std::list<TrackTile*> allTracks, float delta) {
  int i;
  //cout << "Current: " << currentTile << "\n";
  std::list<TrackTile*>::const_iterator auxTile = allTracks.begin();
  for (i = 0; i < ((currentTile +1) % 40) ; i++) {
    ++auxTile;
  }


  glm::vec3 auxPos = glm::vec3(xPosition, yPosition, zPosition);
  glm::vec3 distance = (*auxTile)->getPosition() - auxPos;
  distance.y += yPosition;
  acceleration = glm::normalize(distance);

  glm::vec3 zAxis = glm::vec3(0, 0, 1.0f);

  float dot = acceleration.y * zAxis.y + acceleration.z * zAxis.z;
  float det = acceleration.y * zAxis.z - acceleration.z * zAxis.y;
  angle = atan2(det, dot) * 90/pi;

  //cout << angle << "\n";

  updateMovement(delta);

  /*xPosition += direction.x * delta * 3;
  yPosition += direction.y;
  zPosition += direction.z * delta * 3;*/
  //std::cout << "Dentro: " << xPosition << " " << zPosition << " " << delta << "\n";
}

bool Car::updateMovement(float delta) {
  speed += acceleration * delta * 0.4f;

  //cout << speed.x << "\n";

  if(speed.x > maxSpeed.x)
    speed.x = maxSpeed.x;
  if(speed.x < -maxSpeed.x)
    speed.x = -maxSpeed.x;
  if(speed.z > maxSpeed.z)
    speed.z = maxSpeed.z;
  if(speed.z < -maxSpeed.z)
    speed.z = -maxSpeed.z;

  if(speed.x > 0)
    speed.x -= delta * 0.1;
  if(speed.x < 0)
    speed.x += delta * 0.1;
  if(speed.z > 0)
    speed.z -= delta * 0.1;
  if(speed.z < 0)
    speed.z += delta * 0.1;

  xPosition += speed.x;
  zPosition += speed.z;
}
