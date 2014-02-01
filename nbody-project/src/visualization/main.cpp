#include <iostream>
#include <fstream>

#include "Graphics.h"

#include <GL/glew.h>
#include <GL/glfw.h>

int main(int argc, char** argv){
  std::ifstream input{ "resources/nbody/binary-system-simple.txt" };
  Graphics g { input };

  //Initialize graphics related things (GLUT, GLEW, callbacks)
  g.init(&argc, argv);
  
  g.initFields();
    
  g.startMainLoop();

}
