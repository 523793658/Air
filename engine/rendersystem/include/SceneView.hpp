#ifndef _SceneView_H_
#define _SceneView_H_
#pragma once
#include "rendersystem/include/UniformBuffer.hpp"
#include "rendersystem/include/ShaderParameters.hpp"
#include "rendersystem/include/HardwareResource.hpp"
#include "rendersystem/include/DynamicRHI.hpp"
#include "rendersystem/include/ShowFlags.hpp"
namespace Air
{

	
#define  NUM_LIGHTING_CHANNELS 3


	/*BEGIN_UNIFORM_BUFFER_STRUCT_WITH_CONSTRUCTOR(ForwardDirectionalLightShaderParameters, AIR_CORE_API)
		DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER_EX(Color, DirectionalLightColor, EShaderPricisionModifier::Half)
	END_UNIFORM_BUFFER_STRUCT(ForwardDirectionalLightShaderParameters)*/

	class SceneViewStateInterface;
	class SceneViewExtension;


	class AIR_CORE_API SceneView
	{
	public:
		SceneViewStateInterface* mState;

		bool bIsPlanarReflection{ false };

		bool bIsSceneCapture;

		bool bIsReflectionCapture;

		bool bIsMobileMultiViewEnabled;
		
		/*UnifromBufferRef<ForwardDirectionalLightShaderParameters> mForwardDirectionalLightUniformBuffers[NUM_LIGHTING_CHANNELS + 1];*/
	};

	class AIR_CORE_API SceneViewFamily
	{
	public:
		std::vector<std::shared_ptr<SceneViewExtension>> mViewExtensions;
		std::vector<SceneView*> mViews;

		EngineShowFlags mEngineShowFlags;
	};
}




#endif
