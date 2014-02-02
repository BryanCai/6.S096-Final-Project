#include <nbody/System.h>
#include <gtest/gtest.h>

// To use this, go to nbody-project/make/nbody-test.mk and add
// in the line after


// Changed to non-trivial test.
 
void makeAssertions( nbody::Body **body_ptr, size_t i,
						float assert_pos_x, float assert_pos_y, float assert_pos_z,
						float assert_vel_x, float assert_vel_y, float assert_vel_z,
						float assert_force_x, float assert_force_y, float assert_force_z ) {
	ASSERT_FLOAT_EQ( (*body_ptr)[i].position().x(), assert_pos_x );
	ASSERT_FLOAT_EQ( (*body_ptr)[i].position().y(), assert_pos_y );
	ASSERT_FLOAT_EQ( (*body_ptr)[i].position().z(), assert_pos_z );
	ASSERT_FLOAT_EQ( (*body_ptr)[i].velocity().x(), assert_vel_x );
	ASSERT_FLOAT_EQ( (*body_ptr)[i].velocity().y(), assert_vel_y );
	ASSERT_FLOAT_EQ( (*body_ptr)[i].velocity().z(), assert_vel_z );
	ASSERT_FLOAT_EQ( (*body_ptr)[i].force().x(), assert_force_x );
	ASSERT_FLOAT_EQ( (*body_ptr)[i].force().y(), assert_force_y );
	ASSERT_FLOAT_EQ( (*body_ptr)[i].force().z(), assert_force_z );	
						
}

// Does tests for time/steps = 1, 2. time/step = 0 is given. 
void oneBodyTest( void (*integrator_ptr)( nbody::Body**, size_t, float ), float dt,
					Vector3f pos_vec3f, Vector3f vel_vec3f, Vector3f force_vec3f,
					float assert1_pos_x, float assert1_pos_y, float assert1_pos_z,
					float assert1_vel_x, float assert1_vel_y, float assert1_vel_z,
					float assert1_force_x, float assert1_force_y, float assert1_force_z,
					float assert2_pos_x, float assert2_pos_y, float assert2_pos_z,
					float assert2_vel_x, float assert2_vel_y, float assert2_vel_z,
					float assert2_force_x, float assert2_force_y, float assert2_force_z ) {
	
	nbody::Body **body_ptr = new nbody::Body*[1];
	body_ptr[0] = new nbody::Body[1];
	
	(*body_ptr)[0].position() = pos_vec3f;
	(*body_ptr)[0].velocity() = vel_vec3f;
	(*body_ptr)[0].force() = force_vec3f;
	
	nbody::System sys( *body_ptr, 1 ); 
	sys.update( *integrator_ptr, dt );
	
	makeAssertions( body_ptr, 0,
					assert1_pos_x, assert1_pos_y, assert1_pos_z,
					assert1_vel_x, assert1_vel_y, assert1_vel_z,
					assert1_force_x, assert1_force_y, assert1_force_z );
					
	sys.update( *integrator_ptr, dt );
	
	makeAssertions( body_ptr, 0,
					assert2_pos_x, assert2_pos_y, assert2_pos_z,
					assert2_vel_x, assert2_vel_y, assert2_vel_z,
					assert2_force_x, assert2_force_y, assert2_force_z );
}

TEST( integratorTests, EulerMethod ) {
	oneBodyTest( nbody::Integrator::EulerMethod, 0.1, 
					Vector3f{ 1.0f, 1.0f, 1.0f }, Vector3f{ 1.0f, 2.0f, 3.0f }, Vector3f{ 0.0f, 0.0f, 0.0f },
					1.1f, 1.2f, 1.3f,
					1.0f, 2.0f, 3.0f, 
					0.0f, 0.0f, 0.0f,
					1.2f, 1.4f, 1.6f,
					1.0f, 2.0f, 3.0f,
					0.0f, 0.0f, 0.0f );
}

// I can't do this without 0 acceleration since computeGravitation() assumes that the initial acceleration is 0. I guess we're supposed to assume that
// the bodies start off with no acceleration.
TEST( integratorTests, HeunMethod ) {
	oneBodyTest ( nbody::Integrator::HeunMethod, 0.1, 
					Vector3f{ 1.0f, 1.0f, 1.0f }, Vector3f{ 1.0f, 2.0f, 3.0f }, Vector3f{ 0.0f, 0.0f, 0.0f },
					1.1f, 1.2f, 1.3f,
					1.0f, 2.0f, 3.0f,
					0.0f, 0.0f, 0.0f,
					1.2f, 1.4f, 1.6f,
					1.0f, 2.0f, 3.0f,
					0.0f, 0.0f, 0.0f );
}

