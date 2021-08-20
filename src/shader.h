#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// the program ID
	GLuint ShaderProgramID;

	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// delete the program
	// ------------------------------------------------------------------------
	void deleteProgram()
	{
		glDeleteProgram(ShaderProgramID);
	}

	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(ShaderProgramID);
	}

	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, GLboolean value) const
	{
		glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), (GLint)value);
	}

	// ------------------------------------------------------------------------
	void setInt(const std::string& name, GLint value) const
	{
		glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
	}

	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, GLfloat value) const
	{
		glUniform1f(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
	}

	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, GLfloat x, GLfloat y) const
	{
		glUniform2f(glGetUniformLocation(ShaderProgramID, name.c_str()), x, y);
	}

	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const
	{
		glUniform3f(glGetUniformLocation(ShaderProgramID, name.c_str()), x, y, z);
	}

	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		glUniform4f(glGetUniformLocation(ShaderProgramID, name.c_str()), x, y, z, w);
	}

	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
};

#endif
