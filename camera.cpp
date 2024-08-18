#include"camera.h"


camera::camera(int width,int height,glm::vec3 position){

camera::width=width;
camera::height=height;
camera::Position=position;

}

void camera::Create_and_Send_Matrix(float FOVdeg,float nearPlane,float farPlane,shader& shader,const char* uniform,glm::mat4 World_Matrix){

glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection= glm::mat4(1.0f);

view=glm::lookAt(camera::Position,camera::Position+camera::Orientation,camera::Up);
projection=glm::perspective(glm::radians(45.0f),(float)(800/800),0.1f,100.0f);


glUniformMatrix4fv(glGetUniformLocation(shader.ID,uniform),1,GL_FALSE,glm::value_ptr(projection*view*World_Matrix));
}

void camera::Create_and_Send_Matrix2(float FOVdeg,float nearPlane,float farPlane,shader_Geo& shader,const char* uniform,glm::mat4 World_Matrix){

glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection= glm::mat4(1.0f);

view=glm::lookAt(camera::Position,camera::Position+camera::Orientation,camera::Up);
projection=glm::perspective(glm::radians(45.0f),(float)(800/800),0.1f,100.0f);


glUniformMatrix4fv(glGetUniformLocation(shader.ID,uniform),1,GL_FALSE,glm::value_ptr(projection*view*World_Matrix));
}


void camera::Send_ViewMat(float FOVdeg,float nearPlane,float farPlane,shader& shader,const char* uniform){

glm::mat4 view = glm::mat4(1.0f);
view=glm::lookAt(camera::Position,camera::Position+camera::Orientation,camera::Up);
glUniformMatrix4fv(glGetUniformLocation(shader.ID,uniform),1,GL_FALSE,glm::value_ptr(view));
}


void camera::Send_ProjMat(float FOVdeg,float nearPlane,float farPlane,shader& shader,const char* uniform){

glm::mat4 projection= glm::mat4(1.0f);
projection=glm::perspective(glm::radians(45.0f),(float)(800/800),0.1f,100.0f);
glUniformMatrix4fv(glGetUniformLocation(shader.ID,uniform),1,GL_FALSE,glm::value_ptr(projection));
}
