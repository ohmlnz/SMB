#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"

class Base {
public:
	bool has = false;
};

class CTransform : public Base
{
public:
	Vec2 position = { 0, 0 };
	Vec2 prevPosition = { 0, 0 };
	Vec2 velocity = { 0, 0 };
	Vec2 scale = { 1, 1 };
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
	sf::Sprite sprite;
	std::string name;
	Vec2 size;
	Vec2 midPoint = { 0, 0 };

	CAnimation() {};
	CAnimation(const std::string& name, const Vec2& size, const sf::Sprite& sprite)
		: name(name)
		, size(size)
		, sprite(sprite)
		, midPoint(size.x / 2, size.y / 2)
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
	std::string currentDirection	= "right";
	bool right						= false;
	bool left						= false;
	bool jump						= false;
	bool shoot						= false;
	bool up							= false;
	bool down						= false;
	CState() {};
};