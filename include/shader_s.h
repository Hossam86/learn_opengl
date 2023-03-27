#ifndef LEARN_OPENGL_SHADER_S_H
#define LEARN_OPENGL_SHADER_S_H

#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <fstream>

class Shader {
    Shader(const char *vertexPath, const char *fragmentPath) {
        // retrieve the vertex and fragment shader source code from filepath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderstream;
            std::stringstream fShaderstream;

            // read file's buffer contents into streams
            vShaderstream << vShaderFile;
            fShaderstream << fShaderFile;

            // close file handles
            vShaderFile.close();
            fShaderFile.close();

            //convert stream to string
            vertexCode = vShaderstream.str();
            fragmentCode = fShaderstream.str();
        }
        catch (std::ifstream::failure &e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
    }

};

#endif //LEARN_OPENGL_SHADER_S_H
