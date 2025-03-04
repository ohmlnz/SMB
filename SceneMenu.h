#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include "Action.h"

class SceneMenu : public Scene
{
private:
	sf::Text					m_mainTitle;
	sf::Text					m_menuElements[4];
	int							m_selectedMenu = 1;
public:
	SceneMenu(class GameEngine* gameEngine)
		: Scene(gameEngine)
	{
		init();
	};
	
	void init();
	
	void process(const Action& action);
	void update();
	void render();
};