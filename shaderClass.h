#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<GL/glad.h>
#include<string>
#include<string.h>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class shader{

public:
    GLuint ID;
    shader(const char* vertexfile,const char* fragmentFile);

    void Activate();
    void Delete();


};


#endif // SHADER_CLASS_H
