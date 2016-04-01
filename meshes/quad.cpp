#include "quad.h"
#include "../shader_program.h"

QuadMesh::QuadMesh()
{	
}


QuadMesh::~QuadMesh()
{	
}

void QuadMesh::init( ShaderProgram* shader, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3 )
{
	bind();

	// Setup the vertex array
	GLint position_attrib = shader->getAttribLocation( "position" );
	if( position_attrib != -1 )
	{
    	glEnableVertexAttribArray( position_attrib );
		glVertexAttribPointer( position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	}

	glm::vec3 vertices[] = { v0, v3, v1, v0, v3, v2 };

	// Send the vertices
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

    num_verts_ = 6;
}