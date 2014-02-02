#include <nbody/System.h>
#include <gtest/gtest.h>


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
		
