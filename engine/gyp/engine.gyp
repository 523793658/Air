{
	"targets":[
		{
			"target_name": "alltargets",
			"type": "none",
			'variables': {
				'shaderPath': '../../assets/shader',
			},
			"dependencies":[
				"engine",
				"basic",
				"RenderEngineD3D11",
				"SceneManagerOCTree",
				"SampleCreateWindow",
				"SampleRenderOneBall",
				"InputEngineMsg",
			],
			'sources':
			[
				"<(shaderPath)/lighting.asd",
				"<(shaderPath)/simpleforward.asd",
				"<(shaderPath)/skyBox.asd",
				"<(shaderPath)/toneMap.asd",
				
				
				"<(shaderPath)/shadingModels.hlsl",
				"<(shaderPath)/brdf.hlsl",
				"<(shaderPath)/common.hlsl",
				"<(shaderPath)/skyBox.hlsl",
				"<(shaderPath)/util.hlsl",
				"<(shaderPath)/toneMap.hlsl",
			],
		},
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