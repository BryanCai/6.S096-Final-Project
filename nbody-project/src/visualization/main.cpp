#include <iostream>

#include "Graphics.h"

#include <GL/glew.h>
#include <GL/glfw.h>

int main(int argc, char** argv){
  nbody::Graphics g {};

  //Initialize GLUT
  g.init(&argc, argv);

}
