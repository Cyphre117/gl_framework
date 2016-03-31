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

    camera_->setDirection( glm::vec3(0.5, 0.0, 1.0) );

	// This defaults to the identity matrix
	model_matrix_ = glm::mat4(1.0f);
    view_matrix_ = camera_->view();
    projection_matrix_ = camera_->projection();

	glUniformMatrix4fv( uniform_model_matrix_, 1, GL_FALSE, glm::value_ptr( model_matrix_ ) );
    glUniformMatrix4fv( uniform_view_matrix_, 1, GL_FALSE, glm::value_ptr( view_matrix_ ) );
    glUniformMatrix4fv( uniform_projection_matrix_, 1, GL_FALSE, glm::value_ptr( projection_matrix_ ) );
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLint position_attrib = basic_shader_.getAttribLocation( "position" );
	if( position_attrib != -1 ) {
    	glEnableVertexAttribArray( position_attrib );
		glVertexAttribPointer( position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	}

    //  Position
    GLfloat vertices[] = {
        -0.5f, 0.0f,  0.5f, // Top-left
         0.5f, 0.0f, -0.5f, // Bottom-right
         0.5f, 0.0f,  0.5f, // Top-right
        -0.5f, 0.0f,  0.5f,	// Top-left
         0.5f, 0.0f, -0.5f, // Bottom-right
        -0.5f, 0.0f, -0.5f  // Bottom-left
    };

    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

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
	text_->putString("Hello world", -1, 1, 32 );

	camera_->setDirection( glm::rotate( camera_->direction(), -0.05f * timer_->dt() * input_->xMotion(), camera_->up() ) );
	camera_->setDirection( glm::rotate( camera_->direction(), 0.05f * timer_->dt() * input_->yMotion(), glm::cross( camera_->up(), camera_->direction() ) ) );

	if( input_->isDown( SDL_SCANCODE_UP ) ) {
	camera_->setPosition( camera_->position() + camera_->direction() * ( 5.0f * timer_->dt() ) );
	}
	if( input_->isDown( SDL_SCANCODE_DOWN ) ) {
	camera_->setPosition( camera_->position() + camera_->direction() * ( -5.0f * timer_->dt() ) );
	}

	return true;
}

bool Game::graphics()
{
	window_->clear();

	basic_shader_.bind();
    view_matrix_ = camera_->view();
    projection_matrix_ = camera_->projection();
    glUniformMatrix4fv( uniform_view_matrix_, 1, GL_FALSE, glm::value_ptr( view_matrix_ ) );
    glUniformMatrix4fv( uniform_projection_matrix_, 1, GL_FALSE, glm::value_ptr( projection_matrix_ ) );

    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glDrawArrays( GL_TRIANGLES, 0, 6 );

	text_->render();

	window_->present();
	return true;
}

bool Game::physics()
{
	return true;
}