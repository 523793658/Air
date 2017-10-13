#include "Engine.h"
#include "input_system/include/InputFactory.hpp"
#include "Camera.hpp"
#include "CameraController.hpp"

namespace Air
{
	CameraController::CameraController()
		:mRotationScaler(0.05f), mMoveScaler(1), mCamera(nullptr)
	{

	}

	CameraController::~CameraController()
	{

	}

	void CameraController::setScalers(float rotationScaler, float moveScaler)
	{
		mRotationScaler = rotationScaler;
		mMoveScaler = moveScaler;
	}

	void CameraController::attachCamera(Camera& camera)
	{
		mCamera = &camera;
	}

	void CameraController::detachCamera()
	{
		mCamera = nullptr;
	}



	TrackballCameraController::TrackballCameraController(bool use_input_engine /* = true */, uint32_t rotate_button /* = MB_Left */, uint32_t zoom_button /* = MB_Right */, uint32_t move_button /* = MB_Middle */)
		: mMoveButton(move_button), mRotateButton(rotate_button), mZoomButton(zoom_button)
	{
		if (use_input_engine)
		{
			InputActionDefine actons[] =
			{
				InputActionDefine(Turn, MS_X),
				InputActionDefine(Turn, MS_Y),
				InputActionDefine(Turn, TS_Pan),
				InputActionDefine(Turn, TS_Pan),
				InputActionDefine(ZoomInOut, TS_Zoom)
			};
			InputEngine& inputEngine = Engine::getInstance().getInputFactoryInstance().getInputEngineInstance();
			InputActionMap actionMap;
			actionMap.addActions(actons, actons + std::size(actons));
			ActionHandlerT input_handler = MakeSharedPtr<InputSignal>();
			input_handler->connect(std::bind(&TrackballCameraController::inputeHandler, this, std::placeholders::_1, std::placeholders::_2));
			inputEngine.actionMap(actionMap, input_handler);
		}
	}

	void TrackballCameraController::inputeHandler(InputEngine const & sender, InputAction const & action)
	{
		if (mCamera)
		{
			switch (action.first)
			{
			case Turn:
				switch (action.second->type)
				{
				case InputEngine::IDT_Mouse:
				{
					InputMouseActionParamPtr param = checked_pointer_cast<InputMouseActionParam>(action.second);
					float xd = static_cast<float>(param->mMoveVec.x());
					float yd = static_cast<float>(param->mMoveVec.y());
					if (param->mButtonsState & mRotateButton)
					{
						this->rotate(xd, yd);
					}
					else if (param->mButtonsState & mZoomButton)
					{
						this->zoom(xd, yd);
					}
					else if (param->mButtonsState & mMoveButton)
					{
						this->move(xd, yd);
					}
				}
				break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
	}

	void TrackballCameraController::attachCamera(Camera& camera)
	{
		CameraController::attachCamera(camera);
		mReverseTarget = false;
		mTarget = mCamera->getLookAt();
		mRight = mCamera->getRightVec();
	}

	void TrackballCameraController::move(float offset_x, float offset_y)
	{
		float3 offset = mRight * (-offset_x * mMoveScaler * 2);
		float3 pos = mCamera->getEyePos() + offset;
		mTarget += offset;

		offset = mCamera->getUpVec() * (offset_y * mMoveScaler * 2);
		pos += offset;
		mTarget += offset;
		mCamera->setViewParams(pos, mTarget, mCamera->getUpVec());
	}

	void TrackballCameraController::rotate(float offset_x, float offset_y)
	{
		Quaternion q = MathLib::rotation_axis(mRight, offset_y * mRotationScaler);
		float4x4 mat = MathLib::transformation<float>(nullptr, nullptr, nullptr, &mTarget, &q, nullptr);
		float3 pos = MathLib::transform_coord(mCamera->getEyePos(), mat);
		q = MathLib::rotation_axis(float3(0.0f, 1, 0.0f), offset_x * mRotationScaler);
		mat = MathLib::transformation<float>(nullptr, nullptr, nullptr, &mTarget, &q, nullptr);
		pos = MathLib::transform_coord(pos, mat);
		mRight = MathLib::transform_quat(mRight, q);
		float3 dir;
		if (mReverseTarget)
		{
			dir = pos - mTarget;
		}
		else
		{
			dir = mTarget - pos;
		}
		float dist = MathLib::length(dir);
		dir /= dist;
		float3 up = MathLib::cross(dir, mRight);
		mCamera->setViewParams(pos, pos + dir * dist, up);
	}

	void TrackballCameraController::zoom(float offset_x, float offset_y)
	{
		float3 offset = mCamera->getForwardVec() * ((offset_x + offset_y) * mMoveScaler * 2);

		float3 pos = mCamera->getEyePos() + offset;
		if (MathLib::dot(mTarget - pos, mCamera->getForwardVec()) <= 0)
		{
			mReverseTarget = true;
		}
		else
		{
			mReverseTarget = false;
		}
		mCamera->setViewParams(pos, pos + mCamera->getForwardVec() * mCamera->getLookAtDist(), mCamera->getUpVec());

	}

}