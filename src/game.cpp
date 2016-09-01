#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "game.h"
#include "input.h"
#include "time.h"
#include "camera.h"
#include "window.h"
#include "text_renderer.h"

Game::Game() :
window_(nullptr)
{}

void Game::init()
{
	// We can only set parameters when the shader is bound
    basic_shader_.loadVertexSourceFile( "vertex.vs" );
    basic_shader_.loadFragmentSourceFile( "fragment.fs" );
	basic_shader_.init();
	basic_shader_.bind();

	uniform_model_matrix_ = basic_shader_.getUniformLocation( "model" );
	uniform_view_matrix_ = basic_shader_.getUniformLocation( "view" );
	uniform_projection_matrix_ = basic_shader_.getUniformLocation( "projection" );

	// This defaults to the identity matrix
	glm::mat4 model_matrix_ = glm::mat4(1.0f);
    glm::mat4 view_matrix_ = camera_->view();
    glm::mat4 projection_matrix_ = camera_->projection();

	glUniformMatrix4fv( uniform_model_matrix_, 1, GL_FALSE, glm::value_ptr( model_matrix_ ) );
    glUniformMatrix4fv( uniform_view_matrix_, 1, GL_FALSE, glm::value_ptr( view_matrix_ ) );
    glUniformMatrix4fv( uniform_projection_matrix_, 1, GL_FALSE, glm::value_ptr( projection_matrix_ ) );
    
    quad_[0].init(
    	&basic_shader_,
        glm::vec3(-1.5f,  2.0f, -1.5f),
        glm::vec3( 1.5f,  2.0f, -1.5f),
    	glm::vec3(-1.5f, -2.0f, -1.5f),
		glm::vec3( 1.5f, -2.0f, -1.5f)
    );

    quad_[1].init(
    	&basic_shader_,
        glm::vec3(-0.5f, 0.0f,  0.5f),
        glm::vec3( 0.5f, 0.0f,  0.5f),
        glm::vec3(-0.5f, 0.0f, -0.5f),
        glm::vec3( 0.5f, 0.0f, -0.5f) 
    );

    quad_[2].init(
    	&basic_shader_,
    	glm::vec3(-1.5f, -2.0f,  1.5f),
		glm::vec3( 1.5f, -2.0f,  1.5f),
		glm::vec3(-1.5f, -2.0f, -1.5f),
		glm::vec3( 1.5f, -2.0f, -1.5f)    	
    	);

    default_ = texture_manager_.load( "default.bmp" );

    // Lock and hide the cursor
    input_->lockCursor();
}

void Game::shutdown()
{
    for( int i = 0; i < 3; ++i )
	   quad_[i].shutdown();

	input_->unlockCursor();
	basic_shader_.shutdown();
}

bool Game::frame()
{
	if( !update() )
		return false;

	if( !physics() )
		return false;

	if( !graphics() )
		return false;

	return true;
}

bool Game::update()
{
	text_->putString("Hello world", -0.95, 0.95, 32 );

	camera_->update( time_->dt() );

	return true;
}

bool Game::graphics()
{
	window_->clear();

	basic_shader_.bind();
    glm::mat4 view_matrix_ = camera_->view();
    glm::mat4 projection_matrix_ = camera_->projection();
    glUniformMatrix4fv( uniform_view_matrix_, 1, GL_FALSE, glm::value_ptr( view_matrix_ ) );
    glUniformMatrix4fv( uniform_projection_matrix_, 1, GL_FALSE, glm::value_ptr( projection_matrix_ ) );

    quad_[0].bind();
    default_.bind( 0 );
    quad_[0].draw();

    quad_[1].bind();
    default_.bind( 0 );
    quad_[1].draw();

	quad_[2].bind();
    default_.bind( 0 );
	quad_[2].draw();
	
	text_->render();

	window_->present();
	return true;
}

bool Game::physics()
{
    // Apply gravity
    camera_->addVelocity( glm::vec3( 0.0f, -0.5f, 0.0f ) );

    // If the camera is below the XZ plane
    if( camera_->position().y <= 0.0f )
    {
        glm::vec3 pos = camera_->position();
        glm::vec3 vel = camera_->velocity();

        // Stop the camera from falling
        camera_->setVelocity( glm::vec3( vel.x, 0.0f, vel.z ) );
        // Set the positon back onto the plane
        camera_->setPosition( glm::vec3( pos.x, 0.0f, pos.z ) );

        // TODO: fix shitty friction
        //camera_->setVelocity( camera_->velocity() * 0.9f );
    }

	return true;
}