#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Camera.h"


class DebugCrosshair{
public:
	DebugCrosshair();
	~DebugCrosshair();

	void Update(const Camera& cam);
	void BindVAO() const;
	void UnBindVAO() const;

private:
	VertexArray m_VAO;
	VertexBuffer m_VBO;

	float m_Vertices[18] = {
	-0.02f, 0.0f,  0.0f,  0.02f, 0.0f,  0.0f,
	 0.0f, -0.02f, 0.0f,  0.0f,  0.02f, 0.0f,
	 0.0f,  0.0f, -0.02f, 0.0f,  0.0f,  0.02f
	};
};
