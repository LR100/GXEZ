#include "GXEZ/Tool/FPSComputer.h"

namespace GXEZ
{
	FPSComputer::FPSComputer()
	{
		_fps = 60;
		_frame = 1;
		_passedTime = 0;
	}

	FPSComputer::~FPSComputer()
	{
	}

	void FPSComputer::AddFrame()
	{
		_frame++;
		_timer.SetElapsedTime();
		_passedTime += _timer.GetElapsedSecond();
		if (_passedTime > 1.0f)
		{
			_fps = (_frame / _passedTime);
			_frame = 1;
			_passedTime = 0;
		}
	}

	float FPSComputer::GetFps()
	{
		return (_fps);
	}
}
