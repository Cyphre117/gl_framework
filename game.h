#ifndef GAME_H
#define GAME_H

#include "application_state.h"

class Window;
class TextRenderer;

class Game : public ApplicationState
{
public:
	Game();
	virtual ~Game() {}

	void init() override;
	void shutdown() override;
	bool frame() override;

	void setWindow( Window* window ) { window_ = window; }
	void setTextRenderer( TextRenderer* text ) { text_ = text; }

private:

	Window* window_;
	TextRenderer* text_;

	bool update();
	bool graphics();
	bool physics();
};

#endif