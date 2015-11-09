# How to build MSOUND

**Please note that manual compilation is not supposed to be a mandatory step to using MSOUND "out of the box". Our GitHub Releases page features the most current version of MSOUND pre-compiled binaries for all platforms with very sensible choices of audio APIs. [Go to the latest release here](https://github.com/TGM-Oldenburg/Msound/releases/latest).**

If you want to compile MSOUND yourself because the audio API of your choosing is not available in the pre-compiled binaries: the package includes separate build scripts for all three major platforms, `compile_msound_linux.m` for Linux OSes, `compile_msound_mac.m` for Mac OS X, and `compile_msound_win.m` for Windows. Since there are quirks to each and everyone of the platforms, this guide splits into sections for compiling on [Linux](#compiling-on-linux), [Mac OS X](#compiling-on-mac-os-x), and [Windows](#compiling-on-windows).


## Compiling on Linux

Compiling on Linux depends on the system-wide PortAudio library, as retrieved using the package manager of our OS flavour. For example on Ubuntu-based distros you need to install portaudio using `apt-get`:

```bash
apt-get install portaudio19-dev
```

This will install the current stable version of the portaudio v19 release cycle (which is 20140130, SVN rev 1919).

Please note that the installed library dictates the audio APIs available to Msound at compile time. Therefore if the system-wide PortAudio ships with OSS and ALSA, there is now way to compile Msound with JACK aswell, except when you custom-compile PortAudio. If you want JACK support in Msound, compile PortAudio as follows. Make sure to change `/path/to/msound/` in step 3 into your path to the MSOUND package.

```bash
# 1.) Download the latest version of PortAudio and extract it
wget http://www.portaudio.com/archives/pa_stable_v19_20140130.tgz
tar xzfv pa_stable_v19_20140130.tgz
cd portaudio

# 2.) Configure and compile with desired host API(s) (always use --with-pic!)
./configure --with-jack --with-alsa --with-oss --with-pic
make

# 3.) Copy the lib and header file over to the Msound repository directory
cp lib/.libs/libportaudio.a include/portaudio.h /path/to/msound/
```

Now you are ready to just execute `compile_msound_linux.m` in MATLAB and you're done!

**Our build environment:**

* Ubuntu (GNOME) 15.10
* MATLAB 2015a
* gcc v5.2.1
* `portaudio19-dev` via apt-get
