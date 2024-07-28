#pragma once

/// @def MemoryFence()
///
/// Compilers nowadays do not emit an mfence instruction on atomic stores on x86.
/// This is a convenience macro to force this instruction on x86 across different
/// compilers gcc, clang, and msvc.
/// @see https://godbolt.org/z/qq1PevdzG

#if defined(_MSC_VER) && (defined(_M_AMD64) || defined(_M_IX86))
#include <intrin.h>
#define MemoryFence _mm_mfence
#elif __has_builtin(__builtin_ia32_mfence)
#define MemoryFence __builtin_ia32_mfence
#else
#define MemoryFence void
#endif