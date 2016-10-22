{
	"targets":[
		{
			"target_name": "alltargets",
			"type": "none",
			"dependencies":[
				"engine",
				"basic",
				"SampleCreateWindow",
			],
		}
	],
	
	'includes': [
		'basic.gypi',
		'engine.gypi',
		'sample.gypi',
	],
	
}