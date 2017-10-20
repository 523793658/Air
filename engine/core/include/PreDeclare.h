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

	class RenderStateObject;
	typedef std::shared_ptr<RenderStateObject> RenderStateObjectPtr;

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

	class RenderEffectConstantBuffer;
	typedef std::shared_ptr<RenderEffectConstantBuffer> RenderEffectConstantBufferPtr;

	class SharedConstantBuffer;

	class RenderEnvironment;

	class PostProcessChain;
	typedef std::shared_ptr<PostProcessChain> PostProcessChainPtr;


	class SamplerStateObject;
	typedef std::shared_ptr<SamplerStateObject> SamplerStateObjectPtr;

	struct TextureSubresource;

	class RenderMaterial;
	typedef std::shared_ptr<RenderMaterial> RenderMaterialPtr;

	struct SamplerStateDesc;
	struct RasterizerStateDesc;
	struct DepthStencilStateDesc;
	struct BlendStateDesc;




	class InputEngine;
	class InputDevice;
	typedef std::shared_ptr<InputDevice> InputDevicePtr;

	class InputKeyboard;
	typedef std::shared_ptr<InputKeyboard> InputKeyboardPtr;

	class InputMouse;
	typedef std::shared_ptr<InputMouse> InputMousePtr;

	class InputJoystick;
	typedef std::shared_ptr<InputJoystick> InputJoystickPtr;

	class InputTouch;
	typedef std::shared_ptr<InputTouch> InputTouchPtr;

	class InputSensor;
	typedef std::shared_ptr<InputSensor> InputSensorPtr;

	class InputFactory;

	struct InputActionParam;
	typedef std::shared_ptr<InputActionParam> InputActionParamPtr;

	struct InputKeyboardActionParam;
	typedef std::shared_ptr<InputKeyboardActionParam> InputKeyboardActionParamPtr;
	struct InputMouseActionParam;
	typedef std::shared_ptr<InputMouseActionParam> InputMouseActionParamPtr;
	struct InputJoystickActionParam;
	typedef std::shared_ptr<InputJoystickActionParam> InputJoystickActionParamPtr;
	struct InputTouchActionParam;
	typedef std::shared_ptr<InputTouchActionParam> InputTouchActionParamPtr;
	struct InputSensorActionParam;
	typedef std::shared_ptr<InputSensorActionParam> InputSensorActionParamPtr;

	class LightSource;
	typedef std::shared_ptr<LightSource> LightSourcePtr;

	class AmbientLightSource;
	typedef std::shared_ptr<AmbientLightSource> AmbientLightSourcePtr;


	struct VertexElement;

	class InputFactory;
}




#endif