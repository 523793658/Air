# 第三方库的配置
#
{
	'conditions' : [
		[ 'OS == "win"',
			{
				'msvs_disabled_warnings': [
					4100,		# 警告：未引用的形参
					4152,		# 警告：表达式中的函数/数据指针转换
					4200,		# 警告：使用了非标准扩展 : 结构/联合中的零大小数组
					4505,		# 警告：未引用的本地函数已移除(局部函数定义了，但未使用)
					4800,		# 警告：将值强制为布尔值“true”或“false”(性能警告)
				], 
				'msbuild_configuration_attributes': {
						'CharacterSet': '1',												# unicode字符集
						#'OutputDirectory': '$(SolutionDir)..\\..\\tools\\',         		#输出目录
						#'IntermediateDirectory': '$(Configuration)\\$(ProjectName)\\obj\\',   #中间目录
				},
				'msbuild_settings': {
						'ClCompile': {
							'TreatWChar_tAsBuiltInType': 'true',  							#将WChar_t视为内置类型
							'ProgramDataBaseFileName': '$(OutDir)pdb\\$(Configuration)\\$(TargetName).pdb', #程序数据库文件名
							'PreprocessorDefinitions': ['WIN32','_USING_V120_SDK71_'],      #预处理器定义
						},
						'Link': {
							'MinimumRequiredVersion':'5.01',								#用来支持XP
							'SubSystem': 'Console',											#控制台
							'ImportLibrary' : '$(IntDir)..\\lib\\$(TargetName).lib', 		#到入库
							'ProgramDatabaseFile': '$(OutDir)pdb\\$(Configuration)\\$(TargetName).pdb', #生成程序数据库文件
							'GenerateDebugInformation': 'false',      						#生成调试信息
							'OutputFile': '$(OutDir)bin\\$(Configuration)\\$(TargetName)$(TargetExt)',	#输出文件
							'ImageHasSafeExceptionHandlers': 'false',						#映像具有安全异常处理程序
						},
				},
				'configurations': {
					'Debug': {
						'msbuild_settings': {
							'ClCompile': {
								'BasicRuntimeChecks': 'EnableFastChecks',					#基本运行时检查（/RTC1）
								'DebugInformationFormat': 'EditAndContinue', 				# editAndContiue (/ZI)
								'Optimization': 'Disabled',           						# optimizeDisabled (/Od)
								'PreprocessorDefinitions': ['_DEBUG'],						#预处理器定义
								'RuntimeLibrary': 'MultiThreadedDebug',         			# rtMultiThreadedDebugDLL (/MTd)
								'IntrinsicFunctions': 'false'								#启用内部函数（否）
							},
							'Link': {
								'GenerateDebugInformation': 'true', 						#生成调试信息
							},
							'Manifest':{
								'EnableDpiAwareness': 'PerMonitorHighDPIAware'
							},
						},
						'msbuild_configuration_attributes': {
							'LinkIncremental': 'true',
							'GenerateManifest': 'true',
						},
					},
					'Release': {
						'defines': [
							'NDEBUG',
						],
						'defines!': [
							'_DEBUG',
						],
						'msbuild_settings': {
							'ClCompile': {
								'DebugInformationFormat': 'ProgramDatabase',      			# programDatabase (/Zi)
								'Optimization': 'MaxSpeed',                					# optimizeDisabled (/O2)
								'WholeProgramOptimization': 'true', 						#/GL
								'PreprocessorDefinitions': ['NDEBUG'],
								'RuntimeLibrary': 'MultiThreaded',              			# rtMultiThreadedDLL (/MT)
								'IntrinsicFunctions': 'true'								#启用内部函数（是）
							},
							'Link': {
								'GenerateDebugInformation': 'true',							#生成调试信息
							},
							'Manifest':{
								'EnableDpiAwareness': 'PerMonitorHighDPIAware'
							},
						},
						'msbuild_configuration_attributes': {
							'LinkIncremental': 'false',										#不启用增量链接
							'GenerateManifest': 'true',									#不生成manifest
						},
					},
				},
			},
		],
		['OS == "android"',
			{
				'ldflags': 
				[
					'-lz',
					'-llog',
					'-ldl',
					'-lGLESv2',
					'-lEGL',
					'-landroid',
				],
			},
		],
		[ 'OS == "ios" or OS == "iossim"' ,
			{
		  		# 'mac_bundle':1,
				'defines': [
					'IOS',
				],
				'link_settings' : {
					'libraries': [
						'$(SDKROOT)/System/Library/Frameworks/Foundation.framework',
					],
				},
				'xcode_settings' : {
					'CODE_SIGN_IDENTITY': 'iPhone Distribution: Xiamen Woobest Interactive Network Technology Co.,Ltd (245A5Z9XVD)',
					'IPHONEOS_DEPLOYMENT_TARGET': '8.1',
					'DEAD_CODE_STRIPPING': 'NO',
					#'WRAPPER_EXTENSION': 'framework',
					'SYMROOT' : '<(XCODE_BUILD_DIR)/',
					'ARCHS' : '$(ARCHS_STANDARD)',
					'VALID_ARCHS' : 'arm64 armv7 armv7s',
					'SUPPORTED_PLATFORMS' : 'iphoneos iphonesimulator',
					'SDKROOT' : 'iphoneos8.1',
				}, 
			}, 	
		], 
	],    
}
