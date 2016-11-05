{
	'variables': {
		'source_path': '../sample',
	},
	'include_dirs': [
		"../../external/boost",
		"../core/include",
		"../sample/include",
		"../",
	],
	'includes': [
		'../../tools/gyp/common.gypi',
	],
	'dependencies': [
	],
	'conditions': [
		[ 'OS == "win"',
			{
				'msbuild_configuration_attributes': {
					'CharacterSet': '1',
				},
				'msbuild_settings': {
					'ClCompile': {
						'TreatWarningAsError': 'true',
					},
				},
			},
		],
	],
	'sources': [
		'<(source_path)/include/SampleCommon.hpp',
		'<(source_path)/src/SampleCommon.cpp',
	],
}