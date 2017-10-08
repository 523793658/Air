#ifndef _CameraController_H_
#define _CameraController_H_
#pragma once
#include "boost/lexical_cast.hpp"
#include "PreDeclare.h"
namespace Air
{
	class AIR_CORE_API CameraController : boost::noncopyable
	{
	public:	
		CameraController();
		virtual ~CameraController();
		void setScalers(float rotationScaler, float moveScaler);

		virtual void attachCamera(Camera& camera);
		virtual void detachCamera();

	protected:
		float mRotationScaler;
		float mMoveScaler;
		Camera* mCamera;
	};

	class AIR_CORE_API TrackbollCameraController : public CameraController
	{
	public:
		TrackbollCameraController(bool use_input_engine = true, uint32_t rotate_button = MB_LEFT)

	};



}
#endif
