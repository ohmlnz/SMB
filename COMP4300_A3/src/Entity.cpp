#include "Entity.h"

void Entity::destroy()
{
	m_active = false;
};

bool Entity::isActive()
{
	return m_active;
}

size_t& Entity::id()
{
	return m_id;
}

std::string& Entity::tag()
{
	return m_tag;
}