#include "Player.h"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int xoffset, yoffset;
float Yaw, Pitch;
Game::Player::Player() :
	chunkPos{ 0,0,0 },
	pos(10,10,10)
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.z = sin(glm::radians(Pitch));
	front.y = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	camera.forward = glm::normalize(front);
	camera.right = glm::normalize(glm::cross(camera.forward, glm::vec3(0, 0, 1)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	camera.up = glm::normalize(glm::cross(camera.right, camera.forward));
};

void Game::Player::input(SDL_Event* e)
{
	if (e->type == SDL_MOUSEMOTION)
	{
		xoffset = e->motion.xrel;
		yoffset = e->motion.yrel;

		Yaw -= xoffset * 0.2f;
		Pitch -= yoffset * 0.2f;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
		
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.z = sin(glm::radians(Pitch));
		front.y = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		camera.forward = glm::normalize(front);
		// also re-calculate the Right and Up vector
		camera.right = glm::normalize(glm::cross(camera.forward, glm::vec3(0,0,1)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		camera.up = glm::normalize(glm::cross(camera.right, camera.forward));
	}
	if (e->type == SDL_KEYDOWN)
	{
		switch (e->key.keysym.sym) {
		case SDLK_w:
			move[0] = true;
			break;
		case SDLK_s:
			move[1] = true;
			break;
		case SDLK_a:
			move[2] = true;
			break;
		case SDLK_d:
			move[3] = true;
			break;
		}
	}
	if (e->type == SDL_KEYUP)
	{
		switch (e->key.keysym.sym) {
		case SDLK_w:
			move[0] = false;
			break;
		case SDLK_s:
			move[1] = false;
			break;
		case SDLK_a:
			move[2] = false;
			break;
		case SDLK_d:
			move[3] = false;
			break;
		}
	}
}

void Game::Player::update(double delta)
{
	if (move[0])
	{
		pos += 0.05f * (float)delta * camera.forward;
	}
	if (move[1])
	{
		pos -= 0.05f * (float)delta * camera.forward;
	}
	if (move[2])
	{
		pos -= 0.05f * (float)delta * camera.right;
	}
	if (move[3])
	{
		pos += 0.05f * (float)delta * camera.right;
	}
	camera.view = glm::lookAt(pos, pos + camera.forward, camera.up);
}

void Game::Player::render()
{

}
