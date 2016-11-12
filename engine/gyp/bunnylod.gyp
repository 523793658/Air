{
	"targets":[
		{
			"target_name": "alltargets",
			"type": "executable",
			"dependencies":[
			],
			"include_dirs":[
				'../bunnylod',
			],
			
			"sources":[
				'../bunnylod/bunnygut.cpp',
				'../bunnylod/font.cpp',
				'../bunnylod/font.h',
				'../bunnylod/list.h',
				'../bunnylod/progmesh.cpp',
				'../bunnylod/progmesh.h',
				'../bunnylod/rabdata.cpp',
				'../bunnylod/rabdata.h',
				'../bunnylod/vector.h',
				'../bunnylod/vector.cpp',
				'../bunnylod/winmain.cpp',
			],
			'conditions': [
				[ 'OS == "win"',
					{
						'msbuild_settings': 
						{
							'Link': {
								
								'AdditionalDependencies': [
									"opengl32.lib",
									"glu32.lib",
								],
							},
						},
						'configurations': {
							'Debug':{
								'msbuild_settings': {
									'Link': {
										'SubSystem': 'Windows',	
									},
								},
							},
							'Release':{
								'msbuild_settings': {
									'Link': {
										'SubSystem': 'Windows',	
									},
								},
							},
						},
					},
				],
			],
			'includes': [
				'../../tools/gyp/common.gypi',
			],
		}
	],
}