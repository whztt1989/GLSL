#include "Shader.h"
#include "VBO.h"
#include <glm/glm.hpp>
#include "Pipeline.h"
#include "Texture.h"
#pragma comment(lib, "glew32.lib")

GLuint g_IBO;
CShader* g_Shader = new CShader;
CVBO<SVertex>* g_VBO = new CVBO<SVertex>;
SVertex VerticesData[4] = {SVertex(glm::vec3(-1.0f, -1.0f, 0.5773f), glm::vec2(0.0f, 0.0f)),
						   SVertex(glm::vec3(0.0f, -1.0f, -1.15475f), glm::vec2(0.5f, 0.0f)),
						   SVertex(glm::vec3(1.0f, -1.0f, 0.5773f), glm::vec2(1.0f, 0.0f)),
						   SVertex(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 1.0f)) };
unsigned int Indices[] = { 0, 3, 1,
						   1, 3, 2,
						   2, 3, 0,
						   0, 1, 2 };

glm::mat4 World;
float Scale = 0.0f;
CTexture* pTexture = NULL;
SDirectionalLight DirectionalLight;

//******************************************************************
//FUNCTION:
void PrintMatrix3( float matrix[16] )
{
	assert( matrix != 0 );
	printf( "%8.2f%8.2f%8.2f\n"
		"%8.2f%8.2f%8.2f\n"
		"%8.2f%8.2f%8.2f\n",
		matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8]);
	printf("\n");
}

//******************************************************************
//FUNCTION:
void PrintMatrix( float matrix[16] )
{
	assert( matrix != 0 );
	printf( "%8.2f%8.2f%8.2f%8.2f\n"
		"%8.2f%8.2f%8.2f%8.2f\n"
		"%8.2f%8.2f%8.2f%8.2f\n"
		"%8.2f%8.2f%8.2f%8.2f\n",
		matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10], matrix[11],
		matrix[12], matrix[13], matrix[14], matrix[15] );
	printf("\n");
}

//******************************************************************
//FUNCTION:
void initGlew()
{
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Fail to initialize GLEW" << std::endl;
		exit(0);
	}
}

//******************************************************************
//FUNCTION:
void initLight()
{
	DirectionalLight.m_AmbientColor = glm::vec3(0.3, 0.3, 0.3);
	DirectionalLight.m_DiffuseColor = glm::vec3(0.9, 0.9, 0.9);
	DirectionalLight.m_SpecularColor = glm::vec3(1.0, 0, 0);
	DirectionalLight.m_Direction = glm::vec3(1.0, 1.0, 1.0);
}

