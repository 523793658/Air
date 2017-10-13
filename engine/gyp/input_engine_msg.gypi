{
	"target_defaults":{
	},
	"targets":[
		{
			"target_name": "InputEngineMsg",
			"dependencies": [
				'basic',
				'engine',
			],
			'variables': {
				'outputSubDir': 'Input\\',
			},
			"type": "shared_library",
			"include_dirs":[
				"../../external/boost",
				"../core/include",
				"../",
				"../input_system_msg/include",
			],
			"includes":[
				"../../tools/gyp/common.gypi",
			],
			"sources":[
				"../input_system_msg/include/MInputFactory.hpp",
				"../input_system_msg/include/MInput.hpp",
				
				"../input_system_msg/src/MInputFactory.cpp",
				"../input_system_msg/src/MInput.cpp",
			],
			"conditions":[
				[
					"OS == 'win'",
					{
				
						"msbuild_settings":{
							"ClCompile":{
								'PreprocessorDefinitions': [
									"AIR_INPUT_MSG_IE_SOURCE",
								],
							},
						},
						"configurations":{
							"Debug":{
								"msbuild_settings":{
									'Link': {
										'AdditionalDependencies':[
											
										],
										'AdditionalLibraryDirectories':[
											
										],
									},
								},
							},
							"Debug_x64":{
								"msbuild_settings":{
									'Link': {
										'AdditionalDependencies':[
										],
										'AdditionalLibraryDirectories':[
										],
									},
									
								},
							},
							"Release":{
								"msbuild_settings":{
									'Link': {
										'AdditionalDependencies':[
										],
										'AdditionalLibraryDirectories':[
										],
									},
									
								},
							},
							"Release_x64":{
								"msbuild_settings":{
									'Link': {
										'AdditionalDependencies':[
										],
										'AdditionalLibraryDirectories':[
										],
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