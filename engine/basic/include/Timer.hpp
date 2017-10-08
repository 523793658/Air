#ifndef _Basic_Timer_H_
#define _Basic_Timer_H_
namespace Air
{
	class Timer
	{
	public:
		Timer();

		void restart();

		double getElapsed() const;

		double getElapsedMax() const;

		double getElapsedMin() const;

		double getCurrentTime() const;

	private:
		double mStartTime;
	};
}
#endif