{
	"targets":[
		{
			"target_name": "Effects11",
			"type": "static_library",
			"include_dirs":[
				"../include",
			],
			"includes":[
				"../../tools/gyp/common.gypi",
			],
			"sources":[
				"../include/d3dx11effect.h",
				"../include/d3dxGlobal.h",
				"../include/EffectBinaryFormat.h",
				"../include/EffectLoad.h",
				"../include/EffectStateBase11.h",
				"../include/EffectStates11.h",
				"../include/SOParser.h",
				"../include/Effect.h",
				"../include/IUnknownImp.h",
				"../include/pchfx.h",
				"../include/EffectVariable.inl",

				"../src/d3dxGlobal.cpp",
				"../src/EffectAPI.cpp",
				"../src/EffectLoad.cpp",
				"../src/EffectNonRuntime.cpp",
				"../src/EffectReflection.cpp",
				"../src/EffectRuntime.cpp",
				
			],
		},
	],
}