#ifndef HELPERS_H
#define HELPERS_H

#define DeleteNull(p) if(p){delete p; p = nullptr;}
#define btVec3_glmVec3(v) glm::vec3(v.x(), v.y(), v.z())
#define glmVec3_btVec3(v) btVector3(v.x, v.y, v.z)

#endif