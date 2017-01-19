#include "physics_debug_renderer.h"

PhysicsDebugRenderer::PhysicsDebugRenderer()
: draw_mode_( btIDebugDraw::DBG_DrawWireframe )
{
}


PhysicsDebugRenderer::~PhysicsDebugRenderer()
{
	// TODO: should possibly move to shutdown() function and call explicitly from the physics_world
    glDeleteBuffers( 1, &vbo_ );
    glDeleteVertexArrays( 1, &vao_ );
}

bool PhysicsDebugRenderer::init( ShaderProgram* shader )
{
    glGenVertexArrays(1, &vao_ );
    glGenBuffers(1, &vbo_ );

    glBindVertexArray( vao_ );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
    shader->bind();

    // Setup the vertex array
	GLint posAttrib = shader->getAttribLocation( "vPosition" );
	if( posAttrib != -1 )
	{
    	glEnableVertexAttribArray( posAttrib );
    	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0 );
	}
	GLint colAttrib = shader->getAttribLocation( "vColour" );
	if( colAttrib != -1 )
	{
		glEnableVertexAttribArray( colAttrib );
		glVertexAttribPointer( colAttrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)) );
	}

	return true;
}

void PhysicsDebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	lines_.push_back(from.x());
	lines_.push_back(from.y());
	lines_.push_back(from.z());
	lines_.push_back(color.x());
	lines_.push_back(color.y());
	lines_.push_back(color.z());
	lines_.push_back(to.x());
	lines_.push_back(to.y());
	lines_.push_back(to.z());
	lines_.push_back(color.x());
	lines_.push_back(color.y());
	lines_.push_back(color.z());
}

void PhysicsDebugRenderer::flushLines()
{
    glBindVertexArray( vao_ );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_ );

    glBufferData( GL_ARRAY_BUFFER, sizeof(lines_[0]) * lines_.size(), lines_.data(), GL_DYNAMIC_DRAW );
	glDrawArrays( GL_LINES, 0, lines_.size()/6 );

	std::cout << "Drawing " << lines_.size()/6 << " verts" << std::endl;

	lines_.clear();
}