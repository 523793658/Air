{
	"target_defaults":{
	},
	"targets":[
		{
			"target_name": "RenderEngineD3D11",
			"dependencies": [
				'engine',
			],
			"type": "shared_library",
			"include_dirs":[
				"../../external/boost",
				"../../external/dxsdk/include",
				"../core/include",
				"../",
				"../render_engine_d3d11/include",
			],
			"includes":[
				"../../tools/gyp/common.gypi",
			],
			"sources":[
				"../render_engine_d3d11/include/D3D11RenderFactory.hpp",
				"../render_engine_d3d11/include/D3D11RenderFactoryInternal.hpp",
				"../render_engine_d3d11/include/D3D11RenderEngine.hpp",
				"../render_engine_d3d11/include/D3D11Typedefs.hpp",
				"../render_engine_d3d11/include/D3D11RenderLayout.hpp",
				"../render_engine_d3d11/include/D3D11AdapterList.hpp",
				"../render_engine_d3d11/include/D3D11Adapter.hpp",
				"../render_engine_d3d11/include/D3D11VideoMode.hpp",
				"../render_engine_d3d11/include/D3D11RenderWindow.hpp",
				
				
				"../render_engine_d3d11/src/D3D11RenderFactory.cpp",
				"../render_engine_d3d11/src/D3D11RenderEngine.cpp",
				"../render_engine_d3d11/src/D3D11RenderLayout.cpp",
				"../render_engine_d3d11/src/D3D11AdapterList.cpp",
				"../render_engine_d3d11/src/D3D11Adapter.cpp",
				"../render_engine_d3d11/src/D3D11VideoMode.cpp",
				"../render_engine_d3d11/src/D3D11RenderWindow.cpp",
			],
			"conditions":[
				[
					"OS == 'win'",
					{
						"msbuild_configuration_attributes":{
							"CharacterSet": "1",
						}, 
						"msbuild_settings":{
							"ClCompile":{
								'PreprocessorDefinitions': [
									"AIR_D3D11_RE_SOURCE",
								],
							},
						},
						"configurations":{
							"Debug":{
								"msbuild_settings":{
									'Link': {
										'GenerateDebugInformation': 'true',
										'AdditionalDependencies':[
										],
									},
								},
							},
						},
					},
				],
			],
		},
	],
}