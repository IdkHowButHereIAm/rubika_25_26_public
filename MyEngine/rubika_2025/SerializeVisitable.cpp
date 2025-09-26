#include "SerializeVisitable.h"

void SerializeVisitable::SerializeVec2f(rapidxml::xml_document<>& xmlDoc, rapidxml::xml_node<>* parent, const char* nodeName, const sf::Vector2f& vec)
{
	if (!parent)
	{
		return;
	}

	rapidxml::xml_node<>* vecNode = xmlDoc.allocate_node(rapidxml::node_type::node_element, nodeName);
	parent->append_node(vecNode);

	std::string xStr = std::to_string(vec.x);
	char* xmlValue = xmlDoc.allocate_string(xStr.data());
	vecNode->append_attribute(xmlDoc.allocate_attribute("X", xmlValue));

	std::string yStr = std::to_string(vec.y);
	xmlValue = xmlDoc.allocate_string(yStr.data());
	vecNode->append_attribute(xmlDoc.allocate_attribute("Y", xmlValue));
}

void SerializeVisitable::SerializeBool(rapidxml::xml_document<>& xmlDoc, rapidxml::xml_node<>* parent, const char* nodeName, bool value)
{
	if (!parent)
	{
		return;
	}

	const char* xmlValue = value ? "1" : "0";
	rapidxml::xml_node<>* vecNode = xmlDoc.allocate_node(rapidxml::node_type::node_element, nodeName, xmlValue);
	parent->append_node(vecNode);
}

void SerializeVisitable::SerializeUInt(rapidxml::xml_document<>& xmlDoc, rapidxml::xml_node<>* parent, const char* nodeName, unsigned value)
{
	if (!parent)
	{
		return;
	}

	std::string xStr = std::to_string(value);
	char* xmlValue = xmlDoc.allocate_string(xStr.data());

	rapidxml::xml_node<>* vecNode = xmlDoc.allocate_node(rapidxml::node_type::node_element, nodeName, xmlValue);
	parent->append_node(vecNode);
}

void SerializeVisitable::SerializeString(rapidxml::xml_document<>& xmlDoc, rapidxml::xml_node<>* parent, const char* nodeName, const std::string& value)
{
	if (!parent)
	{
		return;
	}

	char* xmlValue = xmlDoc.allocate_string(value.data());

	rapidxml::xml_node<>* vecNode = xmlDoc.allocate_node(rapidxml::node_type::node_element, nodeName, xmlValue);
	parent->append_node(vecNode);
}

void SerializeVisitable::SerializeFloat(rapidxml::xml_document<>& xmlDoc, rapidxml::xml_node<>* parent, const char* nodeName, float value)
{
	if (!parent)
	{
		return;
	}

	std::string xStr = std::to_string(value);
	char* xmlValue = xmlDoc.allocate_string(xStr.data());

	rapidxml::xml_node<>* vecNode = xmlDoc.allocate_node(rapidxml::node_type::node_element, nodeName, xmlValue);
	parent->append_node(vecNode);
}

void SerializeVisitable::DeserializeVec2f(const rapidxml::xml_node<>* node, const char* nodeName, sf::Vector2f& vec)
{
	if (!node)
	{
		return;
	}

	rapidxml::xml_node<>* vecNode = node->first_node(nodeName);
	if (!vecNode)
	{
		return;
	}

	rapidxml::xml_attribute<>* xAttr = vecNode->first_attribute("X");
	if (xAttr)
	{
		vec.x = atof(xAttr->value());
	}

	rapidxml::xml_attribute<>* yAttr = vecNode->first_attribute("Y");
	if (yAttr)
	{
		vec.y = atof(yAttr->value());
	}
}

void SerializeVisitable::DeserializeBool(const rapidxml::xml_node<>* node, const char* nodeName, bool& value)
{
	if (!node)
	{
		return;
	}

	rapidxml::xml_node<>* vecNode = node->first_node(nodeName);
	if (!vecNode)
	{
		return;
	}

	value = strcmp(vecNode->value(), "1") == 0;
}

void SerializeVisitable::DeserializeFloat(const rapidxml::xml_node<>* node, const char* nodeName, float& value)
{
	if (!node)
	{
		return;
	}

	rapidxml::xml_node<>* vecNode = node->first_node(nodeName);
	if (!vecNode)
	{
		return;
	}

	value = atof(vecNode->value());
}

void SerializeVisitable::DeserializeUInt(const rapidxml::xml_node<>* node, const char* nodeName, unsigned& value)
{
	if (!node)
	{
		return;
	}

	rapidxml::xml_node<>* vecNode = node->first_node(nodeName);
	if (!vecNode)
	{
		return;
	}

	value = atoll(vecNode->value());
}

void SerializeVisitable::DeserializeString(const rapidxml::xml_node<>* node, const char* nodeName, std::string& value)
{
	if (!node)
	{
		return;
	}

	rapidxml::xml_node<>* vecNode = node->first_node(nodeName);
	if (!vecNode)
	{
		return;
	}

	value = vecNode->value();
}
