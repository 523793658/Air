{
	"targets":[
		{
			"target_name": "rapidxml",
			"type": "static_library",
			"dependencies":[
			],
			"include_dirs":[
				"../src",
			],
			"includes":[
				"../../../tools/gyp/external.gypi",
			],
			'variables': {
				'outputSubDir': '',
			},
			"sources":[
				"../src/rapidxml.hpp",
				"../src/rapidxml_iterators.hpp",
				"../src/rapidxml_print.hpp",
				"../src/rapidxml_utils.hpp",
			],
			'conditions': [
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
		}
	],
}