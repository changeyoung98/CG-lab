#ifndef Cube_h
#define Cube_h
#include<glm\glm.hpp>

glm::vec3 pos[] =
{
	//Front
	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),

	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),

	//back
	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),

	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),

	//left
	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),

	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),

	//right
	glm::vec3(1.0f, -1.0f, -1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),

	glm::vec3(1.0f, -1.0f, -1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),

	//up
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),

	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),

	//down
	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),

	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),
};

glm::vec4 color[] =
{
	//Front
	glm::vec4(1, 0, 0, 1),
	glm::vec4(1, 0, 0, 1),
	glm::vec4(1, 0, 0, 1),

	glm::vec4(1, 0, 0, 1),
	glm::vec4(1, 0, 0, 1),
	glm::vec4(1, 0, 0, 1),

	//back
	glm::vec4(1, 0, 0, 1),
	glm::vec4(1, 0, 0, 1),
	glm::vec4(1, 0, 0, 1),

	glm::vec4(1, 0, 0, 1),
	glm::vec4(1, 0, 0, 1),
	glm::vec4(1, 0, 0, 1),

	//left
	glm::vec4(0, 1, 0, 1),
	glm::vec4(0, 1, 0, 1),
	glm::vec4(0, 1, 0, 1),

	glm::vec4(0, 1, 0, 1),
	glm::vec4(0, 1, 0, 1),
	glm::vec4(0, 1, 0, 1),

	//right
	glm::vec4(0, 1, 0, 1),
	glm::vec4(0, 1, 0, 1),
	glm::vec4(0, 1, 0, 1),

	glm::vec4(0, 1, 0, 1),
	glm::vec4(0, 1, 0, 1),
	glm::vec4(0, 1, 0, 1),

	//up
	glm::vec4(0, 0, 1, 1),
	glm::vec4(0, 0, 1, 1),
	glm::vec4(0, 0, 1, 1),

	glm::vec4(0, 0, 1, 1),
	glm::vec4(0, 0, 1, 1),
	glm::vec4(0, 0, 1, 1),

	//down
	glm::vec4(0, 0, 1, 1),
	glm::vec4(0, 0, 1, 1),
	glm::vec4(0, 0, 1, 1),

	glm::vec4(0, 0, 1, 1),
	glm::vec4(0, 0, 1, 1),
	glm::vec4(0, 0, 1, 1),
};



glm::vec3 normal[] =
{
	//Front
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),

	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),

	//back
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(0.0f, 0.0f, -1.0f),

	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(0.0f, 0.0f, -1.0f),

	//left
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f),

	//right
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),

	//up
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),

	//down
	glm::vec3(0.0f, -1.0f, 0.0f),
	glm::vec3(0.0f, -1.0f, 0.0f),
	glm::vec3(0.0f, -1.0f, 0.0f),
	glm::vec3(0.0f, -1.0f, 0.0f),
	glm::vec3(0.0f, -1.0f, 0.0f),
	glm::vec3(0.0f, -1.0f, 0.0f),
};

glm::vec2 uv[] =
{
	//front
	glm::vec2(0, 0),
	glm::vec2(1, 0),
	glm::vec2(1, 1),

	glm::vec2(0, 0),
	glm::vec2(1, 1),
	glm::vec2(0, 1),

	//back
	glm::vec2(0, 0),
	glm::vec2(1, 0),
	glm::vec2(1, 1),

	glm::vec2(0, 0),
	glm::vec2(1, 1),
	glm::vec2(0, 1),

	//left
	glm::vec2(0, 0),
	glm::vec2(1, 0),
	glm::vec2(1, 1),

	glm::vec2(0, 0),
	glm::vec2(1, 1),
	glm::vec2(0, 1),

	//right
	glm::vec2(0, 0),
	glm::vec2(1, 0),
	glm::vec2(1, 1),

	glm::vec2(0, 0),
	glm::vec2(1, 1),
	glm::vec2(0, 1),

	//top
	glm::vec2(0, 0),
	glm::vec2(1, 0),
	glm::vec2(1, 1),

	glm::vec2(0, 0),
	glm::vec2(1, 1),
	glm::vec2(0, 1),

	//down
	glm::vec2(0, 0),
	glm::vec2(1, 0),
	glm::vec2(1, 1),

	glm::vec2(0, 0),
	glm::vec2(1, 1),
	glm::vec2(0, 1)
};

#endif