#include "Context.h"
#include "Engine.h"
#include "basic/include/Log.hpp"
#include "SingletonManager.hpp"
#include "dxsdk/Include/d3dcommon.h"
#include <boost/lexical_cast.hpp>
#include "rendersystem/include/RenderEngine.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderEffect.hpp"
#include "rendersystem/include/ShaderObject.hpp"
#include "ResLoader.h"

#if AIR_IS_DEV_PLATFORM

#ifdef AIR_PLATFORM_WINDOWS
#define  CALL_D3DCOMPILER_DIRECTLY
#endif

#ifdef CALL_D3DCOMPILER_DIRECTLY
#include "SALWrapper.hpp"
#include "d3dcompiler.h"
#include "d3dcommon.h"
#else
#define S_OK                                        0x00000000

#define D3DCOMPILE_DEBUG                            0x00000001
#define D3DCOMPILE_SKIP_VALIDATION                  0x00000002
#define D3DCOMPILE_SKIP_OPTIMIZATION                0x00000004
#define D3DCOMPILE_PACK_MATRIX_ROW_MAJOR            0x00000008
#define D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR         0x00000010
#define D3DCOMPILE_PARTIAL_PRECISION                0x00000020
#define D3DCOMPILE_FORCE_VS_SOFTWARE_NO_OPT         0x00000040
#define D3DCOMPILE_FORCE_PS_SOFTWARE_NO_OPT         0x00000080
#define D3DCOMPILE_NO_PRESHADER                     0x00000100
#define D3DCOMPILE_AVOID_FLOW_CONTROL               0x00000200
#define D3DCOMPILE_PREFER_FLOW_CONTROL              0x00000400
#define D3DCOMPILE_ENABLE_STRICTNESS                0x00000800
#define D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY   0x00001000
#define D3DCOMPILE_IEEE_STRICTNESS                  0x00002000
#define D3DCOMPILE_OPTIMIZATION_LEVEL0              0x00004000
#define D3DCOMPILE_OPTIMIZATION_LEVEL1              0x00000000
#define D3DCOMPILE_OPTIMIZATION_LEVEL2              0x0000c000
#define D3DCOMPILE_OPTIMIZATION_LEVEL3              0x00008000
#define D3DCOMPILE_WARNINGS_ARE_ERRORS              0x00040000

struct D3D_SHADER_MACRO
{
	LPCSTR Name;
	LPCSTR Definition;
};
#endif






namespace
{
	using namespace Air;

	std::mutex singleton_mutex;

	class D3DCompilerLoader
	{
	public:
		class IncludeInterface : public ID3DInclude
		{
		private:
			IncludeInterface()
			{
				mShaderDir = Engine::getInstance().getConfig().mShaderPath;
			}

		public:

			static IncludeInterface* getInstance()
			{
				if (!mInstance)
				{
					std::lock_guard<std::mutex> lock(singleton_mutex);
					if (!mInstance)
					{
						mInstance = new IncludeInterface();
					}
				}
				return mInstance;
			}
		private:
			HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType,
				LPCSTR pFileName,
				LPCVOID pParentData,
				LPCVOID *ppData,
				UINT *pBytes) override
			{
				std::string finalPath;
				switch (IncludeType)
				{
				case D3D_INCLUDE_LOCAL:
					finalPath = mShaderDir + "\\" + pFileName;
					break;
				default:
					break;
				}

				ResIdentifierPtr res = ResLoader::getInstance().open(finalPath);
				if (res)
				{
					uint32_t fileSize = res->size();
					uint8_t * data  = (uint8_t *)malloc(fileSize);
					res->read(data, fileSize);
					*pBytes = fileSize;
					*ppData = data;
					return S_OK;
				}
				return E_FAIL;
			}
			HRESULT __stdcall Close(LPCVOID pData) override
			{
				free((void*)pData);
				return S_OK;
			}
		private:
			std::string mShaderDir;
			static IncludeInterface* mInstance;
		};

