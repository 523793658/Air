#ifndef _ElementFormat_H_
#define _ElementFormat_H_
#include <vector>
#include <stdint.h>
namespace Air
{
	enum ElementChannel
	{
		EC_R = 0UL,
		EC_G = 1UL,
		EC_B = 2UL,
		EC_A = 3UL,
		EC_D = 4UL,
		EC_S = 5UL,
		EC_BC = 6UL,
		EC_E = 7UL,
		EC_ETC = 8UL
	};

	enum ElementChannelType
	{
		ECT_UNorm = 0UL,
		ECT_SNorm = 1UL,
		ECT_UInt = 2UL,
		ECT_SInt = 3UL,
		ECT_Float = 4UL,
		ECT_UNorm_SRGB = 5UL,
		ECT_Typeless = 6UL,
		ECT_SharedExp = 7UL
	};

	enum ElementAccessHint
	{
		EAH_CPU_Read = 1UL << 0,
		EAH_CPU_Write = 1UL << 1,
		EAH_GPU_Read = 1UL << 2,
		EAH_GPU_Write = 1UL << 3,
		EAH_GPU_Unordered = 1UL << 4,
		EAH_GPU_Structured = 1UL << 5,
		EAH_Generate_Mips = 1UL << 6,
		EAH_Immutable = 1UL << 7,
		EAH_Raw = 1UL << 8,
		EAH_Append = 1UL << 9,
		EAH_Counter = 1UL << 10,
		EAH_DrawIndirectArgs = 1UL << 11
	};

	// element format is a 64-bit value:
	// 00000000 T3[4] T2[4] T1[4] T0[4] S3[6] S2[6] S1[6] S0[6] C3[4] C2[4] C1[4] C0[4]

	template <uint64_t ch0, uint64_t ch1, uint64_t ch2, uint64_t ch3,
		uint64_t ch0_size, uint64_t ch1_size, uint64_t ch2_size, uint64_t ch3_size,
		uint64_t ch0_type, uint64_t ch1_type, uint64_t ch2_type, uint64_t ch3_type>
	struct MakeElementFormat4
	{
		static uint64_t const value = (ch0 << 0) | (ch1 << 4) | (ch2 << 8) | (ch3 << 12)
		| (ch0_size << 16) | (ch1_size << 22) | (ch2_size << 28) | (ch3_size << 34)
		| (ch0_type << 40) | (ch1_type << 44) | (ch2_type << 48) | (ch3_type << 52);
	};

	template <uint64_t ch0, uint64_t ch1, uint64_t ch2,
		uint64_t ch0_size, uint64_t ch1_size, uint64_t ch2_size,
		uint64_t ch0_type, uint64_t ch1_type, uint64_t ch2_type>
	struct MakeElementFormat3
	{
		static uint64_t const value = MakeElementFormat4<ch0, ch1, ch2, 0, ch0_size, ch1_size, ch2_size, 0, ch0_type, ch1_type, ch2_type, 0>::value;
	};

	template <uint64_t ch0, uint64_t ch1,
		uint64_t ch0_size, uint64_t ch1_size,
		uint64_t ch0_type, uint64_t ch1_type>
	struct MakeElementFormat2
	{
		static uint64_t const value = MakeElementFormat3<ch0, ch1, 0, ch0_size, ch1_size, 0, ch0_type, ch1_type, 0>::value;
	};

	template <uint64_t ch0,
		uint64_t ch0_size,
		uint64_t ch0_type>
	struct MakeElementFormat1
	{
		static uint64_t const value = MakeElementFormat2<ch0, 0, ch0_size, 0, ch0_type, 0>::value;
	};

	struct ElementInitData
	{
		void const * data;
		uint32_t mRowPitch;
		uint32_t mSlicePitch;
	};

	enum ElementFormat : uint64_t
	{
		// Unknown element format.
		EF_Unknown = 0,

		// 8-bit element format, all bits alpha.
		EF_A8 = MakeElementFormat1<EC_A, 8, ECT_UNorm>::value,

