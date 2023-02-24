#pragma once
#include "Components.h"
#include <string>
#include <tuple>

class EntityManger;

typedef std::tuple<
	CTransform,
	CLifeSpan,
	CInput,
	CBoundingBox,
	CAnimation,
	CGravity,
	CState
> ComponentTuple;

class Entity
{
private:
	size_t			m_id		= 0;
	std::string		m_tag		= "Default";
	bool			m_active	= true;
	ComponentTuple  m_components;
	

public:
	// TODO: make this constructor private
	Entity(const size_t& id, const std::string& tag) 
		: m_id(id)
		, m_tag(tag)
	{}
	void			destroy();
	bool			isActive();
	size_t&			id();
	std::string&	tag();

	template <typename T>
	bool hasComponent()
	{
		return getComponent<T>().has;
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template <typename T>
	T& getComponent()
	{
		return std::get<T>(m_components);
	}

	template <typename T>
	void removeComponent()
	{
		getComponent<T>() = T();
	}
};