	public:
		~D3DCompilerLoader()
		{
#ifdef CALL_D3DCOMPILER_DIRECTLY
			::FreeLibrary(mModD3DCompiler);
#endif
		}
		static D3DCompilerLoader& getInstance()
		{
			static D3DCompilerLoader initer;
			return initer;
		}

		HRESULT D3DCompile(std::string const & src_data, D3D_SHADER_MACRO const * defines, char const * entry_point, char const* target, uint32_t flags1, uint32_t flags2, std::vector<uint8_t> & code, std::string & error_msgs) const
		{
#ifdef CALL_D3DCOMPILER_DIRECTLY
			
			ID3DBlob* code_blob = nullptr;
			ID3DBlob* error_msgs_blob = nullptr;
			HRESULT hr = mDynamicD3DCompile(src_data.c_str(), static_cast<UINT>(src_data.size()), nullptr, defines, IncludeInterface::getInstance(), entry_point, target, flags1, flags2, &code_blob, &error_msgs_blob);
			if (code_blob)
			{
				uint8_t const * p = static_cast<uint8_t const *>(code_blob->GetBufferPointer());
				code.assign(p, p + code_blob->GetBufferSize());
				code_blob->Release();
			}
			else
			{
				code.clear();
			}
			if (error_msgs_blob)
			{
				char const * p = static_cast<char const*>(error_msgs_blob->GetBufferPointer());
				error_msgs.assign(p, p + error_msgs_blob->GetBufferSize());
				error_msgs_blob->Release();
			}
			else
			{
				error_msgs.clear();
			}
			return hr;
#else
			std::string mark = boost::lexical_cast<std::string>(static_cast<void const *>(src_data.c_str()));
			std::string compile_input_file = entry_point + mark + "Input.tmp";
			std::string compile_output_file = entry_point + mark + "Output.tmp";

			uint32_t buffer_size;

			std::ofstream ofs(compile_input_file.c_str(), std::ios_base::binary);

			buffer_size = static_cast<uint32_t>(src_data.size());
			ofs.write(reinterpret_cast<char const *>(&buffer_size), sizeof(buffer_size));
			ofs.write(src_data.c_str(), buffer_size);

			uint32_t idx = 0;
			while ((defines[idx].Definition != nullptr) && (defines[idx].Name != nullptr))
			{
				++idx;
			}

			ofs.write(reinterpret_cast<char const *>(&idx), sizeof(idx));

			idx = 0;
			while ((defines[idx].Definition != nullptr) && (defines[idx].Name != nullptr))
			{
				ofs << defines[idx].Name << std::endl;
				ofs << defines[idx].Definition << std::endl;
				++idx;
			}

			ofs.close();

			std::ostringstream ss;
			std::string d3dcompiler_wrapper_name = "D3DCompilerWrapper";
#ifdef KLAYGE_DEBUG
			d3dcompiler_wrapper_name += "_d";
#endif
#ifdef KLAYGE_PLATFORM_WINDOWS
			ss << d3dcompiler_wrapper_name << ".exe";
#else
			static bool first = true;
			if (first)
			{
				ss << WINE_PATH << "wineserver -p";
				system(ss.str().c_str());
				// We should hold on a persistant wineserver, or XCode will lost connection after wineserver instance close and wine may not be able to find '.exe.so' file
				first = false;
				ss.str(std::string());
			}
			d3dcompiler_wrapper_name += ".exe.so";
			std::string wrapper_path = ResLoader::Instance().Locate(d3dcompiler_wrapper_name);
			ss << WINE_PATH << "wine " << wrapper_path;
#endif
			ss << " compile";
			ss << " " << compile_input_file;
			ss << " " << entry_point << " " << target;
			ss << " " << flags1 << " " << flags2;
			ss << " " << compile_output_file;
			if (system(ss.str().c_str()) != 0)
			{
				return -1;
			}

			std::ifstream ifs(compile_output_file.c_str(), std::ios_base::binary);

			uint32_t hr;
			ifs.read(reinterpret_cast<char*>(&hr), sizeof(hr));

			ifs.read(reinterpret_cast<char*>(&buffer_size), sizeof(buffer_size));
			if (buffer_size > 0)
			{
				code.resize(buffer_size);
				ifs.read(reinterpret_cast<char*>(&code[0]), buffer_size);
			}
			else
			{
				code.clear();
			}

			ifs.read(reinterpret_cast<char*>(&buffer_size), sizeof(buffer_size));
			if (buffer_size > 0)
			{
				error_msgs.resize(buffer_size);
				ifs.read(&error_msgs[0], buffer_size);
			}
			else
			{
				error_msgs.clear();
			}

			ifs.close();

			remove(compile_input_file.c_str());
			remove(compile_output_file.c_str());

			return hr;
#endif
		}
		HRESULT D3DReflect(std::vector<uint8_t> const & shader_code, void** reflector)
		{
#ifdef CALL_D3DCOMPILER_DIRECTLY
			static GUID const IID_ID3D11ShaderReflection_47
				= { 0x8d536ca1, 0x0cca, 0x4956,{ 0xa8, 0x37, 0x78, 0x69, 0x63, 0x75, 0x55, 0x84 } };
			return mDynamicD3DReflect(&shader_code[0], static_cast<UINT>(shader_code.size()), IID_ID3D11ShaderReflection_47, reflector);
#else
			AIR_UNUSED(shader_code);
			AIR_UNUSED(reflector);
			return S_OK;
#endif
		}