		// 16-bit element format, 5 bits for red, 6 bits for green and 5 bits for blue.
		EF_R5G6B5 = MakeElementFormat3<EC_R, EC_G, EC_B, 5, 6, 5, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,
		// 16-bit element format, 1 bits for alpha, 5 bits for red, green and blue.
		EF_A1RGB5 = MakeElementFormat4<EC_A, EC_R, EC_G, EC_B, 1, 5, 5, 5, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,
		// 16-bit element format, 4 bits for alpha, red, green and blue.
		EF_ARGB4 = MakeElementFormat4<EC_A, EC_R, EC_G, EC_B, 4, 4, 4, 4, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,

		// 8-bit element format, 8 bits for red.
		EF_R8 = MakeElementFormat1<EC_R, 8, ECT_UNorm>::value,
		// 8-bit element format, 8 bits for signed red.
		EF_SIGNED_R8 = MakeElementFormat1<EC_R, 8, ECT_SNorm>::value,
		// 16-bit element format, 8 bits for red, green.
		EF_GR8 = MakeElementFormat2<EC_G, EC_R, 8, 8, ECT_UNorm, ECT_UNorm>::value,
		// 16-bit element format, 8 bits for signed red, green.
		EF_SIGNED_GR8 = MakeElementFormat2<EC_G, EC_R, 8, 8, ECT_SNorm, ECT_SNorm>::value,
		// 24-bit element format, 8 bits for red, green and blue.
		EF_BGR8 = MakeElementFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,
		// 24-bit element format, 8 bits for signed red, green and blue.
		EF_SIGNED_BGR8 = MakeElementFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,
		// 32-bit element format, 8 bits for alpha, red, green and blue.
		EF_ARGB8 = MakeElementFormat4<EC_A, EC_R, EC_G, EC_B, 8, 8, 8, 8, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,
		// 32-bit element format, 8 bits for alpha, red, green and blue.
		EF_ABGR8 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,
		// 32-bit element format, 8 bits for signed alpha, red, green and blue.
		EF_SIGNED_ABGR8 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,
		// 32-bit element format, 2 bits for alpha, 10 bits for red, green and blue.
		EF_A2BGR10 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,
		// 32-bit element format, 2 bits for alpha, 10 bits for signed red, green and blue.
		EF_SIGNED_A2BGR10 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,

		// 32-bit element format, 8 bits for alpha, red, green and blue.
		EF_R8UI = MakeElementFormat1<EC_R, 8, ECT_UInt>::value,
		// 32-bit element format, 8 bits for alpha, red, green and blue.
		EF_R8I = MakeElementFormat1<EC_R, 8, ECT_SInt>::value,
		// 16-bit element format, 8 bits for red, green.
		EF_GR8UI = MakeElementFormat2<EC_G, EC_R, 8, 8, ECT_UInt, ECT_UInt>::value,
		// 16-bit element format, 8 bits for red, green.
		EF_GR8I = MakeElementFormat2<EC_G, EC_R, 8, 8, ECT_SInt, ECT_SInt>::value,
		// 24-bit element format, 8 bits for red, green and blue.
		EF_BGR8UI = MakeElementFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_UInt, ECT_UInt, ECT_UInt>::value,
		// 24-bit element format, 8 bits for red, green and blue.
		EF_BGR8I = MakeElementFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_SInt, ECT_SInt, ECT_SInt>::value,
		// 32-bit element format, 8 bits for alpha, red, green and blue.
		EF_ABGR8UI = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value,
		// 32-bit element format, 8 bits for signed alpha, red, green and blue.
		EF_ABGR8I = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value,
		// 32-bit element format, 2 bits for alpha, 10 bits for red, green and blue.
		EF_A2BGR10UI = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value,
		// 32-bit element format, 2 bits for alpha, 10 bits for red, green and blue.
		EF_A2BGR10I = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value,

