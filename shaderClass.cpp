#include "shaderClass.h"


// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename){
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}
shader::shader(const char* vertexFile,const char* fragmentFile){



std::string vertexCode=get_file_contents(vertexFile);
std::string fragmentCode=get_file_contents(fragmentFile);

const char* vertexSource = vertexCode.c_str();
const char* fragmentSource = fragmentCode.c_str();


    //Create vertex shader, link it to source and compile it
    GLuint vertexShader= glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexSource,NULL);
    glCompileShader(vertexShader);


    //Create fragment shader, link it to source and compile it
    GLuint fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentSource,NULL);
    glCompileShader(fragmentShader);




    //create Shader program now
     ID =glCreateProgram();
    //Attach the shaders to the shader program (only one shader program for two shaders though)
    glAttachShader(ID,vertexShader);
    glAttachShader(ID,fragmentShader);
    //Link the program
    glLinkProgram(ID);



    //Delete the now useless shader objects because they are now part of shaderProgram
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



}
void shader::Activate(){

 glUseProgram(ID);


}
void shader::Delete(){
glDeleteProgram(ID);
}
