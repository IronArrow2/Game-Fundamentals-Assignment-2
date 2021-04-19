#include <random>
#include <time.h>
#include "SideScrollerState.h"

SideScrollerState::SideScrollerState()
{
	m_iBSpawn = 0;
}

std::string SideScrollerState::update()
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
			m_obstacles.push_back(new Obstacle({0, 0, 124, 123}, {1024, 450, 124, 123}, 4));
			break;
		case 1:
			//ground blade 2
			m_obstacles.push_back(new Obstacle({ 124, 0, 127, 127 }, { 1024, 450, 127, 127 }, 4));
			break;
		case 2:
			//ground blade 3
			m_obstacles.push_back(new Obstacle({ 251, 0, 111, 111 }, { 1024, 450, 111, 111 }, 4));
			break;
		case 3:
			//ground spikes 1
			m_obstacles.push_back(new Obstacle({ 462, 0, 128, 76 }, { 1024, 445, 128, 76 }, 4));
			break;
		case 4:
			//ground spikes 2
			m_obstacles.push_back(new Obstacle({ 590, 0, 156, 81 }, { 1024, 445, 156, 81 }, 4));
			break;
		case 5:
			//overhead blade 1
			m_obstacles.push_back(new Obstacle({ 0, 260, 270, 210 }, { 1024, 230, 270, 210 }, 4));
			m_obstacles.push_back(new Obstacle({388, 0, 16, 260 }, { 1149, 0, 16, 260 }, 4));
			break;
		case 6:
			//overhead blade 2
			m_obstacles.push_back(new Obstacle({ 270, 260, 172, 193 }, { 1024, 250, 172, 193 }, 4));
			m_obstacles.push_back(new Obstacle({ 362, 0, 26, 212 }, { 1090, 68, 26, 212 }, 4));
			m_obstacles.push_back(new Obstacle({420, 0, 26, 115}, { 1090, -27, 26, 115 }, 4));
			break;
		case 7:
			//overhead blade 3
			m_obstacles.push_back(new Obstacle({ 442, 260, 186, 223 }, { 1024, 220, 186, 223 }, 4));
			m_obstacles.push_back(new Obstacle({ 388, 0, 16, 260 }, { 1110, -20, 16, 260 }, 4));
			break;
		case 8:
			//overhead blade 4
			m_obstacles.push_back(new Obstacle({ 628, 260, 366, 85 }, { 1024, 360, 366, 85 }, 4));
			m_obstacles.push_back(new Obstacle({ 362, 0, 26, 212 }, { 1128, 166, 26, 212 }, 4));
			m_obstacles.push_back(new Obstacle({ 362, 0, 26, 212 }, { 1128, -26, 26, 212 }, 4));
			m_obstacles.push_back(new Obstacle({ 362, 0, 26, 212 }, { 1248, 166, 26, 212 }, 4));
			m_obstacles.push_back(new Obstacle({ 362, 0, 26, 212 }, { 1248, -26, 26, 212 }, 4));
			break;
		}
		m_iBSpawn = 0;
	}

	m_pPlayer->Update();
	return "";
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
		}
		m_pPlayer->HandleEvents(event);
	}

	return "";
}

std::string SideScrollerState::checkCollision()
{
	return "";
}

bool SideScrollerState::enter(SDL_Window* window, SDL_Renderer* renderer)
{
	srand(time(NULL));
	m_pWindow = window;
	m_pRenderer = renderer;

	m_backgrounds.reserve(10);
	m_backgrounds.push_back(new Background({ 0,0,1024,768 }, { 0,0,1024,768 }, 1));
	m_backgrounds.push_back(new Background({ 0,0,1024,768 }, { 1024,0,1024,768 }, 1));
	m_backgrounds.push_back(new Background({ 1024,0,256,512 }, { 0,0,256,512 }, 3));
	m_backgrounds.push_back(new Background({ 1024,0,256,512 }, { 256,0,256,512 }, 3));
	m_backgrounds.push_back(new Background({ 1024,0,256,512 }, { 512,0,256,512 }, 3));
	m_backgrounds.push_back(new Background({ 1024,0,256,512 }, { 768,0,256,512 }, 3));
	m_backgrounds.push_back(new Background({ 1024,0,256,512 }, { 1024,0,256,512 }, 3));
	m_backgrounds.push_back(new Background({ 1024,512,512,256 }, { 0,512,512,256 }, 4));
	m_backgrounds.push_back(new Background({ 1024,512,512,256 }, { 512,512,512,256 }, 4));
	m_backgrounds.push_back(new Background({ 1024,512,512,256 }, { 1024,512,512,256 }, 4));

	m_pPlayer = new SideScrollerPlayer({1458, 0, 46, 76}, {100, 430, 46, 76});

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

	return true;
}

std::string SideScrollerState::getStateID()
{
	return stateID;
}
