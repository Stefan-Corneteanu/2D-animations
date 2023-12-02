// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

// Include random number generation headers
#include <cstdlib>
#include <ctime>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

#include "shader.hpp"

//variables
GLFWwindow* window;
const int width = 1024, height = 768;

//Callback for adjusting the viewport when resizing the window
void window_callback(GLFWwindow* window, int new_width, int new_height)
{
	glViewport(0, 0, new_width, new_height);
}

//Variables necessary for chosing the right shape to draw and for the right transformation

int no_sides = 0; //how many sides does our figure have
//To draw the shape based on triangles we will use the fact that the minimum number of triangles we can form 
//with the points of a shape such that we use all the points of the shape is(no_points-2), and the number of points is equal to the one of the sides
//initial value is 0 to enhance the idea that nothing should be drawn until we press a key

int offset = -1; //how many indices do we have to skip to reach those that we need, negative value given to enhance the idea that initially nothing
//should be drawn

float dx = 0.0f, dy = 0.0f; //the shape must be drawn in a random position. Rather than modifying the data in the vbo we will
//translate the figure by a vector of coordinates dx dy (the coordinates will be homogenous and normalized)

int transform_flag = 0; //the value of this flag will determine if the shape will suffer no transformation (0)
//will be translated (1), scaled (2) or rotated (3)

float tx = 0.0f; //variables for the translation of the shape left to right
float sx = 1.0f, sy = 1.0f; //variables for the scaling of the shape
float angle = 0.0f; //rotation angle for the rotation transform

bool translation_return = false;
bool scaling_return = false;

glm::mat4 trans(1.0f); //the transform matrix
glm::vec3 rnd; //to be used for the random position of the shape

