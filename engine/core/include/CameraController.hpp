#ifndef _CameraController_H_
#define _CameraController_H_
#pragma once
#include "boost/lexical_cast.hpp"
#include "input_system/include/Input.hpp"
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

	class AIR_CORE_API TrackballCameraController : public CameraController
	{
	public:
		TrackballCameraController(bool use_input_engine = true, uint32_t rotate_button = MB_Left, uint32_t zoom_button = MB_Right, uint32_t move_button = MB_Middle);

		virtual void attachCamera(Camera& camera);

		void move(float offset_x, float offset_y);
		void rotate(float offset_x, float offset_y);
		void zoom(float offset_x, float offset_y);

	private:
		bool mReverseTarget;
		float3 mTarget;
		float3 mRight;
		uint32_t mMoveButton;
		uint32_t mRotateButton;
		uint32_t mZoomButton;

		enum
		{
			Turn,
			ZoomInOut
		};
	private:
		void inputeHandler(InputEngine const & sender, InputAction const & action);
	};



}
#endif
