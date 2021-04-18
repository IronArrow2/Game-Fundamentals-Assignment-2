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

void SideScrollerPlayer::crouch()
{
	//makes the player enter the slide animation
}

void SideScrollerPlayer::jump()
{
	//makes the player enter the jump animation
}

void SideScrollerPlayer::moveForward()
{
	//makes the player enter the movement animation and moves them forward
}

void SideScrollerPlayer::moveBack()
{
	//makes the player enter the move animation and moves backwards
}

void SideScrollerPlayer::stop()
{
	//makes the player enter the idle animation and stop moving
}

SideScrollerPlayer::SideScrollerPlayer(SDL_Rect s, SDL_Rect d) : /*Animated*/Player(s, d) {}

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
			m_rSrc = { 1946, 0, 55, 78 };
			m_rDst = { m_rDst.x, m_rDst.y, 55, 78 };
			break;
		case 7:
			m_rSrc = { 2032, 0, 62, 77 };
			m_rDst = { m_rDst.x, m_rDst.y, 62, 77 };
			break;
		case 8:
			m_rSrc = { 2107, 0, 54, 77 };
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
		break;
	case PlayerState::SLIDE:
		break;
	case PlayerState::DEAD:
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
}
