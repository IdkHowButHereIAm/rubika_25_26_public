#include "Renderer.h"

#include <Engine/Render/Drawable/IDrawable.h>
#include <Engine/Gameplay/Component/Transform/Transform.h>
#include <Engine/Gameplay/Entity/Entity.h>

Renderer::Renderer(Entity& entity): IComponent(entity)
{}

Renderer::~Renderer()
{
	for (const DrawableInfo& info : Drawables)
	{
		delete info.Drawable;
	}

	Drawables.clear();
}

void Renderer::Start()
{
	TransformComponent = GetEntity().GetComponent<Transform>();

	for (const DrawableInfo& info : Drawables)
	{
		if (info.HasRelativeTransform)
		{
			info.Drawable->SetWorldTransform(TransformComponent->GetMatrix() * info.RelativeTransform);
		}
		else
		{
			info.Drawable->SetWorldTransform(TransformComponent->GetMatrix());
		}

		info.Drawable->Start();
	}
}

void Renderer::Update(float fDeltaTime)
{
	for (const DrawableInfo& info : Drawables)
	{
		if (info.Drawable->IsVisible())
		{
			if (info.HasRelativeTransform)
			{
				info.Drawable->SetWorldTransform(TransformComponent->GetMatrix() * info.RelativeTransform);
			}
			else
			{
				info.Drawable->SetWorldTransform(TransformComponent->GetMatrix());
			}

			info.Drawable->Update(fDeltaTime);
		}
	}
}

void Renderer::Destroy()
{}

void Renderer::SetDrawableRelativePosition(const IDrawable* drawable, const sf::Vector2f& position)
{
	for (DrawableInfo& info : Drawables)
	{
		if (info.Drawable == drawable)
		{
			info.RelativePosition = position;
			info.ComputeTransform();
			return;
		}
	}
}

void Renderer::SetDrawableRelativeRotation(const IDrawable* drawable, float rotation)
{
	for (DrawableInfo& info : Drawables)
	{
		if (info.Drawable == drawable)
		{
			info.RelativeRotation = rotation;
			info.ComputeTransform();
			return;
		}
	}
}

void Renderer::SetDrawableRelativeScale(const IDrawable* drawable, const sf::Vector2f& scale)
{
	for (DrawableInfo& info : Drawables)
	{
		if (info.Drawable == drawable)
		{
			info.RelativeScale = scale;
			info.ComputeTransform();
			return;
		}
	}
}

void Renderer::Draw(sf::RenderWindow& window) const
{
	for (const DrawableInfo& info : Drawables)
	{
		if (info.Drawable->IsVisible())
		{
			info.Drawable->Draw(window);
		}
	}
}

Renderer::DrawableInfo::DrawableInfo()
{
	FriendlyName = "";
	Drawable = nullptr;
	RelativePosition = sf::Vector2f();
	RelativeRotation = 0.f;
	RelativeScale = sf::Vector2f(1.f, 1.f);
	RelativeTransform = sf::Transform::Identity;
	HasRelativeTransform = false;
}

void Renderer::DrawableInfo::ComputeTransform()
{
	RelativeTransform = sf::Transform::Identity;
	RelativeTransform.translate(RelativePosition);
	RelativeTransform.rotate(sf::degrees(RelativeRotation));
	RelativeTransform.scale(RelativeScale);

	HasRelativeTransform = RelativeTransform != sf::Transform::Identity;
}
