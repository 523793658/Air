{
	"target_defaults":{
	},
	"targets":[
		{
			"target_name": "RenderEngineD3D11",
			"dependencies": [
				'basic',
				'engine',
			],
			'variables': {
				'outputSubDir': 'render\\',
			},
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
				"../render_engine_d3d11/include/D3D11FrameBuffer.hpp",
				"../render_engine_d3d11/include/D3D11Mapping.hpp",
				"../render_engine_d3d11/include/D3D11Texture.hpp",
				"../render_engine_d3d11/include/D3D11RenderView.hpp",
				"../render_engine_d3d11/include/D3D11GraphicsBuffer.hpp",
				"../render_engine_d3d11/include/D3D11ShaderObject.hpp",
				"../render_engine_d3d11/include/D3D11RenderStateObject.hpp",
				
				
				"../render_engine_d3d11/src/D3D11RenderFactory.cpp",
				"../render_engine_d3d11/src/D3D11RenderEngine.cpp",
				"../render_engine_d3d11/src/D3D11RenderLayout.cpp",
				"../render_engine_d3d11/src/D3D11AdapterList.cpp",
				"../render_engine_d3d11/src/D3D11Adapter.cpp",
				"../render_engine_d3d11/src/D3D11VideoMode.cpp",
				"../render_engine_d3d11/src/D3D11RenderWindow.cpp",
				"../render_engine_d3d11/src/D3D11FrameBuffer.cpp",
				"../render_engine_d3d11/src/D3D11Mapping.cpp",
				"../render_engine_d3d11/src/D3D11Texture.cpp",
				"../render_engine_d3d11/src/D3D11Texture1D.cpp",
				"../render_engine_d3d11/src/D3D11Texture2D.cpp",
				"../render_engine_d3d11/src/D3D11Texture3D.cpp",
				"../render_engine_d3d11/src/D3D11TextureCube.cpp",
				"../render_engine_d3d11/src/D3D11RenderView.cpp",
				"../render_engine_d3d11/src/D3D11GraphicsBuffer.cpp",
				"../render_engine_d3d11/src/D3D11ShaderObject.cpp",
				"../render_engine_d3d11/src/D3D11RenderStateObject.cpp",
			],
			"conditions":[
				[
					"OS == 'win'",
					{
				
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
										'AdditionalDependencies':[
											'dxgi.lib',
											'd3d11.lib',
										],
										'AdditionalLibraryDirectories':[
											'../../external/dxsdk/lib/x86',
										],
									},
								},
							},
							"Debug_x64":{
								"msbuild_settings":{
									'Link': {
										'AdditionalDependencies':[
											'dxgi.lib',
											'd3d11.lib',
										],
										'AdditionalLibraryDirectories':[
											'../../external/dxsdk/lib/x64',
										],
									},
									
								},
							},
							"Release":{
								"msbuild_settings":{
									'Link': {
										'AdditionalDependencies':[
											'dxgi.lib',
											'd3d11.lib',
										],
										'AdditionalLibraryDirectories':[
											'../../external/dxsdk/lib/x86',
										],
									},
									
								},
							},
							"Release_x64":{
								"msbuild_settings":{
									'Link': {
										'AdditionalDependencies':[
											'dxgi.lib',
											'd3d11.lib',
										],
										'AdditionalLibraryDirectories':[
											'../../external/dxsdk/lib/x64',
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