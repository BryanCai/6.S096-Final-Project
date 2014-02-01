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

Graphics::Graphics () {
  _mySim = new nbody::Simulation{};
}

Graphics::Graphics (std::ifstream& input) {
  _mySim = new nbody::Simulation{ input };
}

size_t count = 0;

void Graphics::displayFunc() {
  //Evolve the simulation
  std::cout << "--- EVOLUTION " << ++count << " ---" << std::endl;
  _mySim->evolveSystem( 1e4, 0.000001 );

  //Update the data
  Graphics::updateData();

  //Draw the stuffs!
  glClear( GL_COLOR_BUFFER_BIT );

  glBegin( GL_POINTS );
  for( size_t i = 0; i < _vertices.size(); i++ ){
    glColor3f( (i + 0.0f) / _vertices.size(), 1.0f - (i + 0.0) / _vertices.size(), 0.5f + 0.5f * (i + 0.0) / _vertices.size() );
    for( size_t v = 0; v < _vertices[i].size() - 1; v++ ){
      glVertex2f( _vertices[i][v].x(), _vertices[i][v].y() );
    }
  }
  glEnd();
  glPointSize(4.0f);
  glBegin( GL_POINTS );
  for( size_t i = 0; i < _vertices.size(); i++ ){
    glColor3f( (i + 0.0f) / _vertices.size(), 1.0f - (i + 0.0) / _vertices.size(), 0.5f + 0.5f * (i + 0.0) / _vertices.size() );
    glVertex2f( _vertices[i][_vertices[i].size() - 1].x(), _vertices[i][_vertices[i].size() - 1].y() );
  }
  glEnd();
  glPointSize(2.0f);
  glFinish();
  //Swap buffers and display
  glutSwapBuffers();
}

void Graphics::updateData() {
  for(size_t i = 0; i < _mySim->getSystem()->numBodies(); i++){
    Vector3<float> pos = _mySim->getSystem()->getBody(i).position();
    //std::cout << "Original position for body " << i << ": " << pos << std::endl;
    Vector3<float> add = {pos.x() / WIDTH * SCALE, pos.y() / HEIGHT * SCALE, pos.z()};
    //std::cout << "Scaled down position: " << add << std::endl;
    _vertices[i].push_back(add);
  }
}

void Graphics::keyboardFunc(unsigned char key, int x, int y) {
  glutPostRedisplay();
  key = key;
  x = x;
  y = y;
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
  glutKeyboardFunc(keyboardFunc);
  glPointSize(2.0f);
}

void Graphics::initFields(){
  const nbody::System* system_ptr = _mySim->getSystem();
  size_t nBodies = system_ptr->numBodies();
  for(size_t i = 0; i < nBodies; i++){
    std::vector<Vector3<float> > v;
    _vertices.push_back(v);
  }
}

//Starts the glut main loop
void Graphics::startMainLoop(){
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glutMainLoop();
}
