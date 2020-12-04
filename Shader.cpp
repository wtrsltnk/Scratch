
#include <glad/glad.h>
#include "Shader.h"
#include "vector3.h"

Shader::Shader() : vs(0), fs(0), program(0)
{ }

Shader* Shader::createShader(const string& vsFile, const string& fsFile)
{
	Shader* shader = new Shader();
	shader->vs = Shader::compileShader(vsFile, GL_VERTEX_SHADER);
	shader->fs = Shader::compileShader(fsFile, GL_FRAGMENT_SHADER);

	if (shader->vs != -1 && shader->fs != -1)
	{
		shader->program = glCreateProgram();

		glAttachShader(shader->program, shader->vs);
		glAttachShader(shader->program, shader->fs);

		glLinkProgram(shader->program);  
		GLint isLinked;
		GLint maxLength;
		glGetProgramiv(shader->program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			glGetProgramiv(shader->program, GL_INFO_LOG_LENGTH, &maxLength);

			/* The maxLength includes the NULL character */
			char* infoLog = new char[maxLength+1];

			glGetProgramInfoLog(shader->program, maxLength, &maxLength, infoLog);

			cout << infoLog << endl;
			delete [] infoLog;
			delete shader;
			return 0;
		}  
		return shader;    
	}
	return 0;
}

int Shader::compileShader(const string& file, GLenum type)
{
	string content;
	ifstream infile;

	infile.open(file.c_str());
	if (infile.good() != false)
	{
		char in[2] = { 1, 0 };
		in[0] = infile.get();
		while (infile.good() && in[0] >= 0)
		{
			content.append(in);
			in[0] = infile.get();
		}
		infile.close();

		GLuint s = glCreateShader(type);
		const char* tmp = content.c_str();
		glShaderSource(s, 1, (const GLchar**)&tmp, 0);
		glCompileShader(s);

		GLint isCompiled;
		GLint maxLength;
		glGetShaderiv(s, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			glGetShaderiv(s, GL_INFO_LOG_LENGTH, &maxLength);

			/* The maxLength includes the NULL character */
			char* infoLog = new char[maxLength];

			glGetShaderInfoLog(s, maxLength, &maxLength, infoLog);

			cout << infoLog << endl;
			delete [] infoLog;
			return -1;
		}
		return s;
	}
	else
		cout << "File not loaded: " << file << endl;
	
	return -1;
}

Shader::~Shader()
{
	if (this->program != 0)
	{
		if (this->vs != 0)
		{
			glDetachShader(this->program, this->vs);
			glDeleteShader(this->vs);
		}
		if (this->fs != 0)
		{
			glDetachShader(this->program, this->fs);
			glDeleteShader(this->fs);
		}
		glDeleteProgram(this->program);
	}
	else
	{
		if (this->vs != 0)
			glDeleteShader(this->vs);
		if (this->fs != 0)
			glDeleteShader(this->fs);
	}
}

void Shader::use()
{
	glUseProgram(this->program);
}

unsigned int Shader::getUniform(const char* name)
{
	return glGetUniformLocation(this->program, name);
}

void Shader::setUniformVector(unsigned int uniform, const Vector3& vec)
{
	glUniform3fv(uniform, 3, vec.mVector);
}

void Shader::useNoShader()
{
	glUseProgram(0);
}
