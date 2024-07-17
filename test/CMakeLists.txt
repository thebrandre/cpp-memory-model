add_executable(UnitTests)
target_sources(UnitTests
        PRIVATE
        "AtomicMinMaxTest.cpp"
        "Catch2CustomMain.cpp"
        "ConcurrentStackTest.cpp"
        "DekkersAlgorithmTest.cpp"
        "ProducerConsumerRace.cpp"
        "TbbQueueTest.cpp")

target_compile_features(UnitTests PRIVATE cxx_std_23)
target_link_libraries(UnitTests PRIVATE CppMemoryModel spdlog::spdlog Catch2::Catch2 TBB::tbb)
