#pragma once

#include <string>

#include "IComponent.h"
#include <vector>

class SpriteComponent;
class TransformComponent;

namespace sf
{
	class RenderWindow;
}

class Entity
{
public:
	/*
	* Constructor of the Entity class. The friendlyName parameter is for debugging purpose
	*/
	Entity(std::string friendlyName = "");

	/*
	* Destructor of the Entity class. All allocated resources must be released
	*/
	virtual ~Entity();

	/*
	* First function that will be called when an entity will be added to the current scene.
	*/
	virtual void Start();

	/*
	* Function that will be called each frame if our entity is part of the current scene.
	*/
	virtual void Update(float fDeltaTime);

	/*
	* Last function that will be called before an entity is removed of the current scene.
	*/
	virtual void Destroy();

	/*
	* This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	  * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;



	/*
* Add a new component of type C to the list of components.
* Be sure that we don't have the same component already registered.
* If an error occurs, return nullptr. Otherwise, creates the component and returns its pointer.
*/
	template <typename C>
	C* AddComponent();

	/*
	* Get an existing component of type C that has previously be added.
	* If an error occurs or if there is no component of type C, returns nullptr.
	* Otherwise, returns its pointer.
	*/
	template <typename C>
	C* GetComponent() const;


	protected:

	std::vector<IComponent*> Components;
};

template <typename C>
C* Entity::AddComponent()
{
	C* comp = nullptr;
	if (GetComponent<C>() == nullptr)
	{
		comp = new C(*this);
		Components.push_back(comp);
	}
	return comp;
}

template <typename C>
C* Entity::GetComponent() const
{
	static_assert (!std::is_base_of_v<C, IComponent>, "Must use Component");
	
	for (auto &component : Components)
		if ( C* out = dynamic_cast<C*>(component))
			return out;
	
	return nullptr;
}


