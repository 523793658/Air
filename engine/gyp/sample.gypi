{
	'targets': [
		{
			'target_name': 'SampleCreateWindow',
			'product_name': 'SampleCreateWindow',
			'type': 'executable',
			"dependencies": [
				"engine",
			],
			'includes': [
				'sample_common.gypi',
			],
			'sources': [
				'../sample/include/SampleCreateWindow.hpp',
				'../sample/src/SampleCreateWindow.cpp',
			],
		},
	],
}