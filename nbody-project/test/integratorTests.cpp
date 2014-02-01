#include <nbody/Integrator.h>
#include <gtest/gtest.h>


// To use this, go to nbody-project/make/nbody-test.mk and add
// $(BUILD_DIR_PROJ_TEST)/integratorTests.o \
// in the line after
// $(BUILD_DIR_PROJ_TEST)/vectorTest.o \
// EDIT: YOU NEED TO DELETE THE COMMENTS RELATED TO THE MAKE FILE. 
// For some reason it messes with the actual make file.


// Changed to a less trivial test. 

TEST( integratorTests, EulerMethod ) {
	nbody::Body **body_ptr = new nbody::Body*[1];
	body_ptr[0] = new nbody::Body[1];
	(*body_ptr[0]).position() = Vector3f { 1.0f, 1.0f, 1.0f };
	(*body_ptr[0]).velocity() = Vector3f { 1.0f, 2.0f, 3.0f };
	(*body_ptr[0]).force() = Vector3f { 1.0f, 1.0f, 1.0f };
	nbody::Integrator::EulerMethod( body_ptr, 1, 0.1f );

	ASSERT_FLOAT_EQ( (*body_ptr[0]).position().x(), 1.11f );
	ASSERT_FLOAT_EQ( (*body_ptr[0]).position().y(), 1.21f );
	ASSERT_FLOAT_EQ( (*body_ptr[0]).position().z(), 1.31f );
	ASSERT_FLOAT_EQ( (*body_ptr[0]).velocity().x(), 1.1f );
	ASSERT_FLOAT_EQ( (*body_ptr[0]).velocity().y(), 2.1f );
	ASSERT_FLOAT_EQ( (*body_ptr[0]).velocity().z(), 3.1f );
	ASSERT_FLOAT_EQ( (*body_ptr[0]).force().x(), 1.0f );
	ASSERT_FLOAT_EQ( (*body_ptr[0]).force().y(), 1.0f );
	ASSERT_FLOAT_EQ( (*body_ptr[0]).force().z(), 1.0f );

	delete [] body_ptr[0];
	delete [] body_ptr;
}
