#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Game {
namespace GFX {
	struct Camera {
		glm::vec3 up, forward, right;

		glm::mat4 model, view, proj;

		Camera(int w, int h) {
			up = glm::vec3(0, 0, 1);
			forward = glm::vec3(0, 1, 0);
			right = glm::vec3(1, 0, 0);

			model = glm::mat4(1.0f);
			view = glm::mat4(1.0f);
			proj = glm::perspective(glm::radians(90.0f), (float)w / (float)h, 0.1f, 1000.0f);
			view = glm::translate(view, glm::vec3(0.0f, -5.0f, 0.0f));
		}
	};
}
}
#endif