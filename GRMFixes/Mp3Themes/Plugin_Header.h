// Header file
// Multi-platform file can not have `#pragma once`!!!

#include <algorithm>
#include <mutex>
#include <unordered_map>

// dynamic namespace
namespace NAMESPACE
{
	enum FadeMode
	{
		Out = 0,
		In = 1,
	};

	enum TransitionType
	{
		MilesSoundSystem = 0,
		DirectMusic = 1,
	};

	struct Transition
	{
		TransitionType type;
		std::string fileName;
		int handle;
		FadeMode mode;
		int duration;
		float volume;
	};
}