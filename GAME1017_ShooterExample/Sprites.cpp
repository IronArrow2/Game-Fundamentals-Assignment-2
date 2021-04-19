#include <algorithm> // For min/max.
#include <cmath>	 // For cos/sin.
#include "Sprites.h"
#include "Engine.h"
using namespace std;

Sprite::Sprite(){}
Sprite::Sprite(SDL_Rect s, SDL_Rect d) :m_rSrc(s), m_rDst(d) {}
SDL_Rect* Sprite::GetSrcP() { return &m_rSrc; }
SDL_Rect* Sprite::GetDstP() { return &m_rDst; }
double Sprite::GetAngle() { return m_angle; }

AnimatedSprite::AnimatedSprite(int a, int fm, int sm, SDL_Rect s, SDL_Rect d): 
	Sprite(s, d), m_iFrameMax(fm), m_iSpriteMax(sm)
{
	// Added this here (and not in header line) just because. Note: initializers ARE resolved before body statements.
	m_angle = a; 
	m_iSprite = m_iFrame = 0; // Chaining assignments to the same value.
}

void AnimatedSprite::Animate()
{
	m_iFrame++;
	if (m_iFrame == m_iFrameMax)
	{
		m_iFrame = 0;
		m_iSprite++;
		if (m_iSprite == m_iSpriteMax)
		{
			m_iSprite = 0;
		}
	}
	m_rSrc.x = m_rSrc.w * m_iSprite;
}

Player::Player(SDL_Rect s, SDL_Rect d) : /*Animated*/Sprite(s, d) {}

Bullet::Bullet(SDL_Rect s, SDL_Rect d, int spd) :
	Sprite(s, d), m_speed(spd), m_active(true) {}

void Bullet::Update()
{
	m_rDst.x += m_speed;
}

Enemy::Enemy(SDL_Rect s, SDL_Rect d, vector<Bullet*>* bVec, Mix_Chunk* c, int fr): /*Animated*/Sprite(s, d),
	m_pBulletVec(bVec), m_pPew(c), m_bulletTimer(0), m_timerMax(fr) {}

void Enemy::Update()
{
	//Animate();
	m_rDst.x -= 3;
	if (m_bulletTimer++ == m_timerMax)
	{
		m_bulletTimer = 0;
		m_pBulletVec->push_back(new Bullet( {843,977,13,37}, {m_rDst.x,m_rDst.y-28,13,37}, -10 ));
		Mix_PlayChannel(-1, m_pPew, 0);
	}
}

Button::Button(SDL_Rect s, SDL_Rect d)
{
	pressed = false;
	m_rSrc = s;
	m_rDst = d;
}

bool Button::handleEvent(SDL_Event* e)
{
	pressed = false;
	if (e->type == SDL_MOUSEBUTTONDOWN)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		pressed = true;

		//Mouse is left of the button
		if (x < m_rDst.x)
		{
			pressed = false;
		}
		//Mouse is right of the button
		else if (x > m_rDst.x + m_rDst.w)
		{
			pressed = false;
		}
		//Mouse above the button
		else if (y < m_rDst.y)
		{
			pressed = false;
		}
		//Mouse below the button
		else if (y > m_rDst.y + m_rDst.h)
		{
			pressed = false;
		}
		//else mouse is outside button
	}
	return pressed;
}

void Background::Update()
{
	// Check if at end of journey.
	if (GetDstP()->x <= m_endX) // Instead of m_endX: (m_startX - GetDst()->w)
		GetDstP()->x = m_startX;
	// Scroll it.
	GetDstP()->x -= m_scrollSpeed;
}

void Background::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer,TEMA::GetTexture("bg"), GetSrcP(), GetDstP());
}

void SideScrollerPlayer::slide()
{
	//makes the player enter the slide animation
	currentState = PlayerState::SLIDE;
	m_rDst = { m_rDst.x, m_rDst.y + 20, 77, 60 };
}

void SideScrollerPlayer::jump()
{
	//makes the player enter the jump animation and sends them upward
	currentState = PlayerState::JUMP;
	jumpingUp = true;
}

void SideScrollerPlayer::moveForward()
{
	//makes the player enter the movement animation and moves them forward
	switch (currentState)
	{
	case PlayerState::MOVE:
		m_rDst.x = m_rDst.x + 4;
		break;
	case PlayerState::JUMP:
		m_rDst.x = m_rDst.x + 2;
		break;//moving while jumping is more difficult
	case PlayerState::SLIDE:
		m_rDst.x = m_rDst.x + 2;
		break;//and while sliding
	case PlayerState::DEAD:
		break;//can't move if you're dead
	}
}

