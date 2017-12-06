{
	"target_defaults":{
	},
	"targets":[
		{
			"target_name": "engine",
			"dependencies": [
				'basic',
			],
			'variables': {
				'outputSubDir': '',
				'shaderPath' : "../rendersystem/shader",
			},
			"type": "shared_library",
			"include_dirs":[
				"../../external/boost",
				"../core/include",
				"../",
				"../basic",
				"../../external/dxsdk/Include",
			],
			"includes":[
				"../../tools/gyp/common.gypi",
				"./common.gypi",
			],
			"sources":[
				"../core/include/Context.h",
				"../core/include/ElementFormat.h",
				"../core/include/Engine.h",
				"../core/include/PreDeclare.h",
				"../core/include/ResLoader.h",
				"../core/include/Camera.hpp",
				"../core/include/ResourcePlugin.h",
				"../core/include/SALWrapper.hpp",
				"../core/include/CameraController.hpp",
				"../core/include/ConfigTranslator.hpp",
				"../core/include/Event.hpp",
				"../core/include/GameObject.hpp",
				"../core/include/ComponentBase.hpp",
				"../core/include/Type.hpp",
				"../core/include/SingletonManager.hpp",
				"../core/include/World.hpp",

				
				"../core/src/Engine.cpp",
				"../core/src/Context.cpp",
				"../core/src/ElementFormat.cpp",
				"../core/src/ResLoader.cpp",
				"../core/src/Camera.cpp",
				"../core/src/ResourcePlugin.cpp",
				"../core/src/CameraController.cpp",
				"../core/src/ConfigTranslator.cpp",
				"../core/src/Event.cpp",
				"../core/src/GameObject.cpp",
				"../core/src/Type.cpp",
				"../core/src/ComponentBase.cpp",
				"../core/src/SingletonManager.cpp",
				"../core/src/World.cpp",

				
				"../rendersystem/include/RenderFactory.h",
				"../rendersystem/include/RenderEngine.hpp",
				"../rendersystem/include/RenderSettings.hpp",
				"../rendersystem/include/RenderDeviceCaps.hpp",
				"../rendersystem/include/FrameBuffer.hpp",
				"../rendersystem/include/Viewport.hpp",
				"../rendersystem/include/RenderView.hpp",
				"../rendersystem/include/Texture.hpp",
				"../rendersystem/include/RenderLayout.hpp",
				"../rendersystem/include/Mesh.hpp",
				"../rendersystem/include/Renderable.hpp",
				"../rendersystem/include/RenderableHelper.hpp",
				"../rendersystem/include/RenderEffect.hpp",
				"../rendersystem/include/GraphicsBuffer.hpp",
				"../rendersystem/include/RenderMaterial.hpp",
				"../rendersystem/include/SimpleMeshFactory.hpp",
				"../rendersystem/include/ShaderObject.hpp",
				"../rendersystem/include/RenderStateObject.hpp",
				"../rendersystem/include/TexCompression.hpp",
				"../rendersystem/include/TexCompressionBC.hpp",
				"../rendersystem/include/ForwardRenderingLayer.hpp",
				"../rendersystem/include/Light.hpp",
				"../rendersystem/include/RenderEnvironment.hpp",
				"../rendersystem/include/ForwardRenderEnvironment.hpp",
				"../rendersystem/include/SkyBox.hpp",
				"../rendersystem/include/PostProcess.hpp",
				"../rendersystem/include/ToneMapping.hpp",
				"../rendersystem/include/HDRPostProcess.hpp",
				"../rendersystem/include/MaterialCompiler.hpp",
				"../rendersystem/include/RenderLayer.hpp",
				"../rendersystem/include/RenderLayerPass.hpp",
				"../rendersystem/include/RenderPipeline.hpp",
				"../rendersystem/include/ShadowMap.hpp",
				"../rendersystem/include/SceneRenderer.hpp",
				"../rendersystem/include/ScenePrivate.hpp",
				"../rendersystem/include/SceneInterface.hpp",
				"../rendersystem/include/DeferredShadingRenderer.hpp",
				"../rendersystem/include/CommandList.hpp",
				"../rendersystem/include/SceneView.hpp",
				"../rendersystem/include/Material.hpp",
				"../rendersystem/include/MaterialInterface.hpp",
				"../rendersystem/include/RHIDefinitions.hpp",
				"../rendersystem/include/DeferredLightingLayer.hpp",
				"../rendersystem/include/DeferredRenderEnvironment.hpp",
				"../rendersystem/include/ShaderCompiler.hpp",
				"../rendersystem/include/VertexFactory.hpp",
				"../rendersystem/include/UniformBuffer.hpp",
				
				"../rendersystem/src/RenderSettings.cpp",
				"../rendersystem/src/RenderFactory.cpp",
				"../rendersystem/src/FrameBuffer.cpp",
				"../rendersystem/src/RenderEngine.cpp",
				"../rendersystem/src/Viewport.cpp",
				"../rendersystem/src/RenderView.cpp",
				"../rendersystem/src/Texture.cpp",
				"../rendersystem/src/RenderLayout.cpp",
				"../rendersystem/src/Mesh.cpp",
				"../rendersystem/src/Renderable.cpp",
				"../rendersystem/src/RenderableHelper.cpp",
				"../rendersystem/src/RenderEffect.cpp",
				"../rendersystem/src/GraphicsBuffer.cpp",
				"../rendersystem/src/RenderMaterial.cpp",
				"../rendersystem/src/SimpleMeshFactory.cpp",
				"../rendersystem/src/ShaderObject.cpp",
				"../rendersystem/src/TexCompressionBC.cpp",
				"../rendersystem/src/RenderStateObject.cpp",
				"../rendersystem/src/TexCompression.cpp",
				"../rendersystem/src/ForwardRenderingLayer.cpp",
				"../rendersystem/src/Light.cpp",
				"../rendersystem/src/RenderEnvironment.cpp",
				"../rendersystem/src/ForwardRenderEnvironment.cpp",
				"../rendersystem/src/SkyBox.cpp",
				"../rendersystem/src/PostProcess.cpp",
				"../rendersystem/src/ToneMapping.cpp",
				"../rendersystem/src/HDRPostProcess.cpp",
				"../rendersystem/src/MaterialCompiler.cpp",
				"../rendersystem/src/RenderLayer.cpp",
				"../rendersystem/src/RenderLayerPass.cpp",
				"../rendersystem/src/RenderPipeline.cpp",
				"../rendersystem/src/ShadowMap.cpp",
				"../rendersystem/src/SceneRenderer.cpp",
				"../rendersystem/src/ScenePrivate.cpp",
				"../rendersystem/src/SceneInterface.cpp",
				"../rendersystem/src/DeferredShadingRenderer.cpp",
				"../rendersystem/src/CommandList.cpp",
				"../rendersystem/src/SceneView.cpp",
				"../rendersystem/src/SceneVisibility.cpp",
				"../rendersystem/src/Material.cpp",
				"../rendersystem/src/MaterialInterface.cpp",
				"../rendersystem/src/DeferredLightingLayer.cpp",
				"../rendersystem/src/DeferredRenderEnvironment.cpp",
				"../rendersystem/src/ShaderCompiler.cpp",
				"../rendersystem/src/VertexFactory.cpp",
				"../rendersystem/src/UniformBuffer.cpp",
				
				
				"../rendersystem/shaderCore/ShaderCore.cpp",
				"../rendersystem/shaderCore/ShaderCore.hpp",
				"../rendersystem/shaderCore/ShaderFormat.hpp",
				"../rendersystem/shaderCore/ShaderFormat.cpp",
				"../rendersystem/shaderCore/ShaderFormatModule.cpp",
				"../rendersystem/shaderCore/ShaderFormatModule.hpp",
				"../rendersystem/shaderCore/Shader.hpp",
				"../rendersystem/shaderCore/Shader.cpp",
				"../rendersystem/shaderCore/ShaderPipelineCompiler.cpp",
				
				"../scene_manager/include/SceneManager.hpp",
				"../scene_manager/include/SceneObject.hpp",
				"../scene_manager/include/SceneObjectHelper.hpp",
				"../scene_manager/src/SceneManager.cpp",
				"../scene_manager/src/SceneObject.cpp",
				"../scene_manager/src/SceneObjectHelper.cpp",
				
				"../packing_system/include/LZMACodec.h",
				"../packing_system/src/LZMACodec.cpp",
				
				"../input_system/include/InputFactory.hpp",
				"../input_system/include/Input.hpp",
				"../input_system/src/InputFactory.cpp",
				"../input_system/src/Input.cpp",
				
				"../app/include/Window.hpp",
				"../app/include/App3D.hpp",
				"../app/src/WindowWin.cpp",
				"../app/src/App3D.cpp",
				
				"../component/include/StaticMeshRenderer.hpp",
				"../component/include/PrimitiveComponent.hpp",
				"../component/include/SkinMeshRenderer.hpp",
				
				
				"../component/src/PrimitiveComponent.cpp",
				"../component/src/StaticMeshRenderer.cpp",
				"../component/src/SkinMeshRenderer.cpp",
				
				"<(shaderPath)/simpleforward.asd",
				"<(shaderPath)/skyBox.asd",
				"<(shaderPath)/toneMap.asd",
				"<(shaderPath)/Luminance.asd",
				"<(shaderPath)/simplest.asd",
				
				
				"<(shaderPath)/shadingModels.hlsl",
				"<(shaderPath)/brdf.hlsl",
				"<(shaderPath)/common.hlsl",
				"<(shaderPath)/skyBox.hlsl",
				"<(shaderPath)/util.hlsl",
				"<(shaderPath)/toneMap.hlsl",
				"<(shaderPath)/Luminance.hlsl",
				"<(shaderPath)/shadowPCF.hlsl",
				"<(shaderPath)/pixelShaderMain.hlsl",
				"<(shaderPath)/vertexShaderMain.hlsl",
				"<(shaderPath)/DeferredLightingCommon.hlsl",
				"<(shaderPath)/DeferredShadingCommon.hlsl",
				"<(shaderPath)/BasePassPixelShader.hlsl",
				"<(shaderPath)/PixelShaderOutputCommon.hlsl",
				"<(shaderPath)/MaterialTemplate.hlsl",
				"<(shaderPath)/LocalVertexFactoryCommon.hlsl",
				"<(shaderPath)/LocalVertexFactory.hlsl",
				"<(shaderPath)/Definitions.hlsl",
				"<(shaderPath)/VertexFactoryCommon.hlsl",
				"<(shaderPath)/BasePassCommon.hlsl",
				"<(shaderPath)/LightAccumulator.hlsl",
			],
			"conditions":[
				[
					"OS == 'win'",
					{
						"msbuild_configuration_attributes":{
							"CharacterSet": "1",
						}, 
						"msbuild_settings":{
							"ClCompile":{
								'PreprocessorDefinitions': [
									"AIR_CORE_SOURCE",
								],
							},
							'Link': {
								'AdditionalDependencies':[
									"lzma.lib",
								],
							},
						},
						"configurations":{
							"Debug":{
							"msbuild_settings":{
									"ClCompile":{
										'RuntimeLibrary': 'MultiThreadedDebugDLL',
									},
									'Link': {
										'GenerateDebugInformation': 'true',
										'AdditionalDependencies':[
										],
									},
								},
							},
							"Release":{
							"msbuild_settings":{
									"ClCompile":{
										'RuntimeLibrary': 'MultiThreadedDLL',
									},
								},
							},
						},
					},
				],
			],
		},
	],
}