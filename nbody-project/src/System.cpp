#include <nbody/constants.h>
#include <nbody/System.h>
#include <nbody/Vector3.h>

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace nbody {

  inline void System::interactBodies( size_t i, size_t j, float softFactor, Vector3f &acc ) const {
    Vector3f r = _body[j].position() - _body[i].position();
    float distance = r.norm() + softFactor;
    float invDist = 1.0f / distance;
    float invDistCubed = cube( invDist );
    acc = acc + NEWTON_G * _body[j].mass() * invDistCubed * r;
  }
  
  inline void System::interactBodies( Body i, Body j, float softFactor, Vector3f &acc ) const {
    Vector3f r = j.position() - i.position();
    float distance = r.norm() + softFactor;
    float invDist = 1.0f / distance;
    float invDistCubed = cube( invDist );
    acc = acc + NEWTON_G * j.mass() * invDistCubed * r;
  }
  
  void System::computeGravitation() {
    for( size_t i = 0; i < _nBodies; ++i ) {
      Vector3f acc{ 0.0f, 0.0f, 0.0f };
      for( size_t j = 0; j < _nBodies; ++j ) {
        if( i != j ) {
          interactBodies( i, j, _softFactor, acc );
        }
      }
      _body[i].force() = acc;
    }
  }
  
  void System::computeGravitationRK4( float dt ) {
    Vector3f *k1 = new Vector3f[_nBodies];
    Vector3f *k2 = new Vector3f[_nBodies];
    Vector3f *k3 = new Vector3f[_nBodies];
    Vector3f *k4 = new Vector3f[_nBodies];
    Vector3f zeroForce{ 0.0f, 0.0f, 0.0f };
	for( size_t i = 0; i < _nBodies; ++i ) {
	  k1[i] = Vector3f{ 0.0f, 0.0f, 0.0f };
	  for( size_t j = 0; j < _nBodies; ++j ) {
	  	if( i != j ) {
	  	  interactBodies( _body[i], _body[j], _softFactor, k1[i] );
	  	}
	  }
	}
	for( size_t i = 0; i < _nBodies; ++i ) {
	  k2[i] = Vector3f{ 0.0f, 0.0f, 0.0f };
	  for( size_t j = 0; j < _nBodies; ++j ) {
	    if( i != j ) {
	      Vector3f ivelocity = _body[i].velocity() + k1[i]*(dt/2);
	      Vector3f jvelocity = _body[j].velocity() + k1[j]*(dt/2);
	      Vector3f iposition = _body[i].position() + ivelocity*(dt/2);
	      Vector3f jposition = _body[j].position() + jvelocity*(dt/2);
	      interactBodies( Body{ iposition, ivelocity, zeroForce, _body[i].mass() },
	      				  Body{ jposition, jvelocity, zeroForce, _body[j].mass() },
	      				  _softFactor, k2[i] );
	    }
	  }
	}
	for( size_t i = 0; i < _nBodies; ++i ) {
	  k3[i] = Vector3f{ 0.0f, 0.0f, 0.0f };
	  for( size_t j = 0; j < _nBodies; ++j ) {
	    if( i != j ) {
	      Vector3f ivelocity = _body[i].velocity() + k2[i]*(dt/2);
	      Vector3f jvelocity = _body[j].velocity() + k2[j]*(dt/2);
	      Vector3f iposition = _body[i].position() + ivelocity*(dt/2);
	      Vector3f jposition = _body[j].position() + jvelocity*(dt/2);
	      interactBodies( Body{ iposition, ivelocity, zeroForce, _body[i].mass() },
	      				  Body{ jposition, jvelocity, zeroForce, _body[j].mass() },
	      				  _softFactor, k3[i] );
	    }
	  }
	}
	for( size_t i = 0; i < _nBodies; ++i ) {
	  k4[i] = Vector3f{ 0.0f, 0.0f, 0.0f };
	  for( size_t j = 0; j < _nBodies; ++j ) {
	    if( i != j ) {
	      Vector3f ivelocity = _body[i].velocity() + k3[i]*dt;
	      Vector3f jvelocity = _body[j].velocity() + k3[j]*dt;
	      Vector3f iposition = _body[i].position() + ivelocity*dt;
	      Vector3f jposition = _body[j].position() + jvelocity*dt;
	      interactBodies( Body{ iposition, ivelocity, zeroForce, _body[i].mass() },
	      				  Body{ jposition, jvelocity, zeroForce, _body[j].mass() },
	      				  _softFactor, k4[i] );
	    }
	  }
	}
	
	for( size_t i = 0; i < _nBodies; ++i ) {
	  _body[i].force() = (1.0f/6.0f)*(k1[i] + 2.0f*k2[i] + 2.0f*k3[i] + k4[i]);	}
	delete[] k1;
	delete[] k2;
	delete[] k3;
	delete[] k4;
  }


  void System::integrateSystem( float dt ) {
    Vector3f r, v, a;
    for( size_t i = 0; i < _nBodies; ++i ) {
      r = _body[i].position();
      v = _body[i].velocity();
      a = _body[i].force();

      v = v + ( a * dt );
      v = v * _dampingFactor;
      r = r + v * dt;

      _body[i].position() = r;
      _body[i].velocity() = v;
    }
  }

  void System::integrateSystemWith( Integrator myIntegrator, float dt ) {
     myIntegrator( &(this->_body), this->_nBodies, dt );
  }

  void System::update( void (*integrator_ptr)( Body**, size_t, float ), float dt ) {
    //computeGravitationRK4( dt );
    //integrateSystem( dt );
    computeGravitation();
    Integrator anIntegrator ( *integrator_ptr );
    integrateSystemWith( anIntegrator, dt );
  }

  void System::readState( std::istream &input ) {
    input >> _nBodies;
    if( _nBodies > MAX_BODIES_RECOMMENDED ) {
      throw std::runtime_error( "Too many input bodies" );
    }
    _body = new Body[_nBodies];
    for( size_t i = 0; i < _nBodies; ++i ) {
      input >> _body[i];
    }
  }

  void System::writeState( std::ostream &output ) const {
    output << _nBodies << "\n";
    for( size_t i = 0; i < _nBodies; ++i ) {
      output << _body[i] << "\n";
    }
  }

} // namespace nbody
