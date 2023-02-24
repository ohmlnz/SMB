#pragma once
#include<SFML/Window.hpp>
#include <memory>
#include <iostream>
#include <map>
#include "Action.h"
#include "EntityManager.h"

class Scene
{
protected:
	class GameEngine*			m_gameEngine		= nullptr;
	EntityManager				m_entityManager;
	int							m_currentFrame		= 0;
	std::map<int, std::string>	m_actionMap;
public:
	Scene(class GameEngine* gameEngine)
		: m_gameEngine(gameEngine)
	{}	

	virtual void process(const Action& action) = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	void registerAction(sf::Keyboard::Key inputKey, const std::string& type)
	{
		m_actionMap[inputKey] = type;
	};

	std::map<int, std::string>& getActionMap()
	{
		return m_actionMap;
	};
};