		// 16-bit element format, 16 bits for red.
		EF_R16 = MakeElementFormat1<EC_R, 16, ECT_UNorm>::value,
		// 16-bit element format, 16 bits for signed red.
		EF_SIGNED_R16 = MakeElementFormat1<EC_R, 16, ECT_SNorm>::value,
		// 32-bit element format, 16 bits for red and green.
		EF_GR16 = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_UNorm, ECT_UNorm>::value,
		// 32-bit element format, 16 bits for signed red and green.
		EF_SIGNED_GR16 = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_SNorm, ECT_SNorm>::value,
		// 48-bit element format, 16 bits for alpha, blue, green and red.
		EF_BGR16 = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,
		// 48-bit element format, 16 bits for signed alpha, blue, green and red.
		EF_SIGNED_BGR16 = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,
		// 64-bit element format, 16 bits for alpha, blue, green and red.
		EF_ABGR16 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,
		// 64-bit element format, 16 bits for signed alpha, blue, green and red.
		EF_SIGNED_ABGR16 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,
		// 32-bit element format, 32 bits for red.
		EF_R32 = MakeElementFormat1<EC_R, 32, ECT_UNorm>::value,
		// 32-bit element format, 32 bits for signed red.
		EF_SIGNED_R32 = MakeElementFormat1<EC_R, 32, ECT_SNorm>::value,
		// 64-bit element format, 16 bits for red and green.
		EF_GR32 = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_UNorm, ECT_UNorm>::value,
		// 64-bit element format, 16 bits for signed red and green.
		EF_SIGNED_GR32 = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_SNorm, ECT_SNorm>::value,
		// 96-bit element format, 16 bits for alpha, blue, green and red.
		EF_BGR32 = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,
		// 96-bit element format, 16 bits for signed_alpha, blue, green and red.
		EF_SIGNED_BGR32 = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,
		// 128-bit element format, 16 bits for alpha, blue, green and red.
		EF_ABGR32 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,
		// 128-bit element format, 16 bits for signed alpha, blue, green and red.
		EF_SIGNED_ABGR32 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,

		// 16-bit element format, 16 bits for red.
		EF_R16UI = MakeElementFormat1<EC_R, 16, ECT_UInt>::value,
		// 16-bit element format, 16 bits for signed red.
		EF_R16I = MakeElementFormat1<EC_R, 16, ECT_SInt>::value,
		// 32-bit element format, 16 bits for red and green.
		EF_GR16UI = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_UInt, ECT_UInt>::value,
		// 32-bit element format, 16 bits for signed red and green.
		EF_GR16I = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_SInt, ECT_SInt>::value,
		// 48-bit element format, 16 bits for alpha, blue, green and red.
		EF_BGR16UI = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_UInt, ECT_UInt, ECT_UInt>::value,
		// 48-bit element format, 16 bits for signed alpha, blue, green and red.
		EF_BGR16I = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_SInt, ECT_SInt, ECT_SInt>::value,
		// 64-bit element format, 16 bits for alpha, blue, green and red.
		EF_ABGR16UI = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value,
		// 64-bit element format, 16 bits for signed alpha, blue, green and red.
		EF_ABGR16I = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value,
		// 32-bit element format, 32 bits for red.
		EF_R32UI = MakeElementFormat1<EC_R, 32, ECT_UInt>::value,
		// 32-bit element format, 32 bits for signed red.
		EF_R32I = MakeElementFormat1<EC_R, 32, ECT_SInt>::value,
		// 64-bit element format, 16 bits for red and green.
		EF_GR32UI = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_UInt, ECT_UInt>::value,
		// 64-bit element format, 16 bits for signed red and green.
		EF_GR32I = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_SInt, ECT_SInt>::value,
		// 96-bit element format, 16 bits for alpha, blue, green and red.
		EF_BGR32UI = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_UInt, ECT_UInt, ECT_UInt>::value,
		// 96-bit element format, 16 bits for signed_alpha, blue, green and red.
		EF_BGR32I = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_SInt, ECT_SInt, ECT_SInt>::value,
		// 128-bit element format, 16 bits for alpha, blue, green and red.
		EF_ABGR32UI = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value,
		// 128-bit element format, 16 bits for signed alpha, blue, green and red.
		EF_ABGR32I = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value,

