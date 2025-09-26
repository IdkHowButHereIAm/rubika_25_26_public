#pragma once

#include <IVisitor.h>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <filesystem>

class SerializeVisitable;

class SerializeVisitor : public IVisitor
{
public:

	SerializeVisitor();

	void Visit(IVisitable* const visitable) override {};
	virtual void Visit(SerializeVisitable* const visitable, rapidxml::xml_node<>* parent = nullptr);

	void SaveFile(const std::filesystem::path& path);

	rapidxml::xml_document<>& GetXmlDoc();
	const rapidxml::xml_document<>& GetXmlDoc() const;

protected:

	rapidxml::xml_document<> XmlDoc;
};

class DeserializeVisitor : public IVisitor
{
public:
	void Visit(IVisitable* const visitable) override {};
	virtual void Visit(SerializeVisitable* const visitable, rapidxml::xml_node<>* parent = nullptr);

	template<typename Visitable>
	Visitable* LoadFile(const std::filesystem::path& path);
};

template<typename Visitable>
inline Visitable* DeserializeVisitor::LoadFile(const std::filesystem::path& path)
{
	if (!std::is_base_of<SerializeVisitable, Visitable>())
		return nullptr;

	rapidxml::file<> xmlFile(path.string().c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	rapidxml::xml_node<>* rootNode = doc.first_node(Visitable::GetSerializeToken_Static());
	if (!rootNode)
	{
		return nullptr;
	}

	Visitable* loadedObject = new Visitable();
	loadedObject->Accept(this, rootNode);
	return loadedObject;
}
