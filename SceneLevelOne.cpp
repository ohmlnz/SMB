#include "SceneLevelOne.h"
#include "GameEngine.h"

// TODO
// 1. Sounds (shoot, menu selection, background music)
// 2. Editor
// 3. Game over

const short int MAX_JUMP_HEIGHT = 100;

void SceneLevelOne::init(const std::string& assetsPath)
{
	registerAction(sf::Keyboard::A,			"LEFT");
	registerAction(sf::Keyboard::D,			"RIGHT");
	registerAction(sf::Keyboard::Space,		"SHOOT");
	registerAction(sf::Keyboard::W,			"JUMP");
	registerAction(sf::Keyboard::Escape,	"PAUSE");
	registerAction(sf::Keyboard::P,			"DEBUG");

	loadLevel(assetsPath);
}

Vec2 SceneLevelOne::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	const Vec2& midPoint = entity->getComponent<CAnimation>().midPoint;
	sf::Sprite& sprite = entity->getComponent<CAnimation>().animation.getSprite();

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
			entity->addComponent<CAnimation>(animation);

			entity->addComponent<CTransform>(gridToMidPixel(m_playerInfo.PX, m_playerInfo.PY, entity), Vec2(m_playerInfo.SX, m_playerInfo.SY));

			entity->addComponent<CBoundingBox>(Vec2(m_playerInfo.CW, m_playerInfo.CH));
			
			entity->addComponent<CGravity>(m_playerInfo.GY);
			entity->addComponent<CState>();
		}
		else
		{
			inFile >> m_asset.animation >> m_asset.position.x >> m_asset.position.y;
			auto entity = m_entityManager.addEntity(m_asset.type);
	
			Animation& animation = m_gameEngine->assets().getAnimation(m_asset.animation);
			entity->addComponent<CAnimation>(animation);

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
	// TODO: lots of object creation -- solve this
	m_currentAction = action;

	if (action.type() == "START")
	{
		if (action.name() == "DEBUG")
		{
			m_debugMode = !m_debugMode;
		}
	}

	else if (action.type() == "END")
	{
		if (action.name() == "SHOOT")
		{
			m_player->getComponent<CState>().shoot = true;
		}
	}
};

void SceneLevelOne::update()
{
	m_entityManager.update();

	sHandleState();
	sHandleShooting();
	sHandleTransform();
	sHandleGravity();
	sHandleCollisions();
	sHandleLife();
	sHandleCamera();

	m_currentFrame++;
};

void SceneLevelOne::sHandleGravity()
{
	for (auto entity : m_entityManager.getEntities())
	{
		if (entity->hasComponent<CGravity>() && entity->hasComponent<CState>())
		{
			if (!entity->getComponent<CState>().up)
			{
				entity->getComponent<CTransform>().position.y += entity->getComponent<CTransform>().velocity.y;
				entity->getComponent<CTransform>().velocity.y += entity->getComponent<CGravity>().gravity;
			}
		}
	}
}

void SceneLevelOne::sHandleTransform()
{
	for (auto entity : m_entityManager.getEntities())
	{
		if (entity->hasComponent<CTransform>())
		{
			sf::Sprite& sprite = entity->getComponent<CAnimation>().animation.getSprite();
			Vec2& position = entity->getComponent<CTransform>().position;
			Vec2& velocity = entity->getComponent<CTransform>().velocity;
			entity->getComponent<CTransform>().previousPosition = position;

			if (entity->hasComponent<CState>())
			{
				if (entity->getComponent<CState>().right)
				{
					entity->getComponent<CTransform>().scale.x = 1;
					position.x += velocity.x;
				}
				else if (entity->getComponent<CState>().left)
				{
					entity->getComponent<CTransform>().scale.x = -1;
					position.x -= velocity.x;
				}
				
				if (entity->getComponent<CState>().up)
				{
					position.y -= velocity.y;

					if (abs(position.y - entity->getComponent<CState>().initialJumpPosition) >= MAX_JUMP_HEIGHT)
					{
						m_player->getComponent<CState>().up = false;
					}
				}
			}

			if (entity->tag() != "Player")
			{
				position += velocity;
			}

			// TODO: add delta time
			const Vec2& scale = entity->getComponent<CTransform>().scale;
			sprite.setScale(scale.x, scale.y);
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
			if (m_physics.isCollision(m_player, entity))
			{
				short int collisionDirection = m_physics.resolveOverlap(m_player, entity);
				
				// if player lands on top of another entity
				if (collisionDirection == 2)
				{
					// reset y velocity back to its initial value
					m_player->getComponent<CTransform>().velocity.y = m_playerInfo.SY;

					//if (m_player->getComponent<CAnimation>().animation.getName() != "Stand")
					//{
					//	Animation& animation = m_gameEngine->assets().getAnimation("Stand");
					//	m_player->getComponent<CAnimation>().animation = animation;
					//}

					m_player->getComponent<CState>().isJumping = false;
				}
				else if (m_player->getComponent<CState>().up)
				{
					m_player->getComponent<CState>().up = false;
				}

				if (collisionDirection == 3 && entity->getComponent<CAnimation>().animation.getName() == "Brick")
				{
					entity->destroy();
				}
			}

			for (auto bullet : m_entityManager.getEntities("Buster"))
			{
				if (m_physics.isCollision(bullet, entity)) {
					bullet->destroy();
					entity->destroy();
				}
			}
		}
	}
}

