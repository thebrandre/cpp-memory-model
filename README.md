# C++ Memory Model


## Samples

### Basics

* [Beware of the optimizer!](https://godbolt.org/z/rMGao4K1f)
* [Beware of of *sequenced before*](https://godbolt.org/z/698G4Kxv4)
* [Message passing with release-acquire ordering](https://godbolt.org/z/4hbK7dx7K)
* [Spinlock mutex](https://godbolt.org/z/GbzoKKnqf)
* [How memory order maps to x86-64 and ARM64 assembly](https://godbolt.org/z/GbzoKKnqf)
* [Sequential consistency: Dekker's algorithm simplified](https://godbolt.org/z/oe3svbWv1)

### Exercise: producer-consumer communication

* [Producer/consumer broken](https://godbolt.org/z/bh39zEdbM)
* [Producer/consumer broken (no atomics)](https://godbolt.org/z/6r7z8rTrv)
* [Producer/consumer fixed](https://godbolt.org/z/sEsa4h8sr)
* [Producer/multiple consumers](https://godbolt.org/z/d3oY6hh1G)

### Deep dive

* [Dekker's algorithm with atomics vs fences](https://godbolt.org/z/YP4cY6zM1), where the latter is [probably](https://godbolt.org/z/xr8E3nMGG) correct?
* [Dekker's algorithm failing with release/acquire](https://godbolt.org/z/GWY85ofE5)
* [How memory order maps to x86-64 and ARM64 assembly and optimizations](https://godbolt.org/z/drreTbhbr)
* [How memory order may prevent auto vectorization](https://godbolt.org/z/31W9P8Y36)
* [Might fail in relaxed ordering (but not in x86's *total store order model*)](https://godbolt.org/z/99xfvfjP3) -- [at least one of the reader threads needs to fire](https://godbolt.org/z/n4d8f8ca5)
* [atomic min/max in x86-64](https://godbolt.org/z/PjKjvfnGe) or [maybe weaker (no difference on x86)?](https://godbolt.org/z/r8TMzjea5)
* [difference between std::atomic of and std::atomic_flag? (at least with clang and gcc on Linux)](https://godbolt.org/z/Y1GGY1ox7)

## Resources

* [intro.multithread](https://eel.is/c++draft/intro.multithread)
	* [*sequenced before*](https://eel.is/c++draft/intro.execution#def:sequenced_before)
	* [*synchronizes with*](https://eel.is/c++draft/atomics.order)
	* [*inter-thread happens before*](https://eel.is/c++draft/intro.multithread#def:inter-thread_happens_before)
	* [*(simply/strongly) happens before*](https://eel.is/c++draft/intro.multithread#def:happens_before)
	* [*modification order*](https://eel.is/c++draft/intro.races#def:modification_order)
* [atomics.order](https://eel.is/c++draft/atomics.order)
* [atomics.fences](https://eel.is/c++draft/atomics.fences)
* [*Atomic minimum/maximum* (P0493R3)](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p0493r3.pdf)
* [Anthony Williams: Implementing Dekker's algorithm with Fences](https://www.justsoftwaresolutions.co.uk/threading/implementing_dekkers_algorithm_with_fences.html)
* [std::atomic_thread_fence](https://en.cppreference.com/w/cpp/atomic/atomic_thread_fence)
* [std::atomic](https://en.cppreference.com/w/cpp/atomic/atomic)
* [Order of evaluation: "Sequenced before" rules](https://en.cppreference.com/w/cpp/language/eval_order)
* [Intel® 64 and IA-32 Architectures Software Developer's Manual Volume 3A: System Programming Guide, Part 1: Chapter 9.2 Memory Ordering](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
	* *Neither Loads Nor Stores Are Reordered with Like Operations*
	* *Stores Are Not Reordered With Earlier Loads*
	* *Loads May Be Reordered with Earlier Stores to Different Locations*
* [*Atomic Smart Pointers* (N4058)](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4058.pdf)
* [*C++ Concurrency in Action*, Anthony Williams, 2nd ed.](https://www.manning.com/books/c-plus-plus-concurrency-in-action-second-edition)
* [*Revising atomic_shared_ptr for C++20* (P0718R2)](https://www.manning.com/books/c-plus-plus-concurrency-in-action-second-edition)
* [Does lock xchg have the same behavior as mfence?](https://stackoverflow.com/a/50279772/75883571)
* [*Repairing sequential consistency in C/C++11*, Ori Lahav et al. (2017)](https://doi.org/10.1145/3062341.3062352) [PDF](https://plv.mpi-sws.org/scfix/paper.pdf)
* [*Common Compiler Optimisations are Invalid in the C11 Memory Model and what we can do about it*, Viktor Vafeiadis et al. (2015)](https://doi.org/10.1145/2676726.2676995) [PDF](https://people.mpi-sws.org/~viktor/papers/popl2015-c11comp.pdf)
* [*How to miscompile programs with "benign" data races*, Hans-J. Boehm (2011)](https://doi.org/10.5555/2001252.2001255) [PDF](https://www.usenix.org/legacy/events/hotpar11/tech/final_files/Boehm.pdf)
* [*The C11 and C++11 Concurrency Model*, Mark John Batty (2014)](https://www.cs.kent.ac.uk/people/staff/mjb211/docs/toc.pdf)
* [*Cooperating Sequential Processes*, E.W. Dijkstra (1968)](https://doi.org/10.1007/978-1-4757-3472-0_2) [PDF](https://pure.tue.nl/ws/files/4279816/344354178746665.pdf)
* [*The synchronization of independent processes*, L. Lamport, Acta Informatica 7, 15–34 (1976)](https://doi.org/10.1007/BF00265219) [PDF](https://lamport.azurewebsites.net/pubs/synchronization.pdf)
* [*How to Make a Multiprocessor Computer That Correctly Executes Multiprocess Programs*, L. Lamport, IEEE Transactions on Computers, vol. C-28, no. 9, pp. 690-691, Sept. 1979](https://doi.org/10.1109/TC.1979.1675439) [PDF](https://www.microsoft.com/en-us/research/uploads/prod/2016/12/How-to-Make-a-Multiprocessor-Computer-That-Correctly-Executes-Multiprocess-Programs.pdf)
* [*A Primer on Memory Consistency and Cache Coherence* Daniel J. Sorin et al. (2011)](https://doi.org/10.1007/978-3-031-01733-9) [PDF](https://course.ece.cmu.edu/~ece847c/S15/lib/exe/fetch.php?media=part2_2_sorin12.pdf)
* [*Revising the C++ memory model* (P0668R5)](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0668r5.html)
* [*Analyzing the memory ordering models of the Apple M1* Lars Wrenger et al. (2024)](https://doi.org/10.1016/j.sysarc.2024.103102)
* [*How emulation works on Arm* (Windows 11 24H2's emulator Prism)](https://learn.microsoft.com/en-us/windows/arm/apps-on-arm-x86-emulation), see also [Visual Studio 2019's /volatileMetadata](https://learn.microsoft.com/en-us/cpp/build/reference/volatile?view=msvc-170)
* [*Memory consistency and event ordering in scalable shared-memory multiprocessors* Kourosh Gharachorloo et al. (1990)](https://doi.org/10.1145/325096.325102)

## Talks

* [ACCU 2017: Frank Birbacher *Atomic's memory orders, what for?*](https://youtu.be/A_vAG6LIHwQ)
* [C++ and Beyond 2012: Herb Sutter  *Atomic Weapons 1 of 2*](https://youtu.be/A8eCGOqgvH4)
* [C++ and Beyond 2012: Herb Sutter  *Atomic Weapons 2 of 2*](https://youtu.be/KeLBd2EJLOU)
* [CppCon 2014: Herb Sutter *Lock-Free Programming (or, Juggling Razor Blades), Part I*](https://youtu.be/c1gO9aB9nbs)
* [CppCon 2014: Herb Sutter *Lock-Free Programming (or, Juggling Razor Blades), Part II*](https://youtu.be/CmxkPChOcvw)
* [CppCon 2017: Fedor Pikus *C++ atomics, from basic to advanced. What do they really do?*](https://youtu.be/ZQFzMfHIxng)
* [CppCon 2016: Hans Boehm *Using weakly ordered C++ atomics correctly*](https://youtu.be/M15UKpNlpeM)
* [CppCon 2021: Filipe Mulonde *The Foundation of C++ Atomics: The Knowledge You Need to Correctly Use C++ Atomics*](https://youtu.be/BfEnMRWLjgQ)
* [Pavel Yosifovich *Concurrency and the C++ Memory Model*](https://youtu.be/NZ_ncor_Lj0)
* [CppCon 2020: Paul E. McKenney & Hans Boehm *A Relaxed Guide to memory_order_relaxed*](https://youtu.be/cWkUqK71DZ0)
* [CppCon 2016: JF Bastien *No Sane Compiler Would Optimize Atomics*](https://youtu.be/IB57wIf9W1k)