{
	"target_defaults":{
	},
	"targets":[
		{
			"target_name": "engine",
			"dependencies": [
				"basic",
			],
			"type": "shared_library",
			"include_dirs":[
				"../../external/boost",
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
				"../core/include/DllLoader.h",
				"../core/include/ResLoader.h",
				"../core/include/Camera.hpp",
				"../core/include/SceneManager.hpp",
				"../core/src/Engine.cpp",
				"../core/src/Context.cpp",
				"../core/src/ElementFormat.cpp",
				"../core/src/DllLoader.cpp",
				"../core/src/ResLoader.cpp",
				"../core/src/Camera.cpp",
				"../core/src/SceneManager.cpp",
				
				"../rendersystem/include/RenderFactory.h",
				"../rendersystem/include/RenderSettings.hpp",
				"../rendersystem/src/RenderSettings.cpp",
				"../rendersystem/src/RenderFactory.cpp",
				
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