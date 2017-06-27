#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
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
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	bool uniformExists(const std::string &name) const;
private:
	// the program ID
	GLuint ID;
};

#endif