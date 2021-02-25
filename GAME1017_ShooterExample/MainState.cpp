#include "MainState.h"
#define WIDTH 1024
#define HEIGHT 768
#define FPS 60
#define BGSCROLL 2 // Could these scroll/speed values be handled in the class? Yes. Consider it!
#define PSPEED 6

MainState::MainState()
{
	m_iESpawn = 0;
	m_iESpawnMax = 60;
	m_bCanShoot = true;
	m_bEBNull = m_bENull = m_bPBNull = false;
}

std::string MainState::update()
{
	// Scroll the backgrounds. Check if they need to snap back.
	for (int i = 0; i < 2; i++)
		bgArray[i].GetDstP()->x -= BGSCROLL;
	if (bgArray[1].GetDstP()->x <= 0)
	{
		bgArray[0].GetDstP()->x = 0;
		bgArray[1].GetDstP()->x = 1024;
	}
	// Player animation/movement.
	m_player->Animate(); // Oh! We're telling the player to animate itself. This is good! Hint hint.
	if (KeyDown(SDL_SCANCODE_A, m_iKeystates) && m_player->GetDstP()->x > m_player->GetDstP()->h)
		m_player->GetDstP()->x -= PSPEED;
	else if (KeyDown(SDL_SCANCODE_D, m_iKeystates) && m_player->GetDstP()->x < WIDTH / 2)
		m_player->GetDstP()->x += PSPEED;
	if (KeyDown(SDL_SCANCODE_W, m_iKeystates) && m_player->GetDstP()->y > 0)
		m_player->GetDstP()->y -= PSPEED;
	else if (KeyDown(SDL_SCANCODE_S, m_iKeystates) && m_player->GetDstP()->y < HEIGHT - m_player->GetDstP()->w)
		m_player->GetDstP()->y += PSPEED;

	// Enemy animation/movement.
	for (int i = 0; i < (int)m_vEnemies.size(); i++)
	{
		m_vEnemies[i]->Update(); // Oh, again! We're telling the enemies to update themselves. Good good!
		if (m_vEnemies[i]->GetDstP()->x < -56)
		{
			delete m_vEnemies[i];
			m_vEnemies[i] = nullptr;
			m_bENull = true;
		}
	}
	if (m_bENull) CleanVector<Enemy*>(m_vEnemies, m_bENull); // Better to have a logic check (if) than a function call all the time!
	// Update enemy spawns.
	if (m_iESpawn++ == m_iESpawnMax)
	{
		m_vEnemies.push_back(new Enemy({ 0,100,40,56 }, { WIDTH,56 + rand() % (HEIGHT - 114),40,56 }, &m_vEBullets, m_vSounds[0],
			30 + rand() % 91)); // Randomizing enemy bullet spawn to every 30-120 frames.
		m_iESpawn = 0;
	}
	// Update the bullets. Player's first.
	for (int i = 0; i < (int)m_vPBullets.size(); i++)
	{
		m_vPBullets[i]->Update();
		if (m_vPBullets[i]->GetDstP()->x > WIDTH)
		{
			delete m_vPBullets[i];
			m_vPBullets[i] = nullptr;
			m_bPBNull = true;
		}
	}
	if (m_bPBNull) CleanVector<Bullet*>(m_vPBullets, m_bPBNull);
	// Now enemy bullets. Is Update() getting a little long?
	for (int i = 0; i < (int)m_vEBullets.size(); i++)
	{
		m_vEBullets[i]->Update();
		if (m_vEBullets[i]->GetDstP()->x < -10)
		{
			delete m_vEBullets[i];
			m_vEBullets[i] = nullptr;
			m_bEBNull = true;
		}
	}
	if (m_bEBNull) CleanVector<Bullet*>(m_vEBullets, m_bEBNull);
	return checkCollision();
}

void MainState::render()
{
	// Render stuff. Background first.
	for (int i = 0; i < 2; i++)
		SDL_RenderCopy(m_pRenderer, m_pBGText, bgArray[i].GetSrcP(), bgArray[i].GetDstP());
	// Player.
	SDL_RenderCopyEx(m_pRenderer, m_pSprText, m_player->GetSrcP(), m_player->GetDstP(), m_player->GetAngle(), &m_pivot, SDL_FLIP_NONE);
	//SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);
	//SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 255, 128);
	//SDL_RenderFillRect(m_pRenderer, m_player->GetDstP());
	// Player bullets.	
	for (int i = 0; i < (int)m_vPBullets.size(); i++)
	{
		SDL_RenderCopyEx(m_pRenderer, m_pSprText, m_vPBullets[i]->GetSrcP(), m_vPBullets[i]->GetDstP(), 90, &m_pivot, SDL_FLIP_NONE);
		//SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 0, 128);
		//SDL_RenderFillRect(m_pRenderer, m_vPBullets[i]->GetDstP());
	}
	// Enemies.
	for (int i = 0; i < (int)m_vEnemies.size(); i++)
	{
		SDL_RenderCopyEx(m_pRenderer, m_pSprText, m_vEnemies[i]->GetSrcP(), m_vEnemies[i]->GetDstP(), -90, &m_pivot, SDL_FLIP_NONE);
		//SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, 128);
		//SDL_RenderFillRect(m_pRenderer, m_vEnemies[i]->GetDstP());
	}
	// Enemy bullets.
	for (int i = 0; i < (int)m_vEBullets.size(); i++)
		SDL_RenderCopy(m_pRenderer, m_pSprText, m_vEBullets[i]->GetSrcP(), m_vEBullets[i]->GetDstP());
	//SDL_RenderPresent(m_pRenderer);
}

