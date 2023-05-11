#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Animation.h"

class Base {
public:
	bool has = false;
};

class CTransform : public Base
{
public:
	Vec2 position			= { 0, 0 };
	Vec2 velocity			= { 0, 0 };
	Vec2 scale				= { 1, 1 };
	Vec2 overlap			= { 0, 0 };
	Vec2 previousPosition	= { 0, 0 };
	CTransform() {};
	CTransform(Vec2 position)
		: position(position)
	{}
	CTransform(Vec2 position, Vec2 velocity)
		: position(position)
		, velocity(velocity)
	{};
};

class CInput : public Base
{
public:
	CInput() {};
};

class CLifeSpan : public Base
{
public:
	int life;
	int currentLife = 0;
	CLifeSpan() {};
	CLifeSpan(const int life)
		: life(life)
	{};
};

class CBoundingBox : public Base
{
public:
	Vec2 size = { 0, 0 };
	Vec2 halfSize = { 0, 0 };
	CBoundingBox() {};
	CBoundingBox(const Vec2& size)
		: size(size)
		, halfSize(size.x / 2, size.y / 2)
	{};
};

class CAnimation : public Base
{
public:
	Animation animation;
	Vec2 midPoint = { 0, 0 };

	CAnimation() {};
	CAnimation(const Animation& animation)
		: animation(animation)
		, midPoint(Vec2(animation.getSize().x / 2, animation.getSize().y / 2))
	{};
};

class CGravity : public Base
{
public:
	float gravity = 0;
	CGravity() {};
	CGravity(float gravity)
		: gravity(gravity)
	{};
};

class CState : public Base
{

public:
	enum State {
		STANDING,
		JUMPING,
		WALKING,
		SHOOTING
	};

	bool shoot = false; 
	bool up = false;
	bool isJumping = false;
	bool left = false;
	bool right = false;
	short int initialJumpPosition = 0;
	// TODO: find better way
	std::string currentDirection = "right";

	State currentState = State::STANDING;
	State previousState;
	CState() {};
};