{
	"target_defaults":{
	},
	"targets":[
		{
			"target_name": "basic",
			"type": "static_library",
			"include_dirs":[
				"..",
				"../../external/boost",
				"../../external/FlameMath",
			],
			"includes":[
				"../../tools/gyp/common.gypi",
			],
			'variables': {
				'outputSubDir': '',
			},
			"sources":[
				"../basic/include/Thread.h",
				"../basic/include/CPP11.h",
				"../basic/include/Config.h",
				"../basic/include/Basic.h",
				"../basic/include/Types.h",
				"../basic/include/PreDeclare.h",
				"../basic/include/ResIdentifier.h",
				"../basic/include/Util.h",
				"../basic/include/DllLoader.h",
				"../basic/include/Log.hpp",
				"../basic/include/Timer.hpp",
				"../basic/include/Math.hpp",
				"../basic/include/ThrowErr.hpp",
				"../basic/include/COMPtr.hpp",
				"../basic/include/Color.hpp",
				
				"../basic/src/Thread.cpp",
				"../basic/src/CPP11.cpp",
				"../basic/src/Config.cpp",
				"../basic/src/Basic.cpp",
				"../basic/src/ResIdentifier.cpp",
				"../basic/src/Util.cpp",
				"../basic/src/Timer.cpp",
				"../basic/src/DllLoader.cpp",
				"../basic/src/Log.cpp",
				"../basic/src/Math.cpp",
				"../basic/src/ThrowErr.cpp",
				"../basic/src/COMPtr.cpp",
				"../basic/src/Color.cpp",
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
									"AIR_BASIC_SOURCE",
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