#ifndef _Type_H_
#define _Type_H_
#pragma once
 


namespace Air
{
	class AIR_CORE_API AType
	{
		AType(std::string name);
		std::string const & getName() const;
	private:
		std::string mTypeName;
	};
}



#endif
