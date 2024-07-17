cmake_minimum_required (VERSION 3.24)
project ("CppMemoryModel")

if (WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0A00 _UNICODE UNICODE)
endif ()

if (MSVC)
    add_compile_options(/utf-8)
endif ()

set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")

find_package(Catch2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)

add_library(CppMemoryModel)
target_sources(CppMemoryModel PUBLIC
    "AtomicMinMax.h"
    "ConcurrentStack.h"
    "DekkersAlgorithm.cpp"
    "DekkersAlgorithm.h"
    "MemoryFence.h"
    "StupidSelfishMutex.h")

if (WIN32)
	target_sources(CppMemoryModel PUBLIC "utf8.manifest")
endif()

target_compile_features(CppMemoryModel PUBLIC cxx_std_23)
target_include_directories(CppMemoryModel PUBLIC ".")

add_subdirectory(benchmark)
add_subdirectory(test)
