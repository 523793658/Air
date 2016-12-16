#ifndef _BASIC_PREDECLARE_H_
#define _BASIC_PREDECLARE_H_
#pragma once

#ifdef AIR_COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <memory>


#ifdef KLAYGE_COMPILER_GCC
#pragma GCC diagnostic pop
#endif

namespace Air
{
	class ResIdentifier;
	typedef std::shared_ptr<ResIdentifier> ResIdentifierPtr;
	class DllLoader;

	class XMLDocument;
	typedef std::shared_ptr<XMLDocument> XMLDocumentPtr;
	class XMLNode;
	typedef std::shared_ptr<XMLNode> XMLNodePtr;
	class XMLAttribute;
	typedef std::shared_ptr<XMLAttribute> XMLAttributePtr;

	class AABBox;
	typedef std::shared_ptr<AABBox> AABBoxPtr;

#ifdef FLAMEMATH
	typedef struct Frustum Frustum;
#else
	class Frustum;
#endif
	typedef std::shared_ptr<Frustum> FrustumPtr;

	class bad_join;
	class thread_pool;
}



#endif