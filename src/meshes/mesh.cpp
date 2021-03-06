#include <meshes/mesh.h>

void Mesh::init()
{
    glGenVertexArrays(1, &vao_ );
    glGenBuffers(1, &vbo_ );
}

void Mesh::shutdown()
{
    glDeleteBuffers( 1, &vbo_ );
    glDeleteVertexArrays( 1, &vao_ );
}

void Mesh::bind()
{	
    glBindVertexArray( vao_ );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
}

void Mesh::draw()
{	
	glDrawArrays( GL_TRIANGLES, 0, num_verts_ );
}