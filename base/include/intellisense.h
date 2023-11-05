#pragma once

// This doesn't fix everything, since it'll be years and years before Microsoft
// ever supports C23. It sorta helps a bit though.

#ifdef __INTELLISENSE__
#define __int128_t int64_t
#define __uint128_t uint64_t
#define __VA_OPT__(x) x
#endif

