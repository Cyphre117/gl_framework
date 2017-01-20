#include "game.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <system/game_time.h>
#include <system/camera.h>
#include <system/window.h>
#include <system/input_manager.h>
#include <graphics/text_renderer.h>
#include <graphics/texture_manager.h>
#include <physics/physics_world.h>
#include <physics/physics_debug_renderer.h>
#include <system/window.h>

Game::Game()
: texture_manager_(nullptr)
, input_manager_(nullptr)
, audio_manager_(nullptr)
, physics_world_(nullptr)
, window_(nullptr)
, player_(nullptr)
, time_(nullptr)
, sphere_1_(nullptr)
{}

bool Game::init()
{
    bool success = true;

    texture_manager_ = TextureManager::get();
    input_manager_ = InputManager::get();
    audio_manager_ = AudioManager::get();
    physics_world_ = PhysicsWorld::get();
    window_ = Window::get();
    player_ = Player::get();

    // Do this to set the audio listener position to the camera
    // position from the moment the app is run
    player_->update(0);

	// We can only set parameters when the shader is bound
    basic_shader_.loadVertexSourceFile( "vertex.vs" );
    basic_shader_.loadFragmentSourceFile( "fragment.fs" );
	if( !basic_shader_.init() ) success = false;

	basic_shader_.bind();
	basic_model_matrix_uniform_ = basic_shader_.getUniformLocation( "model" );
	basic_view_matrix_uniform_ = basic_shader_.getUniformLocation( "view" );
	basic_projection_matrix_uniform_ = basic_shader_.getUniformLocation( "projection" );

    debug_lines_shader_.loadVertexSourceFile( "debug_lines.vs" );
    debug_lines_shader_.loadFragmentSourceFile( "debug_lines.fs" );
    if( !debug_lines_shader_.init() ) success = false;

    debug_lines_shader_.bind();
    debug_view_matrix_uniform_ = debug_lines_shader_.getUniformLocation( "view" );
    debug_projection_matrix_uniform_ = debug_lines_shader_.getUniformLocation( "projection" );

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

    bunny_ = texture_manager_->load( "bunny.png" );

    // Register the physics debug drawer
    PhysicsDebugRenderer* debug_renderer = new PhysicsDebugRenderer();
    debug_renderer->init( &debug_lines_shader_ );
    physics_world_->setDebugDrawer( debug_renderer );

    audio_buffer_ = audio_manager_->load( "powerup.wav" );
    audio_source_ = audio_manager_->newSource();
    audio_source_.setLooping( true );
    audio_source_.play( audio_buffer_ );

    // Lock and hide the cursor
    input_manager_->lockCursor();

    physics_world_->addRigidBody( glm::vec3(0, 0, 0), 0.0, new btStaticPlaneShape(btVector3(0, 1, 0), 0) );
    physics_world_->addRigidBody( glm::vec3(2, 5, 0), 1, new btConeShape(0.4, 1) );
    physics_world_->addRigidBody( glm::vec3(2, 4, 2), 1, new btBoxShape(btVector3(0.5, 0.2, 0.8)) );
    physics_world_->addRigidBody( glm::vec3(0, 3, -2), 1, new btCylinderShape(btVector3(0.3, 1, 0.3)) );

    sphere_1_ = physics_world_->addRigidBody( glm::vec3(0, 10, 0), 1, new btSphereShape(0.5) );

    return success;
}

void Game::shutdown()
{
    for( int i = 0; i < 3; ++i )
	   quad_[i].shutdown();

    texture_manager_->unload( &bunny_ );
    audio_manager_->unload( &audio_buffer_ );
    audio_manager_->deleteSource( &audio_source_ );
	input_manager_->unlockCursor();

	basic_shader_.shutdown();
}

bool Game::frame()
{
	if( !update() )
		return false;

    physics_world_->update( time_->dt() );

	if( !graphics() )
		return false;

	return true;
}

bool Game::update()
{
	text_->putString("Hello world", -0.95, 0.95, 32 );

	player_->update( time_->dt() );

	return true;
}

bool Game::graphics()
{
	window_->clear();
    glm::mat4 model_matrix_ = glm::mat4(1.0f);
    glm::mat4 view_matrix_ = player_->camera()->view();
    glm::mat4 projection_matrix_ = player_->camera()->projection();

	basic_shader_.bind();
    glUniformMatrix4fv( basic_model_matrix_uniform_, 1, GL_FALSE, glm::value_ptr( model_matrix_ ) );
    glUniformMatrix4fv( basic_view_matrix_uniform_, 1, GL_FALSE, glm::value_ptr( view_matrix_ ) );
    glUniformMatrix4fv( basic_projection_matrix_uniform_, 1, GL_FALSE, glm::value_ptr( projection_matrix_ ) );

    quad_[0].bind();
    bunny_.bind();
    quad_[0].draw();

    quad_[2].bind();
    bunny_.bind();
    quad_[2].draw();

    float matrix[16];
    btTransform t;
    sphere_1_->getMotionState()->getWorldTransform( t );
    t.getOpenGLMatrix( matrix );
    audio_source_.setPosition( t.getOrigin().x(), t.getOrigin().y(), t.getOrigin().z() );
    glUniformMatrix4fv( basic_model_matrix_uniform_, 1, GL_FALSE, matrix );

    //player_->camera()->setLookAt( glm::vec3(t.getOrigin().x(), t.getOrigin().y(), t.getOrigin().z()) );

    quad_[1].bind();
    bunny_.bind();
    quad_[1].draw();

    debug_lines_shader_.bind();
    glUniformMatrix4fv( debug_view_matrix_uniform_, 1, GL_FALSE, glm::value_ptr( view_matrix_ ) );
    glUniformMatrix4fv( debug_projection_matrix_uniform_, 1, GL_FALSE, glm::value_ptr( projection_matrix_ ) );
    physics_world_->world()->debugDrawWorld();
	
	text_->render();

	window_->present();
	return true;
}