{
	"target_defaults":{
	},
	"targets":[
		{
			"target_name": "ScriptEngineMono",
			"dependencies": [
				'basic',
				'engine',
			],
			'variables': {
				'outputSubDir': 'ScriptEngine\\',
			},
			"type": "shared_library",
			"include_dirs":[
				"../../external/mono/include/mono-2.0",
				"../core/include",
				"../",
				"../script_engine/include",
			],
			"includes":[
				"../../tools/gyp/common.gypi",
			],
			"sources":[
				"../script_engine_mono/MonoScriptEngineFactory.hpp",
				"../script_engine_mono/MonoScriptEngineFactory.cpp",
				"../script_engine_mono/MonoScriptEngine.hpp",
				"../script_engine_mono/MonoScriptEngine.cpp",
			],
			"conditions":[
				[
					"OS == 'win'",
					{
				
						"msbuild_settings":{
							"ClCompile":{
								'PreprocessorDefinitions': [
									"AIR_SCRIPT_ENGINE_MONO_SOURCE",
								],
							},
							'Link': {
								'AdditionalDependencies':[
									"mono-2.0-sgen.lib",
								],
								'AdditionalLibraryDirectories':[
									"external/mono/lib",
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