		HRESULT D3DStripShader(std::vector<uint8_t> const & shader_code, uint32_t strip_flags, std::vector<uint8_t>& stripped_code)
		{
#ifdef CALL_D3DCOMPILER_DIRECTLY
			ID3DBlob* stripped_blob = nullptr;
			HRESULT hr = mDynamicD3DStripShader(&shader_code[0], static_cast<UINT>(shader_code.size()), strip_flags, &stripped_blob);
			uint8_t const * p = static_cast<uint8_t const *>(stripped_blob->GetBufferPointer());
			stripped_code.assign(p, p + stripped_blob->GetBufferSize());
			stripped_blob->Release();
			return hr;
#else
			AIR_UNUSED(shader_code);
			AIR_UNUSED(strip_flags);
			AIR_UNUSED(stripped_code);
			return S_OK;
#endif
		}


	private:
		D3DCompilerLoader()
		{
#ifdef CALL_D3DCOMPILER_DIRECTLY
			mModD3DCompiler = ::LoadLibraryEx(TEXT("d3dcompiler_47.dll"), nullptr, 0);
			AIR_ASSUME(mModD3DCompiler != nullptr);
			mDynamicD3DCompile = reinterpret_cast<pD3DCompile>(::GetProcAddress(mModD3DCompiler, "D3DCompile"));
			mDynamicD3DReflect = reinterpret_cast<D3DReflectFunc>(::GetProcAddress(mModD3DCompiler, "D3DReflect"));
			mDynamicD3DStripShader = reinterpret_cast<D3DStripShaderFunc>(::GetProcAddress(mModD3DCompiler, "D3DStripShader"));
#endif
		}


#ifdef CALL_D3DCOMPILER_DIRECTLY
		typedef HRESULT(WINAPI *D3DReflectFunc)(LPCVOID pSrcData, SIZE_T SRCDataSize, REFIID pInterface, void** ppReflector);
		typedef HRESULT(WINAPI *D3DStripShaderFunc)(LPCVOID pShaderBytecode, SIZE_T bytecodeLength, UINT uStripFlags, ID3DBlob** ppStrippedBlob);

