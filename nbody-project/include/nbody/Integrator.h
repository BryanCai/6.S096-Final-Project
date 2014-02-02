#ifndef _INTEGRATOR_H
#define _INTEGRATOR_H

#include <nbody/Vector3.h>
#include <nbody/constants.h>
#include <nbody/Body.h>

// The implementation for HeunMethod seems to be correct. The result is quite close to the one using Euler.

namespace nbody {

// A functor specifically for integrators.
	class Integrator {
	typedef void (* integrator_ptr)( Body**, size_t, float );
	private:
		integrator_ptr _integrator;
	
	public:
		// Sets the integrator to anIntegrator. I don't use this here, but maybe it'll be useful.
		void setIntegrator( integrator_ptr anIntegrator ) { _integrator = anIntegrator; }
			
		Integrator(integrator_ptr anIntegrator) : _integrator( anIntegrator ) { }
				
		void operator() ( Body **body_ptr, size_t nBodies, float dt ) {
			(*_integrator)( body_ptr, nBodies, dt);
		}
			
		// body_ptr is a pointer to body (like _body used in System class), which is a Body array
		static void EulerMethod( Body **body_ptr, size_t nBodies, float dt );
			
		static void integrateBodies( Body i, Body j, float softFactor, Vector3f &acc );

		static void calculateGravitation( Body *body, size_t nBodies);
			
		static void HeunMethod( Body **body_ptr, size_t nBodies, const float dt );
	};

} //namespace nbody

#endif
