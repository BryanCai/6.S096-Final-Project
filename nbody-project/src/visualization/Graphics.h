#ifndef GRAPHICS_H
#define GRAPIHCS_H

#define WIDTH 1024
#define HEIGHT 768
#define SCALE 1000

#include <nbody/Vector3.h>
#include <nbody/Simulation.h>

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

class Graphics {

  static void displayFunc();
  static void keyboardFunc(unsigned char key, int x, int y);

  public:
    static nbody::Simulation *_mySim;
    static std::vector<std::vector<Vector3<float> > > _vertices;

    Graphics ();
    Graphics (std::ifstream& input);
    void init (int* argc, char** argv);
    void initFields ();
    void startMainLoop();
    static void updateData();
};

#endif
