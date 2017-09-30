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
			},
			"type": "shared_library",
			"include_dirs":[
				"../../external/boost",
				"../core/include",
				"../",
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
				
				"../core/src/Engine.cpp",
				"../core/src/Context.cpp",
				"../core/src/ElementFormat.cpp",
				"../core/src/ResLoader.cpp",
				"../core/src/Camera.cpp",
				"../core/src/ResourcePlugin.cpp",
				
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
				
				"../scene_manager/include/SceneManager.hpp",
				"../scene_manager/include/SceneObject.hpp",
				"../scene_manager/include/SceneObjectHelper.hpp",
				"../scene_manager/src/SceneManager.cpp",
				"../scene_manager/src/SceneObject.cpp",
				"../scene_manager/src/SceneObjectHelper.cpp",
				
				"../packing_system/include/LZMACodec.h",
				"../packing_system/src/LZMACodec.cpp",
				
				"../app/include/Window.hpp",
				"../app/include/App3D.hpp",
				"../app/src/WindowWin.cpp",
				"../app/src/App3D.cpp",
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