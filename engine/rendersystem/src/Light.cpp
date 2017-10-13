#include "Engine.h"
#include "scene_manager/include/SceneManager.hpp"
#include "rendersystem/include/Light.hpp"

namespace Air

{
	LightSource::LightSource(LightType type) : mType(type)
	{

	}

	LightSource::~LightSource()
	{

	}


	LightType LightSource::getType() const
	{
		return mType;
	}
	int32_t LightSource::getAttrib() const
	{
		return mAttr;
	}
	void LightSource::setAttrib(int32_t attrib)
	{
		mAttr |= attrib;
	}

	bool LightSource::isEnabled() const
	{
		return mEnable;
	}
	void LightSource::setEnabled(bool enabled)
	{
		mEnable = enabled;
	}

	void LightSource::addToSceneManager()
	{
		Engine::getInstance().getSceneManangerInstance().addLight(this->shared_from_this());
	}

	void LightSource::delFromSceneManager()
	{
		Engine::getInstance().getSceneManangerInstance().delLight(this->shared_from_this());
	}

	Color const & LightSource::getColor() const
	{
		return mColor;
	}
	void LightSource::setColor(Color const & color)
	{
		mColor = color;
	}

	float3 const & LightSource::getPosition() const
	{
		return mPosition;
	}

	void LightSource::setPosition(float3 const & pos)
	{
		mPosition = pos;
	}



	AmbientLightSource::AmbientLightSource() : LightSource(LT_Ambient)
	{

	}

	AmbientLightSource::~AmbientLightSource()
	{

	}


	PointLightSource::PointLightSource() : LightSource(LT_Point)
	{

	}
	PointLightSource::~PointLightSource()
	{

	}

	DirectLightSource::DirectLightSource() : LightSource(LT_Directional)
	{

	}
	DirectLightSource::~DirectLightSource()
	{

	}


}