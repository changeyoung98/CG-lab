#pragma once

#include<iostream>
#include<vector>
#include"glm\glm.hpp"
#include"Mesh.h"


class Node
{
public:
	char								name[50];						//节点名称;
	glm::mat4						transformation;				//变换信息;
	char								parentName[50];		//父节点名称;
};