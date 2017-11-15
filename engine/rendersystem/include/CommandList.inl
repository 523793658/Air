namespace Air
{
	FORCEINLINE void CommandListImmediate::immediateFlush(EImmediateFlushType::Type flushType)
	{
		BOOST_ASSERT((!mDrawUpData.mOutVertexData && ! mDrawUpData.mOutIndexData));
		switch (flushType)
		{
		case Air::EImmediateFlushType::WaitForOutstandingTasksOnly:
		{
			waitForTasks();
		}
			break;
		case Air::EImmediateFlushType::DispatchToRHIThread:
		{
			if (hasCommands())
			{
				GRHICommandList.executeList(*this);
			}
		}
			break;
		case Air::EImmediateFlushType::WaitForDispatchToRHIThread:
		{
			if (hasCommands())
			{
				GRHICommandList.executeList(*this);
			}
			waitForDispatch();
		}
			break;
		case Air::EImmediateFlushType::FlushRHIThread:
		{
			if (hasCommands())
			{
				GRHICommandList.executeList(*this);
			}
			waitForDispatch();
			if (GRenderingThread)
			{
				waitForRHIThreadTasks();
			}
			waitForTasks(true);
		}
			break;
		case Air::EImmediateFlushType::FlushRHIThreadFlushResources:
		{
			if (hasCommands())
			{
				GRHICommandList.executeList(*this);
			}
			waitForDispatch();
			waitForRHIThreadTasks();
			waitForTasks(true);
			RHIResource::flushPendingDeletes();
		}
			break;
		default:
			BOOST_ASSERT(false);
			break;
		}
	}

}