// Does test for time/steps = 1, 2. time/step = 0 is given.
// Can't get this to work. Might just not do this. 
/*void twoBodyTest( void (*integrator_ptr)( nbody::Body**, size_t, float ), float dt,
					Vector3f pos1_vec3f, Vector3f vel1_vec3f, Vector3f force1_vec3f,
					float assert1_pos1_x, float assert1_pos1_y, float assert1_pos1_z,
					float assert1_vel1_x, float assert1_vel1_y, float assert1_vel1_z,
					float assert1_force1_x, float assert1_force1_y, float assert1_force1_z,
					float assert2_pos1_x, float assert2_pos1_y, float assert2_pos1_z,
					float assert2_vel1_x, float assert2_vel1_y, float assert2_vel1_z,
					float assert2_force1_x, float assert2_force1_y, float assert2_force1_z,
					Vector3f pos2_vec3f, Vector3f vel2_vec3f, Vector3f force2_vec3f,
					float assert1_pos2_x, float assert1_pos2_y, float assert1_pos2_z,
					float assert1_vel2_x, float assert1_vel2_y, float assert1_vel2_z,
					float assert1_force2_x, float assert1_force2_y, float assert1_force2_z,
					float assert2_pos2_x, float assert2_pos2_y, float assert2_pos2_z,
					float assert2_vel2_x, float assert2_vel2_y, float assert2_vel2_z,
					float assert2_force2_x, float assert2_force2_y, float assert2_force2_z ) {
	nbody::Body **body_ptr = new nbody::Body*[1];
	*body_ptr = new nbody::Body[2];
	(*body_ptr)[0].position() = pos1_vec3f;
	(*body_ptr)[0].velocity() = vel1_vec3f;
	(*body_ptr)[0].force() = force1_vec3f;
	
	(*body_ptr)[1].position() = pos2_vec3f;
	(*body_ptr)[1].velocity() = vel2_vec3f;
	(*body_ptr)[1].force() = force2_vec3f;
	
	nbody::System sys( *body_ptr, 2 );
	sys.update( *integrator_ptr, dt );
	
	makeAssertions( body_ptr, 0,
					assert1_pos1_x, assert1_pos1_y, assert1_pos1_z,
					assert1_vel1_x, assert1_vel1_y, assert1_vel1_z,
					assert1_force1_x, assert1_force1_y, assert1_force1_z );
	makeAssertions( body_ptr, 0,
					assert1_pos2_x, assert1_pos2_y, assert1_pos2_z,
					assert1_vel2_x, assert1_vel2_y, assert1_vel2_z,
					assert1_force2_x, assert1_force2_y, assert1_force2_z );
	
	sys.update( *integrator_ptr, dt );

	makeAssertions( body_ptr, 1,
					assert2_pos1_x, assert2_pos1_y, assert2_pos1_z,
					assert2_vel1_x, assert2_vel1_y, assert2_vel1_z,
					assert2_force1_x, assert2_force1_y, assert2_force1_z );
	makeAssertions( body_ptr, 1,
					assert2_pos2_x, assert2_pos2_y, assert2_pos2_z,
					assert2_vel2_x, assert2_vel2_y, assert2_vel2_z,
					assert2_force2_x, assert2_force2_y, assert2_force2_z );
					
	delete [] *body_ptr;
	delete [] body_ptr;
}

TEST( integratorTests, EulerTwoBody ) {
	twoBodyTest( nbody::Integrator::EulerMethod, 0.1, 
					Vector3f{ 0.0f, 0.0f, 0.0f }, Vector3f{ 1.0f, 2.0f, 3.0f }, Vector3f{ 0.0f, 0.0f, 0.0f },
					0.1003704f, 0.2007407f, 0.3007407f,
					1.0037037f, 2.0074074f, 3.0074074f,
					0.0370370f, 0.0740740f, 0.0740740f,
					0.2013575f, 0.4025093f, 0.6024065f,
					1.0098715f, 2.0176858f, 3.0166572f,
					0.0616782f, 0.1027844f, 0.0924983f,
					Vector3f{ 1.0f, 2.0f, 2.0f }, Vector3f{ 3.0f, 2.0f, 1.0f, }, Vector3f{ 0.0f, 0.0f, 0.0f },
					1.2988889f, 2.1977778f, 2.0977778f,
					2.9962963f, 1.9925926f, 0.9925926f,
					-0.0037037f, -0.0074074f, -0.0074074f,
					1.5986424f, 2.3974907f, 2.1975935f,
					2.9901285f, 1.9823142f, 0.9833428f,
					-0.0616782f, -0.1027844f, -0.0924983f );
}*/
		
