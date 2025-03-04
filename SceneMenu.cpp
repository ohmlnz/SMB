#include "SceneMenu.h"
#include "GameEngine.h"

void SceneMenu::init()
{
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::Down, "DOWN");
	registerAction(sf::Keyboard::Enter, "PLAY");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_mainTitle.setFont(m_gameEngine->assets().getFont("Mario"));
	m_mainTitle.setCharacterSize(65);
	m_mainTitle.setFillColor(sf::Color::White);
	m_mainTitle.setString("Super Megaman Bros!");
	m_mainTitle.setOrigin(m_mainTitle.getGlobalBounds().width / 2, 0);
	m_mainTitle.setPosition(m_gameEngine->currentWindow().getSize().x / 2.0, 20);

	for (size_t i = 0; i < sizeof(m_menuElements) / sizeof(m_menuElements[0]); i++)
	{
		m_menuElements[i].setFont(m_gameEngine->assets().getFont("Mario"));
		m_menuElements[i].setCharacterSize(35);
		m_menuElements[i].setFillColor(sf::Color::White);

		if (i == 0)
		{
			m_menuElements[i].setString("Select a level:");
			m_menuElements[i].setPosition(15, 200);
			continue;
		}

		std::string text = "Level " + std::to_string(i);
		m_menuElements[i].setString(text);

		m_menuElements[i].setPosition(25, 200 + (100 * (i + 1 / 2)));
	}
}

void SceneMenu::process(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "PLAY")
		{
			if (m_selectedMenu == 1)
			{
				m_gameEngine->changeScene("LevelOne", std::make_shared<SceneLevelOne>(m_gameEngine, "../bin/level1.txt"));
			}
		}
		else if (action.name() == "UP")
		{
			m_selectedMenu--;
			
			if (m_selectedMenu == 0)
			{
				m_selectedMenu = 3;
			}
		}
		else if (action.name() == "DOWN")
		{
			m_selectedMenu++;

			if (m_selectedMenu == 4)
			{
				m_selectedMenu = 1;
			}
		}
	}
};

void SceneMenu::update()
{
	for (size_t i = 0; i < sizeof(m_menuElements) / sizeof(m_menuElements[0]); i++)
	{
		if (i == m_selectedMenu)
		{
			m_menuElements[m_selectedMenu].setFillColor(sf::Color::Green);
		}
		else
		{
			m_menuElements[i].setFillColor(sf::Color::White);
		}
	}
};


void SceneMenu::render()
{
	m_gameEngine->currentWindow().draw(m_mainTitle);
	
	for (const auto& element : m_menuElements)
	{
		m_gameEngine->currentWindow().draw(element);
	}
}