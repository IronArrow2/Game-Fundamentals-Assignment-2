#include <random>
#include <time.h>
#include "SideScrollerState.h"

SideScrollerState::SideScrollerState()
{
	m_iBSpawn = 0;
}

std::string SideScrollerState::update()
{
	if (!playerDead)
	{
		for (unsigned int i = 0; i < m_backgrounds.size(); i++)
		{
			m_backgrounds[i]->Update();
		}
		for (unsigned int i = 0; i < m_obstacles.size(); i++)
		{
			m_obstacles[i]->Update();
			if (m_obstacles[i]->GetDstP()->x < -m_obstacles[i]->GetDstP()->w)
			{
				delete m_obstacles[i];
				m_obstacles[i] = nullptr;
				m_bONull = true;
			}
		}
		if (m_bONull) CleanVector<Obstacle*>(m_obstacles, m_bONull);

		if (m_iBSpawn++ == m_iBSpawnMax)
		{//create new obstacle/set of obstacles
			int temp = rand() % 9 + 1;
			switch (temp)
			{
			case 9:
				//ground blade 1
				m_obstacles.push_back(new Obstacle({ 0, 0, 124, 123 }, { WIDTH, 570, 124, 123 }, 4));
				break;
			case 1:
				//ground blade 2
				m_obstacles.push_back(new Obstacle({ 124, 0, 127, 127 }, { WIDTH, 570, 127, 127 }, 4));
				break;
			case 2:
				//ground blade 3
				m_obstacles.push_back(new Obstacle({ 251, 0, 111, 111 }, { WIDTH, 570, 111, 111 }, 4));
				break;
			case 3:
				//ground spikes 1
				m_obstacles.push_back(new Obstacle({ 462, 0, 128, 76 }, { WIDTH, 565, 128, 76 }, 4));
				break;
			case 4:
				//ground spikes 2
				m_obstacles.push_back(new Obstacle({ 590, 0, 156, 81 }, { WIDTH, 565, 156, 81 }, 4));
				break;
			case 5:
				//overhead blade 1
				m_obstacles.push_back(new Obstacle({ 0, 260, 270, 210 }, { WIDTH, 350, 270, 210 }, 4));
				m_obstacles.push_back(new Obstacle({ 388, 0, 16, 260 }, { WIDTH + 125, -30, 16, 260 }, 4));
				m_obstacles.push_back(new Obstacle({ 388, 0, 16, 260 }, { WIDTH + 125, 130, 16, 260 }, 4));
				break;
			case 6:
				//overhead blade 2
				m_obstacles.push_back(new Obstacle({ 270, 260, 172, 193 }, { WIDTH, 370, 172, 193 }, 4));
				m_obstacles.push_back(new Obstacle({ 362, 0, 26, 212 }, { WIDTH + 66, 188, 26, 212 }, 4));
				m_obstacles.push_back(new Obstacle({ 362, 0, 26, 212 }, { WIDTH + 66, -4, 26, 212 }, 4));
				break;
			case 7:
				//overhead blade 3
				m_obstacles.push_back(new Obstacle({ 442, 260, 186, 223 }, { WIDTH, 340, 186, 223 }, 4));
				m_obstacles.push_back(new Obstacle({ 388, 0, 16, 260 }, { WIDTH + 86, 100, 16, 260 }, 4));
				m_obstacles.push_back(new Obstacle({ 404, 0, 16, 130 }, { WIDTH + 86, -30, 16, 130 }, 4));
				break;
			case 8:
				//overhead blade 4
				m_obstacles.push_back(new Obstacle({ 628, 260, 366, 85 }, { WIDTH, 480, 366, 85 }, 4));
				m_obstacles.push_back(new Obstacle({ 362, 0, 26, 212 }, { WIDTH + 104, 286, 26, 212 }, 4));
				m_obstacles.push_back(new Obstacle({ 362, 0, 26, 212 }, { WIDTH + 104, -98, 26, 212 }, 4));
				m_obstacles.push_back(new Obstacle({ 362, 0, 26, 212 }, { WIDTH + 104, 94, 26, 212 }, 4));
				m_obstacles.push_back(new Obstacle({ 362, 0, 26, 212 }, { WIDTH + 224, 286, 26, 212 }, 4));
				m_obstacles.push_back(new Obstacle({ 362, 0, 26, 212 }, { WIDTH + 224, 94, 26, 212 }, 4));
				m_obstacles.push_back(new Obstacle({ 362, 0, 26, 212 }, { WIDTH + 224, -98, 26, 212 }, 4));
				break;
			}
			m_iBSpawn = 0;
		}
	}
	else
	{
		playerDeathTimer++;
		if (m_pPlayer->GetDstP()->y <= 450)
		{
			m_pPlayer->GetDstP()->y = m_pPlayer->GetDstP()->y + 3;
		}
		if (playerDeathTimer == playerDeathTimerMax)
		{
			return "LOSE";
		}
	}

	m_pPlayer->Update();
	for (unsigned int i = 0; i < m_foregrounds.size(); i++)
	{
		m_foregrounds[i]->Update();
	}

	return checkCollision();
}

void SideScrollerState::render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.
	// Render stuff, background first
	for (unsigned int i = 0; i < m_backgrounds.size(); i++)
	{
		m_backgrounds[i]->Render(m_pRenderer);
	}
	for (unsigned int i = 0; i < m_obstacles.size(); i++)
	{
		m_obstacles[i]->Render(m_pRenderer);
	}
	for (unsigned int i = 0; i < m_foregrounds.size(); i++)
	{
		m_foregrounds[i]->Render(m_pRenderer);
	}

	m_pPlayer->Render(m_pRenderer);
}

