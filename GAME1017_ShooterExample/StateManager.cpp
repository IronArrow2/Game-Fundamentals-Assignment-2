#include "StateManager.h"

StateManager::StateManager()
{
	gameStates.push_back(nullptr);
}

void StateManager::pushState(GameState* pState, SDL_Window* window, SDL_Renderer* renderer)
{
	gameStates.push_back(pState);
	gameStates.back()->enter(window, renderer);
}

void StateManager::changeState(GameState* pState, SDL_Window* window, SDL_Renderer* renderer)
{
	if (!gameStates.empty()) 
	{
		if (gameStates.back()->getStateID() == pState->getStateID()) 
		{
			return;
			// do nothing    
		}
		if(gameStates.back()->exit())    
		{      
			delete gameStates.back();     
			gameStates.pop_back();    
		}  
	}  
	// push back our new state
	gameStates.push_back(pState);  
	// initialise it  
	gameStates.back()->enter(window, renderer);
}

void StateManager::popState()
{
	if (!gameStates.empty()) 
	{ 
		if (gameStates.back()->exit()) 
		{ 
			delete gameStates.back();      
			gameStates.pop_back(); 
		} 
	}
}

std::string StateManager::updateCurrentState()
{
	if (!gameStates.empty())
	{
		return gameStates.back()->update();
	}
	else
	{
		return "";
	}
}

void StateManager::renderCurrentState()
{
	if (!gameStates.empty()) 
	{ 
		if (gameStates.back()->getStateID() == "PAUSE")
		{
			for (int i = 1; i < gameStates.size(); i++)
			{
				gameStates[i]->render();
			}
		}
		else
		{
			gameStates.back()->render();
		}
	}
}

std::string StateManager::handleEventsCurrentState()
{
	if (!gameStates.empty())
	{
		return gameStates.back()->handleEvents();
	}
	else
	{
		return "";
	}
}
