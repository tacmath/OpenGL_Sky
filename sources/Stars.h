#pragma once

#include "VAO.h"
#include "VBO.h"
#include "shader.h"
#include <random>
#include <memory>

#define NB_STARS 300

#define STAR_SIZE 0.008f
#define HALF_STAR_SIZE (STAR_SIZE * 0.5f)

#include <iostream>
#include <GLFW/glfw3.h>
class Stars
{
private:
	VBO		vbo;
	VAO		vao;
	Shader	shader;

public:

	Stars() {
		shader.Load("shaders/starVS.glsl", "shaders/starFS.glsl");
		InitStars();
		glEnable(GL_CULL_FACE);
	}

	~Stars() {
		vao.Delete();
		vbo.Delete();
		shader.Delete();
	}

	void Draw() {
		shader.Activate();
		glm::mat4 projection = glm::perspective(glm::radians(60.0f), 1.7f, 0.1f, 100.0f) * glm::rotate(glm::mat4(1), -(float)glfwGetTime() *0.2f, glm::vec3(1, 0, 0));
		shader.setMat4("matrix", projection);
		vao.Bind();
		glDrawArrays(GL_TRIANGLES, 0, NB_STARS * 6);
	}

private:
	void InitStars() {
		glm::vec4 starData[] = {glm::vec4(-HALF_STAR_SIZE,-HALF_STAR_SIZE, 0, 1),
								glm::vec4(-HALF_STAR_SIZE,HALF_STAR_SIZE, 0, 1),
								glm::vec4(HALF_STAR_SIZE,-HALF_STAR_SIZE, 0, 1),

								glm::vec4(HALF_STAR_SIZE,-HALF_STAR_SIZE, 0, 1),
								glm::vec4(-HALF_STAR_SIZE,HALF_STAR_SIZE, 0, 1),
								glm::vec4(HALF_STAR_SIZE,HALF_STAR_SIZE, 0, 1)};

		
		std::unique_ptr<glm::vec3[]> finalStarsData = std::make_unique<glm::vec3[]>(6 * NB_STARS);
		std::random_device random;
		glm::mat4 starMatrix(1);
		glm::vec3 starPos;

		for (unsigned starID = 0; starID < NB_STARS; starID++) {
			starPos = glm::normalize(GetRandomPointInCube());
			starMatrix = glm::translate(glm::mat4(1), starPos)
				* RotateTo(starPos, glm::vec3(0))
				* glm::rotate(glm::mat4(glm::mat3(0.4f + 0.6f * ((float)random() / (random.max())))), ((float)random() / (random.max())) * 360.0f, glm::vec3(0, 0, 1));
			for (unsigned vertex = 0; vertex < 6; vertex++)
				finalStarsData[6 * starID + vertex] = glm::vec3(starMatrix * starData[vertex]);
		}
		vbo.Gen(finalStarsData.get(), sizeof(glm::vec3) * 6 * NB_STARS);
		vao.Gen();
		vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(GLfloat), 0);
	}

	glm::mat4    RotateTo(const glm::vec3& from, const glm::vec3& to) {
		const glm::vec3 forward = glm::normalize(from - to);
		const glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
		const glm::vec3 up = glm::cross(forward, right);
		return glm::mat4({
					right[0],       right[1],       right[2],   0,
					up[0],          up[1],          up[2],      0,
					forward[0],     forward[1],     forward[2], 0,
					0,              0,              0,          1
			});
	}

	glm::vec3 GetRandomPointInCube() {
		std::random_device random;
		glm::vec3 result;
		result.x = ((float)random() / (random.max())) * 2.0f - 1.0f;
		result.y = ((float)random() / (random.max())) * 2.0f - 1.0f;
		result.z = ((float)random() / (random.max())) * 2.0f - 1.0f;
		return result;
	}

};

