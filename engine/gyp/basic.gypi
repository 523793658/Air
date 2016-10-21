{
	"target_defaults":{
	},
	"targets":[
		{
			"target_name": "basic",
			"type": "static_library",
			"include_dirs":[
				"../basic/include",
			],
			"includes":[
				"../../tools/gyp/common.gypi",
			],
			"sources":[
				"../basic/include/Thread.h",
				"../basic/include/CPP11.h",
				"../basic/include/Config.h",
				"../basic/include/Basic.h",
				"../basic/include/Types.h",
				"../basic/include/PreDeclare.h",
				"../basic/include/ResIdentifier.h",
				"../basic/include/Math.hpp",
				"../basic/include/Util.h",
				"../basic/src/Thread.cpp",
				"../basic/src/CPP11.cpp",
				"../basic/src/Config.cpp",
				"../basic/src/Basic.cpp",
				"../basic/src/ResIdentifier.cpp",
				"../basic/src/Util.cpp",
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