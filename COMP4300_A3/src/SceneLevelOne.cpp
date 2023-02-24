#include "SceneLevelOne.h"
#include "GameEngine.h"

// TODO
// 1. Actual animations on all entities
// 2. add physics class
// 3. Jump animation
// 4. Camera
// 5. Editor
// 6. Sounds
// 7. Overall UI revamping

void SceneLevelOne::init(const std::string& assetsPath)
{
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::Space, "SHOOT");
	registerAction(sf::Keyboard::W, "JUMP");
	registerAction(sf::Keyboard::Escape, "PAUSE");

	loadLevel(assetsPath);
}

Vec2 SceneLevelOne::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	const Vec2& midPoint = entity->getComponent<CAnimation>().midPoint;
	sf::Sprite& sprite = entity->getComponent<CAnimation>().sprite;

	sprite.setOrigin(midPoint.x, midPoint.y);
	
	float positionX = gridX * m_gridSize.x + midPoint.x;
	float positionY = m_gameEngine->currentWindow().getSize().y - (gridY * m_gridSize.y) - midPoint.y;
	return Vec2(positionX, positionY);
}

void SceneLevelOne::loadLevel(const std::string& assetsPath)
{
	// reset entity manager
	m_entityManager = EntityManager();

	std::ifstream inFile(assetsPath);

	while (inFile >> m_asset.type)
	{
		if (m_asset.type == "Player")
		{
			inFile >> m_playerInfo.PX >> m_playerInfo.PY >> m_playerInfo.CW >> m_playerInfo.CH >>
			m_playerInfo.SX >> m_playerInfo.SY >> m_playerInfo.SM >> m_playerInfo.GY >> m_playerInfo.B;
			
			auto entity = m_entityManager.addEntity(m_asset.type);
			m_player = entity;

			Animation& animation = m_gameEngine->assets().getAnimation(m_playerInfo.B);
			entity->addComponent<CAnimation>(animation.getName(), animation.getSize(), animation.getSprite());

			entity->addComponent<CTransform>(gridToMidPixel(m_playerInfo.PX, m_playerInfo.PY, entity));

			entity->addComponent<CBoundingBox>(Vec2(80, 80));
			
			entity->addComponent<CGravity>(m_playerInfo.GY);
			entity->addComponent<CState>();
		}
		else
		{
			inFile >> m_asset.animation >> m_asset.position.x >> m_asset.position.y;
			auto entity = m_entityManager.addEntity(m_asset.type);
	
			Animation& animation = m_gameEngine->assets().getAnimation(m_asset.animation);
			entity->addComponent<CAnimation>(animation.getName(), animation.getSize(), animation.getSprite());

			entity->addComponent<CTransform>(gridToMidPixel(m_asset.position.x, m_asset.position.y, entity));


			if (m_asset.type == "Tile")
			{
				const Vec2& size = m_gameEngine->assets().getAnimation(m_asset.animation).getSize();
				entity->addComponent<CBoundingBox>(size);
			}		
		}
	}
}

void SceneLevelOne::process(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "RIGHT")
		{
			m_player->getComponent<CState>().right = true;
			m_player->getComponent<CState>().currentDirection = "right";
		}
		else if (action.name() == "LEFT")
		{
			m_player->getComponent<CState>().left = true;
			m_player->getComponent<CState>().currentDirection = "left";
		}
		else if (action.name() == "JUMP")
		{
			m_player->getComponent<CState>().jump = true;
		}
	}
	else if (action.type() == "END")
	{
		if (action.name() == "RIGHT")
		{
			m_player->getComponent<CState>().right = false;
		}
		else if (action.name() == "LEFT")
		{
			m_player->getComponent<CState>().left = false;
		}
		else if (action.name() == "JUMP")
		{
			m_player->getComponent<CState>().jump = false;
		}
		else if (action.name() == "SHOOT")
		{
			m_player->getComponent<CState>().shoot = true;
		}
	}
};

void SceneLevelOne::update()
{
	m_entityManager.update();

	sHandleShooting();
	sHandleTransform();
	sHandleGravity();
	sHandleCollisions();
	sHandleLife();

	m_currentFrame++;
};

void SceneLevelOne::sHandleGravity()
{
	for (auto entity : m_entityManager.getEntities())
	{
		if (entity->hasComponent<CGravity>() && entity->hasComponent<CState>())
		{
			entity->getComponent<CTransform>().velocity.y += entity->getComponent<CGravity>().gravity;
			entity->getComponent<CTransform>().position.y += entity->getComponent<CTransform>().velocity.y;
		}
	}
}

