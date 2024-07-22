#include "ShaderProgram.hpp"
#include "Logger.hpp"
#include "gl_debug.hpp"

using namespace Engine;

unsigned int ShaderProgram::LoadFromSource(const GLenum shaderType, const std::string& source) const
{
	unsigned int shaderId = glCreateShader(shaderType);
	if (shaderId == 0)
	{
		Logger::LogError("Unable to create shader {0}, type {1}.", debugIdentifier, shaderType);
		GLCheckErrors();
		return 0;
	}

	const char* charSouce = source.c_str();
	GLCHECK(glShaderSource(shaderId, 1, &charSouce, NULL));
	GLCHECK(glCompileShader(shaderId));

	if (!CheckShaderCompileStatus(shaderId))
	{
		GLCHECK(glDeleteShader(shaderId));
		return 0;
	}

	return shaderId;
}

bool ShaderProgram::CheckShaderCompileStatus(const unsigned int shaderId) const
{
	GLint status;
	GLCHECK(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status));
	if (status == GL_FALSE)
	{
		GLint length;
		GLCHECK(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length));
		GLchar* infoLog = new GLchar[length];
		GLCHECK(glGetShaderInfoLog(shaderId, length, NULL, infoLog));

		Engine::Logger::LogError("Shader compilation of {0} failed.\n{1}", debugIdentifier, infoLog);
		DEBUG_BREAK();

		delete[] infoLog;
		return false;
	}
	return true;
}

bool ShaderProgram::CheckShaderProgramLinkStatus(const unsigned int shaderProgramId) const
{
	GLint status;
	GLCHECK(glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &status));
	if (status == GL_FALSE)
	{
		GLint length;
		GLCHECK(glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &length));
		GLchar* infoLog = new GLchar[length];
		GLCHECK(glGetProgramInfoLog(shaderProgramId, length, NULL, infoLog));
		Engine::Logger::LogError("Linking of shader {0} failed.\n{1}", debugIdentifier, infoLog);

		DEBUG_BREAK();
		delete[] infoLog;
		return false;
	}
	return true;
}

ShaderProgram::ShaderProgram(const std::string& debugIdentifier)
{
	programId = 0;
	this->debugIdentifier = debugIdentifier;
}

ShaderProgram::~ShaderProgram()
{
	GLCHECK(glDeleteProgram(this->programId));
	programId = 0;
}

void ShaderProgram::Create(const std::string& vertexSource, const std::string& fragmentSource)
{
	unsigned int vertexShader = LoadFromSource(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentShader = LoadFromSource(GL_FRAGMENT_SHADER, fragmentSource);
	if (vertexShader == 0 || fragmentShader == 0)
	{
		GLCHECK(glDeleteShader(vertexShader));
		GLCHECK(glDeleteShader(fragmentShader));
		return;
	}

	unsigned int shaderProgramId = glCreateProgram();
	if (shaderProgramId == 0)
	{
		Logger::LogError("Unable to create shader program {0}.", debugIdentifier);
		GLCheckErrors();
		return;
	}

	GLCHECK(glAttachShader(shaderProgramId, vertexShader));
	GLCHECK(glAttachShader(shaderProgramId, fragmentShader));
	GLCHECK(glLinkProgram(shaderProgramId));
	
	GLCHECK(glDeleteShader(vertexShader));
	GLCHECK(glDeleteShader(fragmentShader));

	if (!CheckShaderProgramLinkStatus(shaderProgramId))
	{
		GLCHECK(glDeleteProgram(shaderProgramId));
		return;
	}

	programId = shaderProgramId;
}

int ShaderProgram::GetAttribLocation(const std::string& attribName)
{
	int location = glGetAttribLocation(programId, attribName.c_str());
	if (location < 0)
	{
		Logger::LogError("Unable to retrieve attribute location for '{0}'.", attribName);
		DEBUG_BREAK();
		return -1;
	}
	return location;
}

void ShaderProgram::Use() const
{
	GLCHECK(glUseProgram(programId));
}

void ShaderProgram::Unuse() const
{
	GLCHECK(glUseProgram(0));
}
