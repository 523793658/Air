#include "Context.h"
#include "scene_manager/include/SceneManager.hpp"
#include "rendersystem/include/Texture.hpp"
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

	float3 const & LightSource::getColor() const
	{
		return mColor;
	}
	void LightSource::setColor(float3 const & color)
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

	void AmbientLightSource::setSkyLightTexY(TexturePtr const & tex)
	{
		mSkyTexY = tex;
	}

	void AmbientLightSource::setSkyLightTexC(TexturePtr const & tex)
	{
		mSkyTexC = tex;
	}


	int2 AmbientLightSource::getMipmapNum() const
	{
		int32_t mip = mSkyTexY->getNumMipMaps();
		return int2(mip, mip);
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

	float3 const & DirectLightSource::getDirection() const
	{
		return mDirection;
	}

	void DirectLightSource::setDirection(float3 const & dir)
	{
		mDirection = dir;
	}


}