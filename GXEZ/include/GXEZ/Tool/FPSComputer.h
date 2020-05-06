#ifndef _FPSCOMPUTER_H_
#define _FPSCOMPUTER_H_

#include "GXEZ/Tool/Timer.h" 

namespace GXEZ
{

	class FPSComputer
	{
	public:
		FPSComputer();
		~FPSComputer();

		void	AddFrame();
		float	GetFps();

	private:

		float	_passedTime;
		Timer	_timer;
		float	_frame;
		float	_fps;
	};

}

#endif /* !_FPSCOMPUTER_H_ */
