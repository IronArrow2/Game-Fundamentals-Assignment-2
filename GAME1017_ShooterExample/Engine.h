#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include "Sprites.h"
#include "StateManager.h"
#include "TitleState.h"
#include "Pause.h"
#include "ShooterState.h"
#include "LoseState.h"

class Engine
{
// I am avoiding in-class initialization.
private: // Private properties.
	bool m_bRunning; // Loop control flag
	Uint32 m_start, m_end, m_delta, m_fps; // Fixed timestep variables.
	SDL_Window* m_pWindow; // This represents the SDL window.
	SDL_Renderer* m_pRenderer; // This represents the buffer to draw to.
	StateManager* m_pStateManager; //This represents the state manager

private: // Private methods.
	Engine();
	bool Init(const char* title, int xpos, int ypos, int width, int height, int flags);
	void Wake();
	void Sleep();
	void HandleEvents();
	
	void Update();
	void Render();
	void Clean();
public: // Public methods.
	//void CheckCollision();
	static Engine* getInstance() { static Engine* instance = new Engine(); return instance; };
	~Engine();
	int Run();
	SDL_Renderer* GetRenderer();
};