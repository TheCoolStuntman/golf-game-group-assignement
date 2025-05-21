#pragma once

#include <concepts>

namespace Math {

	template <typename T>
	requires std::is_integral_v<T> || std::is_floating_point_v<T>
	inline T clamp(T v, T min, T max) {
		return v < min ? min : (v > max ? max : v);
	}

	template <typename T>
	requires std::is_integral_v<T> || std::is_floating_point_v<T>
	inline T max(T v, T max) {
		return v > max ? max : v;
	}

}