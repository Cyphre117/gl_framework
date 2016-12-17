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

	uniform_model_matrix_ = basic_shader_.getUniformLocation( "model" );
	uniform_view_matrix_ = basic_shader_.getUniformLocation( "view" );
	uniform_projection_matrix_ = basic_shader_.getUniformLocation( "projection" );

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

    audio_buffer_ = audio_manager_->load( "powerup.wav" );
    audio_source_ = audio_manager_->newSource();
    audio_source_.setLooping( true );
    audio_source_.play( audio_buffer_ );

    // Lock and hide the cursor
    input_manager_->lockCursor();

    //*
    {
        // Setup some basic physics things for testing
        btTransform t;
        t.setIdentity();
        t.setOrigin( btVector3(0,0,0) );
        btStaticPlaneShape* plane = new btStaticPlaneShape( btVector3( 0, 1, 0 ), 0 );
        btMotionState* motion = new btDefaultMotionState( t );
        btRigidBody::btRigidBodyConstructionInfo info( 0.0, motion, plane );
        btRigidBody* ground = new btRigidBody( info );
        physics_world_->world()->addRigidBody( ground );
    }//*/

    //*
    {
        float radius = 0.5;
        float x = 0.0f, y = 10.0f, z = 0.0f;
        float mass = 0.5f;

        btSphereShape* sphere = new btSphereShape( radius );
        btVector3 inertia( 0, 0, 0 );           // Inertia is 0 for static object
        sphere->calculateLocalInertia( mass, inertia );

        btTransform t;                          // Stores position and rotation
        t.setIdentity();
        t.setOrigin( btVector3( x, y, z ) );    // Set position
        btMotionState* motion = new btDefaultMotionState( t );
        btRigidBody::btRigidBodyConstructionInfo info( mass, motion, sphere, inertia );
        sphere_1_ = new btRigidBody( info );    // Create the body
        physics_world_->world()->addRigidBody( sphere_1_ );                    // Register it with the world
    }//*/

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

	if( !physics() )
		return false;

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

	basic_shader_.bind();
    glm::mat4 model_matrix_ = glm::mat4(1.0f);
    glm::mat4 view_matrix_ = player_->camera()->view();
    glm::mat4 projection_matrix_ = player_->camera()->projection();
    glUniformMatrix4fv( uniform_model_matrix_, 1, GL_FALSE, glm::value_ptr( model_matrix_ ) );
    glUniformMatrix4fv( uniform_view_matrix_, 1, GL_FALSE, glm::value_ptr( view_matrix_ ) );
    glUniformMatrix4fv( uniform_projection_matrix_, 1, GL_FALSE, glm::value_ptr( projection_matrix_ ) );

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
    glUniformMatrix4fv( uniform_model_matrix_, 1, GL_FALSE, matrix );

    quad_[1].bind();
    bunny_.bind();
    quad_[1].draw();
	
	text_->render();

	window_->present();
	return true;
}

bool Game::physics()
{
    physics_world_->update( time_->dt() );

    /*
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
    }*/

	return true;
}