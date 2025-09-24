#pragma once

#include <string>

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
};
