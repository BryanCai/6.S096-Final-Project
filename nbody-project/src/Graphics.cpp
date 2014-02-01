#include <nbody/Graphics.h>

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <GL/glut.h>

#include <glm/glm.hpp>

namespace nbody {
  Graphics::Graphics () : _vertices{}, _VBOs{} {
  }

  void Graphics::displayFunc(){
    glClear(GL_COLOR_BUFFER_BIT);
    
  }

  //Initializes GLUT and creates a window
  void Graphics::init (int* argc, char** argv) {
    std::cout << "Initializing GLUT\n";
    glutInit(argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(50,50);
    glutCreateWindow("N-body Problem");
    
    //Callbacks
    glutDisplayFunc(displayFunc);

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        std::cout << "Unable to initialize GLEW" << std::endl;
        std::cout << glewGetErrorString(res) << std::endl;
        std::exit(1);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutMainLoop();
 }

} //namespace nbody
