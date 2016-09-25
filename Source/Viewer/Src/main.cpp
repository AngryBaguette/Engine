/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#include "Common.hpp"
#include "Memory.hpp"
#include "RefCounted.hpp"
#include "Array.hpp"
#include "RHI.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>


#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#define SHADER_DIR "C:\\Dev\\GitHub\\Engine\\Shader\\"

#define CHECKGLERROR CheckGLError(__FILE__, __LINE__)

/************************************************************************/
static void CheckGLError(const char* pFile, int32_t pLine)
{
#define GL_GET_DEBUG_MESSAGE_LOG_COUNT 10
	char lBuffer[2048];
	GLsizei lLengths[GL_GET_DEBUG_MESSAGE_LOG_COUNT];
	GLenum lSeverities[GL_GET_DEBUG_MESSAGE_LOG_COUNT];

	GLuint count = GL_GET_DEBUG_MESSAGE_LOG_COUNT;
	GLsizei bufSize = sizeof(lBuffer);
	GLenum *sources = NULL;
	GLenum *types = NULL;
	GLuint *ids = NULL;
	GLenum *severities = lSeverities;
	GLsizei *lengths = lLengths;
	GLchar *messageLog = lBuffer;
	GLuint lNbMessage = glGetDebugMessageLog(count, bufSize, sources, types, ids, severities, lengths, messageLog);
	if
		(lNbMessage)
	{
		unsigned int offset = 0;
		for
			(unsigned int i = 0; i < lNbMessage; ++i)
		{
			std::cout << messageLog + offset << std::endl;
			offset += lengths[i];
		}
	}
}

/************************************************************************/
static bool CheckShaderCompilStatus(uint32_t pShader)
{
	// Check Fragment Shader
	GLint result;
	GLint messageLength;
	glGetShaderiv(pShader, GL_COMPILE_STATUS, &result); CHECKGLERROR;
	glGetShaderiv(pShader, GL_INFO_LOG_LENGTH, &messageLength); CHECKGLERROR;
	if (messageLength > 0) {
		std::vector<char> message(messageLength + 1);
		glGetShaderInfoLog(pShader, messageLength, NULL, &message[0]); CHECKGLERROR;
		printf("%s\n", &message[0]);
	}

	return result EQ GL_TRUE;
}

/************************************************************************/
static bool CheckProgramLinkStatus(uint32_t pShader)
{
	// Check Fragment Shader
	GLint result;
	GLint messageLength;
	glGetProgramiv(pShader, GL_LINK_STATUS, &result); CHECKGLERROR;
	glGetProgramiv(pShader, GL_INFO_LOG_LENGTH, &messageLength); CHECKGLERROR;
	if (messageLength > 0) {
		std::vector<char> message(messageLength + 1);
		glGetProgramInfoLog(pShader, messageLength, NULL, &message[0]); CHECKGLERROR;
		printf("%s\n", &message[0]);
	}

	return result EQ GL_TRUE;
}


/** Representation in memory of attribute data */
enum class EVertexAttributeFormat : uint8_t
{
	Float1,
	Float2,
	Float3,
	Float4,
	UByte4,
	UByte4N,	// Generally used for color

	Count
};

// https://www.opengl.org/sdk/docs/man4/html/glVertexAttribPointer.xhtml
static void TranslateVertexAttributeFormat(EVertexAttributeFormat pFormat, GLenum& pType, GLint& pSize, GLboolean& pIsNormalized)
{
	// EVertexAttributeFormat				= {   Float1,   Float2,   Float3,   Float4,           UByte4,          UByte4N
	static const GLenum lsToGLType[]		= { GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE };
	static const uint8_t lsToGLSize[]		= {        1,        2,        3,        4,                4,                4 };
	static const bool lsToGLNormalized[]	= {    false,    false,    false,     false,           false,            false };	
	//static_assert(ARRAY_COUNT(lsToGLSize) EQ ARRAY_COUNT(lsToGLNormalized) EQ ARRAY_COUNT(lsToGLType) EQ (uint8_t)EVertexAttributeFormat::Count, "Number of enum changed, update this table");

	pType = lsToGLType[(uint8_t)pFormat];
	pSize = lsToGLSize[(uint8_t)pFormat];
	pIsNormalized = lsToGLNormalized[(uint8_t)pIsNormalized];
}

