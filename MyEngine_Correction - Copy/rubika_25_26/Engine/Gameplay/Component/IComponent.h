#pragma once

#include <string>

class Entity;

class IComponent
{
public:
	IComponent() = delete;
	IComponent(Entity& entity);

	virtual ~IComponent();

	virtual void Start() = 0;

	virtual void Update(float fDeltaTime) = 0;

	virtual void Destroy() = 0;

	bool NeedUpdate() const;

	static IComponent* FactoryByName(const char* serializeToken, Entity& entity);

protected:
	Entity& GetEntity() const;

private:
	Entity& ParentEntity;
};
