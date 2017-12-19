#ifndef _EngineType_H_
#define _EngineType_H_
#pragma once
namespace Air
{

	namespace EWorldType
	{
		enum Type
		{
			None,
			Game,
			Editor,
			PIE,
			EditorPreview,
			GamePreview,
			Inactive
		};
	}

enum class ETeleportType
{
	None,

	TeleportPhysics
};

}

#endif
