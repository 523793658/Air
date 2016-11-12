{
	"target_defaults":{
	},
	"targets":[
		{
			"target_name": "engine",
			"dependencies": [
				'basic',
			],
			"type": "shared_library",
			"include_dirs":[
				"../../external/boost",
				"../../external/FlameMath",
				"../core/include",
				"../",
			],
			"includes":[
				"../../tools/gyp/common.gypi",
			],
			"sources":[
				"../core/include/Context.h",
				"../core/include/ElementFormat.h",
				"../core/include/Engine.h",

				"../core/include/PreDeclare.h",
				"../core/include/ResLoader.h",
				"../core/include/Camera.hpp",
				"../core/include/SceneManager.hpp",
				"../core/src/Engine.cpp",
				"../core/src/Context.cpp",
				"../core/src/ElementFormat.cpp",
				"../core/src/ResLoader.cpp",
				"../core/src/Camera.cpp",
				"../core/src/SceneManager.cpp",
				
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
				
				"../rendersystem/src/RenderSettings.cpp",
				"../rendersystem/src/RenderFactory.cpp",
				"../rendersystem/src/FrameBuffer.cpp",
				"../rendersystem/src/RenderEngine.cpp",
				"../rendersystem/src/Viewport.cpp",
				"../rendersystem/src/RenderView.cpp",
				"../rendersystem/src/Texture.cpp",
				"../rendersystem/src/RenderLayout.cpp",
				"../rendersystem/src/Mesh.cpp",
				
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