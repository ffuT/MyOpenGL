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
	return m_SpecularStrength;
}

void Shape::SetTransform(const glm::mat4 transform){
	m_Transform = transform;
	m_dirty = true;
}

void Shape::SetScale(const glm::mat4 scale){
	m_Scale = scale;
	m_dirty = true;
}

void Shape::SetScale(const float scale){
	m_Scale = glm::scale(glm::mat4(1.0), glm::vec3(scale));
	m_dirty = true;
}

void Shape::SetRotation(const glm::mat4 rotation){
	m_Rotation = rotation;
	m_dirty = true;
}

void Shape::SetColor(const glm::vec4 color){
	m_Color = color;
}

void Shape::SetSpecular(const float spec){
	m_SpecularStrength = spec;
}

void Shape::SetTextID(const char* name){
	m_ObjectTextID = name;
}

glm::mat4 Shape::GetModelMatrix() const {
	if (m_dirty) {
		m_cachedModelMatrix = m_Transform * (m_Rotation * m_Scale);
		m_dirty = false;
	} 
	return m_cachedModelMatrix;
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

