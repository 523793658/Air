#ifndef _CPP_Reflection_H_
#define _CPP_Reflection_H_
#pragma once
namespace Air
{

	class Register
	{
	public:
		void* getInstance();
	};

}


#define REGISTER_CLASS_NAME(ClassManager, class_name)		\
	class Register##ClassManager##class_name{	\
	public:										\
		static void * instance()				\
		{										\
			return new class_name;				\
		}										\
	private:									\
		static const Register##ClassManager _static_register; \
	};											\
	const Register##ClassManager Register##ClassManager##class_name::_static_register( \

#endif
