#ifndef _Air_Render_Layout_H_
#define _Air_Render_Layout_H_
#pragma once
#include "PreDeclare.h"

namespace Air
{
	class AIR_CORE_API RenderLayout
	{
	public:
		enum TopologyType
		{
			TT_PointList,
			TT_LineList,
			TT_LineStrip,
			TT_TriangleList,
			TT_TriangleStrip,
			TT_LineList_Adj,
			TT_LineStrip_Adj,
			TT_TriangleList_Adj,
			TT_TriangleStrip_Adj,
			TT_1_Ctrl_Pt_PatchList,
			TT_2_Ctrl_Pt_PatchList,
			TT_3_Ctrl_Pt_PatchList,
			TT_4_Ctrl_Pt_PatchList,
			TT_5_Ctrl_Pt_PatchList,
			TT_6_Ctrl_Pt_PatchList,
			TT_7_Ctrl_Pt_PatchList,
			TT_8_Ctrl_Pt_PatchList,
			TT_9_Ctrl_Pt_PatchList,
			TT_10_Ctrl_Pt_PatchList,
			TT_11_Ctrl_Pt_PatchList,
			TT_12_Ctrl_Pt_PatchList,
			TT_13_Ctrl_Pt_PatchList,
			TT_14_Ctrl_Pt_PatchList,
			TT_15_Ctrl_Pt_PatchList,
			TT_16_Ctrl_Pt_PatchList,
			TT_17_Ctrl_Pt_PatchList,
			TT_18_Ctrl_Pt_PatchList,
			TT_19_Ctrl_Pt_PatchList,
			TT_20_Ctrl_Pt_PatchList,
			TT_21_Ctrl_Pt_PatchList,
			TT_22_Ctrl_Pt_PatchList,
			TT_23_Ctrl_Pt_PatchList,
			TT_24_Ctrl_Pt_PatchList,
			TT_25_Ctrl_Pt_PatchList,
			TT_26_Ctrl_Pt_PatchList,
			TT_27_Ctrl_Pt_PatchList,
			TT_28_Ctrl_Pt_PatchList,
			TT_29_Ctrl_Pt_PatchList,
			TT_30_Ctrl_Pt_PatchList,
			TT_31_Ctrl_Pt_PatchList,
			TT_32_Ctrl_Pt_PatchList
		};

		void setTopologyType(TopologyType type)
		{
			mTopologyType = type;
		}

		uint32_t getNumVertexStreams() const
		{
			return static_cast<uint32_t>(mVertexStreams.size());
		}
	protected:

		TopologyType mTopologyType;

		struct StreamUnit
		{
			
		};

		std::vector<StreamUnit> mVertexStreams;

	};
}




#endif