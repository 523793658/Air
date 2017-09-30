#include "basic/include/Basic.h"

#include "basic/include/Util.h"

#include "basic/include/ResIdentifier.h"

#include "boost/lexical_cast.hpp"
#include "rapidxml/src/rapidxml.hpp"
#include "rapidxml/src/rapidxml_print.hpp"


#include "basic/include/XMLDom.hpp"


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

	XMLAttributePtr XMLDocument::allocAttributeFloat(std::string const & name, float value)
	{
		return this->allocAttributeString(name, boost::lexical_cast<std::string>(value));
	}
	XMLAttributePtr XMLDocument::allocAttributeString(std::string const & name, std::string const & value)
	{
		return MakeSharedPtr<XMLAttribute>(mDoc.get(), name, value);
	}






	void XMLDocument::setRootNode(XMLNodePtr const & new_node)
	{
		static_cast<rapidxml::xml_document<>*>(mDoc.get())->remove_all_nodes();
		static_cast<rapidxml::xml_document<>*>(mDoc.get())->append_node(static_cast<rapidxml::xml_node<>*>(new_node->mNode));
		mRoot = new_node;
	}


	XMLNode::XMLNode(void* node) : mNode(node)
	{
		if (mNode != nullptr)
		{
			mName = std::string(static_cast<rapidxml::xml_node<>*>(mNode)->name(), static_cast<rapidxml::xml_node<>*>(mNode)->name_size());
		}

	}
	XMLNode::XMLNode(void* doc, XMLNodeType type, std::string const & name):mName(name)
	{
		rapidxml::node_type xtype;
		switch (type)
		{
		case Air::XNT_Document:
			xtype = rapidxml::node_document;
			break;
		case Air::XNT_Element:
			xtype = rapidxml::node_element;
			break;
		case Air::XNT_Data:
			xtype = rapidxml::node_data;
			break;
		case Air::XNT_CData:
			xtype = rapidxml::node_cdata;
			break;
		case Air::XNT_Comment:
			xtype = rapidxml::node_comment;
			break;
		case Air::XNT_Declaration:
			xtype = rapidxml::node_declaration;
			break;
		case Air::XNT_Doctype:
			xtype = rapidxml::node_doctype;
			break;
		case Air::XNT_PI:
		default:
			xtype = rapidxml::node_pi;
			break;
		}
		mNode = static_cast<rapidxml::xml_document<>*>(doc)->allocate_node(xtype, name.data(), nullptr, name.size());
	}

	std::string const & XMLNode::getName() const
	{
		return mName;
	}
	XMLNodeType XMLNode::getType() const
	{
		rapidxml::node_type type = static_cast<rapidxml::xml_node<>*>(mNode)->type();
		switch (type)
		{
		case rapidxml::node_document:
			return XNT_Document;
		case rapidxml::node_element:
			return XNT_Element;
		case rapidxml::node_data:
			return XNT_Data;
		case rapidxml::node_cdata:
			return XNT_CData;
		case rapidxml::node_comment:
			return XNT_Comment;
		case rapidxml::node_declaration:
			return XNT_Declaration;
		case rapidxml::node_doctype:
			return XNT_Doctype;
		case rapidxml::node_pi:
		default:
			return XNT_PI;
		}
	}
	XMLNodePtr XMLNode::getParent()
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(mNode)->parent();
		if (node)
		{
			return MakeSharedPtr<XMLNode>(node);
		}
		else
		{
			return XMLNodePtr();
		}


	}
	XMLAttributePtr XMLNode::getFirstAttrib(std::string const & name)
	{
		rapidxml::xml_attribute<>* attr = static_cast<rapidxml::xml_node<>*>(mNode)->first_attribute(name.data(), name.size());
		if (attr)
		{
			return MakeSharedPtr<XMLAttribute>(attr);
		}
		else
		{
			return XMLAttributePtr();
		}
	}
	XMLAttributePtr XMLNode::getLastAttrib(std::string const & name)
	{
		rapidxml::xml_attribute<>* attr = static_cast<rapidxml::xml_node<>*>(mNode)->last_attribute(name.data(), name.size());
		if (attr)
		{
			return MakeSharedPtr<XMLAttribute>(attr);
		}
		{
			return XMLAttributePtr();
		}
	}
	XMLAttributePtr XMLNode::getFirstAttrib()
	{
		rapidxml::xml_attribute<>* attr = static_cast<rapidxml::xml_node<>*>(mNode)->first_attribute();
		return attr ? MakeSharedPtr<XMLAttribute>(attr) : XMLAttributePtr();
	}
	XMLAttributePtr XMLNode::getLastAttrib()
	{
		rapidxml::xml_attribute<>* attr = static_cast<rapidxml::xml_node<>*>(mNode)->last_attribute();
		return attr ? MakeSharedPtr<XMLAttribute>(attr) : XMLAttributePtr();
	}
 
	XMLAttributePtr XMLNode::getAttrib(std::string const & name)
	{
		return this->getFirstAttrib(name);
	}
	bool XMLNode::tryConvertAttrib(std::string const & name, int32_t& val, int32_t default_val)
	{
		val = default_val;
		XMLAttributePtr attr = this->getAttrib(name);
		return attr ? attr->tryConvert(val) : true;
	}
	bool XMLNode::tryConvertAttrib(std::string const & name, uint32_t& val, uint32_t default_val)
	{
		val = default_val;
		XMLAttributePtr attr = this->getAttrib(name);
		return attr ? attr->tryConvert(val) : true;
	}
	bool XMLNode::tryConvertAttrib(std::string const & name, float& val, float default_val)
	{
		val = default_val;
		XMLAttributePtr attr = this->getAttrib(name);
		return attr ? attr->tryConvert(val) : true;
	}
 
	int32_t XMLNode::getAttribInt(std::string const & name, int32_t default_val)
	{
		XMLAttributePtr attr = this->getAttrib(name);
		return attr ? attr->getValueInt() : default_val;
	}
	uint32_t XMLNode::getAttribUInt(std::string const & name, uint32_t default_val)
	{
		XMLAttributePtr attr = this->getAttrib(name);
		return attr ? attr->getValueUInt() : default_val;
	}
	float XMLNode::getAttribFloat(std::string const & name, float default_val)
	{
		XMLAttributePtr attr = this->getAttrib(name);
		return attr ? attr->getValueFloat() : default_val;
	}
	std::string XMLNode::getAttribString(std::string const & name, std::string default_val)
	{
		XMLAttributePtr attr = this->getAttrib(name);
		return attr ? attr->getValueString() : default_val;
	}

	XMLNodePtr XMLNode::getFirstNode(std::string_view name) const
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(mNode)->first_node(name.data());
		return node ? MakeSharedPtr<XMLNode>(node) : XMLNodePtr();
	}
	XMLNodePtr XMLNode::getLastNode(std::string const & name) const
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(mNode)->last_node(name.data());
		return node ? MakeSharedPtr<XMLNode>(node) : XMLNodePtr();
	}
	XMLNodePtr XMLNode::getFirstNode() const
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(mNode)->first_node();
		return node ? MakeSharedPtr<XMLNode>(node) : XMLNodePtr();
	}
	XMLNodePtr XMLNode::getLastNode() const
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(mNode)->last_node();
		return node ? MakeSharedPtr<XMLNode>(node) : XMLNodePtr();
	}

	XMLNodePtr XMLNode::getPrevSibling(std::string const & name)
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(mNode)->previous_sibling(name.data(), name.size());
		return node ? MakeSharedPtr < XMLNode>(node) : XMLNodePtr();
	}
	XMLNodePtr XMLNode::getNextSibling(std::string const & name)
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(mNode)->next_sibling(name.data(), name.size());
		return node ? MakeSharedPtr<XMLNode>(node) : XMLNodePtr();
	}
	XMLNodePtr XMLNode::getPrevSibLing()
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(mNode)->previous_sibling();
		return node ? MakeSharedPtr<XMLNode>(node) : XMLNodePtr();
	}
	XMLNodePtr XMLNode::getNextSibling()
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(mNode)->next_sibling();
		return node ? MakeSharedPtr<XMLNode>(node) : XMLNodePtr();
	}
 
	void XMLNode::insertNode(XMLNodePtr const & location, XMLNodePtr const & new_node)
	{
		static_cast<rapidxml::xml_node<>*>(mNode)->insert_node(static_cast<rapidxml::xml_node<>*>(location->mNode), static_cast<rapidxml::xml_node<>*>(new_node->mNode));
		for (size_t i = 0; i < mChildren.size(); ++i)
		{
			if (mChildren[i]->mNode == location->mNode)
			{
				mChildren.insert(mChildren.begin() + i, new_node);
				break;
			}
		}
	}
	void XMLNode::insertAttrib(XMLAttributePtr const & location, XMLAttributePtr const & new_attr)
	{
		static_cast<rapidxml::xml_node<>*>(mNode)->insert_attribute(static_cast<rapidxml::xml_attribute<>*>(location->mAttr), static_cast<rapidxml::xml_attribute<>*>(new_attr->mAttr));
		for (size_t i = 0; i < mAttrs.size(); ++i)
		{
			if (mAttrs[i]->mAttr == location->mAttr)
			{
				mAttrs.insert(mAttrs.begin() + i, new_attr);
				break;

			}
		}
	}
	void XMLNode::appendNode(XMLNodePtr const & new_node)
	{
		static_cast<rapidxml::xml_node<>*>(mNode)->append_node(static_cast<rapidxml::xml_node<>*>(new_node->mNode));
		mChildren.push_back(new_node);
	}
	void XMLNode::appendAttrib(XMLAttributePtr const & new_attr)
	{
		static_cast<rapidxml::xml_node<>*>(mNode)->append_attribute(static_cast<rapidxml::xml_attribute<>*>(new_attr->mAttr));
		mAttrs.push_back(new_attr);
	}

	void XMLNode::removeNode(XMLNodePtr const & node)
	{
		static_cast<rapidxml::xml_node<>*>(mNode)->remove_node(static_cast<rapidxml::xml_node<>*>(node->mNode));
		for (size_t i = 0; i < mChildren.size(); ++i)
		{
			if (mChildren[i]->mNode == node->mNode)
			{
				mChildren.erase(mChildren.begin() + i);
				break;
			}


		}
	}
	void XMLNode::removeAttrib(XMLAttributePtr const & attr)
	{
		static_cast<rapidxml::xml_node<>*>(mNode)->remove_attribute(static_cast<rapidxml::xml_attribute<>*>(attr->mAttr));
		for (size_t i = 0; i < mAttrs.size(); ++i)
		{
			if (mAttrs[i]->mAttr == attr->mAttr)
			{
				mAttrs.erase(mAttrs.begin() + i);
			}
		}
	}

	bool XMLNode::tryConvert(int32_t& val) const
	{
		return boost::conversion::try_lexical_convert(this->getValueString(), val);
	}
	bool XMLNode::tryConvert(uint32_t& val) const
	{
		return boost::conversion::try_lexical_convert(this->getValueString(), val);
	}
	bool XMLNode::tryConvert(float& val) const
	{
		return boost::conversion::try_lexical_convert(this->getValueString(), val);
	}