		// 16-bit element format, 16 bits floating-point for red.
		EF_R16F = MakeElementFormat1<EC_R, 16, ECT_Float>::value,
		// 32-bit element format, 16 bits floating-point for green and red.
		EF_GR16F = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_Float, ECT_Float>::value,
		// 32-bit element format, 11 bits floating-point for green and red, 10 bits floating-point for blue.
		EF_B10G11R11F = MakeElementFormat3<EC_B, EC_G, EC_R, 10, 11, 11, ECT_Float, ECT_Float, ECT_Float>::value,
		// 48-bit element format, 16 bits floating-point for blue, green and red.
		EF_BGR16F = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_Float, ECT_Float, ECT_Float>::value,
		// 64-bit element format, 16 bits floating-point for alpha, blue, green and red.
		EF_ABGR16F = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_Float, ECT_Float, ECT_Float, ECT_Float>::value,
		// 32-bit element format, 32 bits floating-point for red.
		EF_R32F = MakeElementFormat1<EC_R, 32, ECT_Float>::value,
		// 64-bit element format, 32 bits floating-point for green and red.
		EF_GR32F = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_Float, ECT_Float>::value,
		// 96-bit element format, 32 bits floating-point for blue, green and red.
		EF_BGR32F = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_Float, ECT_Float, ECT_Float>::value,
		// 128-bit element format, 32 bits floating-point for alpha, blue, green and red.
		EF_ABGR32F = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_Float, ECT_Float, ECT_Float, ECT_Float>::value,

		// BC1 compression element format, DXT1
		EF_BC1 = MakeElementFormat1<EC_BC, 1, ECT_UNorm>::value,
		// BC1 compression element format, signed DXT1
		EF_SIGNED_BC1 = MakeElementFormat1<EC_BC, 1, ECT_SNorm>::value,
		// BC2 compression element format, DXT3
		EF_BC2 = MakeElementFormat1<EC_BC, 2, ECT_UNorm>::value,
		// BC2 compression element format, signed DXT3
		EF_SIGNED_BC2 = MakeElementFormat1<EC_BC, 2, ECT_SNorm>::value,
		// BC3 compression element format, DXT5
		EF_BC3 = MakeElementFormat1<EC_BC, 3, ECT_UNorm>::value,
		// BC3 compression element format, signed DXT5
		EF_SIGNED_BC3 = MakeElementFormat1<EC_BC, 3, ECT_SNorm>::value,
		// BC4 compression element format, 1 channel
		EF_BC4 = MakeElementFormat1<EC_BC, 4, ECT_UNorm>::value,
		// BC4 compression element format, 1 channel signed
		EF_SIGNED_BC4 = MakeElementFormat1<EC_BC, 4, ECT_SNorm>::value,
		// BC5 compression element format, 2 channels
		EF_BC5 = MakeElementFormat1<EC_BC, 5, ECT_UNorm>::value,
		// BC5 compression element format, 2 channels signed
		EF_SIGNED_BC5 = MakeElementFormat1<EC_BC, 5, ECT_SNorm>::value,
		// BC6 compression element format, 3 channels
		EF_BC6 = MakeElementFormat1<EC_BC, 6, ECT_UNorm>::value,
		// BC6 compression element format, 3 channels
		EF_SIGNED_BC6 = MakeElementFormat1<EC_BC, 6, ECT_SNorm>::value,
		// BC7 compression element format, 3 channels
		EF_BC7 = MakeElementFormat1<EC_BC, 7, ECT_UNorm>::value,

