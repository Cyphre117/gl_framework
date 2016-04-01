#ifndef MESH_H
#define MESH_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	void bind();
	void draw();

protected:	
    GLuint vao_;
    GLuint vbo_;
    size_t num_verts_;
};

#endif