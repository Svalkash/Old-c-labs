#include "stdafx.h"
#include "Arc.h"

namespace P5v2
{
	int getTimeMillis(std::chrono::steady_clock::time_point base_time)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - base_time).count();
	}

	int to_millis(std::chrono::steady_clock::time_point cur_time, std::chrono::steady_clock::time_point base_time)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(cur_time - base_time).count();
	}
}