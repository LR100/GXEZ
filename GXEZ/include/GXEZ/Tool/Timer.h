#ifndef _MY_TIME_H_
#define _MY_TIME_H_

#include <ctime>

namespace GXEZ
{

	class Timer
	{
	public:
		Timer();
		~Timer();
		void	Reset();
		void	SetElapsedTime();
		float	GetElapsedSecond();
		float	GetElapsedMilliSecond();
	private:
		std::clock_t		_time;
		std::clock_t		_elapsedTime;
		float				_elapsedFTime;
	};

}

#endif /* !_MY_TIME_H_ */