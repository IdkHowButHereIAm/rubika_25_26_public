#pragma once

#include "IVisitable.h"

#include <rapidxml/rapidxml.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

#define GENERATE_SERIALIZE_TOKEN(name) \
	const char* GetSerializeToken_Static() { return name; } \
	const char* GetSerializeToken() const override { return GetSerializeToken_Static(); }

class SerializeVisitor;
class DeserializeVisitor;

class SerializeVisitable : public IVisitable
{
public:
	void Accept(IVisitor*) override {};
	virtual void Accept(SerializeVisitor* visitor, rapidxml::xml_node<>* parent = nullptr) = 0;
	virtual void Accept(DeserializeVisitor* visitor, rapidxml::xml_node<>* node) = 0;

	virtual const char* GetSerializeToken() const = 0;

protected:

	void SerializeVec2f(rapidxml::xml_document<>& xmlDoc, rapidxml::xml_node<>* parent, const char* nodeName, const sf::Vector2f& vec);
	void SerializeBool(rapidxml::xml_document<>& xmlDoc, rapidxml::xml_node<>* parent, const char* nodeName, bool value);
	void SerializeFloat(rapidxml::xml_document<>& xmlDoc, rapidxml::xml_node<>* parent, const char* nodeName, float value);
	void SerializeUInt(rapidxml::xml_document<>& xmlDoc, rapidxml::xml_node<>* parent, const char* nodeName, unsigned value);
	void SerializeString(rapidxml::xml_document<>& xmlDoc, rapidxml::xml_node<>* parent, const char* nodeName, const std::string& value);

	void DeserializeVec2f(const rapidxml::xml_node<>* node, const char* nodeName, sf::Vector2f& vec);
	void DeserializeBool(const rapidxml::xml_node<>* node, const char* nodeName, bool& value);
	void DeserializeFloat(const rapidxml::xml_node<>* node, const char* nodeName, float& value);
	void DeserializeUInt(const rapidxml::xml_node<>* node, const char* nodeName, unsigned& value);
	void DeserializeString(const rapidxml::xml_node<>* node, const char* nodeName, std::string& value);
};