// 
	int32_t XMLNode::getValueInt() const
	{
		return boost::lexical_cast<int32_t>(this->getValueString());
	}
 	uint32_t XMLNode::getValueUint() const
	{
		return boost::lexical_cast<uint32_t>(this->getValueString());
	}
	float XMLNode::getValueFloat() const
	{
		return boost::lexical_cast<float>(this->getValueString());
	}
 	std::string XMLNode::getValueString() const
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(mNode);
		return std::string(node->value(), node->value_size());
	}


	XMLAttribute::XMLAttribute(void* attr) :mAttr(attr)
	{
		if (mAttr != nullptr)
		{
			auto const xml_attr = static_cast<rapidxml::xml_attribute<>*>(mAttr);
			mName = std::string(xml_attr->name(), xml_attr->name_size());
			mValue = std::string(xml_attr->value(), xml_attr->value_size());
		}
	}
	XMLAttribute::XMLAttribute(void* doc, std::string const & name, std::string const & value):mName(name), mValue(value)
	{
		mAttr = static_cast<rapidxml::xml_document<>*>(doc)->allocate_attribute(name.data(), value.data(), name.size(), value.size());
	}

	std::string const & XMLAttribute::getName() const
	{
		return mName;
	}
	XMLAttributePtr XMLAttribute::getNextAttrib(std::string const & name)
	{
		rapidxml::xml_attribute<>* attr = static_cast<rapidxml::xml_attribute<>*>(mAttr)->next_attribute(name.data(), name.size());
		return attr ? MakeSharedPtr<XMLAttribute>(attr) : XMLAttributePtr();
	}
	XMLAttributePtr XMLAttribute::getNextAttrib()
	{
		rapidxml::xml_attribute<>* attr = static_cast<rapidxml::xml_attribute<>*>(mAttr)->next_attribute();
		return attr ? MakeSharedPtr<XMLAttribute>(attr) : XMLAttributePtr();
	}

	bool XMLAttribute::tryConvert(int32_t& val) const
	{
		return boost::conversion::try_lexical_convert<int32_t>(mValue, val);
	}
	bool XMLAttribute::tryConvert(uint32_t& val) const
	{
		return boost::conversion::try_lexical_convert<uint32_t>(mValue, val);
	}
	bool XMLAttribute::tryConvert(float& val) const
	{
		return boost::conversion::try_lexical_convert<float>(mValue, val);
	}

	int32_t XMLAttribute::getValueInt() const
	{
		return boost::lexical_cast<int32_t>(mValue);
	}
	uint32_t XMLAttribute::getValueUInt() const
	{
		return boost::lexical_cast<uint32_t>(mValue);
	}
	float XMLAttribute::getValueFloat() const
	{
		return boost::lexical_cast<float>(mValue);
	}
	std::string const & XMLAttribute::getValueString() const
	{
		return mValue;
	}


}