		// ETC1 compression element format
		EF_ETC1 = MakeElementFormat1<EC_ETC, 1, ECT_UNorm>::value,
		// ETC2 R11 compression element format
		EF_ETC2_R11 = MakeElementFormat2<EC_ETC, EC_ETC, 2, 1, ECT_UNorm, ECT_UNorm>::value,
		// ETC2 R11 compression element format, signed
		EF_SIGNED_ETC2_R11 = MakeElementFormat2<EC_ETC, EC_ETC, 2, 1, ECT_UNorm, ECT_SNorm>::value,
		// ETC2 GR11 compression element format
		EF_ETC2_GR11 = MakeElementFormat2<EC_ETC, EC_ETC, 2, 2, ECT_UNorm, ECT_UNorm>::value,
		// ETC2 GR11 compression element format, signed
		EF_SIGNED_ETC2_GR11 = MakeElementFormat2<EC_ETC, EC_ETC, 2, 2, ECT_UNorm, ECT_SNorm>::value,
		// ETC2 BGR8 compression element format
		EF_ETC2_BGR8 = MakeElementFormat2<EC_ETC, EC_ETC, 2, 3, ECT_UNorm, ECT_UNorm>::value,
		// ETC2 BGR8 compression element format. Standard RGB (gamma = 2.2).
		EF_ETC2_BGR8_SRGB = MakeElementFormat2<EC_ETC, EC_ETC, 2, 3, ECT_UNorm_SRGB, ECT_UNorm_SRGB>::value,
		// ETC2 A1BGR8 compression element format
		EF_ETC2_A1BGR8 = MakeElementFormat2<EC_ETC, EC_ETC, 2, 4, ECT_UNorm, ECT_UNorm>::value,
		// ETC2 A1BGR8 compression element format. Standard RGB (gamma = 2.2).
		EF_ETC2_A1BGR8_SRGB = MakeElementFormat2<EC_ETC, EC_ETC, 2, 4, ECT_UNorm_SRGB, ECT_UNorm_SRGB>::value,
		// ETC2 ABGR8 compression element format
		EF_ETC2_ABGR8 = MakeElementFormat2<EC_ETC, EC_ETC, 2, 5, ECT_UNorm, ECT_UNorm>::value,
		// ETC2 ABGR8 compression element format. Standard RGB (gamma = 2.2).
		EF_ETC2_ABGR8_SRGB = MakeElementFormat2<EC_ETC, EC_ETC, 2, 5, ECT_UNorm_SRGB, ECT_UNorm_SRGB>::value,

		// 16-bit element format, 16 bits depth
		EF_D16 = MakeElementFormat1<EC_D, 16, ECT_UNorm>::value,
		// 32-bit element format, 24 bits depth and 8 bits stencil
		EF_D24S8 = MakeElementFormat2<EC_D, EC_S, 24, 8, ECT_UNorm, ECT_UInt>::value,
		// 32-bit element format, 32 bits depth
		EF_D32F = MakeElementFormat1<EC_D, 32, ECT_Float>::value,

		// 32-bit element format, 8 bits for alpha, red, green and blue. Standard RGB (gamma = 2.2).
		EF_ARGB8_SRGB = MakeElementFormat4<EC_A, EC_R, EC_G, EC_B, 8, 8, 8, 8, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB>::value,
		// 32-bit element format, 8 bits for alpha, red, green and blue. Standard RGB (gamma = 2.2).
		EF_ABGR8_SRGB = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB>::value,
		// BC1 compression element format. Standard RGB (gamma = 2.2).
		EF_BC1_SRGB = MakeElementFormat1<EC_BC, 1, ECT_UNorm_SRGB>::value,
		// BC2 compression element format. Standard RGB (gamma = 2.2).
		EF_BC2_SRGB = MakeElementFormat1<EC_BC, 2, ECT_UNorm_SRGB>::value,
		// BC3 compression element format. Standard RGB (gamma = 2.2).
		EF_BC3_SRGB = MakeElementFormat1<EC_BC, 3, ECT_UNorm_SRGB>::value,
		// BC4 compression element format. Standard RGB (gamma = 2.2).
		EF_BC4_SRGB = MakeElementFormat1<EC_BC, 4, ECT_UNorm_SRGB>::value,
		// BC5 compression element format. Standard RGB (gamma = 2.2).
		EF_BC5_SRGB = MakeElementFormat1<EC_BC, 5, ECT_UNorm_SRGB>::value,
		// BC7 compression element format. Standard RGB (gamma = 2.2).
		EF_BC7_SRGB = MakeElementFormat1<EC_BC, 7, ECT_UNorm_SRGB>::value
	};


	enum StereoMethod
	{
		STM_None,
		STM_ColorAnaglyph_RedCyan,
		STM_ColorAnaglyph_YellowBlue,
		STM_ColorAnaglyph_GreenRed,
		STM_LCDShutter,
		STM_HorizontalInterlacing,
		STM_VerticalInterlacing,
		STM_Horizontal,
		STM_Vertical,
		STM_OculusVR
	};

