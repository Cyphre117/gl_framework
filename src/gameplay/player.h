#ifndef PLAYER_H
#define PLAYER_H

#include <system/singleton.h>
#include <system/camera.h>

class Player : public Singleton
{
public:
	Player();
	~Player() {}

	bool init();
	void shutdown();

	static Player* get() { return self_; }

	void update( float dt );

	// Setters

	// Getters
	Camera* camera() { return &camera_; }

protected:
	static Player* self_;

	void updateAudioListener();

	Camera camera_;
};

#endif