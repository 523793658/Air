#ifndef _Light_H_
#define _Light_H_
#pragma once

#include "PreDeclare.h"
#include "scene_manager/include/SceneObject.hpp"
namespace Air
{

	enum LightType
	{
		LT_Ambient = 0,
		LT_Directional,
		LT_Point,
		LT_Spot,
		LT_SphereArea,
		LT_TubeArea,
		LT_NumLightType
	};

	class AIR_CORE_API LightSource : boost::noncopyable, public std::enable_shared_from_this<LightSource>
	{
	public:
		

		enum LightSrcAttrib
		{
			LSA_NoShadow = 1UL << 0,
			LSA_NoDiffuse = 1UL << 1,
			LSA_NoSpecular = 1UL << 2,
			LSA_IndirectLighting = 1UL << 3,
			LSA_Temporary = 1UL << 4
		};

	public:
		explicit LightSource(LightType type);
		virtual ~LightSource() = 0;

		LightType getType() const;
		int32_t getAttrib() const;
		virtual void setAttrib(int32_t attrib);

		bool isEnabled() const;
		void setEnabled(bool enabled);

		virtual void addToSceneManager();

		virtual void delFromSceneManager();

		Color const & getColor() const;
		void setColor(Color const & color);

		float3 const & getPosition() const;

		void setPosition(float3 const & pos);

	protected:
		Color mColor;

		float3 mPosition;
		int32_t mAttr;
		LightType mType;

		bool mEnable;

	};

	/*class AIR_CORE_API AreaLightSource : public LightSource£¬ public SceneObject
	{
	public:
		AreaLightSource(LightType type);
		virtual ~AreaLightSource() = 0;

		virtual void addToSceneManager() override;
		virtual void delFromSceneManager() override;
	};*/


	class AIR_CORE_API AmbientLightSource : public LightSource
	{
	public:
		AmbientLightSource();
		virtual ~AmbientLightSource();
	};


	class AIR_CORE_API DirectLightSource : public LightSource
	{
	public:
		DirectLightSource();
		virtual ~DirectLightSource();
	};

	class AIR_CORE_API PointLightSource : public LightSource
	{
	public:
		PointLightSource();
		virtual ~PointLightSource();
	};

}
#endif
