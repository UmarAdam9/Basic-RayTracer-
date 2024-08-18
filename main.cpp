#define GLFW_DLL
#include<iostream>
#include<GL/glad.h>
#include<GL/glfw3.h>
#include<math.h>
#include<GL/glm/glm.hpp>
#include<GL/glm/gtc/matrix_transform.hpp>
#include<GL/glm/gtc/type_ptr.hpp>
#include<GL/glm/gtx/rotate_vector.hpp>
#include<GL/glm/gtx/vector_angle.hpp>
#include"shaderClass.h"
#include"camera.h"


int width=800;
int height=800;


float rectangleVertices[] ={
	// Coords    // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};


//had to make an external function because it was giving linking errors when i tried to to use glfwgetkey() inside the camera class
void Input(camera &c,GLFWwindow* window){

if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS) //forward
{
    c.Position+= c.speed * c.Orientation;
}
if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS) //left
{
    c.Position+= c.speed * -glm::normalize(glm::cross(c.Orientation,c.Up));
}
if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS) //Back
{
    c.Position+= c.speed * -c.Orientation;
}
if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS) //Right
{
    c.Position+= c.speed * glm::normalize(glm::cross(c.Orientation,c.Up));
}
if(glfwGetKey(window,GLFW_KEY_R)==GLFW_PRESS) //Up
{
    c.Position+= c.speed * c.Up;
}
if(glfwGetKey(window,GLFW_KEY_F)==GLFW_PRESS)  //Down
{
    c.Position+= c.speed * -c.Up;
}

if(glfwGetKey(window,GLFW_KEY_L)==GLFW_PRESS)  //Down
{
  c.Orientation = glm::rotate(c.Orientation, glm::radians(-3.14f/12.0f), c.Up);}

if(glfwGetKey(window,GLFW_KEY_J)==GLFW_PRESS)  //Down
{
  c.Orientation = glm::rotate(c.Orientation, glm::radians(3.14f/12.0f), c.Up);
}

if(glfwGetKey(window,GLFW_KEY_I)==GLFW_PRESS)  //Down
{
  c.Orientation = glm::rotate(c.Orientation, glm::radians(3.14f/12.0f), glm::normalize(glm::cross(c.Orientation,c.Up)));
}
if(glfwGetKey(window,GLFW_KEY_K)==GLFW_PRESS)  //Down
{
  c.Orientation = glm::rotate(c.Orientation, glm::radians(3.14f/12.0f), -glm::normalize(glm::cross(c.Orientation,c.Up)));
}

// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (c.firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			c.firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees
		float rotX = c.senstivity * (float)(mouseY - (height / 2)) / height;
		float rotY = c.senstivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(c.Orientation, glm::radians(-rotX), glm::normalize(glm::cross(c.Orientation, c.Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, c.Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			c.Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		c.Orientation = glm::rotate(c.Orientation, glm::radians(-rotY), c.Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		c.firstClick = true;
	}


}

int main(int argc, char *argv[]){

    //==Window viewing stuff=====================================================
   //initialize glfw
    glfwInit();
   //Tell GLFW which version we are using and compatibility (core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    //Create a window by using a GLFwindow object
    GLFWwindow *window =glfwCreateWindow(width,height,"First Project",NULL,NULL);
     //Make the created window the current window
    glfwMakeContextCurrent(window);
    //Load glad to configure OpenGL
    gladLoadGL();
    //viewport
    glViewport(0, 0, width, height);


    //Make the Shaders for all the objects=========================================
    shader framebufferProgram("framebuffer.vert.txt", "RaytracerShader.txt");



    //Frame Buffer code===========================================================
        //create an unsigned int
            unsigned int FBO;
        //generate a framebuffer
            glGenFramebuffers(1,&FBO);
        //Bind the framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER,FBO);



        //Now create a texture to wrap on the frame buffer
        unsigned int framebufferTexture;
        glGenTextures(1,&framebufferTexture);
        glBindTexture(GL_TEXTURE_2D,framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL); //Properly study this from the docs??


        //set the parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);    //specifies to openGl that it should use nearest neighbor algorithm to handle when texture is minimized.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);    //specifies to openGl that it should use nearest neighbor algorithm to handle when texture is magnified.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);  //specifies to openGl that it should clamp the texture to the edge of the screen.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);  //specifies to openGl that it should clamp the texture to the edge of the screen.


        //Attach the texture to the framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,framebufferTexture,0);


        //Texture can be accessed from shader!!


        //NOW create a renderbuffer(what is that?) to store the depth and stencil buffer
        unsigned int RBO;
        glGenRenderbuffers(1,&RBO);
        glBindRenderbuffer(GL_RENDERBUFFER,RBO);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,width,height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,RBO); //this attaches the renderbuffer to the frambuffer..?


        //some error checkng also but naah
        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;


        //make its VAO and VBOs

        unsigned int rectVAO, rectVBO;

        glGenVertexArrays(1, &rectVAO);
        glGenBuffers(1, &rectVBO);

        glBindVertexArray(rectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, rectVBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW); //this is new... nevermind its in the VBO constructor

        //Link attribte stuff

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
	    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

     // send texture to the fragment shader
        framebufferProgram.Activate();
        glUniform1i(glGetUniformLocation(framebufferProgram.ID, "screenTexture"), 0);


     //=====================================================

     //Depth buffer (like painters Algorithm)
     glEnable(GL_DEPTH_TEST);
     //create camera class that basically contains the view matrix as well as the projection matrix
     camera camera_1(800,800,glm::vec3(0.0f,0.0f,3.0f));

    //=====rotation Logic for main cube==============================================
    double prevTime=glfwGetTime();
    double currentTime=0.0f;
    double Time=0.0f;
    //================================================================================


    //main While loop=====================================================================================================
    while(!glfwWindowShouldClose(window)){

    // Timing (this strategy new )
    currentTime=glfwGetTime();
    Time=currentTime-prevTime;

    if(Time >= 1/60.0f)
    {
        prevTime=currentTime;
    }

    // Bind the custom framebuffer before clearing the screen etc
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    //clear screen with desired color
     glClearColor(0.0f,0.0f,0.0f,1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     // Enable depth testing since it's disabled when drawing the framebuffer rectangle
     glEnable(GL_DEPTH_TEST);


     Input(camera_1,window);



     // Bind the default framebuffer==================
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Draw the framebuffer rectangle
		framebufferProgram.Activate();

		//send the projection and view Matrices as well as camera Position
         camera_1.Send_ProjMat(45.0f,0.1f,100.0f,framebufferProgram,"ProjMatrix");
         camera_1.Send_ViewMat(45.0f,0.1f,100.0f,framebufferProgram,"ViewMatrix");
         glUniform3f(glGetUniformLocation(framebufferProgram.ID,"camPos") ,camera_1.Position.x, camera_1.Position.y, camera_1.Position.z);




		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
        //=============================================

    //Swap the Buffers
       glfwSwapBuffers(window);
    //Take care of events
      glfwPollEvents();

    }

       //Delete VAOs and VBOs and EBOs and Shaders after we exit the main while loop
        glDeleteFramebuffers(1, &FBO);
        framebufferProgram.Delete();


        //Destroy the window
        glfwDestroyWindow(window);
        //necessary to terminate GLFW after initializing it
        glfwTerminate();



    return 0;
}
