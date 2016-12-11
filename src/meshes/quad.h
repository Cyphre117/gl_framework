#ifndef QUAD_H
#define QUAD_H

#include <meshes/mesh.h>

class ShaderProgram;

class QuadMesh : public Mesh
{
public:
	QuadMesh() {}
	~QuadMesh() {}

 	void init( ShaderProgram* shader, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3 );
 	void bind() override;

protected:
};

#endif