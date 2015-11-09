# How to build MSOUND

**Please note that manual compilation is not supposed to be a mandatory step to using MSOUND "out of the box". Our GitHub Releases page features the most current version of MSOUND pre-compiled binaries for all platforms with very sensible choices of audio APIs. [Go to the latest release here](https://github.com/TGM-Oldenburg/Msound/releases/latest).**

If you want to compile MSOUND yourself because the audio API of your choosing is not available in the pre-compiled binaries: the package includes separate build scripts for all three major platforms, `compile_msound_linux.m` for Linux OSes, `compile_msound_mac.m` for Mac OS X, and `compile_msound_win.m` for Windows. Since there are quirks to each and everyone of the platforms, this guide splits into sections for compiling on [Linux](#compiling-on-linux), [Mac OS X](#compiling-on-mac-os-x), and [Windows](#compiling-on-windows).
