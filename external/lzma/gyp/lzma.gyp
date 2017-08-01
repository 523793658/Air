{
	"targets":[
		{
			"target_name": "lzma",
			"type": "static_library",
			"dependencies":[
			],
			"include_dirs":[
				'../c',
			],
			"includes":[
				"../../../tools/gyp/external.gypi",
			],
			'variables': {
				'outputSubDir': '',
			},
			"sources":[
				'../c/7zTypes.h',
				'../c/Alloc.h',
				'../c/Alloc.c',
				'../c/LzFind.h',
				'../c/LzFind.c',
				'../c/LzFindMt.h',
				'../c/LzFindMt.c',
				'../c/LzHash.h',
				'../c/LzmaDec.h',
				'../c/LzmaDec.c',
				'../c/LzmaEnc.h',
				'../c/LzmaEnc.c',
				'../c/LzmaLib.h',
				'../c/LzmaLib.c',
				'../c/Threads.h',
				'../c/Threads.c',
				
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