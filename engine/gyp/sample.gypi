{
	'targets': [
		{
			'target_name': 'SampleCreateWindow',
			'product_name': 'SampleCreateWindow',
			'type': 'executable',
			'includes': [
				'sample_common.gypi',
			],
			'sources': [
				'../sample/include/SampleCreateWindow.hpp',
				'../sample/src/SampleCreateWindow.cpp',
			],
		},
		{
			'target_name': 'SampleRenderOneBall',
			'product_name': 'SampleRenderOneBall',
			'type': 'executable',
			'includes': [
				'sample_common.gypi',
			],
			"dependencies": [
				'basic',
				'engine',
			],
			'sources': [
				'../sample/include/RenderOneBall.hpp',
				'../sample/src/RenderOneBall.cpp',
			],
		},
	],
}