struct VertexAttribute
{
	VertexAttribute(uint8_t pStreamIndex, EVertexAttributeFormat pFormat, uint32_t pOffset, uint32_t pStride, uint8_t pDivisor) : mStride(pStride), mOffset(pOffset), mStreamIndex(pStreamIndex), mFormat(pFormat), mDivisor(pDivisor) {}

	uint32_t mStride;	// Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
	uint32_t mOffset;	// Offset in bytes in the vertex buffer where start the first vertex
	uint8_t mStreamIndex;
	EVertexAttributeFormat mFormat;
	uint8_t mDivisor;	// Modifies the rate at which generic vertex attributes advance when rendering multiple instances of primitives in a single draw call.If divisor is zero, the attribute at slot index advances once per vertex. If divisor is non-zero, the attribute advances once per divisor instances of the set(s) of vertices being rendered.
};

// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glVertexAttribPointer.xml
struct OpenGLVertexAttribute : public VertexAttribute
{
	OpenGLVertexAttribute(uint8_t pStreamIndex, EVertexAttributeFormat pFormat, uint32_t pOffset, uint32_t pStride, uint8_t pDivisor) 
	: VertexAttribute(pStreamIndex, pFormat, pOffset, pStride, pDivisor)
	{
		TranslateVertexAttributeFormat(mFormat, mType, mTypeSize, mIsNormalized);
	}

	GLint mTypeSize;			// The number of components per generic vertex attribute. Must be 1, 2, 3, or 4
	GLenum mType;				// GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_FIXED, or GL_FLOAT ...
	GLboolean mIsNormalized;	// Specifies whether fixed-point data values should be normalized or converted directly as fixed-point values when they are accessed.
};

typedef  Array<VertexAttribute> VertexAttributeDeclaration;

/*
class VertexFormatResource : public RenderResource
{
	//VertexAttributeDeclarationResource
};
typedef RefPointer<VertexFormatResource> VertexFormatResourcePtr;
*/


/*****************************************************************************/
std::string loadShaderFile(const std::string& pFilename)
{
	std::string source;
	std::ifstream is(pFilename, std::ifstream::in);
	if (is)
	{
		std::string line = "";
		while (std::getline(is, line))
			source += line + "\n";
		is.close();
	}
	return source;
}

GLuint compileShader(const std::string& pFilename, GLenum pShaderType)
{
	GLuint shaderID = -1;
	std::string sourceStr = loadShaderFile(pFilename); assert(sourceStr.size());
	char* const source[] = { (char*)sourceStr.data() };
	shaderID = glCreateShader(pShaderType); CHECKGLERROR;
	glShaderSource(shaderID, 1, source, NULL); CHECKGLERROR;
	glCompileShader(shaderID); CHECKGLERROR;
	bool lSuccess = CheckShaderCompilStatus(shaderID);
	assert(lSuccess);

	return shaderID;
}


GLuint vbo;
GLuint ibo;
GLuint vao;
GLuint positionSlot = 0;
GLuint vertShaderID;
GLuint fragShaderID;
GLuint programID;

