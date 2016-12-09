#ifndef _SampleCreateWindow_H_
#define _SampleCreateWindow_H_
#include "app/include/App3D.hpp"

	class SampleRenderOneBall : public Air::App3DFramework
	{
	public:
		SampleRenderOneBall();
	private:
		uint32_t doUpdate(uint32_t pass);
		void onCreate();

	private:
		Air::SceneObjectPtr mSphere;
	};










#endif