#ifndef _NBODY_GRAPHICS_H
#define _NBODY_GRAPIHCS_H

#define WIDTH 1024
#define HEIGHT 768

#include <nbody/Vector3.h>

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

namespace nbody {
  class Graphics {
    std::vector<std::vector<Vector3<float> > > _vertices;
    std::vector<GLuint> _VBOs;
    static void displayFunc();
    public:
      Graphics ();
      void init (int* argc, char** argv);
  };
}
#endif
