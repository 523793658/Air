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
				'../../external/2018.1.1/Include',
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
											'libfbxsdk.lib',
										],
										'AdditionalLibraryDirectories':[
											'../../external/2018.1.1/lib/vs2015/x86/debug',
										],
									},
								},
							},
							"Debug_x64":{
								"msbuild_settings":{
									'Link': {
										'AdditionalDependencies':[
											'libfbxsdk.lib',
										],
										'AdditionalLibraryDirectories':[
											'../../external/2018.1.1/lib/vs2015/x64/debug',
										],
									},
									
								},
							},
							"Release":{
								"msbuild_settings":{
									'Link': {
										'AdditionalDependencies':[
											'libfbxsdk.lib',
										],
										'AdditionalLibraryDirectories':[
											'../../external/2018.1.1/lib/vs2015/x86/release',
										],
									},
									
								},
							},
							"Release_x64":{
								"msbuild_settings":{
									'Link': {
										'AdditionalDependencies':[
											'libfbxsdk.lib',

										],
										'AdditionalLibraryDirectories':[
											'../../external/2018.1.1/lib/vs2015/x64/release',
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