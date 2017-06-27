#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

class Shader {
public:
	// constructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	// get program ID
	const GLuint& getID() { return ID; }

	// use/activate the shader
	void use() const;

	// utility uniform functions
	void Shader::setBool(const std::string &name, bool value) const;
	void Shader::setInt(const std::string &name, int value) const;
	void Shader::setFloat(const std::string &name, float value) const;
	void Shader::setVec2(const std::string &name, const glm::vec2 &value) const;
	void Shader::setVec2(const std::string &name, float x, float y) const;
	void Shader::setVec3(const std::string &name, const glm::vec3 &value) const;
	void Shader::setVec3(const std::string &name, float x, float y, float z) const;
	void Shader::setVec4(const std::string &name, const glm::vec4 &value) const;
	void Shader::setVec4(const std::string &name, float x, float y, float z, float w);
	void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const;
	void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const;
	void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const;
private:
	// the program ID
	GLuint ID;
};

#endif