//Callback for modifying the shape and performing transforms on pressing a particular key
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//I chose to identify the shapes after their number of sides
	switch (key) {

	case GLFW_KEY_4: //square
		if (action == GLFW_PRESS) {
			//first determine where do we draw the square
			srand(time(0)); //generate random numbers
			dx = rand() % 5001 * 0.0001f; //a float has 4 decimals, and, since we have normalized coordinates
			//we want the value to be between -1.0f and 1.0f. But in order to have our shape fully visible we will have it between -0.5f and 0.5f
			//therefore we pick an integer between 0 and 5000 and multiply it by 0.0001f
			//now we randomly decide if our dx is positive or negative
			if (rand() % 2) {
				dx *= -1;
			}
			dy = rand() % 5001 * 0.0001f; //a float has 4 decimals, and, since we have normalized coordinates
			//we want the value to be between -1.0f and 1.0f. But in order to have our shape fully visible we will have it between -0.5f and 0.5f
			//therefore we pick an integer between 0 and 5000 and multiply it by 0.0001f
			//now we randomly decide if our dy is positive or negative
			if (rand() % 2) {
				dy *= -1;
			}
			no_sides = 4; //a square has 4 sides
			offset = 0; //the first indices are used for the square
		}
		break;

	case GLFW_KEY_6: //hexagon
		if (action == GLFW_PRESS) {
			//first determine where do we draw the hexagon
			srand(time(0)); //generate random numbers
			dx = rand() % 5001 * 0.0001f; //a float has 4 decimals, and, since we have normalized coordinates
			//we want the value to be between -1.0f and 1.0f. But in order to have our shape fully visible we will have it between -0.5f and 0.5f
			//therefore we pick an integer between 0 and 5000 and multiply it by 0.0001f
			//now we randomly decide if our dx is positive or negative
			if (rand() % 2) {
				dx *= -1;
			}
			dy = rand() % 5001 * 0.0001f; //a float has 4 decimals, and, since we have normalized coordinates
			//we want the value to be between -1.0f and 1.0f. But in order to have our shape fully visible we will have it between -0.5f and 0.5f
			//therefore we pick an integer between 0 and 5000 and multiply it by 0.0001f
			//now we randomly decide if our dy is positive or negative
			if (rand() % 2) {
				dy *= -1;
			}
			no_sides = 6; //a hexagon has 6 sides
			offset = 6; //the first 6 indices are used for the square, the first to follow them are those for the hexagon
		}
		break;

	case GLFW_KEY_8: //octagon
		if (action == GLFW_PRESS) {
			//first determine where do we draw the square
			srand(time(0)); //generate random numbers
			dx = rand() % 5001 * 0.0001f; //a float has 4 decimals, and, since we have normalized coordinates
			//we want the value to be between -1.0f and 1.0f. But in order to have our shape fully visible we will have it between -0.5f and 0.5f
			//therefore we pick an integer between 0 and 5000 and multiply it by 0.0001f
			//now we randomly decide if our dx is positive or negative
			if (rand() % 2) {
				dx *= -1;
			}
			dy = rand() % 5001 * 0.0001f; //a float has 4 decimals, and, since we have normalized coordinates
			//we want the value to be between -1.0f and 1.0f. But in order to have our shape fully visible we will have it between -0.5f and 0.5f
			//therefore we pick an integer between 0 and 5000 and multiply it by 0.0001f
			//now we randomly decide if our dy is positive or negative
			if (rand() % 2) {
				dy *= -1;
			}
			no_sides = 8; //an octagon has 8 sides
			offset = 18; //the first 6 indices are used for the square, the following 12 for the hexagon, and the remaining ones (18 in total) are for
			//the octagon
		}
		break;

	case GLFW_KEY_O: //clear the screen
		if (action == GLFW_PRESS) {
			no_sides = 0; //no shape means no sides
			offset = -1; //a value to enhance the idea that there should not be any shape on the screen
			//having no shape, it makes no sense to have any transformations, reset all transform related variables
			transform_flag = 0;
			tx = 0.0f;
			sx = 1.0f;
			sy = 1.0f;
			angle = 0.0f;
			translation_return = false;
			scaling_return = false;
		}
		break;

	case GLFW_KEY_T: //translate
		if (action == GLFW_PRESS) {
			if (transform_flag == 1) { //if we are already translating stop in that position
				transform_flag = 0;
			}
			else //if we are not translating, start translating
				transform_flag = 1;
		}
		break;
	case GLFW_KEY_S: //scale
		if (action == GLFW_PRESS) {
			if (transform_flag == 2) //if we are already scaling stop in that position
				transform_flag = 0;
			else //if we are not scaling, start scaling
				transform_flag = 2;
		}
		break;
	case GLFW_KEY_R: //rotate
		if (action == GLFW_PRESS) {
			if (transform_flag == 3) //if we are already rotating stop in that position
				transform_flag = 0;
			else //if we are not rotating, start rotating
				transform_flag = 3;
		}
		break;
	}


}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "BigHW1", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW (OpenGL Extension Wrangler Library)
	// = a loader library :)
	// OpenGl libraries are contained in binary form (.dll)
	// inside your graphics drivers,
	// and GLEW is a portal between us and the OpenGL
	// that already lives in our computers
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}


	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	// Create some data for the vertices
	GLfloat vertices[] = {
		//square
		0.2f, 0.2f, 0.0f, //top right
		-0.2f, 0.2f, 0.0f, //top left
		-0.2f, -0.2f, 0.0f, //bottom left
		0.2f, -0.2f, 0.0f, //bottom right

		//hexagon
		0.2f, 0.2f, 0.0f, //top right
		-0.2f, 0.2f, 0.0f, //top left
		-0.4f, 0.0f, 0.0f, //left
		-0.2f, -0.2f, 0.0f, //bottom left
		0.2f, -0.2f, 0.0f, //bottom right
		0.4f, 0.0f, 0.0f, //right

		//octagon
		0.1f, 0.2f, 0.0f, //top right
		-0.1f, 0.2f, 0.0f, //top left
		-0.2f, 0.1f, 0.0f, //left top
		-0.2f, -0.1f, 0.0f, //left bottom
		-0.1f, -0.2f, 0.0f, //bottom left
		0.1f, -0.2f, 0.0f, //bottom right
		0.2f, -0.1f, 0.0f, //right bottom
		0.2f, 0.1f, 0.0f //right top

	};


	// Add indices for IBO
	GLuint indices[] = {
		//for square
		0,1,2,
		0,2,3,

		//for hexagon
		4,5,6,
		4,6,7,
		4,7,8,
		4,8,9,

		//for octagon
		10,11,12,
		10,12,13,
		10,13,14,
		10,14,15,
		10,15,16,
		10,16,17
	};
	// Add a VAO
	// !!! VAO must be bound BEFORE the VBO & IBO
	GLuint VAO_ID;
	glGenVertexArrays(1, &VAO_ID);
	glBindVertexArray(VAO_ID);
	// Create a vertex buffer object
	GLuint VBO_ID;
	glGenBuffers(1, &VBO_ID);
	// Bind buffer to a target
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	// Upload data to the buffer
	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertices),
		&vertices[0], //pointer to the data
		GL_STATIC_DRAW);

	// Create IBO, Bind it, upload data
	GLuint IBO_ID;
	glGenBuffers(1, &IBO_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(indices),
		&indices[0],
		GL_STATIC_DRAW);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //solid mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	// Check if the window was closed
	glEnableVertexAttribArray(0);

	// attribute buffer : vertices
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout location in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type of data
		GL_FALSE,           // should be normalized?
		3 * sizeof(GL_FLOAT),   // stride
		0         // array buffer offset
	);

	//Set callback for key pressing
	glfwSetKeyCallback(window, key_callback);
	//Set callback for window resizing
	glfwSetFramebufferSizeCallback(window, window_callback);

	while (!glfwWindowShouldClose(window))
	{
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		glBindVertexArray(VAO_ID);
		if (no_sides != 0) { //we cannot draw a shape that doesn't have sides
			//transform matrix
			trans = glm::mat4(1.0f);
			//translation vector
			rnd = glm::vec3(dx, dy, 0.0f);
			//applying translation
			trans = glm::translate(trans, rnd);
			//we check if there is any additional transform to execute (continuous translation, scaling, or rotation)

			switch (transform_flag) {
			case 0: //no transform
				trans = glm::translate(trans, glm::vec3(tx, 0.0f, 0.0f)); //keep the effects of the translation function
				trans = glm::scale(trans, glm::vec3(sx, sy, 0.0f)); //keep the effects of the scaling function
				trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)); //keep the shape in the current angle
				break;

			case 1: //translation
				if (!translation_return) {//shape did not reach the end of the window, move to the right
					tx += 0.0001f;
					trans = glm::translate(trans, glm::vec3(tx, 0.0f, 0.0f));
					if (no_sides == 4 || no_sides == 8) {//if we have a square or an octagon (rightmost point is at 0.2f if the shape is centered)
						if ((0.2f * sx + dx + tx) >= 1.0f) { //if we reached the border of the shape
							translation_return = true; //go back
						}
					}
					if (no_sides == 6) { //if we have a hexagon (rightmost point is at 0.4f if the shape is centered)
						if (0.4f * sx + dx + tx >= 1.0f) { //if we reached the border of the shape
							translation_return = true; //go back
						}
					}
				}
				else {//shape reached the end of the window, move to the left, back to the starting point
					tx -= 0.0001f;
					trans = glm::translate(trans, glm::vec3(tx, 0.0f, 0.0f));
					if (tx <= 0.0f) { //if we reached the starting point, start again moving towards the right border
						translation_return = false; //start moving to the right again
					}
				}
				trans = glm::scale(trans, glm::vec3(sx, sy, 0.0f)); //keep the effects of the scaling function
				trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)); //keep the shape in the current angle
				break;
			case 2: //scaling
				trans = glm::translate(trans, glm::vec3(tx, 0.0f, 0.0f)); //keep the effects of the translation function
				if (!scaling_return) { //shape did not occupy whole screen, grow it
					sx += 0.001f;
					sy += 0.001f;
					trans = glm::scale(trans, glm::vec3(sx, sy, 0.0f));
					if (no_sides == 4 || no_sides == 6) { //case of the square and of the hexagon
						if ((0.2f * sx + dx + tx >= 1.0f)
							&& (-0.2f * sx + dx + tx <= -1.0f)
							&& (0.2f * sy + dy >= 1.0f)
							&& (-0.2f * sy + dy <= -1.0f)) {
							//if all the points are out of bounds then the screen must be fully covered
							scaling_return = true;
						}
					}
					if (no_sides == 8) { //case of the octagon
						if ((0.1f * sx + dx + tx >= 1.0f)
							&& (-0.1f * sx + dx + tx <= -1.0f)
							&& (0.1f * sy + dy >= 1.0f)
							&& (-0.1f * sy + dy <= -1.0f)) {
							//if all the points are out of bounds then the screen must be fully covered
							scaling_return = true;
						}
					}

				}
				else { //shape occupied whole screen, shrink it
					sx -= 0.001f;
					sy -= 0.001f;
					trans = glm::scale(trans, glm::vec3(sx, sy, 0.0f));
					if (sx <= 0.0f && sy <= 0.0f) {//if we reached initial size, start growing again
						scaling_return = false;
					}
				}
				trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)); //keep the shape in the current angle
				break;
			case 3: //rotating
				angle += 1.0f; //increase rotation angle
				trans = glm::translate(trans, glm::vec3(tx, 0.0f, 0.0f)); //keep the effects of the translation function
				trans = glm::scale(trans, glm::vec3(sx, sy, 0.0f)); //keep the effects of the scaling function
				trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
				break;
			}
			//sending data to vertex shader
			unsigned int transformLoc = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			//draw the result
			glDrawElements(GL_TRIANGLES, (no_sides - 2) * 3, GL_UNSIGNED_INT, (GLvoid*)(offset * sizeof(GLint)));
		}
	}

	// Cleanup VBO
	// De-allocate memory from all buffers
	glDeleteBuffers(1, &VBO_ID);
	glDeleteBuffers(1, &IBO_ID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


