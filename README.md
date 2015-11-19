MSOUND
======

MSOUND is a audio interface for MATLAB that allows you to access your system's sound devices via a unified interface for all major operating systems: via Microsoft Windows Multi Media Extension (MME), Steinberg Audio Streaming Input Output (ASIO) drivers, or Windows Audio Session API (WASAPI) on Windows, via Advanced Linux Sound Architecture (ALSA) or Open Sound System (OSS) on Linux, and via Core Audio (CA) on Mac OS X.


**Table of Contents**

- [Outline](#outline)
- [Usage details](#usage-details)
	- [msound( 'deviceInfo')](#msound-deviceinfo)
	- [msound( 'openRead')](#msound-openread)
	- [msound( 'openWrite')](#msound-openwrite)
	- [msound( 'openRW')](#msound-openrw)
	- [msound( 'getSamples')](#msound-getsamples)
	- [msound( 'putSamples')](#msound-putsamples)
	- [msound( 'stop')](#msound-stop)
	- [msound( 'close')](#msound-close)
	- [msound( 'verbose')](#msound-verbose)
- [Examples](#examples)
  - [Basic audio output](#basic-audio-output)
  - [Basic audio input](#basic-audio-input)
  - [Advanced simultaneous audio input/output](#advanced-simultaneous-audio-inputoutput)
- [License](#license)

## Outline

Every installed audio device is identified by a unique device ID, which must be specified when using such a device. Whereas these IDs vary from system to system, the device ID 0 always corresponds to your system's default audio device. Some devices are capable of playback, while others may be used for recording, only. You may display a list of supported devices by typing the command `MSOUND('deviceInfo')`.

Before a audio device can be used, it has to be opened. MSOUND is able to open devices for playback only, recording only, or duplex recoding and playback. Further information on the required commands may be displayed by typing `MSOUND('help')`. Thereafter MSOUND may be used to "get" recorded audio from the device or to "put" it into the devices' playback buffer.

After a device has been used by MSOUND it must be closed by calling `MSOUND('close')`. Failure to do so may cause the used device to become unavailable to any other application until reboot.

## Usage details

The typical MSOUND usage is `[...] = MSOUND( 'command' [, ...] )`. MSOUND's first input argument is a string, repesenting the command to be executed. Wheras some command require additional input and/or output argument to be present. You may type `MSOUND('help')` to display a list of available commands and required parameters. For the sake of completeness the available command are listed below aswell.


### msound( 'deviceInfo')
```matlab
msound( 'deviceInfo' )
```

Displays a list of available devices and their configurations.

```matlab
stDevices = msound( 'deviceInfo' )
```

Returns a struct array `stDevices` containing detailed information for
all available devices. Whereas the returned structure array contains the
data fields:

| Struct field  | Description                                |
|:--------------|:-------------------------------------------|
| `name`        | The name of the device.                    |
| `api`         | The API used by this device.               |
| `id`          | The device's ID.                           |
| `inputs`      | The maximum number of input channels.      |
| `outputs`     | The maximum number of output channels.     |
| `samplerate`  | The device's default sample rate in Hertz. |
| `samplerates` | A list of supported sample rates in Hertz. |

You may type `unique({stDevices.api})` to retrieve a list of used APIs, Whereas `find(strcmpi({stDevices.api},'WMME'))` returns the IDs of all devices using the Windows MultiMedia Extensions (WMME) API.
```matlab
[ stDevices, defaultIDs ] = msound( 'deviceInfo' )
```

Returns a struct array `stDevices` containing detailed information for all available devices, whereas the second output argument `defaultIDs` contains the IDs of the default input (first element) and default output (second element) device.
```matlab
msound( 'deviceInfo', deviceID )
```

Displays some detailed information on the device specified by its device ID `deviceID`.
```matlab
stDevice = msound( 'deviceInfo', deviceID )
```

Returns a struct array `stDevices` containing detailed information for the device specified by its device ID `deviceID`.
```matlab
[ stDevice, defaultIDs ] = msound( 'deviceInfo', deviceID )
```

Returns a data struct `stDevice` containing detailed information for the device specified by its device ID `deviceID`, whereas the second output argument `defaultIDs` contains the IDs of the default input (first element) and default output (second element) device.

### msound( 'openRead')

```matlab
msound( 'openRead' )
```

Open default input device for recording some single channel audio data using 44.1kHz sample rate and 1024 samples per block. See below for additional optional parameters.

```matlab
latency = msound( 'openRead', deviceID   , ...
                              sampleRate , ...
                              blockSize  , ...
                              numChannels, ...
                              latency     )
```

Open a specific device for recording audio data of specified format. The argument `deviceID` contains the ID of the device to be opened whereas ID 0 or [] indicate the default input device. Arguments `sampleRate`, `blockSize`, and `numChannels` represent the desired audio sample rate in Hertz, the preferred number of sample frames per data block, and the requested number of signal channels. If omitted or set to `[]`, parameters are set to their respective default values (44.1kHz, 1024 sample frames per data block, and 1 signal channel.)

The `latency` input argument may be used to suggest a specific audio processing latency in seconds. If omitted or empty, MSOUND will choose an appropriate default latency. While the input argument is merely a suggestion, the output argument represents an estimation of the actual audio processing latency in seconds, which may be different from the suggestion.  In case of error the returned latency will be zero.

### msound( 'openWrite')

```matlab
msound( 'openWrite' )
```

Open default output device to play back some single channel audio data using 44.1kHz sample rate and 1024 samples per block. See below for additional optional parameters.

```matlab
latency = msound( 'openWrite', deviceID   , ...
                               sampleRate , ...
                               blockSize  , ...
                               numChannels, ...
                               latency     )
```

Open a specific device to play-back audio data of specified format. The argument `deviceID` contains the ID of the device to be opened whereas ID 0 or [] indicate the default output device. Arguments `sampleRate`, `blockSize`, and `numChannels` represent the desired audio sample rate in Hertz, the preferred number of sample frames per data block, and the requested number of signal channels. If omitted or set to [], parameters are set to their respective default values (44.1kHz, 1024 sample frames per data block, and 1 signal channel.)

The `latency` input argument may be used to suggest a specific audio processing latency in seconds. If omitted or empty, MSOUND will choose an appropriate default latency. While the input argument is merely a suggestion, the output argument represents an estimation of the actual audio processing latency in seconds, which may be different from the suggestion.

In case of error the returned latency will be zero.

### msound( 'openRW')

```matlab
msound( 'openRW' )
```

Open default input and output device for single channel duplex play back and recording, using 44.1kHz sample rate and 1024 samples per block. See below for additional optional parameters.

```matlab
latency = msound( 'openRW', deviceIds  , ...
                            sampleRate , ...
                            blockSize  , ...
                            numChannels, ...
                            latency     )
```

Open the specific device(s) for duplex play-back and recording of some audio data using the specified format. The argument `deviceID` contains the IDs of the input device followed by the ID of the output device, whereas ID 0 or [] indicate the respective default device. If only a single ID is specified, the corresponding device is used for both, input and output. The arguments `sampleRate`, `blockSize`, and `numChannels` represent the desired audio sample rate in Hertz, the preferred number of sample frames per data block, and the requested number of signal channels. If omitted or set to `[]`, parameters are set to their respective default values (44.1kHz, 1024 sample frames per data block, and 1 signal channel.) The `numChannels` argument may also be a two element vector containing the desired number of input channels followed by the number of output channels. The `latency` input argument may be used to suggest a specific audio processing latency in seconds. If omitted or empty, MSOUND will choose an appropriate default latency.

While the input argument is merely a suggestion, the output argument represents an estimation of the actual audio processing latency in seconds, which may be different from the suggestion. In case of error the returned latency will be zero. This argument may also be a two element vector; input latency followed by output latency. The returned latency is always a two element row vector.


### msound( 'getSamples')

```matlab
dataIn = msound( 'getSamples' )
```

Gets samples from recording queue and returns them in a `blockSize`-by-`numChannels` matrix `dataIn`.

```matlab
dataIn = msound( 'getSamples', numSampleFrames )
```

Gets samples from recording queue and returns them in a `numSampleFrames`-by-`numChannels` matrix `dataIn`.


### msound( 'putSamples')

```matlab
msound( 'putSamples', dataOut )
```

Puts the `blockSize`-by-`numChannels` matrix `dataIn` into queue for play-back. Note, this function works `X`-by-`numChannels` matrices as well, but there might be some zero-padding in case `X` doesn't match an exact integer multiply of `blockSize`.


### msound( 'stop')

```matlab
msound( 'stop' )
```

Wait until all buffers have been consumed, stop audio processing, close the audio device(s), and terminate audio hardware access. This is a very important step after using MSOUND. If MSOUND isn't terminated properly, some audio devices may become blocked until system reboot.


### msound( 'close')

```matlab
msound( 'close' )
```

Abort audio processing immediately, close the device(s), and terminate audio hardware access. This is a very important step after using MSOUND. If MSOUND isn't terminated properly, audio devices may become some blocked until system reboot.


### msound( 'verbose')

```matlab
msound( 'verbose', X )
```

Toggle verbose state, whereas `X` represents the desired verbose level. If MSOUND is in verbose mode, some additional (debug) information is displayed during use. The verbose level controls the amount of details to be displayed. Level 1 shows some basic messages that might be of interest for debugging m-files using MSOUND. Level 2 on the other hand seems a little over the top for everyday use but it can be very useful for debugging the MSOUND MEX-file. Using a verbose level of 0 disables verbose mode, which is also the default setting. If `X` is omitted, it defaults to level 1.


```matlab
state = msound( 'verbose', X )
```

The output argument returns MSOUND's state of initialization in a struct. It can be used to identify the requirement of using terminate access to the hardware or whether opening is necessary (when MSOUND was already initialized before).


## Examples

### Basic audio output

TBD

### Basic audio input

TBD

### Advanced simultaneous audio input/output

TBD


## License

Copyright 2009  S. Fischer
> Institute of Hearing Technology and Audiology, University of Applied
> Sciences Wilhelmshaven/Oldenburg/Elsfleth, Ofener Str. 16/19,
> 26121 Oldenburg, Germany
> http://www.hoertechnik-audiologie.de

Copyright 2004-2008  S. Fischer, M. Hansen, J. Bitzer
> Institute of Hearing Technology and Audiology, University of Applied
> Sciences Oldenburg/Ostfriesland/Wilhelmshaven, Ofener Str. 16/19,
> 26121 Oldenburg, Germany

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

Visit http://www.gnu.org/licenses or see provided `LICENSE.md` file for more information.
