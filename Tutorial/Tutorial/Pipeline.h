#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define M_PI 3.14159265358979323846

struct SVertex
{
	glm::vec3 m_Pos;
	glm::vec2 m_Tex;
	glm::vec3 m_Normal;

	SVertex(){}

	SVertex(const glm::vec3& vPos, const glm::vec2& vTex)
	{
		m_Pos = vPos;
		m_Tex = vTex;
		m_Normal = glm::vec3(0.0, 0.0, 0.0);
	}
};

struct SDirectionalLight
{
	glm::vec3 m_AmbientColor;
	glm::vec3 m_DiffuseColor;
//	glm::vec3 m_SpecularColor;
	glm::vec3 m_Direction;
};

class CPileline
{
public:
	CPileline();
	~CPileline();

	void init(const unsigned int* vIndices, unsigned int vIndexCount, SVertex* vVertex, unsigned int vVertexCount);
	void translate(float vTranslateX, float vTranslateY, float vTranslateZ);
	void rotate(float vAngle, float vRotateX, float vRotateY, float vRotateZ);
	void scale(float vScaleX, float vScaleY, float vScaleZ);
	void lookAt(const glm::vec3& vEye, const glm::vec3& vLookat, const glm::vec3& vUp);
	void perspective(const float& vFovy, const float& vAspect, const float& vNear, const float& vFar);
	void ortho(const float& vLeft, const float& vRight, const float& vBottom, const float& vTop, const float& vNear, const float& vFar);

	const glm::mat4& getModelMat() const;
	const glm::mat4& getViewMat() const;
	const glm::mat4& getProjectMat() const;

private:
	void __computeVertexNormal(const unsigned int* vIndices, unsigned int vIndexCount, SVertex* vVertex, unsigned int vVertexCount);
	inline float __toRadian(float vDegree);
	inline float __toDegree(float vRadian);

private:
	glm::mat4 m_ModelMat;
	glm::mat4 m_ViewMat;
	glm::mat4 m_ProjectMat;
};