std::string MainState::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_SPACE)
				m_bCanShoot = true;
			break;
		case SDL_QUIT: // User pressed window's 'x' button.
			return "QUIT";
			break;
		case SDL_KEYDOWN: // Try SDL_KEYUP instead.
			if (event.key.keysym.sym == SDLK_ESCAPE)
				return "QUIT";
			else if (event.key.keysym.sym == SDLK_p)
			{
				return "PAUSE";
			}
			break;
		}
		if (KeyDown(SDL_SCANCODE_SPACE, m_iKeystates) && m_bCanShoot)
		{
			m_bCanShoot = false;
			m_vPBullets.push_back(new Bullet({ 376,0,10,100 }, { m_player->GetDstP()->x + 85,m_player->GetDstP()->y + 42,10,100 }, 30));
			Mix_PlayChannel(-1, m_vSounds[1], 0);
		}
	}
	return "";
}

std::string MainState::checkCollision()
{
	// Player vs. Enemy.
	SDL_Rect p = { m_player->GetDstP()->x - 100, m_player->GetDstP()->y, 100, 94 };
	for (int i = 0; i < (int)m_vEnemies.size(); i++)
	{
		SDL_Rect e = { m_vEnemies[i]->GetDstP()->x, m_vEnemies[i]->GetDstP()->y - 40, 56, 40 };
		if (SDL_HasIntersection(&p, &e))
		{
			// Game over!
			cout << "Player goes boom!" << endl;
			Mix_PlayChannel(-1, m_vSounds[2], 0);
			return "LOSE";
		}
	}
	// Player bullets vs. Enemies.
	for (int i = 0; i < (int)m_vPBullets.size(); i++)
	{
		SDL_Rect b = { m_vPBullets[i]->GetDstP()->x - 100, m_vPBullets[i]->GetDstP()->y, 100, 10 };
		for (int j = 0; j < (int)m_vEnemies.size(); j++)
		{
			if (m_vEnemies[j] == nullptr) continue;
			SDL_Rect e = { m_vEnemies[j]->GetDstP()->x, m_vEnemies[j]->GetDstP()->y - 40, 56, 40 };
			if (SDL_HasIntersection(&b, &e))
			{
				Mix_PlayChannel(-1, m_vSounds[2], 0);
				delete m_vEnemies[j];
				m_vEnemies[j] = nullptr;
				delete m_vPBullets[i];
				m_vPBullets[i] = nullptr;
				m_bENull = true;
				m_bPBNull = true;
				break;
			}
		}
	}
	if (m_bENull) CleanVector<Enemy*>(m_vEnemies, m_bENull);
	if (m_bPBNull) CleanVector<Bullet*>(m_vPBullets, m_bPBNull);
	// Enemy bullets vs. player.
	for (int i = 0; i < (int)m_vEBullets.size(); i++)
	{
		if (SDL_HasIntersection(&p, m_vEBullets[i]->GetDstP()))
		{
			// Game over!
			cout << "Player goes boom!" << endl;
			Mix_PlayChannel(-1, m_vSounds[2], 0);
			delete m_vEBullets[i];
			m_vEBullets[i] = nullptr;
			CleanVector<Bullet*>(m_vEBullets, m_bEBNull);
			return "LOSE";
		}
	}
	return "";
}

bool MainState::enter(SDL_Window* window, SDL_Renderer* renderer)
{
	m_pWindow = window;
	m_pRenderer = renderer;

	m_pBGText = IMG_LoadTexture(m_pRenderer, "Img/background.png");
	m_pSprText = IMG_LoadTexture(m_pRenderer, "Img/sprites.png");
	if (Mix_Init(MIX_INIT_MP3) != 0) // Mixer init success.
	{
		Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048); // Good for most games.
		Mix_AllocateChannels(16);
		m_pMusic = Mix_LoadMUS("Aud/game.mp3"); // Load the music track.
					// Load the chunks into the Mix_Chunk vector.
		m_vSounds.reserve(3); // Optional but good practice.
		m_vSounds.push_back(Mix_LoadWAV("Aud/enemy.wav"));
		m_vSounds.push_back(Mix_LoadWAV("Aud/laser.wav"));
		m_vSounds.push_back(Mix_LoadWAV("Aud/explode.wav"));
		/* By the way, you should check to see if any of these loads are failing and
		you can use Mix_GetError() to print out the error message. Wavs can be finicky.*/
	}
	else return false;

	m_iKeystates = SDL_GetKeyboardState(nullptr);
	bgArray[0] = { {0,0,1024,768}, {0, 0, 1024, 768} };
	bgArray[1] = { {0,0,1024,768}, {1024, 0, 1024, 768} };
	m_player = new Player( {0,0,94,100}, {256,384-50,94,100} );
	Mix_PlayMusic(m_pMusic, -1); // Play. -1 = looping.
	Mix_VolumeMusic(16); // 0-MIX_MAX_VOLUME (128).

	return true;
}

bool MainState::exit()
{
	for (int i = 0; i < (int)m_vSounds.size(); i++)
		Mix_FreeChunk(m_vSounds[i]);
	m_vSounds.clear();
	Mix_FreeMusic(m_pMusic);

	return true;
}

std::string MainState::getStateID()
{
	return stateID;
}
