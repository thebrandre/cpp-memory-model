# C++ Memory Model


## Samples

* [Beware of the optimizer!](https://godbolt.org/z/rMGao4K1f)
* [Synchronization with atomics and busy waiting](https://godbolt.org/z/sE7j9abKP) or [patient waiting](https://godbolt.org/z/nKnrds54W)
* [Producer/consumer broken](https://godbolt.org/z/bh39zEdbM)
* [Producer/consumer broken (no atomics)](https://godbolt.org/z/6r7z8rTrv)
* [Producer/consumer fixed](https://godbolt.org/z/sEsa4h8sr)
* [Producer/multiple consumers](https://godbolt.org/z/d3oY6hh1G)
* [Dekker's algorithm with atomics vs fences](https://godbolt.org/z/YP4cY6zM1), where the latter is [probably](https://godbolt.org/z/xr8E3nMGG) correct?
* [Dekker's algorithm failing with release/acquire](https://godbolt.org/z/GWY85ofE5)
* [How memory order maps to x86-64 and ARM64 assembly](https://godbolt.org/z/oTff5aY67)
* [How memory order maps to x86-64 and ARM64 assembly and optimizations](https://godbolt.org/z/drreTbhbr)
* [How memory order may prevent auto vectorization](https://godbolt.org/z/31W9P8Y36)
* [Might fail in relaxed ordering (but not in x86)](https://godbolt.org/z/99xfvfjP3) -- [at least one of the reader threads needs to fire](https://godbolt.org/z/n4d8f8ca5)
* [sequenced before](https://godbolt.org/z/698G4Kxv4)
* [atomic min/max in x86-64](https://godbolt.org/z/PjKjvfnGe) or [maybe weaker (no difference on x86)?](https://godbolt.org/z/r8TMzjea5)

## Resources

* [intro.multithread](https://eel.is/c++draft/intro.multithread)
	* [*Sequenced before*](https://eel.is/c++draft/intro.execution#def:sequenced_before) 
	* [*inter-thread happens before*](https://eel.is/c++draft/intro.multithread#def:inter-thread_happens_before)
	* [*happens before*](https://eel.is/c++draft/intro.multithread#def:happens_before)
	* [*synchronizes with*](https://eel.is/c++draft/atomics.order)
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
* [Repairing sequential consistency in C/C++11, Ori Lahav et al. (2017)](https://doi.org/10.1145/3062341.3062352) [PDF](https://plv.mpi-sws.org/scfix/paper.pdf)
* [Common Compiler Optimisations are Invalid in the C11 Memory Model and what we can do about it](https://doi.org/10.1145/2676726.2676995) [PDF](https://people.mpi-sws.org/~viktor/papers/popl2015-c11comp.pdf)
* [How to miscompile programs with "benign" data races, Boehm, Hans-J. (2011)](https://doi.org/10.5555/2001252.2001255) [PDF](https://www.usenix.org/legacy/events/hotpar11/tech/final_files/Boehm.pdf)
* [The C11 and C++11 Concurrency Model, Mark John Batty (2014)](https://www.cs.kent.ac.uk/people/staff/mjb211/docs/toc.pdf)


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