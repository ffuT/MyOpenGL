#pragma once
#include "Shape.h"
#include "Mesh.h"

class Sphere : public Shape{
public:
    Sphere(const float& radius, Mesh* mesh);
    ~Sphere();
    
    const float m_Radius;
};
