#include "shader.h"
#include "gl/glwin.h"
using namespace bazinga;

shader::Vertex::Vertex (Path file) {
	if (fs::fileExists(file)) {
		GLint status;

		// Load the file
		GLint fsize = fs::getFileSize(file);
		GLchar *fdata = fs::getFileData(file);
		fdata[fsize] = 0;

		id = glCreateShader(GL_VERTEX_SHADER);

		// Upload the source to OpenGL
		glShaderSource (id, 1, (const GLchar**)&fdata, &fsize);
		// Delete our version of it
		delete fdata;

		// Compile it
		glCompileShader (id);

		// Verify if everything went ok
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);

	    if (!status) {
	    	cout << "bazinga: shader: error(s) compiling " << file.getPath() << endl;
	        shader::showInfoLog(id);
	        glDeleteShader(id);
	    } else {
			cout << "bazinga: shader: compiled " << file.getPath() << endl;	    	
	    }
	} else {
		cout << "bazinga: shader: vertex: file not found: " << file.getPath() << endl;
	}
}

shader::Vertex::~Vertex () {
	glDeleteShader(id);
}

GLuint shader::Vertex::getID () {
	return id;
}

shader::Fragment::Fragment (Path file) {
	if (fs::fileExists(file)) {
		GLint status;

		// Load the file
		GLint fsize = fs::getFileSize(file);
		GLchar *fdata = fs::getFileData(file);
		fdata[fsize] = 0;

		id = glCreateShader(GL_FRAGMENT_SHADER);

		// Upload the source to OpenGL
		glShaderSource (id, 1, (const GLchar**)&fdata, &fsize);
		// Delete our version of it
		delete fdata;

		// Compile it
		glCompileShader (id);

		// Verify if everything went ok
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);

	    if (!status) {
			cout << "bazinga: shader: error(s) compiling " << file.getPath() << endl;
	        shader::showInfoLog(id);
	        glDeleteShader(id);
	    } else {
			cout << "bazinga: shader: compiled " << file.getPath() << endl;	    	
	    }
	} else {
		cout << "bazinga: shader: fragment: file not found: " << file.getPath() << endl;
	}
}

shader::Fragment::~Fragment () {
	glDeleteShader(id);
}

GLuint shader::Fragment::getID () {
	return id;
}

shader::Program::Program (shader::Vertex *vert, shader::Fragment* frag) {
	GLint status;

	id = glCreateProgram();

	glAttachShader(id, vert->getID());
	glAttachShader(id, frag->getID());

	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (!status) {
		cout << "bazinga: shader: error(s) linking " << endl;
        shader::showProgramInfoLog(id);
        glDeleteProgram(id);
    } else {
		cout << "bazinga: shader: linked program" << endl;    	
    }
}

shader::Program::~Program () {
	glDeleteProgram(id);
}

void shader::Program::use () {
	glUseProgram(id);
}

void shader::Program::stopUse () {
	glUseProgram(0);
}

void shader::Program::loadUniforms (vector <string> names) {
	for (auto name :names) {
		uniformLocations[name] = glGetUniformLocation(id, name.c_str());
	}
}

void shader::Program::setUniform (string name, GLint value) {
	glUniform1i (uniformLocations[name], value);
}

void shader::Program::setUniform (string name, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4) {
	glUniform4f (uniformLocations[name], v1, v2, v3, v4);
}

void shader::showInfoLog (GLuint id) {
	GLint log_length;
	char *log;

	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
	
	log = new char[log_length];
	
	glGetShaderInfoLog(id, log_length, NULL, log);

	cout << "\t" << log << endl;

	delete log;
}

void shader::showProgramInfoLog (GLuint id) {
	GLint log_length;
	char *log;

	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_length);
	
	log = new char[log_length];
	
	glGetProgramInfoLog(id, log_length, NULL, log);

	cout << "\t" << log << endl;

	delete log;
}