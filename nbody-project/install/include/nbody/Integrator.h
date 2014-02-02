#ifndef _INTEGRATOR_H
#define _INTEGRATOR_H

#include <nbody/Vector3.h>
#include <nbody/constants.h>
#include <nbody/Body.h>

// The implementation for HeunMethod seems to be correct. The result is quite close to the one using Euler.

// IB stands for interactBodies. And CG stands for computeGravitation. We should rename them some later. 

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
			static void EulerMethod( Body **body_ptr, size_t nBodies, float dt ) {
				Vector3f r, v, a;
				for( size_t i = 0; i < nBodies; ++i ) {
				r = (*body_ptr)[i].position();
				v = (*body_ptr)[i].velocity();
				a = (*body_ptr)[i].force();

				v = v + ( a * dt );
				v = v * 1.0f; // 1.0f is damping factor
				r = r + v * dt;

				(*body_ptr)[i].position() = r;
				(*body_ptr)[i].velocity() = v;
				}
			}
			
			inline static void IB( Body i, Body j, float softFactor, Vector3f &acc ) {
				Vector3f r = j.position() - i.position();
				float distance = r.norm() + softFactor;
				float invDist = 1.0f / distance;
				float invDistCubed = cube( invDist );
				acc = acc + NEWTON_G * j.mass() * invDistCubed * r;
			}

			static void CG( Body *body, size_t nBodies) {
				for( size_t i = 0; i < nBodies; ++i ) {
					Vector3f acc{ 0.0f, 0.0f, 0.0f };
					for( size_t j = 0; j < nBodies; ++j ) {
						if( i != j ) {
						IB( body[i], body[j], 1e-9f, acc ); // 1e-9f is softFactor
						}
					}
					body[i].force() = acc;
				}
		    	}
			
			
			static void HeunMethod( Body **body_ptr, size_t nBodies, const float dt ) {
				Body **body_ptr_copy = new Body*[1];
				body_ptr_copy[0] = new Body[nBodies];
				for ( size_t i = 0; i < nBodies; i++ ) {
                                                (*body_ptr_copy)[i] = Body{ (*body_ptr)[i].position(),(*body_ptr)[i].velocity(),(*body_ptr)[i].force(),(*body_ptr)[i].mass() };
						//(*body_ptr_copy)[i].position() = (*body_ptr)[i].position(); 
						//(*body_ptr_copy)[i].velocity() = (*body_ptr)[i].velocity(); 
						//(*body_ptr_copy)[i].force() = (*body_ptr)[i].force();
						//(*body_ptr_copy)[i].mass() = (*body_ptr)[i].mass();

						Vector3f r = (*body_ptr_copy)[i].position();
						Vector3f v = (*body_ptr_copy)[i].velocity();
						Vector3f a = (*body_ptr_copy)[i].force();
						
						v = v + ( a * dt );
						v = v * 1.0f; // 1.0f is dampingFactor
						r = r + v * dt;

						(*body_ptr_copy)[i].position() = r;
						(*body_ptr_copy)[i].velocity() = v;
				}
				
				CG( *body_ptr_copy, nBodies );
				
				for ( size_t i = 0; i < nBodies; i++ ) {
						Vector3f r = (*body_ptr)[i].position();
						Vector3f v = (*body_ptr)[i].velocity();
						Vector3f a = (*body_ptr)[i].force();
						
						Vector3f k_1r = v;
						Vector3f k_1v = a;
						Vector3f k_2r = (*body_ptr_copy)[i].velocity();
						Vector3f k_2v = (*body_ptr_copy)[i].force();
						
						(*body_ptr)[i].position() = r + (dt/2) * (k_1r + k_2r);
						(*body_ptr)[i].velocity() = v + (dt/2) * (k_1v + k_2v);
				}
				
				delete [] body_ptr_copy[0];
				delete [] body_ptr_copy;
			}
	};

/*
* Implementation for integrateSystemWith method
*
* void System::integrateSystemWith( Integrator myIntegrator, float dt ) {
* myIntegrator( &(this->_body), this->_nBodies, dt );
* }
*
*/

/*
* How to use this:
*
* 1. Add System::integrateSystemWith( Integrator myIntegrator, float dt ) function to System class
* 2. Add parameter to System::update( float dt ); --> System::update( void (*integrator_ptr)( Body**, size_t, float ), float dt );
* 3. Instead of integrateSystem( float dt ), call the Integrator constructor and use integrateSystemWith method
* 4. Here's an example:
*
* void System::update( void (*integrator_ptr)(Body**, size_t, float), float dt ) {
* computeGravitation();
* Integrator anIntegrator( *integrator_ptr );
* integrateSystemWith( anIntegrator, dt );
* }
*
* 5. In Simulation.cpp add the method of integration in the _system->update() function call in Simulation::evolveSystem(). Like such:
*
* _system->update( Integrator::EulerMethod, dt );
*
*/

} //namespace nbody

#endif
