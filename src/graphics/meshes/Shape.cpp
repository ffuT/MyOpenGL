#include "Shape.h"
#include <iostream>

Shape::Shape(Mesh* mesh) : m_mesh(mesh) {

}

Shape::Shape(Mesh* mesh, const RigidBody Rbody) : m_mesh(mesh) {
	m_RigidBody = Rbody;
}

Shape::~Shape(){
}

void Shape::Render() const {
	m_mesh->Bind();
	m_mesh->Render();
	m_mesh->Unbind();
}

const RigidBody& Shape::GetRigidBody() const {
	return m_RigidBody;
}

void Shape::syncPhysicsToTransform() {

}

void Shape::syncTransformToPhysics() {
	if (!m_RigidBody.isStatic) {
		const glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_RigidBody.position);
		m_cachedModelMatrix = translate * (GetRotation() * GetScale());
		m_dirty = false;
	}
}

glm::vec4 Shape::GetColor() const {
	return m_Color;
}

Mesh* Shape::GetMesh() const {
	return m_mesh;
}

const char* Shape::GetTextID() const {
	return m_ObjectTextID.c_str();
}

float Shape::GetSpecular() const {
	return m_SpecularStrength;
}

void Shape::SetPosition(const glm::vec3 pos){
	m_RigidBody.position = pos;
	m_dirty = true;
}

void Shape::SetScale(const glm::vec3 scale){
	m_Scale = scale;
	m_dirty = true;
}

void Shape::SetScale(const float scale){
	m_Scale = glm::vec3(scale);
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

void Shape::SetMesh(Mesh* mesh) {
	m_mesh = mesh;
}

void Shape::SetTextID(const char* name){
	m_ObjectTextID = name;
}

glm::mat4 Shape::GetModelMatrix() const {
	if (m_dirty) {
		UpdateModelMatrix();
		m_dirty = false;
	} 
	return m_cachedModelMatrix;
}

void Shape::UpdateModelMatrix() const{
	m_cachedModelMatrix = GetTransform() * (GetRotation() *  GetScale());
}

glm::mat4 Shape::GetTransform() const {
	return glm::translate(glm::mat4(1.0f), m_RigidBody.position);
}

glm::mat4 Shape::GetScale() const{
	return glm::scale(glm::mat4(1.0f), m_Scale);
}

glm::mat4 Shape::GetRotation() const{
	return m_Rotation;
}

