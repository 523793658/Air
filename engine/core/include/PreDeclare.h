#ifndef _PreDeclare_H_
#define _PreDeclare_H_
#include <memory>
#include <basic\include\PreDeclare.h>

namespace Air

{
	struct ConfextCfg;
	class Engine;


	class SceneManager;
	class RenderFactory;
	class ResourcePlugin;

	class App3DFramework;
	class Window;
	typedef std::shared_ptr<Window> WindowPtr;

	class ResLoadingDesc;
	typedef std::shared_ptr<ResLoadingDesc> ResLoadingDescPtr;
	class Camera;
	typedef std::shared_ptr<Camera> CameraPtr;
	class FrameBuffer;
	typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;
	class RenderLayout;
	typedef std::shared_ptr<RenderLayout> RenderLayoutPtr;
	class RenderEngine;
	class GraphicsBuffer;
	typedef std::shared_ptr<GraphicsBuffer> GraphicsBufferPtr;
	class RenderView;
	typedef std::shared_ptr<RenderView> RenderViewPtr;
	class Texture;
	typedef std::shared_ptr<Texture> TexturePtr;

	class SceneObject;
	typedef std::shared_ptr<SceneObject> SceneObjectPtr;

	class SceneObjectHelper;
	typedef std::shared_ptr<SceneObjectHelper> SceneObjectHelperPtr;

	class Renderable;
	typedef std::shared_ptr<Renderable> RenderablePtr;

	class RenderableHelper;
	typedef std::shared_ptr<RenderableHelper> RenderableHelperPtr;

	class RenderTechnique;

	class RenderEffect;
	typedef std::shared_ptr<RenderEffect> RenderEffectPtr;

	class RenderEffectTemplate;
	typedef std::shared_ptr<RenderEffectTemplate> RenderEffectTemplatePtr;

	class RenderEffectParameter;

	struct Viewport;
	typedef std::shared_ptr<Viewport> ViewportPtr;

	struct RenderSettings;

	class StaticMesh;
	typedef std::shared_ptr<StaticMesh> StaticMeshPtr;

	class RenderModel;
	typedef std::shared_ptr<RenderModel> RenderModelPtr;

	class ShaderObject;
	typedef std::shared_ptr<ShaderObject> ShaderObjectPtr;

	class RenderPass;
	typedef std::shared_ptr<RenderPass> RenderPassPtr;

	class RenderEffectAnnotation;
	typedef std::shared_ptr<RenderEffectAnnotation> RenderEffectAnnotationPtr;

	class RenderShaderFragment;

	struct RenderMaterial;
	typedef std::shared_ptr<RenderMaterial> RenderMaterialPtr;




	struct VertexElement;
}




#endif