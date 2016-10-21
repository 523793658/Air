#ifndef _BASIC_XMLDOM_H_
#define _BASIC_XMLDOM_H_
#pragma  once

#include <iosfwd>
#include <vector>

namespace Air
{
	enum XMLNodeType
	{
		XNT_Document,
		XNT_Element,
		XNT_Data,
		XNT_CData,
		XNT_Comment,
		XNT_Declaration,
		XNT_Doctype,
		XNT_PI
	};

	class XMLDocument
	{
	public:
		XMLDocument();
		XMLNodePtr parse(ResIdentifierPtr const & source);
		void print(std::ostream & os);
		XMLNodePtr cloneNode(XMLNodePtr const & node);

		XMLNodePtr allocNode(XMLNodeType type, std::string const & name);
		XMLAttributePtr allocAttributeInt(std::string const & name, int32_t value);
		XMLAttributePtr allocAttributeUInt(std::string const & name, uint32_t value);
		XMLAttributePtr allocAttributeFloat(std::string const & name, float value);
		XMLAttributePtr allocAttributeString(std::string const & name, std::string const & value);

		void getRootNode(XMLNodePtr const & new_node);
	private:
		std::shared_ptr<void> mDoc;
		std::vector<char> mXmlSrc;
		XMLNodePtr mRoot;
	};

	class XMLNode
	{
		friend class XMLDocument;
	public:
		explicit XMLNode(void* node);
		XMLNode(void* doc, XMLNodeType type, std::string const & name);

		std::string const & getName() const;
		XMLNodeType getType() const;
		XMLNodePtr getParent();
		XMLAttributePtr getFirstAttrib(std::string const & name);
		XMLAttributePtr getLastAttrib(std::string const & name);
		XMLAttributePtr getFirstAttrib();
		XMLAttributePtr getLastAttrib();

		XMLAttributePtr getAttrib(std::string const & name);
		bool tryConvertAttrib(std::string const & name, int32_t& val, int32_t default_val);
		bool tryConvertAttrib(std::string const & name, uint32_t& val, uint32_t default_val);
		bool tryConvertAttrib(std::string const & name, float& val, float default_val);

		int32_t getAttribInt(std::string const & name, int32_t default_val);
		uint32_t getAttribUInt(std::string const & name, uint32_t default_val);
		float getAttribFloat(std::string const & name, float default_val);
		std::string getAttribString(std::string const & name, std::string default_val);

		XMLNodePtr getFirstNode(std::string const & name);
		XMLNodePtr getLastNode(std::string const & name);
		XMLNodePtr getFirstNode();
		XMLNodePtr getLastNode();

		XMLNodePtr getPrevSibling(std::string const & name);
		XMLNodePtr getNextSibling(std::string const & name);
		XMLNodePtr getPrevSibLing();
		XMLNodePtr getNextSibling();

		void insertNode(XMLNodePtr const & location, XMLNodePtr const & new_node);
		void insertAttrib(XMLAttributePtr const & location, XMLAttributePtr const & new_attr);
		void appendNode(XMLNodePtr const & new_node);
		void appendAttrib(XMLAttributePtr const & new_attr);

		void removeNode(XMLNodePtr const & node);
		void removeAttrib(XMLAttributePtr const & attr);

		bool tryConvert(int32_t& val) const;
		bool tryConvert(uint32_t& val) const;
		bool tryConvert(float& val) const;

		int32_t getValueInt() const;
		uint32_t getValueUint() const;
		float getValueFloat() const;
		std::string getValueString() const;

	private:
		void* mNode;
		std::string mName;
		std::vector<XMLNodePtr> mChildren;
		std::vector<XMLAttributePtr> mAttrs;
	};

	class XMLAttribute
	{
		friend class XMLDocument;
		friend class XMLNode;
	public:
		explicit XMLAttribute(void* attr);
		XMLAttribute(void* doc, std::string const & name, std::string const & value);

		std::string const & getName() const;
		XMLAttributePtr getNextAttrib(std::string const & name);
		XMLAttributePtr getNextAttrib();

		bool tryConvert(int32_t& val) const;
		bool tryConvert(uint32_t& val) const;
		bool tryConvert(float& val) const;

		int32_t getValueInt() const;
		uint32_t getValueUint() const;
		float getValueFloat() const;
		std::string const & getValueString() const;
	private:
		void* mAttr;
		std::string mName;
		std::string mValue;
	};
}




#endif