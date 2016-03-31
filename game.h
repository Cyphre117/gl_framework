#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>
#include "application_state.h"
#include "shader_program.h"

class Input;
class Timer;
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
	void setTimer( Timer* timer ) { timer_ = timer; }
	void setCamera( Camera* camera ) { camera_ = camera; }
	void setWindow( Window* window ) { window_ = window; }
	void setTextRenderer( TextRenderer* text ) { text_ = text; }

private:

	Input* input_;
	Timer* timer_;
	Window* window_;
	Camera* camera_;
	TextRenderer* text_;

	ShaderProgram basic_shader_;
	GLint uniform_model_matrix_;
	GLint uniform_view_matrix_;
	GLint uniform_projection_matrix_;
	glm::mat4 model_matrix_;
	glm::mat4 view_matrix_;
	glm::mat4 projection_matrix_;


    GLuint vao;
    GLuint vbo;

	bool update();
	bool graphics();
	bool physics();
};

#endif