		HMODULE mModD3DCompiler;
		pD3DCompile mDynamicD3DCompile;
		D3DReflectFunc mDynamicD3DReflect;
		D3DStripShaderFunc mDynamicD3DStripShader;
#endif
	};

	D3DCompilerLoader::IncludeInterface* D3DCompilerLoader::IncludeInterface::mInstance;

}
#endif


namespace Air
{
#if AIR_IS_DEV_PLATFORM
	std::vector<uint8_t> ShaderObject::compileToDXBC(ShaderType type, RenderEffect const & effect, RenderTechnique const & tech, RenderPass const & pass, std::vector<std::pair<char const *, char const *>> const & api_special_macros, char const * func_name, char const * shader_profile, uint32_t flags)
	{
		RenderEngine const & re = SingletonManager::getRenderFactoryInstance().getRenderEngineInstance();
		RenderDeviceCaps const & caps = re.getDeviceCaps();
		std::vector<uint8_t> code;
		std::string const & hlsl_shader_text = effect.getHLSHShader();
		std::string max_sm_str = boost::lexical_cast<std::string>(caps.mMaxShaderModel.getFullVersion());
		std::string max_tex_array_str = boost::lexical_cast<std::string>(caps.mMaxTextureArrayLength);
		std::string max_tex_depth_str = boost::lexical_cast<std::string>(caps.mMaxTextureDepth);
		std::string max_tex_units_str = boost::lexical_cast<std::string>(static_cast<int>(
			caps.mMaxPixelTextureUnits));
		std::string flipping_str = boost::lexical_cast<std::string>(re.requiresFlipping() ? -1 : +1);
		std::string render_to_tex_array_str = boost::lexical_cast<std::string>(caps.mRenderToTextureArraySupport ? 1 : 0);

		std::string err_msg;
		std::vector<D3D_SHADER_MACRO> macros;
		for (uint32_t i = 0; i < api_special_macros.size(); ++i)
		{
			D3D_SHADER_MACRO macro = { api_special_macros[i].first, api_special_macros[i].second };
			macros.push_back(macro);
		}
		{
			D3D_SHADER_MACRO macro = { "AIR_SHADER_MODEL", max_sm_str.c_str() };
			macros.push_back(macro);
		}

		{
			D3D_SHADER_MACRO macro = { "AIR_MAX_TEX_ARRAY_LEN", max_tex_array_str.c_str() };
			macros.push_back(macro);
		}
		{
			D3D_SHADER_MACRO macro = { "AIR_MAX_TEX_DEPTH", max_tex_depth_str.c_str() };
			macros.push_back(macro);
		}
		{
			D3D_SHADER_MACRO macro = { "AIR_MAX_TEX_UNITS", max_tex_units_str.c_str() };
			macros.push_back(macro);
		}
		{
			D3D_SHADER_MACRO macro = { "AIR_FLIPPING", flipping_str.c_str() };
			macros.push_back(macro);
		}
		{
			D3D_SHADER_MACRO macro = { "AIR_RENDER_TO_TEX_ARRAY", render_to_tex_array_str.c_str() };
			macros.push_back(macro);
		}
		if (!caps.mFPColorSupport)
		{
			D3D_SHADER_MACRO macro = { "AIR_NO_FP_COLOR", "1" };
			macros.push_back(macro);
		}
		if (caps.mPackToRGBARequired)
		{
			D3D_SHADER_MACRO macro = { "AIR_PACK_TO_RGBA", "1" };
			macros.push_back(macro);
		}
		{
			D3D_SHADER_MACRO macro_shader_type = { "", "1" };
			switch (type)
			{
			case Air::ShaderObject::ST_VertexShader:
				macro_shader_type.Name = "AIR_VERTEX_SHADER";
				break;
			case Air::ShaderObject::ST_PixelShader:
				macro_shader_type.Name = "AIR_PIXEL_SHADER";
				break;
			case Air::ShaderObject::ST_GeometryShader:
				macro_shader_type.Name = "AIR_GEOMETRY_SHADER";
				break;
			case Air::ShaderObject::ST_ComputeShader:
				macro_shader_type.Name = "AIR_COMPUTE_SHADER";
				break;
			case Air::ShaderObject::ST_HullShader:
				macro_shader_type.Name = "AIR_HULL_SHADER";
				break;
			case Air::ShaderObject::ST_DomainShader:
				macro_shader_type.Name = "AIR_DOMAIN_SHADER";
				break;
			default:
				AIR_UNREACHABLE("Invalid shader type");
				break;
			}
			macros.push_back(macro_shader_type);
		}
		for (uint32_t i = 0; i < tech.getNumMacros(); ++i)
		{
			std::pair<std::string, std::string> const & name_value = tech.getMacroByIndex(i);
			D3D_SHADER_MACRO macro = { name_value.first.c_str(), name_value.second.c_str() };
			macros.push_back(macro);
		}
		for (uint32_t i = 0; i < pass.getNumMacros(); ++i)
		{
			std::pair<std::string, std::string> const & name_value = pass.getMacroByIndex(i);
			D3D_SHADER_MACRO macro = { name_value.first.c_str(), name_value.second.c_str() };
			macros.push_back(macro);
		}
		{
			D3D_SHADER_MACRO macro_end = { nullptr, nullptr };
			macros.push_back(macro_end);
		}
		D3DCompilerLoader::getInstance().D3DCompile(hlsl_shader_text, &macros[0], func_name, shader_profile, flags, 0, code, err_msg);
		if (!err_msg.empty())
		{
			logError("Error when compiling %s:", func_name);
			std::map<int, std::vector<std::string>> err_lines;
			{
				std::istringstream err_iss(err_msg);
				std::string err_str;
				while (err_iss)
				{
					std::getline(err_iss, err_str);
					int err_line = -1;
					std::string::size_type pos = err_str.find("): error X");
					if (pos == std::string::npos)
					{
						pos = err_str.find("): warning X");
					}
					if (pos != std::string::npos)
					{
						std::string part_err_str = err_str.substr(0, pos);
						pos = part_err_str.rfind("(");
						part_err_str = part_err_str.substr(pos + 1);
						std::istringstream(part_err_str) >> err_line;
					}
					std::vector<std::string>& msgs = err_lines[err_line];
					bool found = false;
					for (auto const & msg : msgs)
					{
						if (msg == err_str)
						{
							found = true;
							break;
						}
					}
					if (!found)
					{
						if ((0 == err_str.find("error X")) || (0 == err_str.find("warning X")))
						{
							err_str = "(0): " + err_str;
						}
						msgs.push_back(err_str);
					}
				}
			}
			for (auto iter = err_lines.begin(); iter != err_lines.end(); ++iter)
			{
				if (iter->first >= 0)
				{
					std::istringstream iss(hlsl_shader_text);
					std::string s;
					int line = 1;
					logInfo("...");
					while (iss && ((iter->first - line) >= 3))
					{
						std::getline(iss, s);
						++line;
					}
					while (iss && (abs(line - iter->first) < 3))
					{
						std::getline(iss, s);
						while (!s.empty() && (('\r' == s[s.size() - 1]) || ('\n' == s[s.size() - 1])))
						{
							s.resize(s.size() - 1);
						}
						logInfo("%d %s", line, s.c_str());
						++line;
					}
					logInfo("...");
				}
				for (auto const & msg : iter->second)
				{
					logError(msg.c_str());
				}
			}
		}
		return code;
	}

	void ShaderObject::reflectionDXBC(std::vector<uint8_t> const & code, void** reflector)
	{
		D3DCompilerLoader::getInstance().D3DReflect(code, reflector);
	}

	std::vector<uint8_t> ShaderObject::stripDXBC(std::vector<uint8_t> const & code, uint32_t strip_flags)
	{
		std::vector<uint8_t> ret;
		D3DCompilerLoader::getInstance().D3DStripShader(code, strip_flags, ret);
		return ret;
	}

#endif
}