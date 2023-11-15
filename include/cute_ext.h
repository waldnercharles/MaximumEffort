#pragma once

#include <cute.h>

/**
 * @function cf_ceil
 * @category math
 * @brief    Returns the component-wise ceil of a vector.
 * @remarks  Ceil means the integer-part is rounded up..
 * @related  CF_V2 cf_round cf_lesser_v2 cf_greater_v2 cf_lesser_equal_v2 cf_greater_equal_v2 cf_parallel
 */
CF_INLINE CF_V2 cf_ceil(CF_V2 a)
{
	return cf_v2(ceilf(a.x), ceilf(a.y));
}

namespace Cute
{
	CF_INLINE v2 ceil(v2 a)
	{
		return cf_ceil(a);
	}
}
