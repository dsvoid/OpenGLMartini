// COMP 371 Assignment 1
// Dmitry Svoiski- 26893570
// modified from http://learnopengl.com ,
// then further modified from COMP 371 Lab 02.

#include <GL/glew.h>	// include GL Extension Wrangler
#include <GLFW/glfw3.h>	// include GLFW helper library
#include <stdio.h>
#include <math.h> // for sin, cos
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/ext.hpp> // for debug prints

#define ROT M_PI/32.0

using namespace std;

// initial Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;
// variable Window dimensions
int width = WIDTH;
int height = HEIGHT;
float ratio =width/height;

// shader is defined globally so it can be accessed by key_callback
GLuint shaderProgram;

// model and view are defined globally to preserve state between callbacks
glm::mat4 model = glm::mat4(1.0);
glm::mat4 view;

// scale the model when scrolling the mousewheel
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  model = glm::scale(model, glm::vec3(1.0f+yoffset*0.1f, 1.0f+yoffset*0.1f, 1.0f+yoffset*0.1f));
  GLuint transformLoc = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
}

//rotate the camera around themodel based on cursor's X position in window
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  float radius = 2.0f;
  GLfloat camX = sin(-4*M_PI*xpos/width) * radius;
  GLfloat camZ = cos(-4*M_PI*xpos/width) * radius;
  view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  switch(key) {
    //quit
    case GLFW_KEY_ESCAPE:
      if(action == GLFW_PRESS)
	glfwSetWindowShouldClose(window, GL_TRUE);
      break;
      //rotate model with QWEASD
    case GLFW_KEY_S:
      if(action == GLFW_PRESS || action == GLFW_REPEAT) {
	model = glm::rotate(model, float(ROT), glm::vec3(1,0,0));
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
      }
      break;
    case GLFW_KEY_W:
      if(action == GLFW_PRESS || action == GLFW_REPEAT) {
	model = glm::rotate(model, float(-ROT), glm::vec3(1,0,0));
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
      }
      break;
    case GLFW_KEY_A:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
	model = glm::rotate(model, float(ROT), glm::vec3(0,0,1));
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
      }
      break;
    case GLFW_KEY_D:
      if(action == GLFW_PRESS || action == GLFW_REPEAT) {
	model = glm::rotate(model, float(-ROT), glm::vec3(0,0,1));
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
      }
      break;
    case GLFW_KEY_E:
      if(action == GLFW_PRESS || action == GLFW_REPEAT) {
	model = glm::rotate(model, float(ROT), glm::vec3(0,1,0));
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
      }
      break;
    case GLFW_KEY_Q:
      if(action == GLFW_PRESS || action == GLFW_REPEAT) {
	model = glm::rotate(model, float(-ROT), glm::vec3(0,1,0));
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
      }
      break;
      // render only points
    case GLFW_KEY_P:
      if (action == GLFW_PRESS)
	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      break;
      // render polygons with just lines
    case GLFW_KEY_L:
      if (action == GLFW_PRESS)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
      //render plygons with fill
    case GLFW_KEY_F:
      if(action == GLFW_PRESS)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
      break;
    default:
      break;
  }
}

