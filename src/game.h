#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>
#include "application_state.h"
#include "texture_manager.h"
#include "shader_program.h"
#include "quad.h"

class Input;
class Time;
class Window;
class Camera;
class TextRenderer;

class Game : public ApplicationState
{
public:
	Game();
	virtual ~Game() {}

	void init() override;
	void shutdown() override;

	bool frame() override;

	void setInput( Input* input ) { input_ = input; }
	void setTime( Time* time ) { time_ = time; }
	void setCamera( Camera* camera ) { camera_ = camera; }
	void setWindow( Window* window ) { window_ = window; }
	void setTextRenderer( TextRenderer* text ) { text_ = text; }

private:

	Input* input_;
	Time* time_;
	Window* window_;
	Camera* camera_;
	TextRenderer* text_;

	//TextureManager texture_manager_;
	TextureHandle default_;

	ShaderProgram basic_shader_;
	GLint uniform_model_matrix_;
	GLint uniform_view_matrix_;
	GLint uniform_projection_matrix_;

    QuadMesh quad_[3];

	bool update();
	bool graphics();
	bool physics();
};

#endif