void SceneLevelOne::sHandleShooting()
{
	const short int BULLET_SPEED = 10;

	if (m_player->getComponent<CState>().shoot)
	{
		auto entity = m_entityManager.addEntity("Buster");

		entity->addComponent<CLifeSpan>(60);

		Animation& animation = m_gameEngine->assets().getAnimation("Buster");
		entity->addComponent<CAnimation>(animation);

		const Vec2& playerPosition = m_player->getComponent<CTransform>().position;
		const Vec2& playerSize = m_player->getComponent<CAnimation>().animation.getSize();

		bool isPlayerGoingRight = m_player->getComponent<CState>().currentDirection == "right";
		entity->addComponent<CTransform>(
			Vec2(playerPosition.x, playerPosition.y),
			Vec2(isPlayerGoingRight ? BULLET_SPEED : -BULLET_SPEED, 0));
		 
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

void SceneLevelOne::setAnimation(const std::string& animationName)
{
	Animation& currentAnimation = m_player->getComponent<CAnimation>().animation;
	Animation& animation = m_gameEngine->assets().getAnimation(animationName);

	if (currentAnimation.getName() != animationName)
	{
		currentAnimation = animation;
	}
}

void SceneLevelOne::sHandleState()
{
	CState::State& state = m_player->getComponent<CState>().currentState;

	switch (state)
	{
		case CState::State::STANDING:
		{
			if (m_currentAction.type() == "START")
			{
				if (m_currentAction.name() == "JUMP")
				{
					if (!m_player->getComponent<CState>().isJumping)
					{
						m_player->getComponent<CState>().isJumping = true;
						m_player->getComponent<CState>().initialJumpPosition = m_player->getComponent<CTransform>().position.y;
						m_player->getComponent<CState>().up = true;
						m_player->getComponent<CState>().previousState = CState::State::STANDING;
						state = CState::State::JUMPING;
						setAnimation("Air");
					}
				} 
				else if (m_currentAction.name() == "LEFT")
				{
					m_player->getComponent<CState>().currentDirection = "left";
					m_player->getComponent<CState>().left = true;
					state = CState::State::WALKING;
					setAnimation("Run");
				}
				else if (m_currentAction.name() == "RIGHT")
				{
					m_player->getComponent<CState>().currentDirection = "right";
					m_player->getComponent<CState>().right = true;
					state = CState::State::WALKING;
					setAnimation("Run");
				}
			}
			else if (m_currentAction.type() == "END")
			{
				if (m_currentAction.name() == "LEFT")
				{
					m_player->getComponent<CState>().left = false;
					state = CState::State::STANDING;
					setAnimation("Stand");
				}
				else if (m_currentAction.name() == "RIGHT")
				{
					m_player->getComponent<CState>().right = false;
					state = CState::State::STANDING;
					setAnimation("Stand");
				}
			}

			break;
		}
		case CState::State::JUMPING:
		{
			if (!m_player->getComponent<CState>().isJumping)
			{
				if (m_player->getComponent<CState>().previousState == CState::State::WALKING)
				{
					state = CState::State::WALKING;
					setAnimation("Run");
				}
				else
				{
					state = CState::State::STANDING;
					setAnimation("Stand");
				}
			}

			break;
		}
		case CState::State::WALKING:
		{
			if (m_currentAction.type() == "START")
			{
				if (m_currentAction.name() == "JUMP")
				{
					m_player->getComponent<CState>().isJumping = true;
					m_player->getComponent<CState>().initialJumpPosition = m_player->getComponent<CTransform>().position.y;
					m_player->getComponent<CState>().up = true;
					m_player->getComponent<CState>().previousState = CState::State::WALKING;
					state = CState::State::JUMPING;
					setAnimation("Air");
				}
			}
			else if (m_currentAction.type() == "END")
			{
				if (m_currentAction.name() == "LEFT")
				{
					m_player->getComponent<CState>().left = false;
					state = CState::State::STANDING;
					setAnimation("Stand");
				}
				else if (m_currentAction.name() == "RIGHT")
				{
					m_player->getComponent<CState>().right = false;
					state = CState::State::STANDING;
					setAnimation("Stand");
				}
			}

			break;
		}
	}
}

void SceneLevelOne::sHandleCamera()
{
	auto playerPosition = m_player->getComponent<CTransform>().position;
	if (playerPosition.x > (m_gameEngine->currentWindow().getSize().x / 2))
	{
		m_gameEngine->currentView().setCenter(playerPosition.x, m_gameEngine->currentWindow().getSize().y / 2);
		m_gameEngine->currentWindow().setView(m_gameEngine->currentView());
	}
}

void SceneLevelOne::render()
{
	for (auto entity : m_entityManager.getEntities())
	{
		entity->getComponent<CAnimation>().animation.update();
		m_gameEngine->currentWindow().draw(entity->getComponent<CAnimation>().animation.getSprite());

		// TODO: move this into its own Debug class or add to Physics class
		if (m_debugMode)
		{
			// draws rectangle for each entity's bounding box
			auto position = entity->getComponent<CTransform>().position;
			auto size = entity->getComponent<CBoundingBox>().size;
			auto animation = entity->getComponent<CAnimation>().animation;
		
			sf::RectangleShape DEBUG_rectangle(sf::Vector2f(size.x, size.y));
			DEBUG_rectangle.setFillColor(sf::Color::Transparent);
			DEBUG_rectangle.setOutlineColor(sf::Color::White);
			DEBUG_rectangle.setOutlineThickness(2.0f);
		
			DEBUG_rectangle.setPosition(position.x, position.y);
			DEBUG_rectangle.setOrigin(size.x / 2, size.y / 2);
		
			m_gameEngine->currentWindow().draw(DEBUG_rectangle);
		}
	}
}