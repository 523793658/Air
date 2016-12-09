{
	"target_defaults":{
	},
	"targets":[
		{
			"target_name": "SceneManagerOCTree",
			"dependencies": [
				'basic',
				'engine',
			],
			'variables': {
				'outputSubDir': 'sceneManager\\',
			},
			"type": "shared_library",
			"include_dirs":[
				"../../external/boost",
				"../core/include",
				"../",
				"../scene_manager_octree/include",
			],
			"includes":[
				"../../tools/gyp/common.gypi",
			],
			"sources":[
				"../scene_manager_octree/include/OCTree.hpp",
				"../scene_manager_octree/include/OCTreeFactory.hpp",
				
				"../scene_manager_octree/src/OCTree.cpp",
				"../scene_manager_octree/src/OCTreeFactory.cpp",
			],
			"conditions":[
				[
					"OS == 'win'",
					{
						"msbuild_settings":{
							"ClCompile":{
								'PreprocessorDefinitions': [
									"AIR_SCENE_MANAGER_OC_TREE_SOURCE",
								],
							},
						},
					},
				],
			],
		},
	],
}