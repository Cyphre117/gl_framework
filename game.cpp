#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "game.h"
#include "input.h"
#include "timer.h"
#include "camera.h"
#include "window.h"
#include "text_renderer.h"

Game::Game() :
window_(nullptr),
basic_shader_( "shaders/vertex.vs", "shaders/fragment.fs" )
{	
}

void Game::init()
{
	// We can only set parameters when the shader is bound
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

    // Lock and hide the cursor
    input_->lockCursor();
}

void Game::shutdown()
{
	input_->unlockCursor();
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

	camera_->update( timer_->dt() );

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
    quad_[0].draw();

    quad_[1].bind();
    quad_[1].draw();

	quad_[2].bind();
	quad_[2].draw();


	text_->render();

	window_->present();
	return true;
}

bool Game::physics()
{
	return true;
}