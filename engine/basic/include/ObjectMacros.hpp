#ifndef _ObjectMacros_H_
#define _ObjectMacros_H_
#pragma once
namespace Air
{

	enum EObjectFlags
	{
		RF_NoFlags				= 0x00000000,
		RF_Public				= 0x00000001,
		RF_BeginDestroyed		= 0x00000002,
		RF_FinishDestroyed		= 0x00000004,
		RF_ClassDefaultObject	= 0x00000008,
	};
}


#endif