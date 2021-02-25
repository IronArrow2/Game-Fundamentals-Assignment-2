#pragma once
#include "Sprites.h"
#include "GameState.h"

class MainState : public GameState
{
private:
	bool m_bENull, // These three flags check if we need to clear the respective vectors of nullptrs.
		m_bPBNull,
		m_bEBNull,
		m_bCanShoot; // This restricts the player from firing again unless they release the Spacebar.
	const Uint8* m_iKeystates; // Keyboard state container.
	SDL_Window* m_pWindow; // This represents the SDL window.
	SDL_Renderer* m_pRenderer; // This represents the buffer to draw to.

	SDL_Texture* m_pBGText; // For the bg.
	Sprite bgArray[2];

	SDL_Texture* m_pSprText; // For the sprites.
	Player* m_player;

	int m_iESpawn, // The enemy spawn frame timer properties.
		m_iESpawnMax;
	vector<Enemy*> m_vEnemies;
	vector<Bullet*> m_vPBullets;
	vector<Bullet*> m_vEBullets;
	SDL_Point m_pivot;

	Mix_Music* m_pMusic;
	vector<Mix_Chunk*> m_vSounds;

	std::string stateID = "MAIN";
public:
	MainState();
	std::string update();
	void render();
	std::string handleEvents();
	std::string checkCollision();
	bool enter(SDL_Window* window, SDL_Renderer* renderer);
	bool exit();
	std::string getStateID();
};