	template <int c>
	inline ElementChannel
		getChannel(ElementFormat ef)
	{
		return static_cast<ElementChannel>((static_cast<uint64_t>(ef) >> (4 * c)) & 0xf);
	}

	template <int c>
	inline uint8_t
		getChannelBits(ElementFormat ef)
	{
		return (static_cast<uint64_t>(ef) >> (16 + 6 * c)) & 0x3F;
	}

	inline bool isDepthFormat(ElementFormat format)
	{
		return (EC_D == getChannel<0>(format));
	}

	inline bool isCompressedFormat(ElementFormat format)
	{
		return (EC_BC == getChannel<0>(format)) || (EC_ETC == getChannel<0>(format));
	}

	inline uint8_t
		getNumFormatBits(ElementFormat format)
	{
		switch (format)
		{
		case EF_BC1:
		case EF_SIGNED_BC1:
		case EF_BC1_SRGB:
		case EF_BC4:
		case EF_SIGNED_BC4:
		case EF_BC4_SRGB:
		case EF_ETC2_R11:
		case EF_SIGNED_ETC2_R11:
		case EF_ETC2_BGR8:
		case EF_ETC2_BGR8_SRGB:
		case EF_ETC2_A1BGR8:
		case EF_ETC2_A1BGR8_SRGB:
		case EF_ETC1:
			return 16;

		case EF_BC2:
		case EF_SIGNED_BC2:
		case EF_BC2_SRGB:
		case EF_BC3:
		case EF_SIGNED_BC3:
		case EF_BC3_SRGB:
		case EF_BC5:
		case EF_SIGNED_BC5:
		case EF_BC5_SRGB:
		case EF_BC6:
		case EF_SIGNED_BC6:
		case EF_BC7:
		case EF_BC7_SRGB:
		case EF_ETC2_GR11:
		case EF_SIGNED_ETC2_GR11:
		case EF_ETC2_ABGR8:
		case EF_ETC2_ABGR8_SRGB:
			return 32;
		default:
			BOOST_ASSERT(!isCompressedFormat(format));
			return getChannelBits<0>(format) + getChannelBits<1>(format) + getChannelBits<2>(format) + getChannelBits<3>(format);
		}
	}

	inline uint8_t
		getNumFormatBytes(ElementFormat format)
	{
		return getNumFormatBits(format) / 8;
	}

	template<int c>
	inline ElementChannelType
		channelType(ElementFormat ef)
	{
		return static_cast<ElementChannelType>((static_cast<uint64_t>(ef) >> (40 + 4 * c)) & 0xF);
	}
	template<int c>
	inline ElementFormat
		channelType(ElementFormat ef, ElementChannelType new_c)
	{
		uint64_t ef64 = static_cast<uint64_t>(ef);
		ef64 &= ~(0xFULL << (40 + 4 * c));
		ef64 |= (static_cast<uint64_t>(new_c) << (40 + 4 * c));
		return static_cast<ElementFormat>(ef64);
	}

	inline ElementFormat
		makeSRGB(ElementFormat format)
	{
		if (ECT_UNorm == channelType<0>(format))
		{
			format = channelType<0>(format, ECT_UNorm_SRGB);
		}
		if (getChannel<0>(format) != EC_BC)
		{
			if (ECT_UNorm == channelType<1>(format))
			{
				format = channelType<1>(format, ECT_UNorm_SRGB);
			}
			if (getChannel<0>(format) != EC_ETC)
			{
				if (ECT_UNorm == getChannel<2>(format))
				{
					format = channelType<2>(format, ECT_UNorm_SRGB);
				}
				if (ECT_UNorm == channelType<3>(format))
				{
					format = channelType<3>(format, ECT_UNorm_SRGB);
				}
			}
		}
		return format;
	}




	inline bool
		isSRGB(ElementFormat format)
	{
		return (ECT_UNorm_SRGB == channelType<0>(format));
	}
	inline bool
		isSigned(ElementFormat format)
	{
		return (ECT_SNorm == channelType<0>(format));
	}

	ElementFormat parseFormat(std::string formatStr);

}



#endif