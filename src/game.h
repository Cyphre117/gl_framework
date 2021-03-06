#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>
#include <system/state.h>
#include <gameplay/player.h>
#include <graphics/texture_manager.h>
#include <graphics/shader_program.h>
#include <audio/audio_manager.h>
#include <meshes/quad.h>

class InputManager;
class Time;
class Window;
class Camera;
class TextRenderer;
class TextureManager;
class PhysicsWorld;

class Game : public State
{
public:
	Game();
	virtual ~Game() {}

	bool init() override;
	void shutdown() override;

	bool frame() override;

	void setTime( Time* time ) { time_ = time; }
	void setWindow( Window* window ) { window_ = window; }
	void setTextRenderer( TextRenderer* text ) { text_ = text; }

private:

	TextureManager* texture_manager_;
	InputManager* input_manager_;
	AudioManager* audio_manager_;
	PhysicsWorld* physics_world_;
	TextRenderer* text_;
	Window* window_;
	Player* player_;
	Time* time_;
	
	TextureHandle bunny_;
	AudioSource audio_source_;
	AudioBuffer audio_buffer_;

	btRigidBody* sphere_1_;

	ShaderProgram basic_shader_;
	GLint basic_model_matrix_uniform_;
	GLint basic_view_matrix_uniform_;
	GLint basic_projection_matrix_uniform_;

	ShaderProgram debug_lines_shader_;
	GLint debug_view_matrix_uniform_;
	GLint debug_projection_matrix_uniform_;

    QuadMesh quad_[3];

	bool update();
	bool graphics();
};

#endif