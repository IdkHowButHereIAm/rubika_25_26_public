#include "SerializeVisitor.h"

#include "SerializeVisitable.h"
#include <rapidxml/rapidxml_print.hpp>

#include <fstream>

SerializeVisitor::SerializeVisitor()
{
}

void SerializeVisitor::Visit(SerializeVisitable* visitable, rapidxml::xml_node<>* parent)
{
	visitable->Accept(this, parent);
}

void SerializeVisitor::SaveFile(const std::filesystem::path& path)
{
	std::ofstream outputFile;
	outputFile.open(path.string().c_str());
	if (outputFile.is_open())
	{
		outputFile << XmlDoc;
		outputFile.close();
	}
}

rapidxml::xml_document<>& SerializeVisitor::GetXmlDoc()
{
	return XmlDoc;
}

const rapidxml::xml_document<>& SerializeVisitor::GetXmlDoc() const
{
	return XmlDoc;
}

void DeserializeVisitor::Visit(SerializeVisitable* const visitable, rapidxml::xml_node<>* parent)
{
	visitable->Accept(this, parent);
}
