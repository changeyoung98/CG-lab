#pragma once
#include <GL/glew.h>
#include <iostream>
#include <ctime>
#include <math.h>
#include <glm/glm.hpp>
#include <learnopengl/camera.h>
#include <learnopengl/texture.h>
#include <learnopengl/shader.h>
#include <learnopengl/filesystem.h>

namespace FountainParticle {

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
	//最大速度
#define MAX_VELOC glm::vec3(0.0,-3.0,0.0)
	//最小速度
#define MIN_VELOC glm::vec3(0.0,-1.0,0.0)
	//发射粒子最大周期
#define MAX_LAUNCH 2.0f*1000.0f
	//发射粒子最小周期
#define MIN_LAUNCH 0.3f*1000.0f
	//初始点精灵大小
#define INIT_SIZE 10.0f
#define MAX_SIZE 8.0f
#define MIN_SIZE 3.0f

	const float ANGLE = 120.0f;
	const int MAX_PARTICLES = 10000;//定义粒子发射系统最大的粒子数

									//初始发射器例子数量
	const int INIT_PARTICLES = 5000;

	const glm::vec3 center(0.0f);
	const float radius = 0.01f;

	struct WaterParticle
	{
		float type;
		glm::vec3 position;
		glm::vec3 velocity;
		float lifetimeMills;//年龄
		float size;//粒子点精灵大小
	};

	class FountainParticle
	{
	public:
		FountainParticle()
		{
			mCurVBOIndex = 0;
			mCurTransformFeedbackIndex = 1;
			mFirst = true;
			mTimer = 0;
			const GLchar* varyings[5] = { "Type1","Position1",
				"Velocity1","Age1","Size1"
			};//设置TransformFeedback要捕获的输出变量
			mUpdateShader = new Shader(FileSystem::getPath("shaders/FountainUpdate.vs").c_str(), FileSystem::getPath("shaders/FountainUpdate.fs").c_str(),
				FileSystem::getPath("shaders/FountainUpdate.gs").c_str(), varyings, 5);
			//设置TransformFeedback缓存能够记录的顶点的数据类型

			mRenderShader = new Shader(FileSystem::getPath("shaders/FountainRender.vs").c_str(), FileSystem::getPath("shaders/FountainRender.fs").c_str());
			//设置随机纹理
			InitRandomTexture(512);
			mSparkTexture.loadTexture(FileSystem::getPath("res/textures/folds.jpg"));
			mRenderShader->use();
			mRenderShader->setInt("water", 0);
			InitFountain();
		}

		~FountainParticle()
		{
		}

		
		void Render(float frametimeMills, glm::mat4& worldMatrix,
			glm::mat4 viewMatrix, glm::mat4& projectMatrix)
		{
			mTimer += frametimeMills * 1000.0f;
			UpdateParticles(frametimeMills*1000.0f);
			RenderParticles(worldMatrix, viewMatrix, projectMatrix);
			mCurVBOIndex = mCurTransformFeedbackIndex;
			mCurTransformFeedbackIndex = (mCurTransformFeedbackIndex + 1) & 0x1;
		}
	private:
		unsigned int mCurVBOIndex, mCurTransformFeedbackIndex;
		GLuint mParticleBuffers[2]; //粒子发射系统的两个顶点缓存区
		GLuint mParticleArrays[2];
		GLuint mTransformFeedbacks[2];//粒子发射系统对应的TransformFeedback
		GLuint mRandomTexture;//随机一维纹理
		CTexture mSparkTexture;//Alpha纹理
		CTexture mStartTexture;
		float mTimer;//粒子发射器已经发射的时间
		bool mFirst;
		Shader* mUpdateShader;//更新粒子的GPUProgram
		Shader* mRenderShader;//渲染粒子的GPUProgram