void SideScrollerPlayer::moveBack()
{
	//makes the player enter the move animation and moves backwards
	switch (currentState)
	{
	case PlayerState::MOVE:
		m_rDst.x = m_rDst.x - 4;
		break;
	case PlayerState::JUMP:
		m_rDst.x = m_rDst.x - 2;
		break;//moving while jumping is more difficult
	case PlayerState::SLIDE:
		m_rDst.x = m_rDst.x - 2;
		break;//and while sliding
	case PlayerState::DEAD:
		break;//can't move if you're dead
	}
}

void SideScrollerPlayer::stop()
{
	//makes the player enter the running animation
	if (currentState == PlayerState::SLIDE)
	{
		m_rDst = { m_rDst.x, m_rDst.y - 20, 46, 76 };
	}
	currentAnimFrame = 0;
	currentState = PlayerState::MOVE;
}

void SideScrollerPlayer::Update()
{
	//cycles through animation frames (changes source rectangle) depending on current state
	switch (currentState)
	{
	case PlayerState::MOVE:
		switch (currentAnimFrame)
		{
		case 0:
			m_rSrc = { 1458, 0, 46, 76 };
			m_rDst = {m_rDst.x, m_rDst.y, 46, 76};
			break;
		case 1:
			m_rSrc = { 1539, 0, 47, 76 };
			m_rDst = { m_rDst.x, m_rDst.y, 47, 76 };
			break;
		case 2:
			m_rSrc = { 1621, 0, 51, 76 };
			m_rDst = { m_rDst.x, m_rDst.y, 51, 76 };
			break;
		case 3:
			m_rSrc = { 1701, 0, 47, 76 };
			m_rDst = { m_rDst.x, m_rDst.y, 47, 76 };
			break;
		case 4:
			m_rSrc = { 1782, 0, 47, 76 };
			m_rDst = { m_rDst.x, m_rDst.y, 47, 76 };
			break;
		case 5:
			m_rSrc = { 1863, 0, 46, 76 };
			m_rDst = { m_rDst.x, m_rDst.y, 46, 76 };
			break;
		case 6:
			m_rSrc = { 1944, 0, 55, 78 };
			m_rDst = { m_rDst.x, m_rDst.y, 55, 78 };
			break;
		case 7:
			m_rSrc = { 2025, 0, 62, 77 };
			m_rDst = { m_rDst.x, m_rDst.y, 62, 77 };
			break;
		case 8:
			m_rSrc = { 2106, 0, 54, 77 };
			m_rDst = { m_rDst.x, m_rDst.y, 54, 77 };
			break;
		case 9:
			m_rSrc = { 2187, 0, 47, 76 };
			m_rDst = { m_rDst.x, m_rDst.y, 47, 76 };
			break;
		case 10:
			currentAnimFrame = -1;//reset animation to beginning
			m_rSrc = { 2268, 0, 46, 76 };
			m_rDst = { m_rDst.x, m_rDst.y, 46, 76 };
			break;
		}

		currentAnimFrame++;
		break;
	case PlayerState::JUMP:
		m_rSrc = { 1377, 0, 59, 77 };
		m_rDst = { m_rDst.x, m_rDst.y, 59, 77 };
		if (!(currentJumpHeight >= maxJumpHeight) && jumpingUp)
		{
			m_rDst.y = m_rDst.y - 3;
			currentJumpHeight = currentJumpHeight + 3;
		}
		else if (!(currentJumpHeight <= minJumpHeight) && !jumpingUp)
		{
			m_rDst.y = m_rDst.y + 3;
			currentJumpHeight = currentJumpHeight - 3;
		}
		else if (currentJumpHeight >= maxJumpHeight)
		{
			jumpingUp = false;
		}
		else if (currentJumpHeight <= minJumpHeight)
		{
			stop();
		}
		break;
	case PlayerState::SLIDE:
		currentAnimFrame = 0;
		m_rSrc = { 2349, 0, 77, 60 };
		m_rDst = { m_rDst.x, m_rDst.y, 77, 60 };
		break;
	case PlayerState::DEAD:
		if (currentAnimFrame == 0 || currentAnimFrame == 1 || currentAnimFrame == 2)
		{
			m_rSrc = { 0, 0, 45, 77 };
			m_rDst = { m_rDst.x, m_rDst.y, 45, 77 };
		}
		else if (currentAnimFrame == 3 || currentAnimFrame == 4 || currentAnimFrame == 5)
		{
			m_rSrc = { 81, 0, 47, 77 };
			m_rDst = { m_rDst.x, m_rDst.y, 47, 77 };
		}
		else if (currentAnimFrame == 6 || currentAnimFrame == 7 || currentAnimFrame == 8)
		{
			m_rSrc = { 162, 0, 49, 73 };
			m_rDst = { m_rDst.x, m_rDst.y + 1, 49, 73 };
		}
		else if (currentAnimFrame == 9 || currentAnimFrame == 10 || currentAnimFrame == 11)
		{
			m_rSrc = { 243, 0, 48, 71 };
			m_rDst = { m_rDst.x, m_rDst.y, 48, 71 };
		}
		else if (currentAnimFrame == 12 || currentAnimFrame == 13 || currentAnimFrame == 14)
		{
			m_rSrc = { 324, 0, 47, 69 };
			m_rDst = { m_rDst.x, m_rDst.y + 1, 47, 69 };
		}
		else if (currentAnimFrame == 15 || currentAnimFrame == 16 || currentAnimFrame == 17)
		{
			m_rSrc = { 405, 0, 46, 68 };
			m_rDst = { m_rDst.x, m_rDst.y, 46, 68 };
		}
		else if (currentAnimFrame == 18 || currentAnimFrame == 19 || currentAnimFrame == 20)
		{
			m_rSrc = { 486, 0, 46, 68 };
			m_rDst = { m_rDst.x, m_rDst.y, 46, 68 };
		}
		else if (currentAnimFrame == 21 || currentAnimFrame == 22 || currentAnimFrame == 23)
		{
			m_rSrc = { 567, 0, 47, 69 };
			m_rDst = { m_rDst.x, m_rDst.y, 47, 69 };
		}
		else if (currentAnimFrame == 24 || currentAnimFrame == 25 || currentAnimFrame == 26)
		{
			m_rSrc = { 648, 0, 50, 67 };
			m_rDst = { m_rDst.x, m_rDst.y, 50, 67 };
		}
		else if (currentAnimFrame == 27 || currentAnimFrame == 28 || currentAnimFrame == 29)
		{
			m_rSrc = { 729, 0, 62, 62 };
			m_rDst = { m_rDst.x, m_rDst.y + 1, 62, 62 };
		}
		else if (currentAnimFrame == 30 || currentAnimFrame == 31 || currentAnimFrame == 32)
		{
			m_rSrc = { 810, 0, 71, 61 };
			m_rDst = { m_rDst.x, m_rDst.y + 1, 71, 61 };
		}
		else if (currentAnimFrame == 33 || currentAnimFrame == 34 || currentAnimFrame == 35)
		{
			m_rSrc = { 892, 0, 76, 58 };
			m_rDst = { m_rDst.x, m_rDst.y + 1, 76, 58 };
		}
		else if (currentAnimFrame == 36 || currentAnimFrame == 37 || currentAnimFrame == 38)
		{
			m_rSrc = { 972, 0, 78, 55 };
			m_rDst = { m_rDst.x, m_rDst.y + 1, 78, 55 };
		}
		else if (currentAnimFrame == 39 || currentAnimFrame == 40 || currentAnimFrame == 41)
		{
			m_rSrc = { 1054, 0, 81, 50 };
			m_rDst = { m_rDst.x, m_rDst.y + 1, 81, 50 };
		}
		else if (currentAnimFrame == 42 || currentAnimFrame == 43 || currentAnimFrame == 44)
		{
			m_rSrc = { 1134, 0, 81, 48 };
			m_rDst = { m_rDst.x, m_rDst.y + 1, 81, 48 };
		}
		else if (currentAnimFrame == 45 || currentAnimFrame == 46 || currentAnimFrame == 47)
		{
			m_rSrc = { 1215, 0, 80, 46 };
			m_rDst = { m_rDst.x, m_rDst.y + 1, 80, 46 };
		}
		else if (currentAnimFrame == 48 || currentAnimFrame == 49 || currentAnimFrame == 50)
		{
			m_rSrc = { 1296, 0, 80, 45 };
			m_rDst = { m_rDst.x, m_rDst.y + 1, 80, 45 };
		}
		else
		{
			m_rSrc = { 1296, 0, 80, 45 };
			m_rDst = { m_rDst.x, m_rDst.y, 80, 45 };
		}

		if (!(currentAnimFrame >= 51))
		{
			currentAnimFrame++;
		}
		break;
	}
	
}

void SideScrollerPlayer::Render(SDL_Renderer* renderer)
{
	//renders the current frame of animation to the screen
	SDL_RenderCopy(renderer, TEMA::GetTexture("plr"), GetSrcP(), GetDstP());
}

void SideScrollerPlayer::HandleEvents(SDL_Event event)
{
	//takes in an event and does something with it
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_a)
		{
			moveBack();
		}
		else if (event.key.keysym.sym == SDLK_d)
		{
			moveForward();
		}
		else if ((event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_SPACE) && currentState != PlayerState::JUMP)
		{
			jump();
		}
		else if (event.key.keysym.sym == SDLK_s && currentState != PlayerState::SLIDE)
		{
			slide();
		}
	}
	else if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.sym == SDLK_s)
		{
			stop();
		}
	}
}

void SideScrollerPlayer::die()
{
	currentState = PlayerState::DEAD;
}

void Obstacle::Update()
{
	// Scroll it.
	GetDstP()->x -= m_scrollSpeed;
}

void Obstacle::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, TEMA::GetTexture("obs"), GetSrcP(), GetDstP());
}
