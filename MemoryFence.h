#pragma once

/// @def MemoryFence()
///
/// Compilers nowadays do not emit an mfence instruction on atomic stores on x86.
/// This is a convenience macro to force this instruction on x86 across different
/// compilers gcc, clang, and msvc.
/// @see https://godbolt.org/z/v1vKejcvh

#ifdef __has_builtin
#if __has_builtin(__builtin_ia32_mfence)
#define MemoryFence __builtin_ia32_mfence
#endif
#endif

#ifndef MemoryFence
#if defined(_MSC_VER) && (defined(_M_AMD64) || defined(_M_IX86))
#include <intrin.h>
#define MemoryFence _mm_mfence
#endif
#endif

// Fallback for old versions of gcc
#ifndef MemoryFence
#if defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#define MemoryFence __builtin_ia32_mfence
#endif
#endif

// Generic fallback for targeting other architectures such as ARM. Also works with old gcc versions on x86.
// This fence emits a *data memory barrier* in the inner-sharable domain on ARM (dmb ish).
#ifndef MemoryFence
#include <atomic>
#define MemoryFence []() { std::atomic_thread_fence(std::memory_order_seq_cst); }
#endif