void SceneLevelOne::sHandleTransform()
{
	for (auto entity : m_entityManager.getEntities())
	{
		
		if (entity->hasComponent<CTransform>())
		{
			sf::Sprite& sprite = entity->getComponent<CAnimation>().sprite;
			Vec2& position = entity->getComponent<CTransform>().position;
			entity->getComponent<CTransform>().prevPosition = position;

			if (entity->hasComponent<CState>())
			{
		
				if (entity->getComponent<CState>().right)
				{
					entity->getComponent<CTransform>().scale.x = 1;
					position.x += m_playerInfo.SX;
				}
				else if (entity->getComponent<CState>().left)
				{
					entity->getComponent<CTransform>().scale.x = -1;
					position.x -= m_playerInfo.SX;
				}
				// TODO: remove
				else if (entity->getComponent<CState>().up)
				{
					position.y -= m_playerInfo.SX;
				}
				else if (entity->getComponent<CState>().down)
				{
					position.y += m_playerInfo.SX;
				}
			}

			// TODO: add delta time
			const Vec2& scale = entity->getComponent<CTransform>().scale;
			sprite.setScale(scale.x, scale.y);
			position += entity->getComponent<CTransform>().velocity;
			sprite.setPosition(position.x, position.y);
		}
	}
}

void SceneLevelOne::sHandleCollisions()
{
	for (auto entity : m_entityManager.getEntities())
	{
		if (entity->tag() == "Player" || entity->tag() == "Buster")
		{
			continue;
		}

		if (entity->hasComponent<CBoundingBox>())
		{
			// TODO: move into physics component
			const Vec2& entityPos = entity->getComponent<CTransform>().position;
			const Vec2& playerPrevPosition = m_player->getComponent<CTransform>().prevPosition;
			const Vec2& playerPosition = m_player->getComponent<CTransform>().position;
			const Vec2& entityHalf = entity->getComponent<CBoundingBox>().halfSize;
			const Vec2& playerHalf = m_player->getComponent<CBoundingBox>().halfSize;
			
			Vec2 prevDelta(abs(entityPos.x - playerPrevPosition.x), abs(entityPos.y - playerPrevPosition.y));
			Vec2 prevOverlap(entityHalf.x + playerHalf.x - prevDelta.x, entityHalf.y + playerHalf.y - prevDelta.y);
			Vec2 delta(abs(entityPos.x - playerPosition.x), abs(entityPos.y - playerPosition.y));
			Vec2 overlap(entityHalf.x + playerHalf.x - delta.x, entityHalf.y + playerHalf.y - delta.y);
			
			if (overlap.x > 0 && overlap.y > 0)
			{

				if (prevOverlap.y > 0)
				{
					if (playerPrevPosition.x > playerPosition.x)
					{
						m_player->getComponent<CTransform>().position.x += overlap.x;
					}
					else
					{
						m_player->getComponent<CTransform>().position.x -= overlap.x;
					}
				}
				else if (prevOverlap.x > 0)
				{
					if (playerPrevPosition.y > playerPosition.y)
					{
						m_player->getComponent<CTransform>().position.y += overlap.y;
					}
					else
					{
						m_player->getComponent<CTransform>().position.y -= overlap.y;
						m_player->getComponent<CTransform>().velocity.y = 0;
					}
				}
			}

			for (auto bullet : m_entityManager.getEntities("Buster"))
			{
				// TODO: move into physics component
				const Vec2& entityPos = entity->getComponent<CTransform>().position;
				const Vec2& bulletPosition = bullet->getComponent<CTransform>().position;
				const Vec2& entityHalf = entity->getComponent<CBoundingBox>().halfSize;
				const Vec2& bulletHalf = bullet->getComponent<CBoundingBox>().halfSize;

				Vec2 delta(abs(entityPos.x - bulletPosition.x), abs(entityPos.y - bulletPosition.y));
				Vec2 overlap(entityHalf.x + bulletHalf.x - delta.x, entityHalf.y + bulletHalf.y - delta.y);

				if (overlap.x > 0 && overlap.y > 0)
				{
					bullet->destroy();
					entity->destroy();
				}
			}
		}
	}
}

void SceneLevelOne::sHandleShooting()
{
	if (m_player->getComponent<CState>().shoot)
	{
		auto entity = m_entityManager.addEntity("Buster");

		entity->addComponent<CLifeSpan>(100);

		Animation& animation = m_gameEngine->assets().getAnimation("Buster");
		entity->addComponent<CAnimation>(animation.getName(), animation.getSize(), animation.getSprite());

		const Vec2& playerPosition = m_player->getComponent<CTransform>().position;
		const Vec2& playerSize = m_player->getComponent<CAnimation>().size;

		bool isPlayerGoingRight = m_player->getComponent<CState>().currentDirection == "right";
		entity->addComponent<CTransform>(
			Vec2(playerPosition.x - (playerSize / 2).x, playerPosition.y - (playerSize / 2).y),
			Vec2(isPlayerGoingRight ? 5 : -5, 0));

		entity->addComponent<CBoundingBox>(Vec2(8, 8));

		m_player->getComponent<CState>().shoot = false;
	}
}

void SceneLevelOne::sHandleLife()
{
	for (auto entity : m_entityManager.getEntities())
	{
		if (entity->hasComponent<CLifeSpan>())
		{
			entity->getComponent<CLifeSpan>().currentLife++;

			if (entity->getComponent<CLifeSpan>().currentLife >= entity->getComponent<CLifeSpan>().life)
			{
				entity->destroy();
			}
		}
	}
}

void SceneLevelOne::render()
{
	for (auto entity : m_entityManager.getEntities())
	{
		m_gameEngine->currentWindow().draw(entity->getComponent<CAnimation>().sprite);
	}
}