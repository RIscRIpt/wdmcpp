# wdmcpp

wdmcpp is a Visual Studio Project example, which demonstrates usage of Microsoft's C++ Standard Library in Windows kernel-mode driver (WDM).

# Key Components

* `$(VC_IncludePath)` in "Include Directories" -- enables Visual Studio to find C++ Standard Library header files;
* `libcntpr.lib` in "Additional Dependencies" of Linker Input -- enables linking with NT Kernel C standard functions;
* Definition of C/C++ functions -- see `cpp.cpp`;
* Invoking constructors and destructors -- see `cpp.cpp`;

# References
* http://www.zer0mem.sk/?p=517
* https://docs.microsoft.com/en-us/cpp/c-runtime-library/crt-initialization
* https://gist.github.com/mmozeiko/ae38aeb10add7cb66be4c00f24f8e688
