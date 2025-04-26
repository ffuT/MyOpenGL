#include "Shape.h"
#include <iostream>

Shape::Shape(Mesh* mesh) : m_mesh(mesh) {

}

Shape::~Shape(){
}

void Shape::Render() const {
	m_mesh->Bind();
	m_mesh->Render();
	m_mesh->Unbind();
}

glm::vec4 Shape::GetColor() const {
	return m_Color;
}

const char* Shape::GetTextID() const {
	return m_ObjectTextID.c_str();
}

float Shape::GetSpecular() const {
	return m_SpecularStrenght;
}

void Shape::SetTransform(const glm::mat4 transform){
	m_Transform = transform;
}

void Shape::SetScale(const glm::mat4 scale){
	m_Scale = scale;
}

void Shape::SetRotation(const glm::mat4 rotation){
	m_Rotation = rotation;
}

void Shape::SetColor(const glm::vec4 color){
	m_Color = color;
}

void Shape::SetSpecular(const float spec){
	m_SpecularStrenght = spec;
}

void Shape::SetTextID(const char* name){
	m_ObjectTextID = name;
}

glm::mat4 Shape::GetModelMatrix() const {
	return m_Transform * (m_Rotation * m_Scale);
}

glm::mat4 Shape::GetTransform() const {
	return m_Transform;
}

glm::mat4 Shape::GetScale() const{
	return m_Scale;
}

glm::mat4 Shape::GetRotation() const{
	return m_Rotation;
}

