add_executable(Benchmarks)
target_sources(Benchmarks
        PRIVATE
        "AtomicMinMaxBenchmark.cpp"
        "AtomicLoadBenchmark.cpp"
        "AtomicStoreBenchmark.cpp"
        "DekkersAlgorithmBenchmark.cpp")

target_compile_features(Benchmarks PRIVATE cxx_std_23)
target_link_libraries(Benchmarks PRIVATE CppMemoryModel benchmark::benchmark benchmark::benchmark_main TBB::tbb)
