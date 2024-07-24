#pragma once
#include <concepts>

namespace Engine::Math
{
	template<typename T>
	concept arithmetic = std::integral<T> or std::floating_point<T>;

	template<typename T> requires arithmetic<T>
	T Clamp(T value, T min, T max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}
}