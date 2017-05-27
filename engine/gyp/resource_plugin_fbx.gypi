{
	"target_defaults":{
	},
	"targets":[
		{
			"target_name": "ResourcePluginFBX",
			"dependencies": [
				'basic',
				'engine',
			],
			'variables': {
				'outputSubDir': 'resource_plugin\\',
			},
			"type": "shared_library",
			"include_dirs":[
				"../core/include",
				"../resource_plugin_fbx/include",
				"../",
				'$(FBX_SDK_PATH)Include',
			],
			"includes":[
				"../../tools/gyp/common.gypi",
			],
			"sources":[
				"../resource_plugin_fbx/include/ResourcePluginFBX.h",
				"../resource_plugin_fbx/src/ResourcePluginFBX.cpp",
			],
			"conditions":[
				[
					"OS == 'win'",
					{
				
						"msbuild_settings":{
							"ClCompile":{
								'PreprocessorDefinitions': [
									"AIR_FBX_SOURCE",
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