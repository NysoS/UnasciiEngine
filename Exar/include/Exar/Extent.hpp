#ifndef EXAR_EXTENT_HPP
#define EXAR_EXTENT_HPP

#include "Exar/Types.hpp"

namespace Exar
{
	template <typename Type = u32>
	struct Extent2D
	{
		Type w;
		Type h;
	};

	template <typename Type = u32>
	struct Extent3D
	{
		Type w;
		Type h;
		Type d;
	};
}

#endif // !EXAR_EXTENT_HPP
