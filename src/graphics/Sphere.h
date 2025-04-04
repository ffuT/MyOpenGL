#pragma once
#include "Shape.h"

class Sphere : public Shape{
public:
    Sphere(const float& radius, const int& pointAmount);
    ~Sphere();
    
    const float m_Radius;
    const int m_PointAmount;
};
