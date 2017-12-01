#ifndef _ShaderFormatModule_H_
#define _ShaderFormatModule_H_
#pragma once
#include "basic/include/ModuleInterface.hpp"
namespace Air
{

class IShaderFormat;

class IShaderFormatModule : public IModuleInterface
{
public:
	virtual IShaderFormat* getShaderFormat() = 0;

public:
	~IShaderFormatModule()
	{

	}
};
}

#endif
