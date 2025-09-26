#pragma once

class IVisitor;

class IVisitable
{
public:

	virtual void Accept(IVisitor* const visitor) = 0;
};