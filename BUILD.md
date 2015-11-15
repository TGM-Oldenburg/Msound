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



## Compiling on Mac OS X

Compiling on OS X depends on the PortAudio library being installed system-wide. As the official SVN version includes at least one known bug on the Mac, there is a fork of PortAudio. It is strongly advised to compile that version of the library, and do it *statically* (i.e. configure with `--disable-shared`). Compiling requires `libtool` and `autoconf` to be installed. Compiling can be done as follows:

```bash
# 1.) Clone the latest version of forked PortAudio
git clone https://github.com/Janwillhaus/forked-portaudio.git
cd forked-portaudio

# 2.) Bootstrap and configure
autoreconf -if
./configure --disable-shared

# 3.) Compile and install
make
sudo make install
```

Step 3 causes the portaudio library to be installed system-wide upon which Msound is simply compiled by executing `compile_msound_mac.m` in MATLAB.

**Our build environment:**

* OS X 10.11 El Capitan
* MATLAB 2015b ([patched](http://www.mathworks.com/matlabcentral/answers/246507-why-can-t-mex-find-a-supported-compiler-in-matlab-r2015b-after-i-upgraded-to-xcode-7))
* Xcode 7.1

