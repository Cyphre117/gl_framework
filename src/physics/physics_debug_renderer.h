#ifndef PHYSICS_DEBUG_RENDERER_H
#define PHYSICS_DEBUG_RENDERER_H

#include <bullet/LinearMath/btIDebugDraw.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <graphics/shader_program.h>
#include <iostream>
#include <vector>

class PhysicsDebugRenderer : public btIDebugDraw
{
public:
	PhysicsDebugRenderer();
	~PhysicsDebugRenderer();

	bool init( ShaderProgram* shader );

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {}
	void draw3dText(const btVector3& location, const char* textString) override {}
	void setDebugMode(int debugMode) override { draw_mode_ = (DebugDrawModes)debugMode; }
	int getDebugMode() const override { return draw_mode_; }
	void reportErrorWarning(const char* warningString) override
	{
		std::cout << warningString << std::endl;
	}

	void flushLines() override;

protected:
	DebugDrawModes draw_mode_;

    GLuint vao_;
    GLuint vbo_;
	std::vector<GLfloat> lines_;
};

#endif