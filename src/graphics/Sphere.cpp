#include "Sphere.h"
#include <iostream>

Sphere::Sphere(const float& radius, Mesh* mesh) : m_Radius(1), Shape(mesh) {
    m_Scale = glm::scale(m_Scale, glm::vec3(radius));
}

Sphere::~Sphere(){

}