/*****************************************************************************/
bool initScene()
{
	glm::vec3 position = { -0.25f, -0.25f, 0.0f };

	Array< glm::vec3 > vertices =
	{
		{ -0.25f, -0.25f, 0.0f },
		{ -0.25f, 0.25f, 0.0f },
		{ 0.25f, 0.25f, 0.0f },
		{ 0.25F, -0.25F, 0.0f }
	};

	Array< uint16_t > indexes =
	{
		0, 1, 2,
		2, 3, 0
	};

	// Shader compilation
	{
		vertShaderID = compileShader(SHADER_DIR "simple.vert", GL_VERTEX_SHADER);
		fragShaderID = compileShader(SHADER_DIR "simple.frag", GL_FRAGMENT_SHADER);
	}

	// Link shader
	{
		programID = glCreateProgram();
		glAttachShader(programID, vertShaderID);
		glAttachShader(programID, fragShaderID);
		glLinkProgram(programID);
		bool success = CheckProgramLinkStatus(programID); assert(success);
	}
	

	glGenBuffers(1, &vbo); CHECKGLERROR;
	glBindBuffer(GL_ARRAY_BUFFER, vbo); CHECKGLERROR;
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.count(), vertices.data(), GL_STATIC_DRAW); CHECKGLERROR;

	glGenBuffers(1, &ibo); CHECKGLERROR;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); CHECKGLERROR;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indexes.count(), indexes.data(), GL_STATIC_DRAW); CHECKGLERROR;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	}
	glBindVertexArray(0);




	VertexBufferResourcePtr vb = RHICreateVertexBuffer(vertices.dataSize(), EBufferUsage::Static, vertices.data());
	RHILockVertexBuffer(vb, 0, vb->getSize(), EBufferAccess::WriteOnly);
	RHIUnlockVertexBuffer(vb);

	IndexBufferResourcePtr ib = RHICreateIndexBuffer(sizeof(uint16_t), indexes.dataSize(), EBufferUsage::Static, indexes.data());

	/*
	VertexBuffer* vb = VertexBuffer::create(4);
	vb->addAttribute(VertexBuffer::ePosition, 3, VertexBuffer::eFloat);
	//vb->addAttribute(VertexBuffer::eNormal, 3, VertexBuffer::eFloat);
	//vb->addAttribute(VertexBuffer::eTexCoord0, 2, VertexBuffer::eFloat);
	vb->allocate();
	vb->setAttributeValue(VertexBuffer::ePosition, 0, (uint8_t*)positions, 4);
	*/

	// Load sponza
	/*
	std::string sponzaFile = gDataDir + "Models/sponza.obj";
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(sponzaFile,
	aiProcess_PreTransformVertices |
	aiProcess_CalcTangentSpace |
	aiProcess_Triangulate |
	aiProcess_JoinIdenticalVertices |
	aiProcess_SortByPType);
	*/

	return true;
}


/*****************************************************************************/
void resize(int pWidth, int pHeight)
{
	RHISetViewport(glm::i32vec4(0, 0, pWidth, pHeight), glm::vec2(0, 0));
}

/*****************************************************************************/
void render()
{
	RHIClear(true, glm::vec4(0, 0, 0, 0), true, 0.0f, false, 0);

	glBindVertexArray(vao);

	glUseProgram(programID);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

	glBindVertexArray(0);

	glutSwapBuffers();
}

/*****************************************************************************/
void idle()
{
	render();
}


/*****************************************************************************/
int main(int argc, char** argv)
{
	// Default memory allocator
	Memory::SetMalloc(new DefaultMalloc());

	glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);

	glutCreateWindow("GLUT Viewer");

	glutDisplayFunc(render);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);


	// At the moment OpenGLRHI used glew and glew use an OpenGL Context...
	IDynamicRHI* lRenderer = IDynamicRHI::DynamicLoadRenderer("OpenGL");
	assert(lRenderer);
	IDynamicRHI::SetRenderer(lRenderer);



	GLenum err = glewInit();
	assert(err == GLEW_OK);
	if (GLEW_VERSION_4_1)
	{
		std::cout << "Driver supports OpenGL 4.1\nDetails:" << std::endl;
	}
	if
		(GLEW_KHR_debug)
	{
		glEnable(GL_DEBUG_OUTPUT);
	}



	initScene();

	glutMainLoop();

	return 0;
}