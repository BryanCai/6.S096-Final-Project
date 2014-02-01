#include "Graphics.h"
#include <nbody/Simulation.h>
#include <nbody/Vector3.h>

#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <GL/glut.h>

#include <glm/glm.hpp>

nbody::Simulation* Graphics::_mySim = NULL;
std::vector<std::vector<Vector3<float> > > Graphics::_vertices;
std::vector<GLuint> Graphics::_VBOs;

Graphics::Graphics () {
  _mySim = new nbody::Simulation{};
}

Graphics::Graphics (std::ifstream& input) {
  _mySim = new nbody::Simulation{ input };
}

size_t count = 0;

void Graphics::displayFunc() {
  std::cout << "--- EVOLUTION " << ++count << " ---" << std::endl;
  _mySim->evolveSystem( 1.4, 0.000001 );
  //Graphics::updateData();
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();
  glutPostRedisplay();
}

void Graphics::updateData() {
  for(size_t i = 0; i < _mySim->getSystem()->numBodies(); i++){
    Vector3<float> pos = _mySim->getSystem()->getBody(i).position();
    Vector3<float> add = {pos.x() / WIDTH * SCALE, pos.y() / HEIGHT * SCALE, pos.z()};
    std::cout << i << " " << add << std::endl;
    _vertices[i].push_back(add);
  }
}

//Initializes GLUT, creates a window, and initializes GLEW
void Graphics::init (int* argc, char** argv) {
  //Initialize GLUT
  glutInit(argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(50,50);
  glutCreateWindow("N-body Problem");
    
  //Callbacks
  glutDisplayFunc(displayFunc);
   
  //Initialize GLEW
  GLenum res = glewInit();
  if (res != GLEW_OK) {
    std::cout << "Unable to initialize GLEW" << std::endl;
    std::cout << glewGetErrorString(res) << std::endl;
    std::exit(1);
  }
}

void Graphics::initFields(){
  const nbody::System* system_ptr = _mySim->getSystem();
  size_t nBodies = system_ptr->numBodies();
  for(size_t i = 0; i < nBodies; i++){
    std::vector<Vector3<float> > v;
    _vertices.push_back(v);
    
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    _VBOs.push_back(VBO);
  }
}

//Starts the glut main loop
void Graphics::startMainLoop(){
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glutMainLoop();
}
