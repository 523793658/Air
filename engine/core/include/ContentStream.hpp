#ifndef _ContentStreaming_H_
#define _ContentStreaming_H_
#pragma once
namespace Air
{
	struct StreamingManagerCollection;
	class UPrimitiveComponent;
	struct IStreamingManager
	{
		virtual ~IStreamingManager(){}

		static StreamingManagerCollection& get();

		virtual void notifyPrimitiveUpdated(const PrimitiveComponent* primitive) {}
	};

	struct ITextureStreamingManager : IStreamingManager
	{

	};

	struct IAudioStreamingManager : IStreamingManager
	{

	};

	struct StreamingManagerCollection : IStreamingManager
	{
	public:
		void notifyPrimitiveUpdated(const PrimitiveComponent* primitive) override;
	};
}
#endif
