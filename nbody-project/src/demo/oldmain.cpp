#include "GlutWrapper.h"
#include "Shaders.h"

#include <glload/gl_3_0.h>
#include <glload/gll.hpp>
#include <GL/freeglut.h>

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cmath>

namespace nBodyShaders {
  const std::string vertex1(
    "#version 130\n"
    "in vec4 position;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = position;\n"
    "}\n"
  );

  const std::string fragment1(
    "#version 130\n"
    "out vec4 outputColor;\n"
    "void main()\n"
    "{\n"
    "   outputColor = vec4(1.0f,1.0f,1.0f,1.0f);"
    "}\n"
  );
} // namespace shaders

class NBodyWindow : public GlutWrapper {
  float _elapsedTime;
public:
	NBodyWindow( const std::string &title, 
               Mode debugMode = Mode::NDEBUG );
  ~NBodyWindow();

  void updateElapsedTime();
  void updateBuffer();
  void drawBuffer();

  void display();
  void reshape( int theWidth, int theHeight );
  void keyboard( unsigned char key, int x, int y );
};

NBodyWindow::NBodyWindow( const std::string &title, Mode debugMode ) : 
  GlutWrapper{ title, debugMode }, _elapsedTime{0.0f} {
	_instance = this; 
}

NBodyWindow::~NBodyWindow() {}

void NBodyWindow::reshape( int theWidth, int theHeight ) {
	glViewport( 0, 0, (GLsizei) theWidth, (GLsizei) theHeight );
}

void NBodyWindow::keyboard( unsigned char key, int /*x*/, int /*y*/ ) {
	const char ESCAPE_KEY = 27;
  if( key == ESCAPE_KEY ) {
    glutLeaveMainLoop();
  }
}

void NBodyWindow::updateElapsedTime() {
  // glutGet returns time in milliseconds
  _elapsedTime = glutGet( GLUT_ELAPSED_TIME ) / 1000.0f;
}

void NBodyWindow::updateBuffer() {
  updateElapsedTime();
  size_t nVertices = _bufSize / 4;
	for( size_t i = 0; i < nVertices; ++i ) {
    float xFraction = float( i + 2 * _elapsedTime ) / float( nVertices - 1 );
    float yFraction = float( i - 2 * _elapsedTime ) / float( nVertices - 1 );
		_buf[4*i] = cosf( 2 * 3.1415f * xFraction  );
		_buf[4*i+1] = sinf( 2 * 3.1415f * yFraction );
		_buf[4*i+2] = 0.0f; // ignore z-coordinate
		_buf[4*i+3] = 1.0f; // ignore extra
  }

	glBindBuffer( GL_ARRAY_BUFFER, _positionBufferObject );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( float ) * _bufSize, _buf );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void NBodyWindow::drawBuffer() {
  glUseProgram( _program );
  glBindBuffer( GL_ARRAY_BUFFER, _positionBufferObject );
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, 0 );

  glDrawArrays( GL_QUADS, 0, (GLsizei) _bufSize );

  glDisableVertexAttribArray( 0 );
  glUseProgram( 0 );
}

void NBodyWindow::display() {
  updateBuffer();

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  drawBuffer();

  glutSwapBuffers();
	glutPostRedisplay();
}

int main( int argc, char **argv ) {
  try {
		size_t N = 13;
		size_t bufSize = 4 * N;
		float *buf = new float[bufSize];

		for( size_t i = 0; i < N; ++i ) {
			buf[4*i] = cosf( 2 * 3.1415f * float( i ) / float( N - 1 ) );
			buf[4*i+1] = sinf( 2 * 3.1415f * float( i ) / float( N - 1 ) );
			buf[4*i+2] = 0.0f;
			buf[4*i+3] = 1.0f;
			std::cout << buf[4*i] << " " << buf[4*i+1];
      std::cout << " " << buf[4*i+2] << " " << buf[4*i+3] << "\n";
    }

		Shaders shaders;
		shaders.addToVertexList( nBodyShaders::vertex1 );
		shaders.addToFragmentList( nBodyShaders::fragment1 );
    NBodyWindow window{ "N-Body Simulation", GlutWrapper::NDEBUG };
		window.init( argc, argv, 500, 500, &shaders, bufSize, buf );
		window.run();

		delete [] buf;
    return 0;
  } catch( const std::exception &e ) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
