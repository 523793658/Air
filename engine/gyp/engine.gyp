{
	"targets":[
		{
			"target_name": "alltargets",
			"type": "none",
			"dependencies":[
				"engine",
				"basic",
				"RenderEngineD3D11",
				"SceneManagerOCTree",
				"SampleCreateWindow",
				"SampleRenderOneBall",
				"InputEngineMsg",
			],
		}
	],
	
	'includes': [
		'basic.gypi',
		'engine.gypi',
		'sample.gypi',
		'render_engine_d3d11.gypi',
		'scene_manager_octree.gypi',
		'resource_plugin_fbx.gypi',
		"input_engine_msg.gypi",
	],
	
}