		bool InitFountain()
		{
			WaterParticle particles[MAX_PARTICLES];
			//WaterParticle particles = new WaterParticle[MAX_PARTICLES];
			memset(particles, 0, sizeof(particles));
			GenInitLocation(particles, INIT_PARTICLES);
			glGenTransformFeedbacks(2, mTransformFeedbacks);
			glGenBuffers(2, mParticleBuffers);
			glGenVertexArrays(2, mParticleArrays);
			for (int i = 0; i < 2; i++)
			{
				glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, mTransformFeedbacks[i]);
				glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffers[i]);
				glBindVertexArray(mParticleArrays[i]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mParticleBuffers[i]);
			}
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
			glBindVertexArray(0);
			//绑定纹理
			mUpdateShader->use();
			glBindTexture(GL_TEXTURE_1D, mRandomTexture);
			mUpdateShader->setInt("gRandomTexture", 0);
			//delete[] particles;
			return true;
		}
		void UpdateParticles(float frametimeMills)
		{
			mUpdateShader->use();
			mUpdateShader->setFloat("gDeltaTimeMillis", frametimeMills);
			mUpdateShader->setFloat("gTime", mTimer);
			mUpdateShader->setFloat("MAX_SIZE", MAX_SIZE);
			mUpdateShader->setFloat("MIN_SIZE", MIN_SIZE);
			mUpdateShader->setFloat("MAX_LAUNCH", MAX_LAUNCH);
			mUpdateShader->setFloat("MIN_LAUNCH", MIN_LAUNCH);
			mUpdateShader->setFloat("angle", ANGLE);
			mUpdateShader->setFloat("R", radius);
			mUpdateShader->setVec3("NORMAL", glm::vec3(0, 1, 0));
			//绑定纹理
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_1D, mRandomTexture);

			glEnable(GL_RASTERIZER_DISCARD);//我们渲染到TransformFeedback缓存中去，并不需要光栅化
			glBindVertexArray(mParticleArrays[mCurVBOIndex]);
			glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffers[mCurVBOIndex]);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, mTransformFeedbacks[mCurTransformFeedbackIndex]);

			glEnableVertexAttribArray(0);//type
			glEnableVertexAttribArray(1);//position
			glEnableVertexAttribArray(2);//velocity
			glEnableVertexAttribArray(3);//lifetime
			glEnableVertexAttribArray(4);//size
			glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(WaterParticle), (void*)offsetof(WaterParticle, type));
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(WaterParticle), (void*)offsetof(WaterParticle, position));
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(WaterParticle), (void*)offsetof(WaterParticle, velocity));
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(WaterParticle), (void*)offsetof(WaterParticle, lifetimeMills));
			glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(WaterParticle), (void*)offsetof(WaterParticle, size));
			glBeginTransformFeedback(GL_POINTS);
			if (mFirst)
			{
				glDrawArrays(GL_POINTS, 0, INIT_PARTICLES);
				mFirst = false;
			}
			else {
				glDrawTransformFeedback(GL_POINTS, mTransformFeedbacks[mCurVBOIndex]);
			}
			glEndTransformFeedback();
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glDisableVertexAttribArray(3);
			glDisableVertexAttribArray(4);
			glDisable(GL_RASTERIZER_DISCARD);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void RenderParticles(glm::mat4& worldMatrix,
			glm::mat4& viewMatrix, glm::mat4& projectMatrix)
		{
			glEnable(GL_POINT_SPRITE);
			glEnable(GL_PROGRAM_POINT_SIZE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			mRenderShader->use();
			mRenderShader->setMat4("model", worldMatrix);
			mRenderShader->setMat4("view", viewMatrix);
			mRenderShader->setMat4("projection", projectMatrix);
			//glBindVertexArray(mParticleArrays[mCurTransformFeedbackIndex]);
			//glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,0,mParticleBuffers[mCurTransformFeedbackIndex]);
			glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffers[mCurTransformFeedbackIndex]);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(WaterParticle),
				(void*)offsetof(WaterParticle, position));
			glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(WaterParticle),
				(void*)offsetof(WaterParticle, size));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mSparkTexture.textureID);
			glDrawTransformFeedback(GL_POINTS, mTransformFeedbacks[mCurTransformFeedbackIndex]);
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisable(GL_BLEND);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void InitRandomTexture(unsigned int size)
		{
			srand(time(NULL));
			glm::vec3* pRandomData = new glm::vec3[size];
			for (int i = 0; i < size; i++)
			{
				pRandomData[i].x = float(rand()) / float(RAND_MAX);
				pRandomData[i].y = float(rand()) / float(RAND_MAX);
				pRandomData[i].z = float(rand()) / float(RAND_MAX);
			}
			glGenTextures(1, &mRandomTexture);
			glBindTexture(GL_TEXTURE_1D, mRandomTexture);
			glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, size, 0, GL_RGB, GL_FLOAT, pRandomData);
			glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			delete[] pRandomData;
			pRandomData = nullptr;
		}

		void GenInitLocation(WaterParticle particles[], int nums) {
			srand(time(NULL));
			for (int x = 0; x < nums; x++) {
				glm::vec3 record(0.0f);
				record.x = (2.0f*float(rand()) / float(RAND_MAX) - 1.0f)*radius;
				record.z = (2.0f*float(rand()) / float(RAND_MAX) - 1.0f)*radius;
				while (sqrt(record.x*record.x + record.z*record.z)>radius) {
					record.x = (2.0f*float(rand()) / float(RAND_MAX) - 1.0f)*radius;
					record.z = (2.0f*float(rand()) / float(RAND_MAX) - 1.0f)*radius;
				}
				record.y = 0.0f;
				particles[x].type = PARTICLE_TYPE_LAUNCHER;
				particles[x].position = record;
				particles[x].velocity = glm::vec3(0.0f);
				particles[x].size = INIT_SIZE;//发射器粒子大小
				particles[x].lifetimeMills = (MAX_LAUNCH - MIN_LAUNCH)*(float(rand()) / float(RAND_MAX)) + MIN_LAUNCH;
			}
		}		
	};

}
