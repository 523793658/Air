#ifndef _ShaderFormat_H_
#define _ShaderFormat_H_
#pragma once
namespace Air
{
#define SHADERFORMAT_MODULE_WILDCARD TEXT("*ShaderFormat*")

	class IShaderFormat
	{
	public:
		virtual void compileShader(std::string format, const struct ShaderCompilerInput& input, struct ShaderCompilerOutput& output, const std::string_view & workingDirectory) const = 0;

		virtual uint16_t getVersion(std::string format) const = 0;

		virtual void getSupportedFormats(std::vector<std::string>& format) const = 0;

	public:
		virtual ~IShaderFormat(){}
	};
}
#endif
