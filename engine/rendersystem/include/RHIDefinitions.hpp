#ifndef _RHIDefinitions_H_
#define _RHIDefinitions_H_
#pragma once

enum EShaderFrequency
{
	SF_Vertex			=0,
	SF_Hull				= 1,
	SF_Domain			= 2,
	SF_Pixel			= 3,
	SF_Geometry			= 4,
	SF_Compute			= 5,

	SF_NumFrequencies	 = 6,

	SF_NumBits			= 3,
};

enum EShaderPlatform
{
	SP_PCD3D_SM5			=0,
	SP_OPENGL_SM4,
	SP_PS4,

	SP_PCD3D_SM4,
	SP_OPENGL_SM5,

	SP_NumPlatforms,

	SP_NumBits			=5
};


namespace ERHIFeatureLevel
{
	enum Type
	{
		ES2,
		ES3_1,
		SM4,
		SM5,
		Num
	};




}



#endif
