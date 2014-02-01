#ifndef GRAPHICS_H
#define GRAPIHCS_H

#define WIDTH 1024
#define HEIGHT 768

#include <nbody/Vector3.h>
#include <nbody/Simulation.h>

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

class Graphics {

  nbody::Simulation _mySim;
  std::vector<std::vector<Vector3<float> > > _vertices;
  std::vector<GLuint> _VBOs;
  
  static void displayFunc();

  public:
    Graphics ();
    Graphics (std::ifstream& input);
    void init (int* argc, char** argv);
};

#endif