// convert a line with three separate arguments into a vector of size 3, return it
vector<float> splitToFloat(string line)
{
  stringstream ss;
  ss.str(line);
  vector<float> segments;
  string segment;
  while(getline(ss, segment, ' ')) {
    float f = stof(segment);
    segments.push_back(f);
  }
  return segments;
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
  std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
  // Init GLFW
  glfwInit();
  // Set all the required options for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Create a GLFWwindow object that we can use for GLFW's functions
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "COMP 371 -Assignment 1", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  // Set the required callback functions
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  // Define the viewport dimensions
  glfwGetFramebufferSize(window, &width, &height);

  glViewport(0, 0, width, height);

  // Build and compile our shader program
  // Vertex shader

  // Read the Vertex Shader code from the file
  string vertex_shader_path = "vertex.shader";
  string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_shader_path, ios::in);

  if (VertexShaderStream.is_open()) {
    string Line = "";
    while (getline(VertexShaderStream, Line))
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }
  else {
    printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
    getchar();
    exit(-1);
  }

  // Read the Fragment Shader code from the file
  string fragment_shader_path = "fragment.shader";
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

  if (FragmentShaderStream.is_open()) {
    std::string Line = "";
    while (getline(FragmentShaderStream, Line))
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }
  else {
    printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
    getchar();
    exit(-1);
  }

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
  glCompileShader(vertexShader);
  // Check for compile time errors
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  // Fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
  glCompileShader(fragmentShader);
  // Check for compile time errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  // Link shaders
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // Check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  glDeleteShader(vertexShader); //free up memory
  glDeleteShader(fragmentShader);

  glUseProgram(shaderProgram);

  // initialize model and view orientation
  model = glm::rotate(model, float(M_PI/2), glm::vec3(1,0,0));
  GLuint passModel = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(passModel, 1, GL_FALSE, glm::value_ptr(model));
  mouse_callback(window, 0.0f, 0.0f);

  // variables used for generating rotational / translational sweep
  int numP = 0;
  int numT = 0;
  int numN = 0;
  float angle = 0.0;
  vector<float> P;
  vector<float> T;
  string sweepType;

  // read coordinate data from file
  ifstream infile("input_a1.txt");
  string line;
  if(infile.is_open()) {
    getline(infile, line);
    sweepType = line;
    // read data for translational sweep
    if(sweepType == "0") {
      // input points for P
      getline(infile, line);
      numP = stoi(line);
      for(int i = 0; i < numP; i++) {
	getline(infile, line);
	vector<float> points(splitToFloat(line));
	P.insert(P.end(), points.begin(), points.end());
      }
      // input points for T
      getline(infile, line);
      numT = stoi(line);
      for(int i = 0; i < numT; i++) {
	getline(infile, line);
	vector<float> points(splitToFloat(line));
	T.insert(T.end(), points.begin(), points.end());
      }
    }
    // read data for rotational sweep
    else {
      // input number of points for profile curve
      getline(infile, line);
      numN = stoi(line);
      angle = 2*M_PI/numN;
      // input points for P
      getline(infile, line);
      numP = stoi(line);
      for(int i = 0; i < numP; i++) {
	getline(infile, line);
	vector<float> points(splitToFloat(line));
	P.insert(P.end(), points.begin(), points.end());
      }
    }
    infile.close();
  }	

  // switch from using vectors to using arrays
  GLfloat vertP[(sweepType == "0" ? numT : numN) * numP * 3];
  for(int i = 0; i < numP * 3; i++)
    vertP[i] = P[i];	

  GLfloat vertT[numT*3];
  copy(T.begin(), T.end(), vertT);

  if(sweepType == "0") {
    // set up vertices for translational sweep
    for(int i = 1; i < numT; i++) {
      for(int j = 0; j < numP*3; j++)
	vertP[i*numP*3+j] = vertP[(i-1)*numP*3+j];
      for(int j = 0; j < numP; j++) {
	vertP[(i*numP*3)+(j*3)+0] += vertT[i*3+0] - vertT[i*3-3]; // X
	vertP[(i*numP*3)+(j*3)+1] += vertT[i*3+1] - vertT[i*3-2]; // Y
	vertP[(i*numP*3)+(j*3)+2] += vertT[i*3+2] - vertT[i*3-1]; // Z
      }
    }
  }
  else {
    // set up vertices for rotational sweep
    for(int i = 1; i < numN; i++) {
      for(int j = 0; j < numP*3; j++)
	vertP[i*numP*3+j] = vertP[(i-1)*numP*3+j];
      for(int j = 0; j < numP; j++) {
	float x = vertP[(i*numP*3)+(j*3)+0];
	float y = vertP[(i*numP*3)+(j*3)+1];
	float r = sqrt(pow(x,2) + pow(y,2));
	float t = atan(y/x);
	if(x < 0)
	  t += M_PI;
	else if(y < 0)
	  t += 2*M_PI;
	t += angle;
	x = r*cos(t);
	y = r*sin(t);
	vertP[(i*numP*3)+(j*3)+0] = x;
	vertP[(i*numP*3)+(j*3)+1] = y;
      }
    }
  }

  // create array of indices for triangle strips
  GLuint indices[sweepType == "0" ? (numT-1)*numP*2 : numN*numP*2];

  if(sweepType == "0") {
    //set up indices for translational sweep
    for(int i = 0; i < numT-1; i++) {
      for(int j = 0; j < numP; j++) {
	for(int k = 0; k < 2; k++)
	  indices[(2*numP*i)+(2*j)+k] = numP*i + j + numP*k;
      }
    }
  }
  else {
    // set up indices for rotational sweep
    for(int i = 0; i < numN; i++) {
      for(int j = 0; j < numP; j++) {
	for(int k = 0; k < 2; k++)
	  indices[(2*numP*i)+(2*j)+k] = (numP*i + j + numP*k) % (numP*numN);
      }
    }
  }


  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  GLuint EBO;
  glGenBuffers(1, &EBO);

  // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
  glBindVertexArray(VAO);
  // Copy vertices array in a vertex buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertP), vertP, GL_STATIC_DRAW);
  // Copy our index array in a element buffer for OpenGL to use
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
  // 3. Then set the vertex attributes pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

  glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Game loop
  while (!glfwWindowShouldClose(window))
  {
    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float ratio = width/(float)height;
    glViewport(0,0, width, height);

    // Render
    // Clear the colorbuffer
    glClearColor(0.2f, 0.05f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VAO);

    // Projection 
    glm::mat4 projection;
    projection = glm::ortho(-ratio, ratio, 1.0f, -1.0f, -4.0f, 4.0f);
    //projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    // Pass the matrices to the shader
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    if(sweepType == "0") {
      for(int i = 0; i < numT-1; i++)
	glDrawElements(GL_TRIANGLE_STRIP, 2*numP, GL_UNSIGNED_INT, ((char *)NULL + 2*i*numP*sizeof(GLfloat)));
    }
    else {
      for(int i = 0; i < numN; i++)
	glDrawElements(GL_TRIANGLE_STRIP, 2*numP, GL_UNSIGNED_INT, ((char *)NULL + 2*i*numP*sizeof(GLfloat)));
    }

    glBindVertexArray(0);

    // Swap the screen buffers
    glfwSwapBuffers(window);
  }

  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();
  return 0;
}
