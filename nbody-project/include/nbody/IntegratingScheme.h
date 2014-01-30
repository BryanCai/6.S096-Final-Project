#include <nbody/Vector3.h>
#include <nbody/Body.h>
#include <nbody/constants.h>
#include <vector>

// BASIC SUMMARY OF COMMENTS:
// I HAVEN'T BEEN ABLE TO MAKE THIS WORK!!!

// Based on IndexingScheme class in Lecture 6 notes.

// I believe this is the preferred way of doing this.
// I tried tweaking the original file to make this work, but can't. 
// Basically, we would've needed to make the System class a templated
// class and also make Simulation class a templated class.
// Then it would be easy to swap the Integrator by just changing one
// word in the main method in main.cpp.
//
// I upload this anyway in case someone figures how to properly use templates. But if we don't, just use the Integrator class.
// 

namespace nbody {

	class IntegratingScheme {
		public:
			virtual void integrate( std::vector<Body> body, const float dt );
			IntegratingScheme() {};
			virtual ~IntegratingScheme() {};
	};

	
	class Euler : public IntegratingScheme {
		public:
			Euler() { }
			void integrate( std::vector<Body> body, const float &dt ) {
				// some implementation
				Vector3f r, v, a;
				for( size_t i = 0; i < body.size(); ++i ) {
				  r = body[i].position();
				  v = body[i].velocity();
				  a = body[i].force();

				  v = v + ( a * dt );
				  v = v * 1.0f;
				  r = r + v * dt;

				  body[i].position() = r;
				  body[i].velocity() = v;
				}
			}
	};
	
	// class RK4 : public IntegratingScheme {
		// some stuff
	// };
	
	// class SomeIntegratorName : public IntegratingScheme
		// some stuff
	// };
	
} // namespace nbody
