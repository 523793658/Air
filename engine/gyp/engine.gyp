{
	"targets":[
		{
			"target_name": "alltargets",
			"type": "none",
			"dependencies":[
				"engine",
				"basic",
				"SampleCreateWindow",
				"RenderEngineD3D11",
			],
		}
	],
	
	'includes': [
		'basic.gypi',
		'engine.gypi',
		'sample.gypi',
		'render_engine_d3d11.gypi',
	],
	
}