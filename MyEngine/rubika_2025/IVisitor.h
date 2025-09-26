#pragma once

class IVisitable;

class IVisitor
{
public:

	virtual void Visit(IVisitable* const visitable) = 0;
};
