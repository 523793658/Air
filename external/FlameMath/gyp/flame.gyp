{
	"targets":[
		{
			"target_name": "FlameMath",
			"type": "static_library",
			"dependencies":[
			],
			"include_dirs":[
				'..',
			],
			'variables': {
				'outputSubDir': '',
			},
			"sources":[
				'../FlameMath.h',
				'../FlameMath.cpp',
				'../DirectXCollision.h',
				'../DirectXCollision.inl',
				'../DirectXColors.h',
				'../DirectXMath.h',
				'../DirectXMathConvert.inl',
				'../DirectXMathMatrix.inl',
				'../DirectXMathMisc.inl',
				'../DirectXMathVector.inl',
				'../DirectXPackedVector.h',
				'../DirectXPackedVector.inl',
			],
			'conditions': [
				[
				
					"OS == 'win'",
					{
						"msvs_postbuild":'copy /Y "$(OutDir)lib\\Air$(ProjectName)$(TargetExt)" "..\\..\..\lib\\$(Platform)\\$(Configuration)"',
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
			'includes': [
				'../../../tools/gyp/common.gypi',
			],
		}
	],
}