//******************************************************************
//FUNCTION:
void createIndexBuffer()
{
	glGenBuffers(1, &g_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//******************************************************************
//FUNCTION:
void createVertexBuffer(const unsigned int* vIndices, unsigned int vIndexCount, SVertex* vVertex, unsigned int vVertexCount)
{
	for (unsigned int i=0; i<vIndexCount; i+=3)
	{
		unsigned int Index0 = vIndices[i];
		unsigned int Index1 = vIndices[i+1];
		unsigned int Index2 = vIndices[i+2];

		glm::vec3 V10 = vVertex[Index1].m_Pos - vVertex[Index0].m_Pos;
		glm::vec3 V20 = vVertex[Index2].m_Pos - vVertex[Index0].m_Pos;
		glm::vec3 Normal = glm::cross(V10, V20);
		Normal = glm::normalize(Normal);

		vVertex[Index0].m_Normal += Normal;
		vVertex[Index1].m_Normal += Normal;
		vVertex[Index2].m_Normal += Normal;
	}

	for (unsigned int i=0; i<vVertexCount; i++)
	{
		vVertex[i].m_Normal = glm::normalize(vVertex[i].m_Normal);
	}
}

//******************************************************************
//FUNCTION:
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	initGlew();
	initLight();

	pTexture = new CTexture(GL_TEXTURE_2D, "test.png");
	if (!pTexture->loadImage())
		return;

	createIndexBuffer();
	createVertexBuffer(Indices, 12, VerticesData, 4);
	g_Shader->initShader("0vertex.glsl", "0fragment.glsl");
	
	g_VBO->createVBO(VerticesData, sizeof(VerticesData)/sizeof(SVertex));
}	

//******************************************************************
//FUNCTION:
glm::mat3 computeNormalMatrix(const glm::mat4& vModelView)
{
	glm::mat3 NormalMatrix;
	NormalMatrix[0][0] = vModelView[0][0];
	NormalMatrix[1][0] = vModelView[1][0];
	NormalMatrix[2][0] = vModelView[2][0];

	NormalMatrix[0][1] = vModelView[0][1];
	NormalMatrix[1][1] = vModelView[1][1];
	NormalMatrix[2][1] = vModelView[2][1];

	NormalMatrix[0][2] = vModelView[0][2];
	NormalMatrix[1][2] = vModelView[1][2];
	NormalMatrix[2][2] = vModelView[2][2];

	glm::mat3 Normal = glm::transpose(glm::inverse(NormalMatrix));

	return Normal;
}

//******************************************************************
//FUNCTION:
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CPileline* pPileline = new CPileline;
	
	pPileline->lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
 	//pPileline->translate(0.0, sinf(Scale), 0.0);
 	pPileline->rotate(Scale, 0.0, 1.0, 0);
 	//pPileline->scale(0.5, 0.5, 0.5);
 	pPileline->perspective(60, 1, 1, 10);

	glm::mat4 Model = pPileline->getModelMat();
	glm::mat4 View = pPileline->getViewMat();
	glm::mat4 Project = pPileline->getProjectMat();
	glm::mat4 ModelViewProject = Project * View * Model;
	glm::mat4 ModelView = View * Model;
	glm::mat4 NormalMatrix = glm::transpose(glm::inverse(ModelView));

//	PrintMatrix(&Model[0][0]);
// 	PrintMatrix(&ModelView[0][0]);
// 	PrintMatrix(&NormalMatrix[0][0]);
//	PrintMatrix3(&test[0][0]);

	g_Shader->useShader();
	g_Shader->setMatUniformValue("uMVP", &ModelViewProject[0][0]);
	g_Shader->setMatUniformValue("uModelView", &ModelView[0][0]);
	g_Shader->setMatUniformValue("uEyeNormalMatrix", &NormalMatrix[0][0]);
	g_Shader->setFloatUniformValue("uDirectionalLight.m_AmbientColor", DirectionalLight.m_AmbientColor.x, DirectionalLight.m_AmbientColor.y, DirectionalLight.m_AmbientColor.z);
	g_Shader->setFloatUniformValue("uDirectionalLight.m_DiffuseColor", DirectionalLight.m_DiffuseColor.x, DirectionalLight.m_DiffuseColor.y, DirectionalLight.m_DiffuseColor.z);
	g_Shader->setFloatUniformValue("uDirectionalLight.m_SpecularColor", DirectionalLight.m_SpecularColor.x, DirectionalLight.m_SpecularColor.y, DirectionalLight.m_SpecularColor.z);
	g_Shader->setFloatUniformValue("uDirectionalLight.m_Direction", DirectionalLight.m_Direction.x, DirectionalLight.m_Direction.y, DirectionalLight.m_Direction.z);

  	g_VBO->useVBO(0, 3, 0);
	g_VBO->useVBO(1, 2, (const void*)12);
	g_VBO->useVBO(2, 3, (const void*)20);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_IBO);
	pTexture->bindTexture(GL_TEXTURE0);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
  	g_Shader->banShader();

	glutSwapBuffers();
}

//******************************************************************
//FUNCTION:
void reshape(int vWidth, int vHeight)
{
	glViewport(0, 0, (GLsizei)vWidth, (GLsizei)vHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 1, 1, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//******************************************************************
//FUNCTION:
void idle()
{
	Scale += 0.1f;
	glutPostRedisplay();		
}

int main(int argc, char** argv)
{
    Magick::InitializeMagick(*argv);    
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Perspective");
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	//glutReshapeFunc(reshape);
	glutMainLoop();
	
	return 1;
}

