#pragma once

#include<iostream>
#include<vector>
#include"glm\glm.hpp"
#include"Mesh.h"


class Node
{
public:
	char								name[50];						//�ڵ�����;
	glm::mat4						transformation;				//�任��Ϣ;
	char								parentName[50];		//���ڵ�����;
};