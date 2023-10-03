#include <SDL.h>
#include <Windows.h>
#include <iostream>

const float GRAVITY_FORCE = 0.007;

int walls[][4] = {
	{0, 400, 500, 450},
	{300, 250, 500, 300},
	{500, 250, 550, 450}
};

float playerX = 50;
float playerSpeed = 0.5;
float playerY = 50;
float playerVelocityY = 0;
float playerJumpForce = 1.5;
float playerHeight = 50;
float playerWidth = 50;
bool playerIsGrounded = false;
bool playerIsWalledLeft = false;
bool playerIsWalledRight = false;

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "SDL_Init HAS FAILED. SDL_ERROR : " << SDL_GetError() << std::endl;
	}

	// Creer la fenêtre
	SDL_Window* window = SDL_CreateWindow("The Runner 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

	// Verifier si la fenêtre à bien été crée
	if (window == NULL)
	{
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
	}

	// Creer le renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Game loop
	SDL_Event event;
	bool gameRunning = true;
	while (gameRunning) {
		// Get our controls and events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				gameRunning = false;
			}
		}

		#pragma region INPUTS
		if (GetKeyState(VK_SPACE) & 0x8000 && playerIsGrounded)
		{
			playerVelocityY = -playerJumpForce;
		}
		if (GetKeyState('A') & 0x8000)
		{
			playerX -= playerSpeed;
		}
		if (GetKeyState('D') & 0x8000)
		{
			playerX += playerSpeed;
		}
		#pragma endregion

		#pragma region PHYSICS
		// gravity
		float fallDistance = playerVelocityY;
		playerIsGrounded = false;
		for (int i = 0; i < static_cast<int>(sizeof(walls) / sizeof(int[4])); i++) {
			// check if the wall is under the player
			if (walls[i][1] < playerY + playerHeight / 2) {
				continue;
			}

			// check if the wall is aligned with the player
			if (walls[i][0] > playerX + playerWidth / 2 || walls[i][2] < playerX - playerWidth / 2) {
				continue;
			}

			// calculate distance to the ground
			float groundDistance = walls[i][1] - (playerY + playerHeight / 2);

			// if the ground distance is smaller than the fall distance (will be in the wall next frame)
			if (groundDistance < fallDistance) {
				fallDistance = groundDistance;
				playerIsGrounded = true;
			}
		}
		playerY += fallDistance;

		// change the player y velocity
		if (playerIsGrounded) {
			playerVelocityY = 0;
		}
		else {
			playerVelocityY += GRAVITY_FORCE;
		}
		#pragma endregion

		#pragma region DRAW
		SDL_RenderClear(renderer);

		// player
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect rect;
		rect.x = static_cast<int>(playerX) - static_cast<int>(playerWidth / 2);
		rect.y = static_cast<int>(playerY) - static_cast<int>(playerHeight / 2);
		rect.w = static_cast<int>(playerWidth);
		rect.h = static_cast<int>(playerHeight);
		SDL_RenderDrawRect(renderer, &rect);

		// walls
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		for (int i = 0; i < static_cast<int>(sizeof(walls) / sizeof(int[4])); i++) {
			SDL_Rect rect;
			rect.x = walls[i][0];
			rect.y = walls[i][1];
			rect.w = walls[i][2] - walls[i][0];
			rect.h = walls[i][3] - walls[i][1];
			SDL_RenderDrawRect(renderer, &rect);
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);
		#pragma endregion
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}