std::string SideScrollerState::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // User pressed window's 'x' button.
			return "QUIT";
			break;
		case SDL_KEYDOWN: // Try SDL_KEYUP instead.
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				return "QUIT";
				break;
			}
			else if (event.key.keysym.sym == SDLK_p)
			{
				return "PAUSE";
				break;
			}
			else if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_SPACE)
			{
				Mix_PlayChannel(-1, m_vSounds[0], 0);
			}
		}
		m_pPlayer->HandleEvents(event);
	}

	return "";
}

std::string SideScrollerState::checkCollision()
{
	SDL_Rect p = { m_pPlayer->GetDstP()->x, m_pPlayer->GetDstP()->y, m_pPlayer->GetDstP()->w, m_pPlayer->GetDstP()->h };
	for (int i = 0; i < (int)m_obstacles.size(); i++)
	{
		SDL_Rect o = { m_obstacles[i]->GetDstP()->x, m_obstacles[i]->GetDstP()->y, m_obstacles[i]->GetDstP()->w, m_obstacles[i]->GetDstP()->h };
		if (SDL_HasIntersection(&p, &o))
		{
			if (!playerDead)
			{
				Mix_PlayChannel(-1, m_vSounds[1], 0);
				Mix_HaltMusic();
			}
			// Game over!
			cout << "Player goes boom!" << endl;
			m_pPlayer->die();
			playerDead = true;
			//return "LOSE";
		}
	}
	return "";
}

bool SideScrollerState::enter(SDL_Window* window, SDL_Renderer* renderer)
{
	srand(time(NULL));
	playerDeathTimer = 0;
	m_pWindow = window;
	m_pRenderer = renderer;

	m_backgrounds.reserve(16);
	m_backgrounds.push_back(new Background({ 2730, 768, 1365, 768 }, { 0, 0, 1365, 768 }, 0.5));
	m_backgrounds.push_back(new Background({ 2730, 768, 1365, 768 }, { 1365, 0, 1365, 768 }, 0.5));
	m_backgrounds.push_back(new Background({ 0, 0, 1365, 768 }, { 0, 0, 1365, 768 }, 1));
	m_backgrounds.push_back(new Background({ 0, 0, 1365, 768 }, { 1365, 0, 1365, 768 }, 1));
	m_backgrounds.push_back(new Background({ 0, 768, 1365, 768 }, { 0, 0, 1365, 768 }, 2));
	m_backgrounds.push_back(new Background({ 0, 768, 1365, 768 }, { 1365, 0, 1365, 768 }, 2));
	m_backgrounds.push_back(new Background({ 1365, 0, 1365, 768 }, { 0, 0, 1365, 768 }, 3));
	m_backgrounds.push_back(new Background({ 1365, 0, 1365, 768 }, { 1365, 0, 1365, 768 }, 3));
	m_backgrounds.push_back(new Background({ 2730, 0, 1365, 768 }, { 0, 0, 1365, 768 }, 4));
	m_backgrounds.push_back(new Background({ 2730, 0, 1365, 768 }, { 1365, 0,1365, 768 }, 4));
	m_foregrounds.push_back(new Background({ 4095, 0, 1365, 768 }, { 0, 0, 1365, 768 }, 5));
	m_foregrounds.push_back(new Background({ 4095, 0, 1365, 768 }, { 1365, 0, 1365, 768 }, 5));
	m_foregrounds.push_back(new Background({ 4095, 768, 1365, 768 }, { 0, 0, 1365, 768 }, 5));
	m_foregrounds.push_back(new Background({ 4095, 768, 1365, 768 }, { 1365, 0, 1365, 768 }, 5));
	m_backgrounds.push_back(new Background({ 1365, 768, 1365, 768 }, { 0,0, 1365, 768 }, 0));

	m_pPlayer = new SideScrollerPlayer({1458, 0, 46, 76}, {100, 550, 46, 76});

	if (Mix_Init(MIX_INIT_MP3) != 0) // Mixer init success.
	{
		Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048); // Good for most games.
		Mix_AllocateChannels(16);
		m_pMusic = Mix_LoadMUS("Aud/Jingle_Bells.mp3"); // Load the music track.
					// Load the chunks into the Mix_Chunk vector.
		m_vSounds.reserve(3); // Optional but good practice.
		m_vSounds.push_back(Mix_LoadWAV("Aud/Jump.mp3"));
		m_vSounds.push_back(Mix_LoadWAV("Aud/Death.mp3"));
	}
	else return false;

	Mix_PlayMusic(m_pMusic, -1); // Play. -1 = looping.
	Mix_VolumeMusic(16); // 0-MIX_MAX_VOLUME (128).

	return true;
}

bool SideScrollerState::exit()
{
	for (unsigned int i = 0; i < m_backgrounds.size(); i++)
	{
		delete m_backgrounds[i];
		m_backgrounds[i] = nullptr;
	}
	m_backgrounds.clear();
	m_backgrounds.shrink_to_fit();

	for (int i = 0; i < (int)m_vSounds.size(); i++)
		Mix_FreeChunk(m_vSounds[i]);
	m_vSounds.clear();
	Mix_FreeMusic(m_pMusic);

	return true;
}

std::string SideScrollerState::getStateID()
{
	return stateID;
}
