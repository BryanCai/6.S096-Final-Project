#ifndef _INTEGRATOR_H
#define _INTEGRATOR_H

include <nbody/Vector3.h>
include <nbody/System.h>

typedef void (*integrator_ptr)( System, float );

// A functor specifically for integrators. System class should friend Integrator to give Integrator access to the bodies. 
class Integrator {
	private:
		integrator_ptr _integrator;
	
	public:
		// Sets the integrator to anIntegrator. I don't use this here, but maybe it'll be useful. 
		void setIntegrator( integrator_ptr anIntegrator ) { _integrator = anIntegrator; }
		
		Integrator(integrator_ptr anIntegrator) : _integrator( anIntegrator ) { }
		
		void operator() ( System &system, const float &dt ) {
			_integrator(system, dt); // maybe (*_integrator)(system, dt) or maybe both works 
		}
		
		static void EulerMethod( System system, const float dt ) { };
		static void HeunMethod( System system, const float dt ) { };
		static void RK4( Sustem system, const float dt ) { };
};

// Example of calling integrateSystemWith (first call the constructor for Integrator):
// Integrator anIntegrator( Integrator::EulerMethod ); 
// System::integrateSystemWith( anIntegrator, dt ); 
void System::integrateSystemWith( Integrator myIntegrator, const float &dt ) {
	myIntegrator( *this, dt ); // Or perhaps myIntegrator( System, dt ); ? 
}

//hopefully the following integrators are implemented correctly. Will look over them some other time. 
static void Integrator::EulerMethod( System system, const float dt ) {
	Vector3f r, v, a;
    for( size_t i = 0; i < _nBodies; ++i ) {
      r = system._body[i].position();
      v = system._body[i].velocity();
      a = system._body[i].force();

      v = v + ( a * dt );
      v = v * _dampingFactor;
      r = r + v * dt;

      system._body[i].position() = r;
      system._body[i].velocity() = v;
    }
}

static void Integrator::HeunMethod( System system, const float dt ) {
	Body *b = new Body[_nBodies];
	for ( size_t i = 0; i < _nBodies; i++ ) {
		b[i] = Body{ system._body[i].position(), system._body[i].velocity(), _body[i].force(), _body[i].mass() }; 

		r = b[i].position();
		v = b[i].velocity();
		a = b[i].force();
		
		v = v + ( a * dt );
		v = v * _dampingFactor;
		r = r + v * dt;

		b[i].position() = r;
		b[i].velocity() = v;
	}
	
	for ( size_t i = 0; i < _nBodies; i++ ) {
		Vector3f acc{ 0.0f, 0.0f, 0.0f };
		for (size_t j = 0; j < _nBodies; j++ ) {
			if ( i != j ) {
				system.interactBodies( b[i], b[j], _softFactor, acc );
			}
		}
		b[i].force() = acc;
	}
	
	for ( size_t i = 0; i < _nBodies; i++ ) {
		Vector3f r = system._body[i].position();
		Vector3f v = system._body[i].velocity();
		Vector3f a = system._body[i].force();
		
		Vector3f k_1r = v
		Vector3f k_1v = a
		Vector3f k_2r = b[i].velocity;
		Vector3f k_2v = b[i].force();
		
		r = r + (dt/2) * (k_1r + k_2r);
		v = v + (dt/2) * (k_1v + k_2v); 
	}
	
	delete[] b;
}

#endif
