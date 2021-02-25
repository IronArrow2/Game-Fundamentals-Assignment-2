#pragma once
#include <vector>
#include "GameState.h"

class StateManager
{
private:
	//StateManager();
	std::vector <GameState*> gameStates;
public:
	StateManager();
	//static StateManager* getInstance() { static StateManager* instance = new StateManager(); return instance; };
	void pushState(GameState* pState, SDL_Window* window, SDL_Renderer* renderer);
	void changeState(GameState* pState, SDL_Window* window, SDL_Renderer* renderer);
	void popState();
	std::string updateCurrentState();//calls the update function for the current state
	void renderCurrentState();//calls the render function for the current state
	std::string handleEventsCurrentState();//calls the handleEvents function for the current state
};