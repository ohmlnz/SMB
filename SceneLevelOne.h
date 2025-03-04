#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Physics.h"

struct Player {
	float PX, PY, CW, CH, SX, SY, SM, GY;
	std::string B;
};

class SceneLevelOne : public Scene
{

struct Asset
{
	std::string type, animation;
	Vec2 position;
};

private:
	std::shared_ptr<Entity>		m_player;
	Asset						m_asset;
	Vec2						m_gridSize = { 64, 64 };
	Player						m_playerInfo;
	Physics                     m_physics;
	Action						m_currentAction;

public:
	SceneLevelOne(class GameEngine* gameEngine, const std::string& assetsPath)
		: Scene(gameEngine)
	{
		init(assetsPath);
	};
	void init(const std::string& assetsPath);

	void process(const Action& action);
	void update();
	void render();
	
	void sHandleGravity();
	void sHandleTransform();
	void sHandleCollisions();
	void sHandleShooting();
	void sHandleLife();
	void sHandleState();
	void sHandleCamera();

	void setAnimation(const std::string& animationName);
	void loadLevel(const std::string& assetsPath);
	Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);
};