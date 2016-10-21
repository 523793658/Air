#include "Basic.h"

#include "Util.h"

#include "ResIdentifier.h"

#include "boost/boost/lexical_cast.hpp"

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"


#include "XMLDom.h"


namespace Air
{
	XMLDocument::XMLDocument()
		: mDoc(MakeSharedPtr<rapidxml::xml_document<>>())
	{
		
	}
	XMLNodePtr XMLDocument::parse(ResIdentifierPtr const & source)
	{
		source->seekg(0, std::ios_base::end);
		int len = static_cast<int>(source->tellg());
		source->seekg(0, std::ios_base::beg);
		mXmlSrc.resize(len + 1, 0);
		source->read(&mXmlSrc[0], len);
		static_cast<rapidxml::xml_document<>*>(mDoc.get())->parse<0>(&mXmlSrc[0]);
		mRoot = MakeSharedPtr<XMLNode>(static_cast<rapidxml::xml_document<>*>(mDoc.get())->first_node());
		return mRoot;
	}

	void XMLDocument::print(std::ostream & os)
	{
		os << "<?xml version=\"1.0\"?>" << std::endl << std::endl;
		os << *static_cast<rapidxml::xml_document<>*>(mDoc.get());
	}

	XMLNodePtr XMLDocument::cloneNode(XMLNodePtr const & node)
	{
		return MakeSharedPtr<XMLNode>(static_cast<rapidxml::xml_document<>*>(mDoc.get())->clone_node(static_cast<rapidxml::xml_node<>*>(node->mNode)));
	}

	XMLNodePtr XMLDocument::allocNode(XMLNodeType type, std::string const & name)
	{
		return MakeSharedPtr<XMLNode>(mDoc.get(), type, name);
	}

	XMLAttributePtr XMLDocument::allocAttributeInt(std::string const & name, int32_t value)
	{
		return this->allocAttributeString(name, boost::lexical_cast<std::string>(value));
	}

	XMLAttributePtr XMLDocument::allocAttributeUInt(std::string const & name, uint32_t value)
	{
		return this->allocAttributeString(name, boost::lexical_cast<std::string>(value));
	}



}