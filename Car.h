#ifndef CAR_H
#define CAR_H

#pragma once

#include "Texture.h"
#include "Model.h"

class Car
{
    public:
        Texture *carTex;
        Model *carModel;
        int xPosition;
        int yPosition;
        int zPosition;
        Car();
    protected:
    private:
};

#endif // CAR_H
