#include <meshes/quad.h>
#include <graphics/shader_program.h>

void QuadMesh::init( ShaderProgram* shader, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3 )
{
	Mesh::init();

	this->bind();
	shader->bind();

	// Setup the vertex array
	GLint posAttrib = shader->getAttribLocation( "vPosition" );
	if( posAttrib != -1 )
	{
    	glEnableVertexAttribArray( posAttrib );
    	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0 );
	}
    GLint texAttrib = shader->getAttribLocation( "vTexCoord" );
    if( texAttrib != -1 )
    {
    	glEnableVertexAttribArray( texAttrib );
    	glVertexAttribPointer( texAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)) );
    }

	GLfloat vertices[] = {
		v0.x, v0.y, v0.z, 0.0f, 0.0f,
		v3.x, v3.y, v3.z, 1.0f, 1.0f,
		v1.x, v1.y, v1.z, 1.0f, 0.0f,
		v0.x, v0.y, v0.z, 0.0f, 0.0f,
		v3.x, v3.y, v3.z, 1.0f, 1.0f,
		v2.x, v2.y, v2.z, 0.0f, 1.0f
	};

	// Send the vertices
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

    num_verts_ = 6;

}

void QuadMesh::bind()
{
	Mesh::bind();
}