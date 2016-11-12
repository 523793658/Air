# ����ƽ̨������
#
{
	'conditions' : [
		[ 'OS == "win"',
			{
				'msvs_disabled_warnings': [
					4204,		# ���棺���˷Ǳ�׼��չ : �ǳ����ۺϳ�ʼֵ�趨��
					4100,		# ���棺δ���õ��β�
					4152,		# ���棺���ʽ�еĺ���/����ָ��ת��
					4200,		# ���棺ʹ���˷Ǳ�׼��չ : �ṹ/�����е����С����
					4505,		# ���棺δ���õı��غ������Ƴ�(�ֲ����������ˣ���δʹ��)
					4800,		# ���棺��ֵǿ��Ϊ����ֵ��true����false��(���ܾ���)
					4201,		# ���棺ʹ���˷Ǳ�׼��չ : �����ƵĽṹ/����
					4996,		# ���棺scanf�Ĳ���ȫ����
					4819,		# ���棺ʹ����GB2312�ַ�
					4305,       # ���棺�ضϴ�'������' �� 'С����'.
					4306,       # ���棺��'С����'ת��Ϊ '������'
				], 
				'msbuild_configuration_attributes': {
					'OutputDirectory': '$(SolutionDir)..\\..\\tools\\',         		#���Ŀ¼
					'IntermediateDirectory': '$(Configuration)\\$(ProjectName)\\obj\\',   #�м�Ŀ¼
					'CharacterSet': '2',
				},
				'msbuild_settings': {
					'ClCompile': {
						'TreatWChar_tAsBuiltInType': 'true',  							#��WChar_t��Ϊ��������
						'WarningLevel': 'Level4',										#����ȼ�
						'ProgramDataBaseFileName': '$(OutDir)pdb\\$(Configuration)\\$(TargetName).pdb', #�������ݿ��ļ���
						'PreprocessorDefinitions': [],      #Ԥ����������
						#'ExceptionHandling': '/EHsc',
					},
					'Link': {
						'SubSystem': 'Console',											#����̨
						'ImportLibrary' : '$(IntDir)..\\lib\\$(TargetName).lib', 		#�����
						#'LinkIncremental': 'true',
						'ProgramDatabaseFile': '$(OutDir)pdb\\$(Configuration)\\$(TargetName).pdb', #���ɳ������ݿ��ļ�
						#'GenerateManifest': 'false',
						'GenerateDebugInformation': 'false',      						#���ɵ�����Ϣ

						'ImageHasSafeExceptionHandlers': 'false',						#ӳ����а�ȫ�쳣�������
						'AdditionalDependencies': [										#����������
							'kernel32.lib',
							'gdi32.lib',
							'winspool.lib',
							'comdlg32.lib',
							'advapi32.lib',
							'shell32.lib',
							'ole32.lib',
							'oleaut32.lib',
							'user32.lib',
							'uuid.lib',
							'odbc32.lib',
							'odbccp32.lib',
							'DelayImp.lib',
							'winmm.lib',
							'wsock32.lib',
						],
					},
				},
				'configurations': {
					'Debug': {
						'msbuild_settings': {
							'ClCompile': {
								'BasicRuntimeChecks': 'EnableFastChecks',					#��������ʱ��飨/RTC1��
								'DebugInformationFormat': 'EditAndContinue', 				# editAndContiue (/ZI)
								'Optimization': 'Disabled',           						# optimizeDisabled (/Od)
								'RuntimeTypeInfo': 'false',      							# /GR-
								'RuntimeLibrary': 'MultiThreadedDebugDLL',
								'IntrinsicFunctions': 'false'								#�����ڲ���������
							},
							'Link': {
								'GenerateDebugInformation': 'true',
								'AdditionalDependencies':[
								],
								
							},
						},
						'msbuild_configuration_attributes': {
							'LinkIncremental': 'true',
							'GenerateManifest': 'false',
						},
					},
					'Debug_x64': {
						'msbuild_settings': {
							'ClCompile': {
								'BasicRuntimeChecks': 'EnableFastChecks',					#��������ʱ��飨/RTC1��
								'DebugInformationFormat': 'EditAndContinue', 				# editAndContiue (/ZI)
								'Optimization': 'Disabled',           						# optimizeDisabled (/Od)
								'RuntimeTypeInfo': 'false',      							# /GR-
								'RuntimeLibrary': 'MultiThreadedDebugDLL',
								'IntrinsicFunctions': 'false'								#�����ڲ���������
							},
							'Link': {
								'GenerateDebugInformation': 'true',
								'AdditionalDependencies':[
								],
								'AdditionalLibraryDirectories':[
								],
							},
						},
						'msbuild_configuration_attributes': {
							'LinkIncremental': 'true',
							'GenerateManifest': 'false',
						},
						'msvs_configuration_platform': 'x64',
					},
					'Release': {
						'msbuild_settings': {
							'ClCompile': {
								'DebugInformationFormat': 'ProgramDatabase',      			# programDatabase (/Zi)
								'Optimization': 'MaxSpeed',                					# optimizeDisabled (/O2)
								'WholeProgramOptimization': 'true', 						#/GL
								'RuntimeTypeInfo': 'false',      							# /GR-
								'RuntimeLibrary': 'MultiThreadedDLL',    						# /GR-
								'IntrinsicFunctions': 'true'								#�����ڲ��������ǣ�
							},
							'Link': {
								#'LinkIncremental': 'false',
								#'GenerateManifest': 'true',
								'AdditionalDependencies':[
								],
								'GenerateDebugInformation': 'true',		
							},
						},
						'msbuild_configuration_attributes': {
							'LinkIncremental': 'false',										#��������������
							'GenerateManifest': 'false',									#������manifest
						},
					},
					'Release_x64': {
						'msbuild_settings': {
							'ClCompile': {
								'DebugInformationFormat': 'ProgramDatabase',      			# programDatabase (/Zi)
								'Optimization': 'MaxSpeed',                					# optimizeDisabled (/O2)
								'WholeProgramOptimization': 'true', 						#/GL
								'RuntimeTypeInfo': 'false',      							# /GR-
								'RuntimeLibrary': 'MultiThreadedDLL',    						# /GR-
								'IntrinsicFunctions': 'true'								#�����ڲ��������ǣ�
							},
							'Link': {
								#'LinkIncremental': 'false',
								#'GenerateManifest': 'true',
								'AdditionalDependencies':[
								],
								'GenerateDebugInformation': 'true',		
							},
						},
						'msbuild_configuration_attributes': {
							'LinkIncremental': 'false',										#��������������
							'GenerateManifest': 'false',									#������manifest
						},
						'msvs_configuration_platform': 'x64',
					},
				},
			},
		],
	],    
}
