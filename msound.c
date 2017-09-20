/*------------------------------------------------------------------------//
// MSound - Audio playback and recording interface for MATLAB.            //
// Copyright (C) 2009  S. Fischer                                         //
//   Institute of Hearing Technology and Audiology                        //
//   University of Applied Sciences Wilhelmshaven/Oldenburg/Elsfleth      //
//   Ofener Str. 16/19, 26121 Oldenburg, Germany                          //
//   http://www.hoertechnik-audiologie.de                                 //
// Copyright (C) 2004-2009  S. Fischer, M. Hansen, J. Bitzer,             //
//   Institute of Hearing Technology and Audiology                        //
//   University of Applied Sciences Oldenburg/Ostfriesland/Wilhelmshaven  //
//   Ofener Str. 16/19, 26121 Oldenburg, Germany                          //
//                                                                        //
// This library is free software; you can redistribute it and/or modify   //
// it under the terms of the GNU Lesser General Public License as         //
// published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                        //
//                                                                        //
// This library is distributed in the hope that it will be useful,        //
// but WITHOUT ANY WARRANTY; without even the implied warranty of         //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
// GNU Lesser General Public License for more details.                    //
//                                                                        //
// You should have received a copy of the GNU Lesser General Public       //
// License along with this library; if not, write to the Free Software    //
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,             //
// MA 02110-1301 USA                                                      //
//                                                                        //
// BECAUSE THE LIBRARY IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY   //
// FOR THE LIBRARY, TO THE EXTENT PERMITTED BY APPLICABLE LAW. EXCEPT     //
// WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER    //
// PARTIES PROVIDE THE LIBRARY "AS IS" WITHOUT WARRANTY OF ANY KIND,      //
// EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE        //
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR     //
// PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE      //
// LIBRARY IS WITH YOU. SHOULD THE LIBRARY PROVE DEFECTIVE, YOU ASSUME    //
// THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.             //
//                                                                        //
// IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING  //
// WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR    //
// REDISTRIBUTE THE LIBRARY AS PERMITTED ABOVE, BE LIABLE TO YOU FOR      //
// DAMAGES, INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL   //
// DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE LIBRARY         //
// (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING RENDERED      //
// INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE OF //
// THE LIBRARY TO OPERATE WITH ANY OTHER SOFTWARE), EVEN IF SUCH HOLDER   //
// OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.    //
//                                                                        //
// Visit http://www.gnu.org/licenses or see provided "lgpl-2.1.txt" file  //
// for more information.                                                  //
//------------------------------------------------------------------------*/

/*------------------------------------------------------------------------//
// MSound utilizes the PortAudio portable audio library by Ross Bencina   //
// and Phil Burk. PortAudio ist accompanied by the following copyright,   //
// see below and at http://www.portaudio.com                              //
//                                                                        //
// PortAudio Portable Real-Time Audio Library                             //
// Copyright (c) 1999-2006 Ross Bencina, Phil Burk                        //
//                                                                        //
// Permission is hereby granted, free of charge, to any person obtaining  //
// a copy of this software and associated documentation files (the        //
// "Software"), to deal in the Software without restriction, including    //
// without limitation the rights to use, copy, modify, merge, publish,    //
// distribute, sublicense, and/or sell copies of the Software, and to     //
// permit persons to whom the Software is furnished to do so, subject to  //
// the following conditions:                                              //
//                                                                        //
// The above copyright notice and this permission notice shall be         //
// included in all copies or substantial portions of the Software.        //
//                                                                        //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        //
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     //
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. //
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   //
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   //
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      //
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 //
//                                                                        //
// The text above constitutes the entire PortAudio license; however, the  //
// PortAudio community also makes the following non-binding requests:     //
//                                                                        //
// Any person wishing to distribute modifications to the Software is      //
// requested to send the modifications to the original developer so that  //
// they can be incorporated into the canonical version. It is also        //
// requested that these non-binding requests be included along with the   //
// license above.                                                         //
//------------------------------------------------------------------------*/


/*---------------------------------------------------------------------*//**
\file
\brief The msound main source code file.
\version 1.2
\date 2011-10-10

This is the actual msound MEX-file source code file. It implements the an
audio interface using the PortAudio portable audio library which may be
downloaded at http://www.portaudio.org. In order to build with support for
Steinberg's ASIO interface, the Steinberg ASIO SDK 2 is required, which is
available at http://www.steinberg.net/en/company/3rd_party_developer.html.
\n\n
At the moment it is suggested to use the MATLAB 7.0.4 (R14SP2) External
Interfaces SDK for building the <tt>msound.dll</tt> MEX-file, since it
works fine for MATLAB version 6.5.0 (R13) up to 7.7.0.471 (R2008b). The
down side in doing so is that more recent versions of MATLAB do no longer
support <tt>.dll</tt> MEX-files. In order to use MSound with e.g. MATLAB
R2009, the MATLAB R2009 external interfaces SDK must be used. But be warned,
using SDK versions other than 7.0.4 (R14SP2), is prone to version conflicts
and compatibility issues. Don't say I didn't warn ya! - SF

\note
Be aware that using a different PortAudio (PA) source code than the one,
accompanying this file could resolve existing or introduce new issues with
msound. It happened before (e.g. a PortAudio update introduced a bug that
caused msound to crash whenever the ASIO interface was used.) and it can
happen again. Don't get me wrong, PortAudio is a fine piece of software,
but msound cannot be guaranteed to be compatible to all versions of PA. - SF

\author Sven Fischer
     \n Martin Hansen
     \n J&ouml;rg Bitzer
*//*----------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
History:
v1.2 (2011-10-10)
+ Added return value for call to "verbose", as a struct.  MH

v1.1 (2009-06-11) <- v1.0.2.3
+ Adapted implementation to be used with the MATLAB 7.7.0.471 (R2008b)
  External Interfaces SDK and Matlab's new 48bit data array structures,
  while maintaining compatibility to the MATLAB 7.0.4.365 (R14) Service
  Pack 2 External Interface SDK.
+ Duplex processing now supports separate input and output channel counts.
~ Revised 'DeviceInfo' command to return MATLAB data structs. Note this
  may break compatibility to previous versions of msound.
- Removed redundant 'DeviceCount' and 'DeviceName' commands. Please use
  the revised 'DeviceInfo' command instead. Note this may break
  compatibility to previous versions of msound.
+ Lots of internal changes!
//------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// 32 bit array compatibility mode may be enabled by defining a macro
// named MX_COMPAT_32.
//------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// MATLAB Executable / MEX-file SDK.
//------------------------------------------------------------------------*/
#include "IhaMexFile.h" /* IHA helper file for building MEX-files. */
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// Include generic header files.                                          //
//------------------------------------------------------------------------*/
#include <ctype.h>  /* tolower() */
#include <string.h> /* strcmp() */
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// PortAudio v19 library                                                  //
//------------------------------------------------------------------------*/
#include "portaudio.h" /* PortAudio */
/*------------------------------------------------------------------------//
// If this is Microsoft Visual C++, configure "winmm.lib" library to be   //
// included automatically.                                                //
//------------------------------------------------------------------------*/
#ifdef _MSC_VER
	#pragma comment(lib, "winmm.lib")
#endif
/*------------------------------------------------------------------------//
// Enable / disable PortAudio debug messages. (Requires "pa_debugprint.c" //
// to be compiled also.)                                                  //
//------------------------------------------------------------------------*/
/* #define MSOUND_ENABLE_PA_DEBUG */
#ifdef MSOUND_ENABLE_PA_DEBUG
	#define PA_LOG_API_CALLS
	#define PA_ENABLE_DEBUG_OUTPUT
	#include "pa_debugprint.h"
#endif
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// Definition of some default parameters.                                 //
//------------------------------------------------------------------------*/
/** @brief The current MSOUND version (double).
 *
 *  The current MSOUND version to be displayed on the screen or returned by
 *  the \c version command. (This must be a plain number, i.e. no brackes.)
 *
 *  \see MSOUND_VERSION_STR
 */
#define MSOUND_VERSION 1.2
/** @brief The current MSOUND version's date string.
 *
 *  A string containing the current msound version's date in dd-mmm-yyyy
 *  format, which is to be displayed on the screen or or returned by the
 *  \c version command.
 *
 *  \see MSOUND_VERSION_STR
 */
#define MSOUND_VERSION_DATE "10-Oct-2011"
/** @brief The current MSOUND version string.
 *
 *  The current MSOUND version string to be displayed on the screen or
 *  returned by the \c version command. (This macro will be automatically
 *  generated from #MSOUND_VERSION.
 *
 *  \see MSOUND_VERSION_DATE
 *    \n MSOUND_VERSION
 */
#define MSOUND_VERSION_STR MSOUND_VERSION_2_STR( MSOUND_VERSION )
/*------------------------------------------------------------------------*/
/** @brief Utility macro for generating #MSOUND_VERSION_STR.
 *
 *  Utility macro for generating #MSOUND_VERSION_STR. Do not edit!
 *
 *  \see MSOUND_VERSION_2_STRX(s)
 *    \n MSOUND_VERSION_STR
 */
#define MSOUND_VERSION_2_STR(s) MSOUND_VERSION_2_STRX(s)
/** @brief Utility macro for generating #MSOUND_VERSION_STR.
 *
 *  Utility macro for generating #MSOUND_VERSION_STR. Do not edit!
 *
 *  \see MSOUND_VERSION_2_STR(s)
 *    \n MSOUND_VERSION_STR
 */
#define MSOUND_VERSION_2_STRX(s) # s
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------//
// Definition of some default parameters.                                 //
//------------------------------------------------------------------------*/
/** @brief Default sample rate.
 *
 *  Default sample rate (in Hertz) to be used if unspecified. */
#define MSOUND_DEFAULT_SAMPLE_RATE (44100.0)

/** @brief Default number of input channels.
 *
 *   Default number of input channels, to be used if not specified. */
#define MSOUND_DEFAULT_CHANNELS_IN (1)

/** @brief Default number of output channels.
 *
 *  Default number of output channels, to be used if not specified. */
#define MSOUND_DEFAULT_CHANNELS_OUT (1)

/** @brief Default data block size.
 *
 *  Default data block size, to be used if not specified. */
#define MSOUND_DEFAULT_BLOCK_SIZE (1024)

/** @brief Verbose message prefix string.
 *
 *  Msound verbose message prefix string. */
#define MSOUND_VERBOSE "*** Info (msound):  "

/** @brief Information message prefix string.
 *
 *  Msound information message prefix string. */
#define MSOUND_INFO "*** Info (msound):  "

/** @brief Default sample rates to be checked.
 *
 *  Array of default sample rates (in Hertz) to be checked by the
 *  \c deviceInfo command.
 *
 *  \see MSOUND_DEFAULT_SAMPLE_RATE_COUNT */
static const double pdDefaultSampleRates[] =
{  11025.0,
   22050.0,
   44100.0,
   88200.0,
    8000.0,
   12000.0,
   16000.0,
   24000.0,
   32000.0,
   48000.0,
   96000.0,
  192000.0 };

/** @brief The number of default sample rates to be checked.
 *
 *  The number of default sample rates to be checked by the 'DeviceInfo'
 *  command.
 *
 *  \see pdDefaultSampleRates */
#define MSOUND_DEFAULT_SAMPLE_RATE_COUNT \
( sizeof(pdDefaultSampleRates) / sizeof(double) )
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// Definition and setup of msound's global data buffer.                   //
//------------------------------------------------------------------------*/
/** @brief A structure containing msound's global data buffers.
 *
 *  A structure containing MSOUND's global data buffers.
 */
typedef struct MsoundGlobalBuffer
{
	/** @name General Parameters */
	/*@{*/
	/** @brief Verbose level.
	 *
	 *  Msound's verbose level. Currently the following verbose levels
	 *  are supported by msound:
	 *  \arg \c 0 Disables verbose mode, i.e. no messages are displayed.
	 *            This will be msound's default setting.
	 *  \arg \c 1 Enables verbose mode and causes msound to display
	 *            some basic debug messages.
	 *  \arg \c 2 Enables the extended verbose mode and causes msound
	 *            to display quite a lot of debug messages. In general
	 *            it is not suggested to use this setting for normal
	 *            processing. */
	int iVerbose;

	/** @brief Flag that signalizes that PortAudio has been initialized.
	 *
	 *  This flag is used to store the current PortAudio initialization
	 *  state. If it evaluates \c true, portaudio has been successfully
	 *  initialized. In case this flag evaluates \c false, PortAudio is
	 *  not (properly) initialized. */
	int bIsPaInitialzed;

	/** @brief Buffer for error codes, returned by PortAudio.
	 *
	 *  Buffer to store the most recent PortAudio error code.
	 *
	 *  \see PaError (defined in portaudio.h) */
	PaError iPaError;

	/** @brief PortAudio stream pointer.
	 *
	 *  Pointer to the PortAudio stream used by msound.
	 *
	 *  \see PaStream (defined in portaudio.h) */
	PaStream *pStream;

	/** @brief Device opened for reading flag.
	 *
	 *  This flag (if it evaluates \c true) signalizes that an audio device
	 *  device has been opened for reading.
	 *
	 *  \see bIsOpenForWriting */
	int bIsOpenForReading;

	/** @brief Device opened for writing flag.
	 *
	 *  This flag (if it evaluates \c true) signalizes that an audio device
	 *  device has been opened for writing.
	 *
	 *  \see bIsOpenForReading */
	int bIsOpenForWriting;
	/*@}*/

	/** @name Audio Interface Parameters */
	/*@{*/
	/** @brief The opened audio device's sample rate in Hertz.
	 *
	 *  The opened audio device's sample rate in Hertz.
	 *
	 *  \see dLatencyIn
	 *    \n dLatencyOut */
	double dSampleRate;

	/** @brief Estimated input latency in seconds.
	 *
	 *  The estimated input latency in seconds, or \c 0.0 in case no input
	 *  device is used. Note that this value does most likely differ from
	 *  the user specified suggested input latency.
	 *
	 *  \see dLatencyIn
	 *    \n dSampleRate */
	double dLatencyIn;

	/** @brief Estimated output latency in seconds.
	 *
	 *  The estimated output latency in seconds, or \c 0.0 in case no
	 *  output device is used. Note that this value does most likely
	 *  differ from the user specified suggested output latency.
	 *
	 *  \see dLatencyOut
	 *    \n dSampleRate */
	double dLatencyOut;

	/** @brief Current number of audio input channels.
	 *
	 *  Current number of audio input channels or \c 0 in case no input
	 *  device is used.
	 *
	 *  \see iChannelsOut
	 *    \n iBlockSize */
	mwSize iChannelsIn;

	/** @brief Current number of audio output channels.
	 *
	 *  Current number of audio output channels or \c 0 in case no output
	 *  device is used.
	 *
	 *  \see iChannelsIn
	 *    \n iBlockSize */
	mwSize iChannelsOut;

	/** @brief Currently used block size.
	 *
	 *  Block size, as specified during stream creation.
	 *
	 *  \see iChannelsIn
	 *    \n iChannelsOut */
	mwSize iBlockSize;
	/*@}*/
} MsoundGlobalBuffer;

/** @brief Msound's global static data buffer.
 *
 *  Msound's global static data buffer. This structure is used to store
 *  data buffers that are accessed by msound's various sub routines.
 */
static MsoundGlobalBuffer msound =
{
	(int)       0        , /* Verbose mode is disabled by default.     */
	(int)       0        , /* Portaudio is not initialized by default. */
	(PaError)   paNoError, /* No portaudio error, yet.                 */
	(PaStream*) NULL     , /* PortAudio stream is still unused.        */
	(int)       0        , /* Device in not ready for reading.         */
	(int)       0        , /* Device in not ready for writing.         */
	(double)    0.0      , /* Unspecified sample rate: 0Hz.            */
	(double)    0.0      , /* Zero estimated input latency - unused.   */
	(double)    0.0      , /* Zero estimated output latency - unused.  */
	(mwSize)    0        , /* No input channel count, yet.             */
	(mwSize)    0        , /* No output channel count, yet.            */
	(mwSize)    0          /* Unspecified block size, yet.             */
};
/*------------------------------------------------------------------------*/



/*------------------------------------------------------------------------//
// Enable inline functions for C++ compilers.                             //
//------------------------------------------------------------------------*/
#if defined(__cplusplus)
	/// \brief Inline declaration macro.
	///
	/// This macro is used for declaring some of msounds subroutines as
	/// \c inline, if supported. So in case a c++ compiler is used, this
	/// macro is replaced by the identifier \c inline, otherwise it will
	/// simply vanish into thin air.
	#define MSOUND_CPP_INLINE inline
#else
	/** \brief Inline declaration macro.
	 *
	 * This macro is used for declaring some of msounds subroutines as
	 * \c inline, if supported. So in case a c++ compiler is used, this
	 * macro is replaced by the identifier \c inline, otherwise it will
	 * simply vanish into thin air.
	 */
	#define MSOUND_CPP_INLINE
#endif
/*------------------------------------------------------------------------*/



/** @brief MEX-file function.
 *
 *  The MEX-file entrance function is the main function that is executed
 *  each time the msound MEX-file is called. It processed input and output
 *  arguments, analyzes the provided command string argument and calls the
 *  appropriate sub function.
 *
 *  @param[in]  nlhs Number of requested left-hand side "output" arguments
 *                   for the current function call of \c msound.
 *  @param[out] plhs Left-hand side "output" argument pointer array. This
 *                   array contains \a nlhs \c mxArray pointers, one for
 *                   every requested output argument. In case no output
 *                   arguments are requested (\a nlhs equals zero), it
 *                   still stores a single pointer, which may be used to
 *                   return MATLAB's default output argument \c ans.
 *  @param[in]  nrhs The number of provided right-hand side "input"
 *                   arguments for the current call of \c msound. In case
 *                   no input arguments are provided, the \c msound usage
 *                   is displayed.
 *  @param[in]  prhs Right-hand side "input" argument pointer array. This
 *                   array contains exactly \a nrhs \c mxArray pointers,
 *                   each one of them representing a non-modifiable constant
 *                   input argument. Note that these MATLAB arguments may be
 *                   "empty" although their pointers are valid.
 */
void mexFunction(int nlhs,       mxArray *plhs[],
                 int nrhs, const mxArray *prhs[]);

/** @brief Initialize the msound MEX-file and prepare for further use.
 *
 *  The msound MEX-file's "constructor" function, to be called in case the
 *  MEX-file is loaded to memory. It initializes the PortAudio interface and
 *  Prepares msound for further processing, i.e. execution of any commands.
 *
 *  @see MSOUND_CPP_INLINE
 */
MSOUND_CPP_INLINE
void msoundInitialize(void);

/** @brief The msound MEX-file's "destructor" function.
 *
 *  The msound MEX-file's "destructor" function, to be called in case the
 *  MEX-file is removed from memory. Whereas this function, in order to be
 *  used, need's to be registered by calling the external interfaces SDK
 *  function <tt>int mexAtExit(void (*ExitFcn)(void))</tt>, first. In a
 *  nutshell this function automatically executes the "close" command in
 *  case the MEX-files gets unloaded while the audio I/O is still active.
 *
 *  @see mexAtExit(void (*ExitFcn)(void)) (provided by the MEX-file SDK)
 */
void msoundAtExit(void);
/* No inline function because it is called via function pointer, anyway. */

/** @brief Display the msound usage in the MATLAB command window.
 *
 *  This function displays the msound usage, i.e. it sends msound's calling
 *  convention and a list of available commands and parameters to the
 *  MATLAB console. Normally text will be displayed in the MATLAB command
 *  window, but MATLAB does allow redirection, also.
 *
 *  @see MSOUND_CPP_INLINE
 */
MSOUND_CPP_INLINE
void msoundUsage(void);

/** @brief Msound command \c openRead.
 *
 *  Function that executes msound's \c openRead command. It takes up to
 *  five input arguments (input device ID, input sample rate, block size,
 *  number of channels, and desired latency in seconds) and returns a
 *  single output argument (estimated latency in seconds). See msound usage
 *  for details.
 *
 *  @param[in]  nlhs Number of requested left-hand side "output" arguments
 *                   for the current call of the command.
 *  @param[out] plhs Left-hand side "output" argument pointer array. This
 *                   array contains \a nlhs \c mxArray pointers, one for
 *                   every requested output argument. In case no output
 *                   arguments are requested (\a nlhs equals zero), this
 *                   array still needs to be large enough to store a single
 *                   pointer, which may be used to return MATLAB's default
 *                   output argument \c ans.
 *  @param[in]  nrhs The number of provided right-hand side "input"
 *                   arguments for msound's \c openRead command.
 *  @param[in]  prhs Right-hand side "input" argument pointer array. This
 *                   array contains exactly \a nrhs \c mxArray pointers,
 *                   each one of them representing a non-modifiable constant
 *                   input argument. Note that these MATLAB arguments may be
 *                   "empty" although their pointers are valid.
 *
 *  @see MSOUND_CPP_INLINE
 */
MSOUND_CPP_INLINE
void msoundOpenRead(int nlhs,       mxArray *plhs[],
                    int nrhs, const mxArray *prhs[]);

/** @brief Msound command \c openWrite.
 *
 *  Function that executes msound's \c openWrite command. It takes up to
 *  five input arguments (output device ID, output sample rate, block size,
 *  number of channels, and desired latency in seconds) and returns a
 *  single output argument (estimated latency in seconds). See msound usage
 *  for details.
 *
 *  @param[in]  nlhs Number of requested left-hand side "output" arguments
 *                   for the current call of the command.
 *  @param[out] plhs Left-hand side "output" argument pointer array. This
 *                   array contains \a nlhs \c mxArray pointers, one for
 *                   every requested output argument. In case no output
 *                   arguments are requested (\a nlhs equals zero), this
 *                   array still needs to be large enough to store a single
 *                   pointer, which may be used to return MATLAB's default
 *                   output argument \c ans.
 *  @param[in]  nrhs The number of provided right-hand side "input"
 *                   arguments for msound's \c openWrite command.
 *  @param[in]  prhs Right-hand side "input" argument pointer array. This
 *                   array contains exactly \a nrhs \c mxArray pointers,
 *                   each one of them representing a non-modifiable constant
 *                   input argument. Note that these MATLAB arguments may be
 *                   "empty" although their pointers are valid.
 *
 *  @see MSOUND_CPP_INLINE
 */
MSOUND_CPP_INLINE
void msoundOpenWrite(int nlhs,       mxArray *plhs[],
                     int nrhs, const mxArray *prhs[]);

/** @brief Msound command \c openRW.
 *
 *  Function that executes msound's \c openRW command for duplex processing.
 *  It takes up to five input arguments (input and output device IDs,
 *  sample rate, block size, number of input/output channels, and desired
 *  input/output latency in seconds) and returns a single output argument
 *  (estimated input/output latency in seconds). See msound usage for
 *  details.
 *
 *  @param[in]  nlhs Number of requested left-hand side "output" arguments
 *                   for the current call of the command.
 *  @param[out] plhs Left-hand side "output" argument pointer array. This
 *                   array contains \a nlhs \c mxArray pointers, one for
 *                   every requested output argument. In case no output
 *                   arguments are requested (\a nlhs equals zero), this
 *                   array still needs to be large enough to store a single
 *                   pointer, which may be used to return MATLAB's default
 *                   output argument \c ans.
 *  @param[in]  nrhs The number of provided right-hand side "input"
 *                   arguments for msound's \c openRW command.
 *  @param[in]  prhs Right-hand side "input" argument pointer array. This
 *                   array contains exactly \a nrhs \c mxArray pointers,
 *                   each one of them representing a non-modifiable constant
 *                   input argument. Note that these MATLAB arguments may be
 *                   "empty" although their pointers are valid.
 *
 *  @see MSOUND_CPP_INLINE
 */
MSOUND_CPP_INLINE
void msoundOpenReadWrite(int nlhs,       mxArray *plhs[],
                         int nrhs, const mxArray *prhs[]);

/** @brief Msound command \c stop.
 *
 *  Function that executes msound's \c stop command. The stop command
 *  initiates msound's soft shut down procedure. Msound'll wait for as long
 *  it takes for all internal buffers to be completely processed/consumed.
 *  Then the processing is stopped, the device is closed, and the PortAudio
 *  interface is terminated. This function doesn't take input arguments nor
 *  does it return any output arguments. See msound usage for details.
 *
 *  @param[in]  nlhs Number of requested left-hand side "output" arguments
 *                   for the current call of the command.
 *  @param[out] plhs Left-hand side "output" argument pointer array. This
 *                   array contains \a nlhs \c mxArray pointers, one for
 *                   every requested output argument. In case no output
 *                   arguments are requested (\a nlhs equals zero), this
 *                   array still needs to be large enough to store a single
 *                   pointer, which may be used to return MATLAB's default
 *                   output argument \c ans.
 *  @param[in]  nrhs The number of provided right-hand side "input"
 *                   arguments for msound's \c stop command.
 *  @param[in]  prhs Right-hand side "input" argument pointer array. This
 *                   array contains exactly \a nrhs \c mxArray pointers,
 *                   each one of them representing a non-modifiable constant
 *                   input argument. Note that these MATLAB arguments may be
 *                   "empty" although their pointers are valid.
 *
 *  @see msoundClose(int, mxArray*[], int, const mxArray*[])
 *    \n MSOUND_CPP_INLINE
 */
MSOUND_CPP_INLINE
void msoundStop(int nlhs,       mxArray *plhs[],
                int nrhs, const mxArray *prhs[]);

/** @brief Msound command \c close.
 *
 *  Function that executes msound's \c close command. The close command
 *  will immediately shut down msound and terminate the PortAudio interface.
 *  Unprocessed internal buffers will be lost. This function doesn't take
 *  input arguments nor does it return any output arguments. See msound
 *  usage for details.
 *
 *  \note
 *  The \c stop command should be the normal way of terminating msound.
 *
 *  @param[in]  nlhs Number of requested left-hand side "output" arguments
 *                   for the current call of the command.
 *  @param[out] plhs Left-hand side "output" argument pointer array. This
 *                   array contains \a nlhs \c mxArray pointers, one for
 *                   every requested output argument. In case no output
 *                   arguments are requested (\a nlhs equals zero), this
 *                   array still needs to be large enough to store a single
 *                   pointer, which may be used to return MATLAB's default
 *                   output argument \c ans.
 *  @param[in]  nrhs The number of provided right-hand side "input"
 *                   arguments for msound's \c close command.
 *  @param[in]  prhs Right-hand side "input" argument pointer array. This
 *                   array contains exactly \a nrhs \c mxArray pointers,
 *                   each one of them representing a non-modifiable constant
 *                   input argument. Note that these MATLAB arguments may be
 *                   "empty" although their pointers are valid.
 *
 *  @see msoundStop(int, mxArray*[], int, const mxArray*[])
 *    \n MSOUND_CPP_INLINE
 */
MSOUND_CPP_INLINE
void msoundClose(int nlhs,       mxArray *plhs[],
                 int nrhs, const mxArray *prhs[]);

/** @brief Msound command \c deviceInfo.
 *
 *  Function that executes msound's \c deviceInfo command. This command is
 *  used to retrieve information on available and supported audio devices.
 *  Depending on the provided input and output arguments, it either send
 *  an information text to the MATLAB console or it return a struct array
 *  containing the desired device properties. Again, depending on the
 *  argument either all devices or only a single one are/is considered.
 *  See msound usage for details.
 *
 *  @param[in]  nlhs Number of requested left-hand side "output" arguments
 *                   for the current call of the command.
 *  @param[out] plhs Left-hand side "output" argument pointer array. This
 *                   array contains \a nlhs \c mxArray pointers, one for
 *                   every requested output argument. In case no output
 *                   arguments are requested (\a nlhs equals zero), this
 *                   array still needs to be large enough to store a single
 *                   pointer, which may be used to return MATLAB's default
 *                   output argument \c ans.
 *  @param[in]  nrhs The number of provided right-hand side "input"
 *                   arguments for msound's \c deviceInfo command.
 *  @param[in]  prhs Right-hand side "input" argument pointer array. This
 *                   array contains exactly \a nrhs \c mxArray pointers,
 *                   each one of them representing a non-modifiable constant
 *                   input argument. Note that these MATLAB arguments may be
 *                   "empty" although their pointers are valid.
 *
 *  @see MSOUND_CPP_INLINE
 */
MSOUND_CPP_INLINE
void msoundDeviceInfo(int nlhs,       mxArray *plhs[],
                      int nrhs, const mxArray *prhs[]);



/*------------------------------------------------------------------------*/



/*------------------------------------------------------------------------//
// Msound's main function, called by matlab. See mex-file-SDK for details.//
//------------------------------------------------------------------------*/
void mexFunction(int nlhs,       mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
	/* Pointer to access the matlab string command. */
	char *szCommand = NULL;

	/* Just a counter. */
	mwIndex i = 0;

#ifdef MSOUND_ENABLE_PA_DEBUG
	/* Redirect PortAudio debug messages to the MATLAB command window. */
	PaUtil_SetDebugPrintFunction( (PaUtilLogCallback)&mexPrintf );
#endif


	/*--------------------------------------------------------------------//
	// The initialization of PortAudio is necessary in order to "wake up" //
	// PortAudio, so it is able to respond correctly to any commands.     //
	//--------------------------------------------------------------------*/

	/*////////////////////////////////////////////////////////////////////*/
	/* If PortAudio is currently not initialized */
	if( !msound.bIsPaInitialzed ) { msoundInitialize(); }
	/*////////////////////////////////////////////////////////////////////*/


	/*--------------------------------------------------------------------//
	// Do some simple input argument checking and retrieve msound's       //
	// command string.                                                    //
	//--------------------------------------------------------------------*/

	/*////////////////////////////////////////////////////////////////////*/
	/* If msound was called without any input arguments whatsoever, display
	 * usage and exit. - Exit WITHOUT ERROR since nothing went wrong. */
	if( nrhs == 0 ) { msoundUsage(); return; }

	/* If the first input argument is non-string parameter, issue an error
	 * and exit. Don't display the usage. */
	if( !isString( prhs[0] ) )
	{
		mexErrMsgTxt( "First input argument must be a character string." );
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	/* Retrieve the matlab string command as c-string. Make sure to free
	 * this buffer by calling 'mxFree( szCommand )', if no longer needed. */
	szCommand = mxArrayToString( prhs[0] );

	/* Check for invalid string pointer. */
	if( !szCommand )
	{
		mexErrMsgTxt( "Unable to retrieve command string. Out of Memory?" );
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	/* Display the string we've just received; debug info. */
	if( msound.iVerbose )
	{
		mexPrintf(MSOUND_VERBOSE "Received command '%s'.\n", szCommand);
	}

	/* Convert command string to lower case. */
	for(i=0; szCommand[i]; ++i) { szCommand[i] = tolower(szCommand[i]); }
	/*////////////////////////////////////////////////////////////////////*/


	/*--------------------------------------------------------------------//
	// Use an if-else construct to determine which command has been       //
	// entered and what action to perform.                                //
	//--------------------------------------------------------------------*/

	/*--------------------------------------------------------------------//
	// Play some data ...                                                 //
	//--------------------------------------------------------------------*/
	if( !strcmp( szCommand, "putsamples" ) ) /* formerly: "putSamples" */
	{
		/* Dimensions of a matlab array. */
		mwSize mrows = 0;
		mwSize ncols = 0;

		/* Block size of the currently processed data block. */
		mwSize iBlockSizeCur = 0;

		/* Pointer used for arbitrary data access. */
		const double *pdMatlabData     = NULL; /* Matlab double data. */
		float        *pfPortAudioData  = NULL; /* PortAudio float data. */

		mwIndex c = 0; /* Just a counter. */
		mwIndex f = 0; /* Just a counter. */

		/* Delete the command string, in case we won't get another chance.*/
		mxFree( szCommand );

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Executing command 'putsamples'.\n"); }


		/* Check number of input arguments. */
		if( nrhs != 2 )
		{
			mexErrMsgTxt( "Wrong number of input arguments." );
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Check number of output arguments. */
		if( nlhs != 0 )
		{
			mexErrMsgTxt( "Wrong number of output arguments." );
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Check if the device is prepared for output. */
		if( !msound.bIsOpenForWriting )
		{
			mexErrMsgTxt( "Device not opened for data output!" );
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Check input data type. */
		if( !isNonComplexDoubleMatrix( prhs[1] ) )
		{
			mexErrMsgTxt( "Data must be non-complex double matrix." );
			return; /* Unnecessary, but for the sake of consistency ... */
		}


		mrows = mxGetM( prhs[1] ); /* Get number of rows/sample frames. */
		ncols = mxGetN( prhs[1] ); /* Get number of columns/channels. */
		iBlockSizeCur = mrows; /* Current block size is number of rows. */

		/* Check number of columns/channels. */
		if( ncols != msound.iChannelsOut )
		{
			mexErrMsgTxt("Number of columns doesn't match number"
			             " of channels."                         );
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Check the current block size according to specified block
		 * size. */
		if( iBlockSizeCur != msound.iBlockSize )
		{
			/* Issue a warning message. */
			mexWarnMsgTxt("Number of rows doesn't match specified block"
			              " size. Using zero-padding to make sure only"
			              " complete data blocks are processed."        );

			/* Add some samples to make sure that the current block is a
			 * pure integer multiple of the default block size. */
			iBlockSizeCur += msound.iBlockSize
			               - iBlockSizeCur % msound.iBlockSize;
		}


		/* Access matlab data. */
		pdMatlabData = mxGetPr( prhs[1] );

		/* Create buffer for PortAudio data. */
		 pfPortAudioData = (float* )
		                   mxCalloc(msound.iChannelsOut*iBlockSizeCur,
		                            (mwSize) sizeof(float )          );

		/* If the buffer could not be created, issue an error and exit. */
		if( !pfPortAudioData )
		{
			/* Something went wrong, so delete all buffers that have
			 * been created. */
			if(  pfPortAudioData ) { mxFree(  pfPortAudioData ); }

			mexErrMsgTxt("Out of memory.");
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Convert matlab data to PortAudio format. */
		for(c=0; c<msound.iChannelsOut; ++c)
		{
			for(f=0; f<mrows; ++f)
			{
                pfPortAudioData[f*msound.iChannelsOut+c] = (float) pdMatlabData[c*iBlockSizeCur+f];
            }
		}

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{
			mexPrintf(MSOUND_VERBOSE "Calling Pa_WriteStream().\n");
		}

		/* If the stream is opened, play-back the current data block. */
		if( msound.pStream )
		{
			msound.iPaError = Pa_WriteStream(msound.pStream               ,
			                                 pfPortAudioData             ,
			                                 (unsigned long) iBlockSizeCur);
		}

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 ||
		    ( (msound.iVerbose) && (msound.iPaError != paNoError) ) )
		{
			mexPrintf(MSOUND_VERBOSE
			          "Pa_WriteStream() returned %d (%s).\n",
			                          msound.iPaError       ,
			          Pa_GetErrorText(msound.iPaError)      );
		}


		/* Delete the data buffer again. */
		if(  pfPortAudioData ) { mxFree(  pfPortAudioData ); }

		/* If an error has occurred, display it and exit. */
		if( (msound.iPaError != paNoError          )&&
		    (msound.iPaError != paOutputUnderflowed) )
		{
			mexErrMsgTxt( Pa_GetErrorText(msound.iPaError) );
			return; /* Unnecessary, but for the sake of consistency ... */
		}


	}
	else
	/*--------------------------------------------------------------------//
	// Record some data ...                                               //
	//--------------------------------------------------------------------*/
	if( !strcmp( szCommand, "getsamples" ) ) /* formerly: "getSamples" */
	{
		/* Dimensions of a matlab array. */
		mwSize mrows = 0;
		mwSize ncols = 0;

		/* Block size of the currently processed data block. */
		mwSize iBlockSizeCur = msound.iBlockSize;

		/* Pointer used for arbitrary data access. */
		double *pdMatlabData     = NULL; /* Matlab double data. */
		float  *pfPortAudioData  = NULL; /* PortAudio float data. */
		float **ppfPortAudioData = NULL; /* PortAudio float data. */

		mwIndex c = 0; /* Just a counter. */
        mwIndex f = 0; /* Just a counter. */

		/* Delete the command string, in case we won't get another
		 * chance. */
		mxFree( szCommand );

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{
			mexPrintf(MSOUND_VERBOSE "Executing command 'getsamples'.\n");
		}


		/* Check number of input arguments. */
		if( nrhs > 2 )
		{
			mexErrMsgTxt( "Wrong number of input arguments." );
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Check number of output arguments. */
		if( nlhs > 1 )
		{
			mexErrMsgTxt( "Wrong number of output arguments." );
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Check if the device is prepared for input. */
		if( !msound.bIsOpenForReading )
		{
			mexErrMsgTxt( "Device not opened for data input!" );
			return; /* Unnecessary, but for the sake of consistency ... */
		}


		/* If the block size is specified by a second parameter. */
		if( nrhs > 1 )
		{
			/* Check argument properties. */
			if( !mxIsEmpty(prhs[1]) )
			{
				if( !isNonComplexDoubleScalar( prhs[1] ) )
				{
					mexErrMsgTxt("Argument must be a noncomplex scalar"
					             " double."                            );

					/* Unnecessary, but for the sake of consistency ... */
					return;
				}
				/* Retrieve the requested block size. */
				iBlockSizeCur = (mwSize) mxGetScalar( prhs[1] );

				/* Check specified block size. */
				if( iBlockSizeCur <= 0 )
				{
					mexErrMsgTxt("Specified block size is invalid.");
					/* Unnecessary, but for the sake of consistency ... */
					return;
				}
				else if( iBlockSizeCur != msound.iBlockSize )
				{
					mexWarnMsgTxt("Requested number of samples doesn't"
					              " match specified default block size.");
				}
			}
			else
			{
				mexWarnMsgTxt("Block size argument is empty, using"
				              " default block size."               );
			}
		}


		/* Create buffer for PortAudio data. */
		 pfPortAudioData = (float* )
		                   mxCalloc(msound.iChannelsIn*iBlockSizeCur,
		                            (mwSize) sizeof(float )         );

		/* If the buffer could not be created, issue an error and exit. */
		if( !pfPortAudioData )
		{
			/* Something went wrong, so delete all buffers that have
			 * been created. */
			if(  pfPortAudioData ) { mxFree(  pfPortAudioData ); }
			mexErrMsgTxt("Out of memory?");
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Calling Pa_ReadStream().\n"); }

		/* If the stream is opened, retrieve the current data block. */
		if( msound.pStream )
		{
			msound.iPaError = Pa_ReadStream(msound.pStream               ,
			                                pfPortAudioData              ,
			                                (unsigned long) iBlockSizeCur);
		}

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1                                ||
		  ( msound.iVerbose && (msound.iPaError != paNoError) ) )
		{
			mexPrintf(MSOUND_VERBOSE
			          "Pa_ReadStream() returned %d (%s).\n",
			                          msound.iPaError      ,
			          Pa_GetErrorText(msound.iPaError)     );
		}

		/* Create a matlab buffer and access it. */
		plhs[0] = mxCreateDoubleMatrix(iBlockSizeCur     ,
		                               msound.iChannelsIn, mxREAL);
		pdMatlabData = mxGetPr( plhs[0] );

		/* Convert PortAudio data to matlab format */
		for(f=0; f<iBlockSizeCur; ++f) {
            for(c=0; c<msound.iChannelsIn; ++c) {
                pdMatlabData[c*iBlockSizeCur+f] = (double) pfPortAudioData[f*msound.iChannelsIn+c];
            }
        }


		/* Delete the data buffer again. */
		if(  pfPortAudioData ) { mxFree(  pfPortAudioData ); }

		/* If an error has occurred, display it and exit. */
		if( (msound.iPaError != paNoError        )&&
		    (msound.iPaError != paInputOverflowed) )
		{
			mexErrMsgTxt( Pa_GetErrorText(msound.iPaError) );
			return; /* Unnecessary, but for the sake of consistency ... */
		}


	}
	else
	/*--------------------------------------------------------------------//
	// Open a device for recording ...                                    //
	//--------------------------------------------------------------------*/
	if( !strcmp( szCommand, "openread" ) ) /* formerly: "openRead" */
	{
		/* Delete the command string, in case we won't get another
		 * chance. */
		mxFree( szCommand );

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Executing command 'openread'.\n"); }

		msoundOpenRead( nlhs, plhs, nrhs-1, prhs+1 );

	}
	else
	/*--------------------------------------------------------------------//
	// Open a device for playback ...                                     //
	//--------------------------------------------------------------------*/
	if( !strcmp( szCommand, "openwrite" ) ) /* formerly: "openWrite" */
	{
		/* Delete the command string, in case we won't get another
		 * chance. */
		mxFree( szCommand );

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Executing command 'openwrite'.\n"); }

		msoundOpenWrite( nlhs, plhs, nrhs-1, prhs+1 );

	}
	else
	/*--------------------------------------------------------------------//
	// Open a device for duplex recording and play-back ...               //
	//--------------------------------------------------------------------*/
	if( !strcmp( szCommand, "openrw" ) ) /* formerly: "openrw" */
	{
		/* Delete the command string, in case we won't get another
		 * chance. */
		mxFree( szCommand );

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Executing command 'openrw'.\n"); }

		msoundOpenReadWrite( nlhs, plhs, nrhs-1, prhs+1 );

	}
	else
	/*--------------------------------------------------------------------//
	// Close the device ...                                               //
	//--------------------------------------------------------------------*/
	if( !strcmp( szCommand, "close" ) )
	{
		/* Delete the command string, in case we won't get another
		 * chance. */
		mxFree( szCommand );

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Executing command 'close'.\n"); }

		/* Call "abort and close" function. */
		msoundClose( nlhs, plhs, nrhs-1, prhs+1 );

	}
	else
	/*--------------------------------------------------------------------//
	// Close the device ...                                               //
	//--------------------------------------------------------------------*/
	if( !strcmp( szCommand, "stop" ) )
	{
		/* Delete the command string, in case we won't get another
		 * chance. */
		mxFree( szCommand );

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Executing command 'stop'.\n"); }

		/* Call "stop and close" function. */
		msoundStop( nlhs, plhs, nrhs-1, prhs+1 );

	}
	else
	/*--------------------------------------------------------------------//
	// Display a list of available devices and parameters ...             //
	//--------------------------------------------------------------------*/
	if( !strcmp( szCommand, "deviceinfo" ) ) /* formerly: "DeviceInfo" */
	{
		mxFree( szCommand ); /* Delete the command string. */

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Executing command 'deviceinfo'.\n"); }

		/* Call device information retrieval function. */
		msoundDeviceInfo( nlhs, plhs, nrhs-1, prhs+1 );

	}
	else
	/*--------------------------------------------------------------------//
	// Change verbose level.                                              //
	//--------------------------------------------------------------------*/
	if( !strcmp( szCommand, "verbose" ) )
	{
		/* Delete the command string, in case we won't get another
		 * chance. */
		mxFree( szCommand );

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Executing command 'verbose'.\n"); }


		/* Check number of input arguments. */
		if( nrhs > 2 )
		{
			mexErrMsgTxt( "Wrong number of input arguments." );
			return; /* Unnecessary, but for consistency ... */
		}

		/* Check number of output arguments. */
		if( nlhs > 1 )
		{
			mexErrMsgTxt( "Wrong number of output arguments." );
			return; /* Unnecessary, but for consistency ... */
		}

		/* Set verbose mode to specified level or to level 1 by default. */
		msound.iVerbose = (nrhs > 1) ? (int) mxGetScalar( prhs[1] ) : 1;

		if( msound.iVerbose )
		{
		        mexPrintf("*** Info (msound):  Set verbose to %d.\n", msound.iVerbose);
			mexPrintf(MSOUND_VERBOSE "bIsOpenForReading: %d\n"         ,
			          msound.bIsOpenForReading                         );
			mexPrintf(MSOUND_VERBOSE "bIsOpenForWriting: %d\n"         ,
			          msound.bIsOpenForWriting                         );
			mexPrintf(MSOUND_VERBOSE "bIsPaInitialzed:   %d\n"         ,
			          msound.bIsPaInitialzed                           );
			mexPrintf(MSOUND_VERBOSE "errPa:             %s (%d)\n"    ,
			          Pa_GetErrorText(msound.iPaError), msound.iPaError);
			mexPrintf(MSOUND_VERBOSE "dSampleRate:       %.1f\n"       ,
			          msound.dSampleRate                               );
			mexPrintf(MSOUND_VERBOSE "iChannelsIn:       %d\n"         ,
			          msound.iChannelsIn                               );
			mexPrintf(MSOUND_VERBOSE "iChannelsOut:      %d\n"         ,
			          msound.iChannelsOut                              );
			mexPrintf(MSOUND_VERBOSE "iBlockSize:        %d\n"         ,
			          msound.iBlockSize                                );
		}

		/* If the struct array is to be returned, create it. */
		if( nlhs >= 1 )
		{
		     /* this functionality added by MH, 10.10.2011:
			returning the verbose-status */
		     mxArray *pStructArray = NULL;

		     mxArray *pIsOpenForReading = mxCreateDoubleScalar((double)(msound.bIsOpenForReading ));
		     mxArray *pIsOpenForWriting = mxCreateDoubleScalar((double)(msound.bIsOpenForWriting ));
		     mxArray *pIsPaInitialzed   = mxCreateDoubleScalar((double)(msound.bIsPaInitialzed ));
		     mxArray *pPaError          = mxCreateDoubleScalar((double)(msound.iPaError ));
		     mxArray *pSampleRate       = mxCreateDoubleScalar((double)(msound.dSampleRate ));
		     mxArray *pChannelsIn       = mxCreateDoubleScalar((double)(msound.iChannelsIn ));
		     mxArray *pChannelsOut      = mxCreateDoubleScalar((double)(msound.iChannelsOut ));
		     mxArray *pBlockSize        = mxCreateDoubleScalar((double)(msound.iBlockSize ));

		     if( !pIsOpenForReading || !pIsOpenForWriting || !pIsPaInitialzed || !pPaError
			 || !pSampleRate || !pChannelsIn || !pChannelsOut || !pBlockSize )
		     {
			  mexErrMsgTxt("Error creating internal buffer.");
			  return; /* Unnecessary, but for consistency ... */
		     }

                      /* Create 1x1 struct array to be returned, or return an error in
		       * case of failure. */
		     if(!(pStructArray = mxCreateStructMatrix(1,1,0,NULL)) ||
			(-1 == mxAddField( pStructArray, "IsOpenForReading"  ) ) ||
			(-1 == mxAddField( pStructArray, "IsOpenForWriting"  ) ) ||
			(-1 == mxAddField( pStructArray, "IsPaInitialzed"    ) ) ||
			(-1 == mxAddField( pStructArray, "errPa"  ) ) ||
			(-1 == mxAddField( pStructArray, "SampleRate"  ) ) ||
			(-1 == mxAddField( pStructArray, "ChannelsIn"  ) ) ||
			(-1 == mxAddField( pStructArray, "ChannelsOut" ) ) ||
			(-1 == mxAddField( pStructArray, "BlockSize" ) ) )
		     {
			  mexErrMsgTxt("Error creating internal buffer.");
			  return; /* Unnecessary, but for consistency ... */
		     }

		     /* Setup the structure array to be used as return argument. */
		     plhs[0] = pStructArray;

		     mxSetField( pStructArray, 0, "IsOpenForReading" , pIsOpenForReading );
		     mxSetField( pStructArray, 0, "IsOpenForWriting" , pIsOpenForWriting );
		     mxSetField( pStructArray, 0, "IsPaInitialzed" ,   pIsPaInitialzed );
		     mxSetField( pStructArray, 0, "errPa" ,            pPaError);
		     mxSetField( pStructArray, 0, "SampleRate" ,       pSampleRate );
		     mxSetField( pStructArray, 0, "ChannelsIn" ,       pChannelsIn );
		     mxSetField( pStructArray, 0, "ChannelsOut" ,      pChannelsOut  );
		     mxSetField( pStructArray, 0, "BlockSize" ,        pBlockSize );
		}

	}
	else
	/*--------------------------------------------------------------------//
	// Change block size ...                                              //
	//--------------------------------------------------------------------*/
	if( !strcmp( szCommand, "version" ) )
	{
		/* Delete the command string, in case we won't get another
		 * chance. */
		mxFree( szCommand );

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Executing command 'version'.\n"); }


		/* Check number of input arguments. */
		if( nrhs != 1 )
		{
			mexWarnMsgTxt( "Wrong number of input arguments." );
		}

		/* Check number of output arguments. */
		if( nlhs > 2 )
		{
			mexErrMsgTxt( "Wrong number of output arguments." );
			return; /* Unnecessary, but for consistency ... */
		}

		/* Display verbose information on the screen. */
		if( (nlhs < 1) || msound.iVerbose )
		{
			mexPrintf(MSOUND_VERBOSE "MSOUND v" MSOUND_VERSION_STR
			          " (" MSOUND_VERSION_DATE ")\n"              );
			mexPrintf(MSOUND_VERBOSE "Compile Time " __DATE__ ", "
			          __TIME__ "\n"                               );
		}

		/* Return string. */
		if( nlhs > 0 )
		{
			plhs[0] = mxCreateString("Msound v" MSOUND_VERSION_STR
			                         " (" MSOUND_VERSION_DATE ")" );
		}

		/* Return. */
		if( nlhs > 1 )
		{
			plhs[1] = mxCreateDoubleScalar( (double) MSOUND_VERSION );
		}


	}
	else
	/*--------------------------------------------------------------------//
	// Change block size ...                                              //
	//--------------------------------------------------------------------*/
	if( !strcmp( szCommand, "help" ) )
	{
		/* Delete the command string, in case we won't get another
		 * chance. */
		mxFree( szCommand );

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Executing command 'help'.\n"); }


		/* Check number of input arguments. */
		if( nrhs != 1 )
		{
			mexWarnMsgTxt( "Wrong number of input arguments." );
		}

		/* Check number of output arguments. */
		if( nlhs != 0 )
		{
			mexErrMsgTxt( "Wrong number of output arguments." );
			return; /* Unnecessary, but for consistency ... */
		}

		/* Display usage. */
		msoundUsage();


	}
	/*--------------------------------------------------------------------//
	// Unknown command ...                                                //
	//--------------------------------------------------------------------*/
	else
	{
		/* Delete the command string, in case we won't get another
		 * chance. */
		mxFree( szCommand );

		mexErrMsgTxt("Unknown command. Type msound('help') for usage.");
		return; /* Unnecessary, but for consistency ... */
	}
	/*--------------------------------------------------------------------*/


	return;
}
/*------------------------------------------------------------------------*/



/*------------------------------------------------------------------------*/



/*------------------------------------------------------------------------//
// Initialize the msound MEX-file and prepare for further use.            //
//------------------------------------------------------------------------*/
MSOUND_CPP_INLINE
void msoundInitialize(void)
{
	/* Display verbose information on the screen. */
	if( msound.iVerbose )
	{ mexPrintf(MSOUND_VERBOSE "Initializing PortAudio.\n" ); }
	if( msound.iVerbose > 1 )
	{ mexPrintf(MSOUND_VERBOSE "Calling Pa_Initialize()\n"); }

	/* Clear msound's global settings. */
	msound.iPaError          = paNoError;
	msound.pStream           = (PaStream*) NULL;
	msound.bIsOpenForReading = 0;
	msound.bIsOpenForWriting = 0;
	msound.dSampleRate       = 0.0;
	msound.dLatencyIn        = 0.0;
	msound.dLatencyOut       = 0.0;
	msound.iChannelsIn       = (mwSize) 0;
	msound.iChannelsOut      = (mwSize) 0;
	msound.iBlockSize        = (mwSize) 0;

	/* Initialize PortAudio */
	msound.iPaError = Pa_Initialize();

	/* Display verbose information on the screen. */
	if( msound.iVerbose > 1                                ||
	  ( msound.iVerbose && (msound.iPaError != paNoError) ) )
	{
		mexPrintf(MSOUND_VERBOSE
		          "Pa_Initialize() returned %d (%s).\n",
		                          msound.iPaError      ,
		          Pa_GetErrorText(msound.iPaError)     );
	}

	/* If initialization failed, issue an error message and
	 * terminate. */
	if( msound.iPaError != paNoError )
	{
		mexErrMsgTxt( "Could not initialize PortAudio." );
		/* mexErrMsgTxt( Pa_GetErrorText(msound.iPaError) ); */
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	msound.bIsPaInitialzed = 1;/* Remember that PortAudio is initialized! */

	/* Display verbose information on the screen. */
	if( msound.iVerbose > 1 )
	{
		mexPrintf(MSOUND_VERBOSE
		          "Instituting MEX-file at-exit function.\n");
	}

	mexAtExit( &msoundAtExit ); /* Setup msound's "destructor" function. */

	return;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// The msound MEX-file's "destructor" function.                           //
//------------------------------------------------------------------------*/
void msoundAtExit(void)
{
	if( msound.iVerbose )
	{ mexPrintf(MSOUND_VERBOSE "Executing MEX-file at-exit function.\n"); }

	/* If msound still is initialized */
	if( msound.bIsPaInitialzed )
	{
		/* If a stream is opened, close it. */
		if( msound.pStream )
		{
			/* Display verbose information on the screen. */
			if( msound.iVerbose )
			{ mexPrintf(MSOUND_VERBOSE "Aborting PortAudio stream.\n"); }
			if( msound.iVerbose > 1 )
			{ mexPrintf(MSOUND_VERBOSE "Calling Pa_AbortStream().\n"); }

			/* Immediately abort play-back and stop the stream. */
			Pa_AbortStream( msound.pStream );

			/* Display verbose information on the screen. */
			if( msound.iVerbose )
			{ mexPrintf(MSOUND_VERBOSE "Closing PortAudio stream.\n"); }
			if( msound.iVerbose > 1 )
			{ mexPrintf(MSOUND_VERBOSE "Calling Pa_CloseStream().\n"); }

			/* Close the stopped stream. */
			Pa_CloseStream( msound.pStream );

			/* Display verbose information on the screen. */
			if( msound.iVerbose )
			{ mexPrintf(MSOUND_VERBOSE "Device closed.\n"); }

			/* Clear the stream pointer. */
			msound.pStream = (PaStream*) NULL;
		}

		/* Clear the stream flags. */
		msound.bIsOpenForReading = 0;
		msound.bIsOpenForWriting = 0;

		/* Display verbose information on the screen. */
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Calling Pa_Terminate().\n"); }

		/* Now, since no stream is opened, terminate PortAudio. */
		Pa_Terminate();

		msound.bIsPaInitialzed = 0;

		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "PortAudio terminated.\n"); }
	}

	return;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// Msound command \c openRead.                                            //
//------------------------------------------------------------------------*/
MSOUND_CPP_INLINE
void msoundOpenRead(int nlhs,       mxArray *plhs[],
                    int nrhs, const mxArray *prhs[])
{
	/* PortAudio Stream parameters for sound input. */
	PaStreamParameters inputParameters;

	/* Check number of input arguments. */
	if( nrhs > 5 )
	{
		mexErrMsgTxt( "Wrong number of input arguments." );
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	/* Check number of output arguments. */
	if( nlhs > 1 )
	{
		mexErrMsgTxt( "Wrong number of output arguments." );
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	/* Check if the device is already opened. */
	if( msound.bIsOpenForReading || msound.bIsOpenForWriting )
	{
		mexErrMsgTxt( "Device is already opened!" );
		return; /* Unnecessary, but for the sake of consistency ... */
	}


	/*----------------------------------------------------------------//
	// Setup the device id. But since id 0 should cause msound to     //
	// select the system's default device and 0 is a valid non-default//
	// device on some systems, the user input / output must have an   //
	// offset of one.                                                 //
	//----------------------------------------------------------------*/

	inputParameters.device = -1; /* Default device 0 minus offset 1. */

	/* Use specified input device. */
	if( nrhs > 0 )
	{
		/* If a device argument has been specified. */
		if( !mxIsEmpty(prhs[0]) )
		{
			/* Check argument format. */
			if( !isNonComplexDoubleScalar( prhs[0] ) )
			{
				mexErrMsgTxt("Device argument must be a non-complex"
				             " scalar double."                      );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}

			/* Get the specified device id and remove offset. */
			inputParameters.device = (int) mxGetScalar( prhs[0] ) - 1;

			/* Check device number range. */
			if( inputParameters.device >= Pa_GetDeviceCount() )
			{
				mexErrMsgTxt("Specified device number is invalid.");
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}
		}
	}

	/* Use default input device? */
	if( inputParameters.device < 0 ) /* == -1 */
	{
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Using default input device.\n"); }

		/* Retrieve the default input device's ID. */
		inputParameters.device = Pa_GetDefaultInputDevice();
	}


	/* Set default sample rate parameter. */
	msound.dSampleRate = MSOUND_DEFAULT_SAMPLE_RATE;

	/* Use specified sample rate. */
	if( nrhs > 1 )
	{
		/* Check argument format. */
		if( !isNonComplexDoubleScalar( prhs[1] ) )
		{
			mexErrMsgTxt("Sample rate argument must be a non-complex"
			             " scalar double."                           );
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Get the specified sample rate in Hertz. */
		msound.dSampleRate = mxGetScalar( prhs[1] );

		/* Check for zero or negative sample rates. */
		if( msound.dSampleRate <= 0.0 )
		{
			mexErrMsgTxt("Sample rate argument must be non-zero"
			             " and positive."                       );
			return; /* Unnecessary, but for the sake of consistency ... */
		}
	}


	/* Set default block size parameter. */
	msound.iBlockSize = MSOUND_DEFAULT_BLOCK_SIZE;

	/* Use specified input device. */
	if( nrhs > 2 )
	{
		/* Buffer for block size checking */
		double dBlockSize = 0.0;

		/* Check argument format. */
		if( !isNonComplexDoubleScalar( prhs[2] ) )
		{
			mexErrMsgTxt("Block size argument must be a non-complex"
			             " scalar double."                          );
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Get the specified block size in sample frames. */
		dBlockSize = mxGetScalar( prhs[2] );

		/* Check for zero or negative block sizes. */
		if( dBlockSize <= 0.0 )
		{
			mexErrMsgTxt("Block size argument must be non-zero"
			             " and positive scalar."               );
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Convert and setup block size parameter. */
		msound.iBlockSize = (mwSize) dBlockSize;
	}


	/* Set default channel count. */
	msound.iChannelsIn  = MSOUND_DEFAULT_CHANNELS_IN;
	msound.iChannelsOut = 0;

	/* Setup number of input / output channels. */
	if( nrhs > 3 )
	{
		/* If a device argument has been specified. */
		if( !mxIsEmpty(prhs[3]) )
		{
			/* Check argument format/type. */
			if( !isNonComplexDoubleScalar( prhs[3] ) )
			{
				mexErrMsgTxt("Channel count argument must be a"
				             " non-complex double scalar."     );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}

			/* Retrieve the number of input channels. */
			msound.iChannelsIn  = (mwSize) mxGetScalar( prhs[3] );

			/* Check channel count for invalid data. */
			if( msound.iChannelsIn <= 0 )
			{
				mexErrMsgTxt("The specified channel count is invalid.");
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}
			/* Do not check number of channels against PortAudio's
			 * device info, since the device info may be incorrect
			 * due to bad audio drivers, sometimes. */
		}
	}


	/* Setup requested number of input channels. */
	inputParameters.channelCount = (int) msound.iChannelsIn;

	/* SFi's comment: Please note that this is only the format of the
	 *                data returned by PortAudio. There's no guaranty
	 *                that it actually has been recored in that format.
	 *                Most likely your data is recored in int16 format
	 *                and converted to the specified format afterwards.
	 */
	/* Setup PortAudio to return 32 bit floating point data. */
	inputParameters.sampleFormat = paFloat32;

	/* No host specific stream info, here. */
	inputParameters.hostApiSpecificStreamInfo = NULL;


	/* Setup input device's default latency for "interactive audio". */
	inputParameters.suggestedLatency =
	Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;

	/* Use specified latency. */
	if( nrhs > 4 )
	{
		/* If a latency argument has been specified. */
		if( !mxIsEmpty(prhs[4]) )
		{
			/* Check argument format. */
			if( !isNonComplexDoubleScalar( prhs[4] ) )
			{
				mexErrMsgTxt("Latency argument must be a noncomplex"
				             " scalar double."                      );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}

			/* Get the specified latency. */
			inputParameters.suggestedLatency = (PaTime)
			                                   mxGetScalar( prhs[4] );

			/* Check latency range. */
			if( inputParameters.suggestedLatency <= (PaTime) 0.0 )
			{
				mexErrMsgTxt("Latency must be a nonzero positive"
				             " value in seconds."                );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}
		}
	}


	/* Display verbose information on the screen. */
	if( msound.iVerbose )
	{
		mexPrintf(MSOUND_VERBOSE "- Sample rate     : %.1f Hz\n"
		          MSOUND_VERBOSE "- Block size      : %d\n"
		          MSOUND_VERBOSE "- Input device ID : #%d\n"
		          MSOUND_VERBOSE "- Input channels  : %d\n"
		          MSOUND_VERBOSE "- Input latency   : %.1f ms"
		                         " (suggested)\n"               ,
		          msound.dSampleRate                            ,
		          msound.iBlockSize                             ,
		          inputParameters.device + 1                    ,
		          msound.iChannelsIn                            ,
		          inputParameters.suggestedLatency * 1000.0     );
	}
	if( msound.iVerbose > 1 )
	{ mexPrintf(MSOUND_VERBOSE "Calling Pa_OpenStream().\n"); }

	/* Open PortAudio stream for reading / recording. */
	msound.iPaError = Pa_OpenStream(
	         &msound.pStream                  , /* PortAudio stream.      */
	         &inputParameters                 , /* Input parameters.      */
	         NULL                             , /* No output.             */
	         msound.dSampleRate               , /* Sample rate.           */
	         (unsigned long) msound.iBlockSize, /* Frames per block.      */
	         paNoFlag                         , /* No flags.              */
	         NULL                             , /* Use blocking API.      */
	         NULL                             );/* No callback user data. */

	/* Display verbose information on the screen. */
	if( msound.iVerbose > 1                                ||
	  ( msound.iVerbose && (msound.iPaError != paNoError) ) )
	{
		mexPrintf(MSOUND_VERBOSE
		          "Pa_OpenStream() returned %d (%s).\n",
		                          msound.iPaError      ,
		          Pa_GetErrorText(msound.iPaError)     );
	}

		/* If the device was NOT successfully opened. */
	if( msound.iPaError != paNoError )
	{
		/* The stream pointer should be NULL anyway, but just to be
		 * safe, set the stream pointer to NULL, here. */
		msound.pStream = (PaStream*) NULL;

		mexErrMsgTxt("Device cannot be opened for reading.");
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	/* Display verbose information on the screen. */
	if( msound.iVerbose )
	{ mexPrintf(MSOUND_VERBOSE "Stream successfully opened.\n"); }
	if( msound.iVerbose > 1 )
	{ mexPrintf(MSOUND_VERBOSE "Calling Pa_StartStream().\n"); }

	/* Start the PortAudio stream. */
	msound.iPaError = Pa_StartStream( msound.pStream );

	/* Display verbose information on the screen. */
	if( msound.iVerbose > 1                                ||
	  ( msound.iVerbose && (msound.iPaError != paNoError) ) )
	{
		mexPrintf(MSOUND_VERBOSE
		          "Pa_StartStream() returned %d (%s).\n",
		                          msound.iPaError       ,
		          Pa_GetErrorText(msound.iPaError)      );
	}

	/* If the PortAudio stream was NOT successfully started. */
	if( msound.iPaError != paNoError )
	{
		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Error starting stream.\n"); }
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Calling Pa_CloseStream().\n"); }

		/* Close the PortAudio stream again. */
		Pa_CloseStream( msound.pStream );

		/* Set stream pointer to NULL again. */
		msound.pStream = (PaStream*) NULL;

		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Stream closed.\n"); }

		mexErrMsgTxt("Device cannot be opened for reading.");
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	/* Display verbose information on the screen. */
	if( msound.iVerbose )
	{ mexPrintf(MSOUND_VERBOSE "Stream successfully started.\n"); }

	/* mark stream as opened. */
	msound.bIsOpenForReading = 1;

	/* If everything is fine retrieve some stream parameters. */
	if( msound.iPaError != paNoError )
	{
		const PaStreamInfo* streamInfo = Pa_GetStreamInfo( msound.pStream );
		msound.dSampleRate = (double) streamInfo->sampleRate;
		msound.dLatencyIn  = (double) streamInfo->inputLatency;
		msound.dLatencyOut = (double) streamInfo->outputLatency;
	}

	/* If a return value is requested, return the estimated latency. */
	if( nlhs >= 1 )
	{
		plhs[0] = mxCreateDoubleScalar( msound.dLatencyIn );
	}

	return;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// Msound command openWrite.                                              //
//------------------------------------------------------------------------*/
MSOUND_CPP_INLINE
void msoundOpenWrite(int nlhs,       mxArray *plhs[],
                     int nrhs, const mxArray *prhs[])
{
	/* PortAudio Stream parameters for sound output. */
	PaStreamParameters outputParameters;

	/* Check number of input arguments. */
	if( nrhs > 5 )
	{
		mexErrMsgTxt( "Wrong number of input arguments." );
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	/* Check number of output arguments. */
	if( nlhs > 1 )
	{
		mexErrMsgTxt( "Wrong number of output arguments." );
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	/* Check if the device is already opened. */
	if( msound.bIsOpenForReading || msound.bIsOpenForWriting )
	{
		mexErrMsgTxt( "Device is already opened!" );
		return; /* Unnecessary, but for the sake of consistency ... */
	}


	/*--------------------------------------------------------------------//
	// Setup the device id. But since id 0 should cause msound to         //
	// select the system's default device and 0 is a valid non-default    //
	// device on some systems, the user input / output must have an       //
	// offset of one.                                                     //
	//--------------------------------------------------------------------*/

	outputParameters.device = -1; /* Default device 0 minus offset 1. */

	/* Use specified input device. */
	if( nrhs > 0 )
	{
		/* If a device argument has been specified. */
		if( !mxIsEmpty(prhs[0]) )
		{
			/* Check argument format. */
			if( !isNonComplexDoubleScalar( prhs[0] ) )
			{
				mexErrMsgTxt("Device argument must be a noncomplex"
				             " scalar double."                     );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}

			/* Get the specified device id and remove offset. */
			outputParameters.device = (int) mxGetScalar( prhs[0] ) - 1;

			/* Check device number range. */
			if( outputParameters.device >= Pa_GetDeviceCount() )
			{
				mexErrMsgTxt("Specified device number is Invalid.");
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}
		}
	}

	/* Use default input device? */
	if( outputParameters.device < 0 ) /* == -1 */
	{
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Using default output device.\n"); }

		/* Retrieve the default output device's ID. */
		outputParameters.device = Pa_GetDefaultOutputDevice();
	}


	/* Set default sample rate parameter. */
	msound.dSampleRate = MSOUND_DEFAULT_SAMPLE_RATE;

	/* Use specified sample rate. */
	if( nrhs > 1 )
	{
		/* Check argument format. */
		if( !isNonComplexDoubleScalar( prhs[1] ) )
		{
			mexErrMsgTxt("Sample rate argument must be a non-complex"
			             " scalar double."                           );
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Get the specified sample rate in Hertz. */
		msound.dSampleRate = mxGetScalar( prhs[1] );

		/* Check for zero or negative sample rates. */
		if( msound.dSampleRate <= 0.0 )
		{
			mexErrMsgTxt("Sample rate argument must be non-zero"
			             " and positive."                       );
			return; /* Unnecessary, but for the sake of consistency ... */
		}
	}


	/* Set default block size parameter. */
	msound.iBlockSize = MSOUND_DEFAULT_BLOCK_SIZE;

	/* Use specified input device. */
	if( nrhs > 2 )
	{
		/* Buffer for block size checking */
		double dBlockSize = 0.0;

		/* Check argument format. */
		if( !isNonComplexDoubleScalar( prhs[2] ) )
		{
			mexErrMsgTxt("Block size argument must be a non-complex"
			             " scalar double."                          );
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Get the specified block size in sample frames. */
		dBlockSize = mxGetScalar( prhs[2] );

		/* Check for zero or negative block sizes. */
		if( dBlockSize <= 0.0 )
		{
			mexErrMsgTxt("Block size argument must be non-zero"
			             " and positive scalar."               );
			return; /* Unnecessary, but for the sake of consistency ... */
		}

		/* Convert and setup block size parameter. */
		msound.iBlockSize = (mwSize) dBlockSize;
	}


	/* Set default channel count. */
	msound.iChannelsIn  = 0;
	msound.iChannelsOut = MSOUND_DEFAULT_CHANNELS_OUT;

	/* Setup number of input / output channels. */
	if( nrhs > 3 )
	{
		/* If a device argument has been specified. */
		if( !mxIsEmpty(prhs[3]) )
		{
			/* Check argument format/type. */
			if( !isNonComplexDoubleScalar( prhs[3] ) )
			{
				mexErrMsgTxt("Channel count argument must be a"
				             " non-complex double scalar."     );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}

			/* Retrieve the number of output channels. */
			msound.iChannelsOut = (mwSize) mxGetScalar( prhs[3] );

			/* Check channel count for invalid data. */
			if( msound.iChannelsOut <= 0 )
			{
				mexErrMsgTxt("The specified channel count is invalid.");
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}
			/* Do not check number of channels against PortAudio's
			 * device info, since the device info may be incorrect
			 * due to bad audio drivers, sometimes. */
		}
	}


	/* Setup requested number of output channels. */
	outputParameters.channelCount = (int) msound.iChannelsOut;

	/* SFi's comment: Please note that this is only the format of the
	 *                data accepted by PortAudio. There's no guaranty
	 *                that it actually will be processed in that format.
	 *                Most likely your data will be converted to int16
	 *                format before it is processed, e.g. played.
	 */
	/* Setup PortAudio to accept 32 bit floating point data. */
	outputParameters.sampleFormat = paFloat32;

	/* No host specific stream info, here. */
	outputParameters.hostApiSpecificStreamInfo = NULL;


	/* Setup output device's default latency for "interactive audio". */
	outputParameters.suggestedLatency =
	Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;

	/* Use specified latency. */
	if( nrhs > 4 )
	{
		/* If a latency argument has been specified. */
		if( !mxIsEmpty(prhs[4]) )
		{
			/* Check argument format. */
			if( !isNonComplexDoubleScalar( prhs[4] ) )
			{
				mexErrMsgTxt("Latency argument must be a non-complex"
				             " scalar double."                       );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}

			/* Get the specified latency. */
			outputParameters.suggestedLatency = (PaTime)
			                                    mxGetScalar( prhs[4] );

			/* Check latency range. */
			if( outputParameters.suggestedLatency <= (PaTime) 0.0 )
			{
				mexErrMsgTxt("Latency must be a nonzero positive"
				             " value in seconds."                );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}
		}
	}


	/* Display verbose information on the screen. */
	if( msound.iVerbose )
	{
		mexPrintf(MSOUND_VERBOSE "- Sample rate      : %.1f Hz\n"
		          MSOUND_VERBOSE "- Block size       : %d\n"
		          MSOUND_VERBOSE "- Output device ID : #%d\n"
		          MSOUND_VERBOSE "- Output channels  : %d\n"
		          MSOUND_VERBOSE "- Output latency   : %.1f ms"
		                         " (suggested)\n"                ,
		          msound.dSampleRate                             ,
		          msound.iBlockSize                              ,
		          outputParameters.device + 1                    ,
		          msound.iChannelsOut                            ,
		          outputParameters.suggestedLatency * 1000.0     );
	}
	if( msound.iVerbose > 1 )
	{ mexPrintf(MSOUND_VERBOSE "Calling Pa_OpenStream().\n"); }

	/* Open PortAudio stream for writing / play-back. */
	msound.iPaError = Pa_OpenStream(
		    &msound.pStream                  , /* PortAudio stream.      */
		    NULL                             , /* No input.              */
		    &outputParameters                , /* Output parameters.     */
		    msound.dSampleRate               , /* Sample rate.           */
		    (unsigned long) msound.iBlockSize, /* Frames per block.      */
		    paNoFlag                         , /* No flags.              */
		    NULL                             , /* Use blocking API.      */
		    NULL                             );/* No callback user data. */

	/* Display verbose information on the screen. */
	if( msound.iVerbose > 1                                ||
	  ( msound.iVerbose && (msound.iPaError != paNoError) ) )
	{
		mexPrintf(MSOUND_VERBOSE
		          "Pa_OpenStream() returned %d (%s).\n",
		                          msound.iPaError      ,
		          Pa_GetErrorText(msound.iPaError)     );
	}

	/* If the device was NOT successfully opened. */
	if( msound.iPaError != paNoError )
	{
		/* The stream pointer should be NULL anyway, but just to be
		 * safe, set the stream pointer to NULL, here. */
		msound.pStream = (PaStream*) NULL;

		mexErrMsgTxt("Device cannot be opened for writing.");
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	/* Display verbose information on the screen. */
	if( msound.iVerbose )
	{ mexPrintf(MSOUND_VERBOSE "Stream successfully opened.\n"); }
	if( msound.iVerbose > 1 )
	{ mexPrintf(MSOUND_VERBOSE "Calling Pa_StartStream().\n"); }

	/* Start the PortAudio stream. */
	msound.iPaError = Pa_StartStream( msound.pStream );

	/* Display verbose information on the screen. */
	if( msound.iVerbose > 1                                ||
	  ( msound.iVerbose && (msound.iPaError != paNoError) ) )
	{
		mexPrintf(MSOUND_VERBOSE
		          "Pa_StartStream() returned %d (%s).\n",
		                          msound.iPaError       ,
		          Pa_GetErrorText(msound.iPaError)      );
	}

	/* If the PortAudio stream was NOT successfully started. */
	if( msound.iPaError != paNoError )
	{
		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Error starting stream.\n"); }
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Calling Pa_CloseStream().\n"); }

		/* Close the PortAudio stream again. */
		Pa_CloseStream( msound.pStream );

		/* Set stream pointer to NULL again. */
		msound.pStream = (PaStream*) NULL;

		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Stream closed.\n"); }

		mexErrMsgTxt("Device cannot be opened for writing.");
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	/* Display verbose information on the screen. */
	if( msound.iVerbose )
	{ mexPrintf(MSOUND_VERBOSE "Stream successfully started.\n"); }

	/* mark stream as opened. */
	msound.bIsOpenForWriting = 1;

	/* If everything is fine retrieve some stream parameters. */
	if( msound.iPaError != paNoError )
	{
		const PaStreamInfo* streamInfo = Pa_GetStreamInfo( msound.pStream );
		msound.dSampleRate = (double) streamInfo->sampleRate;
		msound.dLatencyIn  = (double) streamInfo->inputLatency;
		msound.dLatencyOut = (double) streamInfo->outputLatency;
	}

	/* If a return value is requested, return the estimated latency. */
	if( nlhs >= 1 )
	{
		plhs[0] = mxCreateDoubleScalar( msound.dLatencyOut );
	}

	return;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// Msound command openRW.                                                 //
//------------------------------------------------------------------------*/
MSOUND_CPP_INLINE
void msoundOpenReadWrite(int nlhs,       mxArray *plhs[],
                         int nrhs, const mxArray *prhs[])
{
	/* PortAudio Stream parameters for sound input/output. */
	PaStreamParameters inputParameters;
	PaStreamParameters outputParameters;

	/* Check number of input arguments. */
	if( nrhs > 5 )
	{
		mexErrMsgTxt( "Wrong number of input arguments." );
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	/* Check number of output arguments. */
	if( nlhs > 1 )
	{
		mexErrMsgTxt( "Wrong number of output arguments." );
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	/* Check if the device is already opened. */
	if( msound.bIsOpenForReading || msound.bIsOpenForWriting )
	{
		mexErrMsgTxt( "Device is already opened!" );
		return; /* Unnecessary, but for the sake of consistency ... */
	}


	/*--------------------------------------------------------------------//
	// Setup the device id. But since id 0 should cause msound to         //
	// select the system's default device and 0 is a valid non-default    //
	// device on some systems, the user input / output must have an       //
	// offset of one.                                                     //
	//--------------------------------------------------------------------*/

	inputParameters.device  = -1; /* Default device 0 minus offset 1. */
	outputParameters.device = -1; /* Default device 0 minus offset 1. */

	/* Select input / output devices. */
	if( nrhs > 0 )
	{
		/* If a device argument has been specified. */
		if( !mxIsEmpty(prhs[0]) )
		{
			/* Pointer to a matlab data buffer. */
			const double *pdMatlabData = NULL;

			/* Check argument format/type. */
			if( !isNonComplexDoubleVector(prhs[0]) ||
			    (mxGetNumberOfElements(prhs[0]) > 2) )
			{
				mexErrMsgTxt("Device ID argument must be a non-complex"
				             " two element double vector, input"
				             " device number followed by output"
				             " device number."                        );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}

			/* If two device IDs have been provided. */
			if( mxGetNumberOfElements(prhs[0]) == 2 )
			{
				/* Get a pointer to access the matlab array. */
				const double *pdMatlabData = mxGetPr( prhs[0] );

				/* Get the specified input and output device IDs and
				 * remove offset. */
				inputParameters.device  = (int) pdMatlabData[0] - 1;
				outputParameters.device = (int) pdMatlabData[1] - 1;
			}
			/* If only one device ID has been provided. */
			else
			{
				/* Use a single device ID for both, input and
				 * output. */
				inputParameters.device  =
				outputParameters.device = (int) mxGetScalar(prhs[0])-1;
			}

			/* Check device number range. */
			if( (inputParameters.device  >= Pa_GetDeviceCount()) ||
			    (outputParameters.device >= Pa_GetDeviceCount()) )
			{
				mexErrMsgTxt("Specified device number(s)"
				             " is/are invalid."          );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}
		}
	} /* if( nrhs > 0 ) */

	/* Use default input device? */
	if( inputParameters.device < 0 ) /* == -1 */
	{
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Using default input device.\n"); }

		inputParameters.device = Pa_GetDefaultInputDevice();
	}

	/* Use default input device? */
	if( outputParameters.device < 0 ) /* == -1 */
	{
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Using default output device.\n"); }

		outputParameters.device = Pa_GetDefaultOutputDevice();
	}


	/* Set default sample rate parameter. */
	msound.dSampleRate = MSOUND_DEFAULT_SAMPLE_RATE;

	/* Use specified sample rate. */
	if( nrhs > 1 )
	{
		/* Check argument format. */
		if( !isNonComplexDoubleScalar( prhs[1] ) )
		{
			mexErrMsgTxt("Sample rate argument must be a non-complex"
			             " scalar double."                           );
			/* Unnecessary, but for the sake of consistency ... */
			return;
		}

		/* Get the specified sample rate in Hertz. */
		msound.dSampleRate = mxGetScalar( prhs[1] );

		/* Check for zero or negative sample rates. */
		if( msound.dSampleRate <= 0.0 )
		{
			mexErrMsgTxt("Sample rate argument must be non-zero"
			             " and positive."                       );
			/* Unnecessary, but for the sake of consistency ... */
			return;
		}
	}


	/* Set default block size parameter. */
	msound.iBlockSize = MSOUND_DEFAULT_BLOCK_SIZE;

	/* Use specified input device. */
	if( nrhs > 2 )
	{
		/* Buffer for block size checking */
		double dBlockSize = 0.0;

		/* Check argument format. */
		if( !isNonComplexDoubleScalar( prhs[2] ) )
		{
			mexErrMsgTxt("Block size argument must be a non-complex"
			             " scalar double."                          );
			/* Unnecessary, but for the sake of consistency ... */
			return;
		}

		/* Get the specified block size in sample frames. */
		dBlockSize = mxGetScalar( prhs[2] );

		/* Check for zero or negative block sizes. */
		if( dBlockSize <= 0.0 )
		{
			mexErrMsgTxt("Block size argument must be non-zero"
			             " and positive scalar."               );
			/* Unnecessary, but for the sake of consistency ... */
			return;
		}

		/* Convert and setup block size parameter. */
		msound.iBlockSize = (mwSize) dBlockSize;
	}


	/* Set default channel count for input and output. */
	msound.iChannelsIn  = MSOUND_DEFAULT_CHANNELS_IN;
	msound.iChannelsOut = MSOUND_DEFAULT_CHANNELS_OUT;

	/* Setup number of input / output channels. */
	if( nrhs > 3 )
	{
		/* If a device argument has been specified. */
		if( !mxIsEmpty(prhs[3]) )
		{
			/* Check argument format / type. */
			if( !isNonComplexDoubleVector(prhs[3]) ||
			    (mxGetNumberOfElements(prhs[3]) > 2) )
			{
				mexErrMsgTxt("Channel count argument must be a"
				             " non-complex double scalar or two"
				             " element double vector, input"
				             " channel count followed by output"
				             " channel count."                  );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}

			/* If two channel parameters have been provided. */
			if( mxGetNumberOfElements(prhs[3]) == 2 )
			{
				/* Get a pointer to access the matlab array. */
				const double *pdMatlabData = mxGetPr( prhs[3] );

				/* Get the separate channel count parameters for
				 * input and output device. */
				msound.iChannelsIn  = (mwSize) pdMatlabData[0];
				msound.iChannelsOut = (mwSize) pdMatlabData[1];
			}
			/* If a single channel count parameter has been provided,
			 * it is to be used for both devices. */
			else
			{
				/* Use the same number of channels for both, input
				 * and output device. */
				msound.iChannelsIn  =
				msound.iChannelsOut = (mwSize) mxGetScalar( prhs[3] );
			}

			/* Check channel count for invalid data. */
			if( (msound.iChannelsIn <= 0) || (msound.iChannelsOut <= 0) )
			{
				mexErrMsgTxt("Specified channel count(s) is/are"
				             " invalid."                        );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}
			/* Do not check number of channels against PortAudio's
			 * device info, since the device info may be incorrect
			 * due to bad audio drivers, sometimes. */
		}
	} /* if( nrhs > 3 ) */

	/* Setup requested number of input and output channels. */
	inputParameters.channelCount  = (int) msound.iChannelsIn;
	outputParameters.channelCount = (int) msound.iChannelsOut;


	/* SFi's comment: Please note that this is only the format of the
	 *                data returned and accepted by PortAudio. There's
	 *                no guaranty that it actually has been recorded
	 *                nor that is to be processed (played) in that
	 *                format.
	 *                Most likely your data is recored and played in
	 *                int16 format and converted to the specified format
	 *                in-between.
	 */
	/* Setup PortAudio to accept and return 32 bit floating point data. */
	inputParameters.sampleFormat  = paFloat32;
	outputParameters.sampleFormat = paFloat32;

	/* No host specific stream info, here. */
	inputParameters.hostApiSpecificStreamInfo  = NULL;
	outputParameters.hostApiSpecificStreamInfo = NULL;


	/* Setup input device's default latency for "interactive audio". */
	inputParameters.suggestedLatency =
	Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
	outputParameters.suggestedLatency =
	Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;

	/* Use specified latency. */
	if( nrhs > 4 )
		{
		/* If a latency argument has been specified. */
		if( !mxIsEmpty(prhs[4]) )
		{
			/* Check argument format / type. */
			if( !isNonComplexDoubleVector(prhs[4]) ||
			    (mxGetNumberOfElements(prhs[4]) > 2) )
			{
				mexErrMsgTxt("Latency argument must be either a"
				             " non-complex double scalar containing a"
				             " common latency setting for input and"
				             " output, or a non-complex two element"
				             " double vector, input device latency"
				             " followed by output device latency."   );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}

			/* If two latency parameters have been provided. */
			if( mxGetNumberOfElements(prhs[4]) == 2 )
			{
				/* Get a pointer to access the matlab array. */
				const double *pdMatlabData = mxGetPr( prhs[4] );

				/* Get the specified latency. */
				inputParameters.suggestedLatency  = (PaTime)
				                                    pdMatlabData[0];
				outputParameters.suggestedLatency = (PaTime)
				                                    pdMatlabData[1];
			}
			/* If a single latency parameter has been provided, it is
			 * to be used for both devices. */
			else
			{
				/* Get the specified latency. */
				inputParameters.suggestedLatency  =
				outputParameters.suggestedLatency =
					(PaTime) mxGetScalar( prhs[4] );
			}

			/* Check latency range. */
			if( ( inputParameters.suggestedLatency <= (PaTime) 0.0) ||
			    (outputParameters.suggestedLatency <= (PaTime) 0.0) )
			{
				mexErrMsgTxt("Latency must be a nonzero positive"
				             " value in seconds."                );
				/* Unnecessary, but for the sake of consistency ... */
				return;
			}
		}
	}


	/* Display verbose information on the screen. */
	if( msound.iVerbose )
	{
		mexPrintf(MSOUND_VERBOSE "- Sample rate      : %.1f Hz\n"
		          MSOUND_VERBOSE "- Block size       : %d\n"
		          MSOUND_VERBOSE "- Input device ID  : #%d\n"
		          MSOUND_VERBOSE "- Input channels   : %d\n"
		          MSOUND_VERBOSE "- Input latency    : %.1f ms"
		                         " (suggested)\n"
		          MSOUND_VERBOSE "- Output device ID : #%d\n"
		          MSOUND_VERBOSE "- Output channels  : %d\n"
		          MSOUND_VERBOSE "- Output latency   : %.1f ms"
		                         " (suggested)\n"                ,
		          msound.dSampleRate                             ,
		          msound.iBlockSize                              ,
		          inputParameters.device + 1                     ,
		          msound.iChannelsIn                             ,
		          inputParameters.suggestedLatency * 1000.0      ,
		          outputParameters.device + 1                    ,
		          msound.iChannelsOut                            ,
		          outputParameters.suggestedLatency * 1000.0     );
	}
	if( msound.iVerbose > 1 )
	{ mexPrintf(MSOUND_VERBOSE "Calling Pa_OpenStream().\n"); }

	/* Open PortAudio stream for duplex reading and writing. */
	msound.iPaError = Pa_OpenStream(
	        &msound.pStream                  , /* PortAudio stream.      */
	        &inputParameters                 , /* Input parameters.      */
	        &outputParameters                , /* Output parameters.     */
	        msound.dSampleRate               , /* Sample rate.           */
	        (unsigned long) msound.iBlockSize, /* Frames per block.      */
	        paNoFlag                         , /* No flags.              */
	        NULL                             , /* Use blocking API.      */
	        NULL                             );/* No callback user data. */

	/* Display verbose information on the screen. */
	if( msound.iVerbose > 1                                ||
	  ( msound.iVerbose && (msound.iPaError != paNoError) ) )
	{
		mexPrintf(MSOUND_VERBOSE
		          "Pa_OpenStream() returned %d (%s).\n",
		                          msound.iPaError      ,
		          Pa_GetErrorText(msound.iPaError)     );
	}

	/* If the device was NOT successfully opened. */
	if( msound.iPaError != paNoError )
	{
		/* The stream pointer should be NULL anyway, but just to be
			* safe, set the stream pointer to NULL, here. */
		msound.pStream = (PaStream*) NULL;

		mexErrMsgTxt("Device(s) cannot be opened for duplex reading"
		             " and writing."                                );
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	/* Display verbose information on the screen. */
	if( msound.iVerbose )
	{ mexPrintf(MSOUND_VERBOSE "Stream successfully opened.\n"); }
	if( msound.iVerbose > 1 )
	{ mexPrintf(MSOUND_VERBOSE "Calling Pa_StartStream().\n"); }

	/* Start the PortAudio stream. */
	msound.iPaError = Pa_StartStream( msound.pStream );

	/* Display verbose information on the screen. */
	if( msound.iVerbose > 1                                ||
	  ( msound.iVerbose && (msound.iPaError != paNoError) ) )
	{
		mexPrintf(MSOUND_VERBOSE
		          "Pa_StartStream() returned %d (%s).\n",
		                          msound.iPaError       ,
		          Pa_GetErrorText(msound.iPaError)      );
	}

	/* If the PortAudio stream was NOT successfully started. */
	if( msound.iPaError != paNoError )
	{
		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Error starting stream.\n"); }
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Calling Pa_CloseStream().\n"); }

		/* Close the PortAudio stream again. */
		Pa_CloseStream( msound.pStream );

		/* Set stream pointer to NULL again. */
		msound.pStream = (PaStream*) NULL;

		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Stream closed.\n"); }

		mexErrMsgTxt("Device(s) cannot be opened for duplex reading"
		             "and writing."                                 );
		return; /* Unnecessary, but for the sake of consistency ... */
	}

	if( msound.iVerbose )
	{ mexPrintf(MSOUND_VERBOSE "Stream successfully started.\n"); }

	/* mark stream as opened. */
	msound.bIsOpenForReading = 1;
	msound.bIsOpenForWriting = 1;

	/* If everything is fine retrieve some stream parameters. */
	if( msound.iPaError != paNoError )
	{
		const PaStreamInfo* streamInfo = Pa_GetStreamInfo( msound.pStream );
		msound.dSampleRate = (double) streamInfo->sampleRate;
		msound.dLatencyIn  = (double) streamInfo->inputLatency;
		msound.dLatencyOut = (double) streamInfo->outputLatency;
	}

	/* If a return value is requested, return the estimated latency. */
	if( nlhs >= 1 )
	{
		plhs[0] = mxCreateDoubleMatrix( 1, 2, mxREAL);
		mxGetPr( plhs[0] )[0] = msound.dLatencyIn ;
		mxGetPr( plhs[0] )[1] = msound.dLatencyOut;
	}

	return;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// Msound command stop.                                                   //
//------------------------------------------------------------------------*/
MSOUND_CPP_INLINE
void msoundStop(int nlhs,       mxArray *plhs[],
                int nrhs, const mxArray *prhs[])
{
	/* Check number of input arguments. */
	if( nrhs != 0 )
	{
		mexErrMsgTxt( "Wrong number of input arguments." );
		return; /* Unnecessary, but for consistency ... */
	}

	/* Check number of output arguments. */
	if( nlhs != 0 )
	{
		mexErrMsgTxt( "Wrong number of output arguments." );
		return; /* Unnecessary, but for consistency ... */
	}


	/* If no stream is opened, do nothing. */
	if( !msound.pStream )
	{
		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Device already closed.\n"); }
	}
	/* If a stream is opened, close it. */
	else
	{
		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Stopping PortAudio stream.\n"); }
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Calling Pa_StopStream().\n"); }

		/* Stop play-back and stop the stream. */
		Pa_StopStream( msound.pStream );

		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Closing PortAudio stream.\n"); }
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Calling Pa_CloseStream().\n"); }

		/* Close the stopped stream. */
		Pa_CloseStream( msound.pStream );

		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Device closed.\n"); }
	}

	/* Clear the stream pointer / flags. */
	msound.iPaError          = paNoError;
	msound.pStream           = (PaStream*) NULL;
	msound.bIsOpenForReading = 0;
	msound.bIsOpenForWriting = 0;
	/* Clear global parameters. */
	msound.dSampleRate       = 0.0;
	msound.dLatencyIn        = 0.0;
	msound.dLatencyOut       = 0.0;
	msound.iChannelsIn       = (mwSize) 0;
	msound.iChannelsOut      = (mwSize) 0;
	msound.iBlockSize        = (mwSize) 0;

	/* Reset default parameters? */
	/* These parameters are supposed to be available for verbose output,
	 * so they MUST NOT be cleared!
	msound.dSampleRate  = MSOUND_DEFAULT_SAMPLE_RATE;
	msound.iChannelsIn  = MSOUND_DEFAULT_CHANNELS_IN;
	msound.iChannelsOut = MSOUND_DEFAULT_CHANNELS_OUT;
	msound.iBlockSize   = MSOUND_DEFAULT_BLOCK_SIZE;
	*/

	/* Display verbose information on the screen. */
	if( msound.iVerbose > 1 )
	{ mexPrintf(MSOUND_VERBOSE "Calling Pa_Terminate().\n"); }

	/* Now, since no stream is opened, terminate PortAudio. */
	Pa_Terminate();

	msound.bIsPaInitialzed = 0;

	/* Display verbose information on the screen. */
	if( msound.iVerbose )
	{ mexPrintf(MSOUND_VERBOSE "PortAudio terminated.\n"); }

	return;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// Msound command close.                                                  //
//------------------------------------------------------------------------*/
MSOUND_CPP_INLINE
void msoundClose(int nlhs,       mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
	/* Check number of input arguments. */
	if( nrhs != 0 )
	{
		mexErrMsgTxt( "Wrong number of input arguments." );
		return; /* Unnecessary, but for consistency ... */
	}

	/* Check number of output arguments. */
	if( nlhs != 0 )
	{
		mexErrMsgTxt( "Wrong number of output arguments." );
		return; /* Unnecessary, but for consistency ... */
	}


	/* If no stream is opened, do nothing. */
	if( !msound.pStream )
	{
		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Device already closed.\n"); }
	}
	/* If a stream is opened, close it. */
	else
	{
		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Aborting PortAudio stream.\n"); }
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Calling Pa_AbortStream().\n"); }

		/* Immediately abort play-back and stop the stream. */
		Pa_AbortStream( msound.pStream );

		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Closing PortAudio stream.\n"); }
		if( msound.iVerbose > 1 )
		{ mexPrintf(MSOUND_VERBOSE "Calling Pa_CloseStream().\n"); }

		/* Close the stopped stream. */
		Pa_CloseStream( msound.pStream );

		/* Display verbose information on the screen. */
		if( msound.iVerbose )
		{ mexPrintf(MSOUND_VERBOSE "Device closed.\n"); }
	}

	/* Clear the stream pointer / flags. */
	msound.iPaError          = paNoError;
	msound.pStream           = (PaStream*) NULL;
	msound.bIsOpenForReading = 0;
	msound.bIsOpenForWriting = 0;
	/* Clear global parameters. */
	msound.dSampleRate       = 0.0;
	msound.dLatencyIn        = 0.0;
	msound.dLatencyOut       = 0.0;
	msound.iChannelsIn       = (mwSize) 0;
	msound.iChannelsOut      = (mwSize) 0;
	msound.iBlockSize        = (mwSize) 0;

	/* Reset default parameters? */
	/* These parameters are supposed to be available for verbose output,
	 * so they MUST NOT be cleared!
	msound.dSampleRate  = MSOUND_DEFAULT_SAMPLE_RATE;
	msound.iChannelsIn  = MSOUND_DEFAULT_CHANNELS_IN;
	msound.iChannelsOut = MSOUND_DEFAULT_CHANNELS_OUT;
	msound.iBlockSize   = MSOUND_DEFAULT_BLOCK_SIZE;
	*/

	/* Display verbose information on the screen. */
	if( msound.iVerbose > 1 )
	{ mexPrintf(MSOUND_VERBOSE "Calling Pa_Terminate().\n"); }

	/* Now, since no stream is opened, terminate PortAudio. */
	Pa_Terminate();

	msound.bIsPaInitialzed = 0;

	/* Display verbose information on the screen. */
	if( msound.iVerbose )
	{ mexPrintf(MSOUND_VERBOSE "PortAudio terminated.\n"); }

	return;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// Msound command deviceInfo.                                             //
//------------------------------------------------------------------------*/
MSOUND_CPP_INLINE
void msoundDeviceInfo(int nlhs,       mxArray *plhs[],
                      int nrhs, const mxArray *prhs[])
{
	/* Pointer for the struct array to be returned. */
	mxArray *pStructArray = NULL;

	/* Device ID range to be processed (initialized to all devices.)  */
	mwIndex iDevice      = 0;
	mwIndex iDeviceMax   = (mwIndex) Pa_GetDeviceCount();
	mwIndex iDeviceCount = iDeviceMax;

	/* Are we supposed to display some info? */
	int bDisplayInfo = ( nlhs == 0 ) || msound.iVerbose;


	/* Check number of input arguments. */
	if( nrhs > 1 )
	{
		mexErrMsgTxt( "Wrong number of input arguments." );
		return; /* Unnecessary, but for consistency ... */
	}

	/* Check number of output arguments. */
	if( nlhs > 2 )
	{
		mexErrMsgTxt( "Wrong number of output arguments." );
		return; /* Unnecessary, but for consistency ... */
	}


	/* If a device ID argument has been specified. */
	if( nrhs >= 1 )
	{
		/* Default devices are no valid parameters for this function, since
		 * we may not know, if they refer to an input or output device. */
		if( mxIsEmpty( prhs[0] ) )
		{
			mexErrMsgTxt("No default device available for this function.");
			return; /* Unnecessary, but for consistency ... */
		}

		/* Check input argument type. */
		if( !isNonComplexDoubleScalar( prhs[0] ) )
		{
			mexErrMsgTxt("Input argument must be a "
			             "non-complex scalar double.");
			return; /* Unnecessary, but for consistency ... */
		}

		/* Retrieve specified device number / range. */
		iDevice      = (mwIndex) mxGetScalar( prhs[0] ) - 1;
		iDeviceMax   = iDevice + 1;
		iDeviceCount = iDeviceMax - iDevice;

		/* Default devices are no valid parameters for this function, since
		 * we may not know, if they refer to an input or output device. */
		if( iDevice < 0 )
		{
			mexErrMsgTxt("No default device available for this function.");
			return; /* Unnecessary, but for consistency ... */
		}

		/* Check for valid ID range */
		if( iDevice >= (mwIndex) Pa_GetDeviceCount() )
		{
			mexErrMsgTxt("Invalid device number.");
			return; /* Unnecessary, but for consistency ... */
		}
	}


	/* Display some information on the screen. */
	if( bDisplayInfo )
	{
		mexPrintf(MSOUND_INFO "Available devices    : %d\n"
		          MSOUND_INFO "Default input device : %d\n"
		          MSOUND_INFO "Default output device: %d\n"
		          "\n"                                     ,
		          Pa_GetDeviceCount()                      ,
		          (int) Pa_GetDefaultInputDevice()  + 1    ,
		          (int) Pa_GetDefaultOutputDevice() + 1    );
	}

	/* If the struct array is to be returned, create it. */
	if( nlhs >= 1 )
	{
		/* Create struct array to be returned, or return an error in
		 * case of failure. */
		if(!(pStructArray = mxCreateStructMatrix(iDeviceCount,1,0,NULL)) ||
		    (-1 == mxAddField( pStructArray, "name"        ) ) ||
		    (-1 == mxAddField( pStructArray, "api"         ) ) ||
		    (-1 == mxAddField( pStructArray, "id"          ) ) ||
		    (-1 == mxAddField( pStructArray, "inputs"      ) ) ||
		    (-1 == mxAddField( pStructArray, "outputs"     ) ) ||
		    (-1 == mxAddField( pStructArray, "samplerate"  ) ) ||
		    (-1 == mxAddField( pStructArray, "samplerates" ) ) )
		{
			mexErrMsgTxt("Error creating internal buffer.");
			return; /* Unnecessary, but for consistency ... */
		}

		/* Setup the structure array to be used as return argument. */
		plhs[0] = pStructArray;
	}


	/* Process all requested devices. */
	for( iDeviceCount=0; iDevice<iDeviceMax; ++iDevice )
	{
		int j = 0; /* Just a counter. */

		/* Pointer to the device's info structure. */
		const PaDeviceInfo *pDeviceInfo =
			Pa_GetDeviceInfo( (PaDeviceIndex) iDevice );

		/* Data structures and pointers to handle device input and
		 * output properties. */
		PaStreamParameters deviceParametersI;
		PaStreamParameters deviceParametersO;
		PaStreamParameters *pDeviceParametersI = NULL;
		PaStreamParameters *pDeviceParametersO = NULL;

		/* An array whose elements yield true for default sample rates,
		 * that are supported by the current device. */
		int bDefaultSampleRateSupported[ MSOUND_DEFAULT_SAMPLE_RATE_COUNT ];

		/* The total number of supported default sample rates. */
		int iSampleRateCount = 0;


		/* Abort if the returned pointer was invalid. */
		if( !pDeviceInfo )
		{
			mexErrMsgTxt("Error retrieving device info structure.");
			return; /* Unnecessary, but for consistency ... */
		}

		/* Setup parameter structure for input, if supported.*/
		if( pDeviceInfo->maxInputChannels )
		{
			pDeviceParametersI = &deviceParametersI;
			pDeviceParametersI->channelCount=pDeviceInfo->maxInputChannels;
			pDeviceParametersI->device = (PaDeviceIndex) iDevice;
			pDeviceParametersI->hostApiSpecificStreamInfo = NULL;
			pDeviceParametersI->sampleFormat = paFloat32;
			pDeviceParametersI->suggestedLatency =
			                            pDeviceInfo->defaultLowInputLatency;
		}

		/* Setup parameter structure for output, if supported.*/
		if( pDeviceInfo->maxOutputChannels )
		{
			pDeviceParametersO = &deviceParametersO;
			pDeviceParametersO->channelCount=pDeviceInfo->maxOutputChannels;
			pDeviceParametersO->device = (PaDeviceIndex) iDevice;
			pDeviceParametersO->hostApiSpecificStreamInfo = NULL;
			pDeviceParametersO->sampleFormat = paFloat32;
			pDeviceParametersO->suggestedLatency =
			                           pDeviceInfo->defaultLowOutputLatency;
		}

		/* Check if the current device supports default sample rates. */
		for(j=0,iSampleRateCount=0; j<MSOUND_DEFAULT_SAMPLE_RATE_COUNT; ++j)
		{
			/* Set "supported" flag. */
			bDefaultSampleRateSupported[j] = (
				paNoError == Pa_IsFormatSupported(pDeviceParametersI     ,
				                                  pDeviceParametersO     ,
				                                  pdDefaultSampleRates[j]));
			/* Count supported sample rates, only. */
			if( bDefaultSampleRateSupported[j] ) { ++iSampleRateCount; }
		}

		if( pStructArray )
		{
			/* Create current device's structure fields. */
			mxArray *pName = mxCreateString(pDeviceInfo->name);
			mxArray *pApi  = mxCreateString      (
			               Pa_GetHostApiInfo(pDeviceInfo->hostApi)->name  );
            mxArray *pId   = mxCreateDoubleScalar(iDevice+1   );
            mxArray *pIn   = mxCreateDoubleScalar(pDeviceInfo->maxInputChannels );
			mxArray *pOut  = mxCreateDoubleScalar(pDeviceInfo->maxOutputChannels);
			mxArray *pFs   = mxCreateDoubleScalar(pDeviceInfo->defaultSampleRate);
            mxArray *pFs2  = mxCreateDoubleMatrix(1               ,
			                                      iSampleRateCount,
			                                      mxREAL          );
			double *pdFs2  = NULL;

			if( !pName || !pApi || !pId || !pIn || !pOut || !pFs || !pFs2 )
			{
				mexErrMsgTxt("Error creating internal buffer.");
				return; /* Unnecessary, but for consistency ... */
			}

			/* Access the default sample rate matrix. */
			pdFs2 = mxGetPr( pFs2 );

			/* Copy available sample rates into the output matrix. */
			for(j=0; j<MSOUND_DEFAULT_SAMPLE_RATE_COUNT; ++j)
			{
				if( bDefaultSampleRateSupported[j] )
				{
					*(pdFs2++) = pdDefaultSampleRates[j];
				}
			}

			/* Assign device properties to the info struct array. */
			mxSetField( pStructArray, iDeviceCount, "name"        , pName );
			mxSetField( pStructArray, iDeviceCount, "api"         , pApi  );
			mxSetField( pStructArray, iDeviceCount, "id"          , pId   );
			mxSetField( pStructArray, iDeviceCount, "inputs"      , pIn   );
			mxSetField( pStructArray, iDeviceCount, "outputs"     , pOut  );
			mxSetField( pStructArray, iDeviceCount, "samplerate"  , pFs   );
			mxSetField( pStructArray, iDeviceCount, "samplerates" , pFs2  );
			++iDeviceCount;
		}

		if( bDisplayInfo )
		{
			mexPrintf(MSOUND_INFO "=== Device #%d Information ===\n"
			          MSOUND_INFO "Device name         : '%s'\n"
			          MSOUND_INFO "Device API          : '%s'\n"
			          MSOUND_INFO "Max input channels  : %d\n"
			          MSOUND_INFO "Max output channels : %d\n"
			          MSOUND_INFO "Default sample rate : %.0f Hz\n"
			          MSOUND_INFO "Other sample rates  :\n    "    ,
			          (int)iDevice+1                               ,
			          pDeviceInfo->name                            ,
			          Pa_GetHostApiInfo(pDeviceInfo->hostApi)->name,
			          pDeviceInfo->maxInputChannels                ,
			          pDeviceInfo->maxOutputChannels               ,
			          pDeviceInfo->defaultSampleRate               );

			for(j=0; j<MSOUND_DEFAULT_SAMPLE_RATE_COUNT; ++j)
			{
				if( bDefaultSampleRateSupported[j] )
				{
					mexPrintf("  %6.0f", pdDefaultSampleRates[j]);
				}
			}
			mexPrintf(" Hz\n\n");
		}
	}

	/* If a second output argument is requested, return default device
	 * IDs, too. */
	if( nlhs >= 2 ) {
		plhs[1] = mxCreateDoubleMatrix( 1, 2, mxREAL );
		if( plhs[1] )
		{
			mxGetPr(plhs[1])[0] = (double)(Pa_GetDefaultInputDevice()  + 1);
			mxGetPr(plhs[1])[1] = (double)(Pa_GetDefaultOutputDevice() + 1);
		}
	}

	return;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// Display the msound usage in matlabs command window.                    //
//------------------------------------------------------------------------*/
MSOUND_CPP_INLINE
void msoundUsage(void)
{
	/* Retrieve number of supported host APIs. */
	PaHostApiIndex iHostApiCount = Pa_GetHostApiCount();
	PaHostApiIndex iHostApiIndex = 0;

	/* Host API information structure pointer, used to retrieve */
	const PaHostApiInfo *pHostApiInfo = NULL;

	mexPrintf("MSOUND  block processing audio interface.\n"
	          "\n"
	          "    Usage: [...] = msound( 'command'[, ...] )\n"
	          "\n"
	          "MSOUND's first input argument is a string, specifying the command to be\n"
	          "executed. Depending on the command, this argument may be followed by\n"
	          "one or more additional input arguments. The number and type of arguments,\n"
	          "returned by MSOUND is dependent on the executed command.\n"
	          "\n");

	mexPrintf("The following command-strings and corresponding parameters are available:\n"
	          "\n");

	mexPrintf("msound( 'deviceInfo' )\n"
	          "    Displays a list of available devices and their configurations.\n"
	          "\n");
	mexPrintf("stDevices = msound( 'deviceInfo' )\n"
	          "    Returns a struct array \"stDevices\" containing detailed information for\n"
	          "    all available devices. Whereas the returned structure array contains the\n"
	          "    data fields:\n"
	          "        name        -- The name of the device.\n"
	          "        api         -- The API used by this device.\n"
	          "        id          -- The device's ID.\n"
	          "        inputs      -- The maximum number of input channels.\n"
	          "        outputs     -- The maximum number of output channels.\n"
	          "        samplerate  -- The device's default sample rate in Hertz.\n"
	          "        samplerates -- A list of supported sample rates in Hertz.\n"
	          "\n"
	          "    You may type \"unique({stDevices.api})\" to retrieve a list of used APIs,\n"
	          "    Whereas \"find(strcmpi({stDevices.api},'WMME'))\" returns the IDs of all\n"
	          "    devices using the Windows MultiMedia Extensions (WMME) API.\n"
	          "\n");
	mexPrintf("[ stDevices, defaultIDs ] = msound( 'deviceInfo' )\n"
	          "    Returns a struct array \"stDevices\" containing detailed information for\n"
	          "    all available devices, whereas the second output argument \"defaultIDs\"\n"
	          "    contains the IDs of the default input (first element) and default output\n"
	          "    (second element) device.\n"
	          "\n");
	mexPrintf("msound( 'deviceInfo', deviceID )\n"
	          "    Displays some detailed information on the device specified by its device\n"
	          "    ID \"deviceID\".\n"
	          "\n");
	mexPrintf("stDevice = msound( 'deviceInfo', deviceID )\n"
	          "    Returns a struct array \"stDevices\" containing detailed information for\n"
	          "    the device specified by its device ID \"deviceID\".\n"
	          "\n");
	mexPrintf("[ stDevice, defaultIDs ] = msound( 'deviceInfo', deviceID )\n"
	          "    Returns a data struct \"stDevice\" containing detailed information for the\n"
	          "    device specified by its device ID \"deviceID\", whereas the second output\n"
	          "    argument \"defaultIDs\" contains the IDs of the default input (first\n"
	          "    element) and default output (second element) device.\n"
	          "\n");

	mexPrintf("msound( 'openRead' )\n"
	          "    Open default input device for recording some single channel audio data\n"
	          "    using 44.1kHz sample rate and 1024 samples per block. See below for\n"
	          "    additional optional parameters.\n"
	          "\n"                                                        );
	mexPrintf("latency = msound( 'openRead', deviceID   , ...\n"
	          "                              sampleRate , ...\n"
	          "                              blockSize  , ...\n"
	          "                              numChannels, ...\n"
	          "                              latency     )\n"
	          "    Open a specific device for recording audio data of specified format. The\n"
	          "    argument \"deviceID\" contains the ID of the device to be opened whereas\n"
	          "    ID 0 or [] indicate the default input device. Arguments \"sampleRate\",\n"
	          "    \"blockSize\", and \"numChannels\" represent the desired audio sample rate\n"
	          "    in Hertz, the preferred number of sample frames per data block, and the\n"
	          "    requested number of signal channels. If omitted or set to [], parameters\n"
	          "    are set to their respective default values (44.1kHz, 1024 sample frames\n"
	          "    per data block, and 1 signal channel.)\n"
	          "    The \"latency\" input argument may be used to suggest a specific audio\n"
	          "    processing latency in seconds. If omitted or empty, MSOUND will choose\n"
	          "    an appropriate default latency. While the input argument is merely a\n"
	          "    suggestion, the output argument represents an estimation of the actual\n"
	          "    audio processing latency in seconds, which may be different from the\n"
	          "    suggestion.  In case of error the returned latency will be zero.\n"
	          "\n");

	mexPrintf("msound( 'openWrite' )\n"
	          "    Open default output device to play back some single channel audio data\n"
	          "    using 44.1kHz sample rate and 1024 samples per block. See below for\n"
	          "    additional optional parameters.\n"
	          "\n");
	mexPrintf("latency = msound( 'openWrite', deviceID   , ...\n"
	          "                               sampleRate , ...\n"
	          "                               blockSize  , ...\n"
	          "                               numChannels, ...\n"
	          "                               latency     )\n"
	          "    Open a specific device to play-back audio data of specified format. The\n"
	          "    argument \"deviceID\" contains the ID of the device to be opened whereas\n"
	          "    ID 0 or [] indicate the default output device. Arguments \"sampleRate\",\n"
	          "    \"blockSize\", and \"numChannels\" represent the desired audio sample rate\n"
	          "    in Hertz, the preferred number of sample frames per data block, and the\n"
	          "    requested number of signal channels. If omitted or set to [], parameters\n"
	          "    are set to their respective default values (44.1kHz, 1024 sample frames\n"
	          "    per data block, and 1 signal channel.)\n"
	          "    The \"latency\" input argument may be used to suggest a specific audio\n"
	          "    processing latency in seconds. If omitted or empty, MSOUND will choose\n"
	          "    an appropriate default latency. While the input argument is merely a\n"
	          "    suggestion, the output argument represents an estimation of the actual\n"
	          "    audio processing latency in seconds, which may be different from the\n"
	          "    suggestion.  In case of error the returned latency will be zero.\n"
	          "\n");

	mexPrintf("msound( 'openRW' )\n"
	          "    Open default input and output device for single channel duplex play back\n"
	          "    and recording, using 44.1kHz sample rate and 1024 samples per block. See\n"
	          "    below for additional optional parameters.\n"
	          "\n");

	mexPrintf("latency = msound( 'openRW', deviceIds  , ...\n"
	          "                            sampleRate , ...\n"
	          "                            blockSize  , ...\n"
	          "                            numChannels, ...\n"
	          "                            latency     )\n"
	          "    Open the specific device(s) for duplex play-back and recording of some\n"
	          "    audio data using the specified format. The argument \"deviceID\" contains\n"
	          "    the IDs of the input device followed by the ID of the output device,\n"
	          "    whereas ID 0 or [] indicate the respective default device. If only a\n"
	          "    single ID is specified, the corresponding device is used for both,\n"
	          "    input and output. The arguments \"sampleRate\", \"blockSize\", and\n"
	          "    \"numChannels\" represent the desired audio sample rate in Hertz, the\n"
	          "    preferred number of sample frames per data block, and the requested\n"
	          "    number of signal channels. If omitted or set to [], parameters are set\n"
	          "    to their respective default values (44.1kHz, 1024 sample frames per\n"
	          "    data block, and 1 signal channel.) The \"numChannels\" argument may also\n"
	          "    be a two element vector containing the desired number of input channels\n"
	          "    followed by the number of output channels. The \"latency\" input argument\n"
	          "    may be used to suggest a specific audio processing latency in seconds.\n"
	          "    If omitted or empty, MSOUND will choose an appropriate default latency.\n"
	          "    While the input argument is merely a suggestion, the output argument\n"
	          "    represents an estimation of the actual audio processing latency in\n"
	          "    seconds, which may be different from the suggestion. In case of error\n"
	          "    the returned latency will be zero. This argument may also be a two\n"
	          "    element vector; input latency followed by output latency. The returned\n"
	          "    latency is always a two element row vector.\n"
	          "\n");

	mexPrintf("dataIn = msound( 'getSamples' )\n"
	          "    Gets samples from recording queue and returns them in a\n"
	          "    \"blockSize\"-by-\"numChannels\" matrix \"dataIn\".\n"
	          "\n");
	mexPrintf("dataIn = msound( 'getSamples', numSampleFrames )\n"
	          "    Gets samples from recording queue and returns them in a\n"
	          "    \"numSampleFrames\"-by-\"numChannels\" matrix \"dataIn\".\n"
	          "\n");

	mexPrintf("msound( 'putSamples', dataOut )\n"
	          "    Puts the \"blockSize\"-by-\"numChannels\" matrix \"dataIn\" into queue\n"
	          "    for play-back. Note, this function works \"X\"-by-\"numChannels\" matrices\n"
	          "    as well, but there might be some zero-padding in case \"X\" doesn't match\n"
	          "    an exact integer multiply of \"blockSize\".\n"
	          "\n");

	mexPrintf("msound( 'stop' )\n"
	          "    Wait until all buffers have been consumed, stop audio processing, close\n"
	          "    the audio device(s), and terminate audio hardware access. This is a very\n"
	          "    important step after using MSOUND. If MSOUND isn't terminated properly,\n"
	          "    some audio devices may become blocked until system reboot.\n"
	          "\n");
	mexPrintf("msound( 'close' )\n"
	          "    Abort audio processing immediately, close the device(s), and terminate\n"
	          "    audio hardware access. This is a very important step after using MSOUND.\n"
	          "    If MSOUND isn't terminated properly, audio devices may become some\n"
	          "    blocked until system reboot.\n"
	          "\n");

	mexPrintf("state = msound( 'verbose', X )\n"
	          "    Toggle verbose state, whereas \"X\" represents the desired verbose level.\n"
	          "    If MSOUND is in verbose mode, some additional (debug) information is\n"
	          "    displayed during use. The verbose level controls the amount of details\n"
	          "    to be displayed. Level 1 shows some basic messages that might be of\n"
	          "    interest for debugging m-files using MSOUND. Level 2 on the other hand\n"
	          "    seems a little over the top for everyday use but it can be very useful\n"
	          "    for debugging the MSOUND MEX-file. Using a verbose level of 0 disables\n"
	          "    verbose mode, which is also the default setting. If \"X\" is omitted, it\n"
	          "    defaults to level 1.\n"
	          "    The output argument returns MSOUND's state of initialization in a struct.\n"
	          "    It can be used to identify the requirement of using terminate access to\n"
	          "    the hardware or whether opening is necessary (when MSOUND was already\n"
	          "    initialized before).\n"
	          "\n");

	mexPrintf("MSOUND v" MSOUND_VERSION_STR " (" MSOUND_VERSION_DATE ")\n"
	          "Compile Time " __DATE__ ", " __TIME__ "\n"
	          "Copyright (c) 2004-2009  S. Fischer, M. Hansen, J. Bitzer,\n"
	          "Institute of Hearing Technology and Audiology (IHA) at the\n"
	          "University of Applied Sciences Wilhelmshaven/Oldenburg/Elsfleth\n"
	          "\n");

	mexPrintf("This build of MSOUND supports the following API(s):\n");
	/* This is a list of audio APIs, supported by PortAudio, though
	   they're not necessarily fully implemented:
			WMME      - Windows MultiMedia Extensions
			DS        - Microsoft DirectSound
			ASIO      - Steinberg Audio Stream Input/Output
			WASAPI    - Windows Audio Session API
			WDMKS     - Windows Driver Model Kernel-Streaming
			----
			OSS       - Open Sound System
			ALSA      - Advanced Linux Sound Architecture
			JACK      - JACK Audio Connection Kit
			SGI       - Silicon Graphics Irix using AL
			ASIHPI    - AudioScience Hardware Programming Interface API
			COREAUDIO - Core Audio
	*/
	/* If any host APIs are supported, display them on the screen. */
	if( iHostApiCount )
	{
		pHostApiInfo = Pa_GetHostApiInfo( 0 );

		/* Do not display skeleton API, used for development, only. */
		if( pHostApiInfo->type != paInDevelopment )
		{
			mexPrintf( "    %s", pHostApiInfo->name );
		}

		for(iHostApiIndex=1; iHostApiIndex<iHostApiCount; ++iHostApiIndex)
		{
			pHostApiInfo = Pa_GetHostApiInfo( iHostApiIndex );

			/* Do not display skeleton API, used for development, only. */
			if( pHostApiInfo->type != paInDevelopment )
			{
				mexPrintf( ", %s", pHostApiInfo->name );
			}
		}
	}
	mexPrintf("\n");

	mexPrintf("\n");
	mexPrintf("This is free software under the terms of the GNU Lesser General Public\n"
	          "License. This software is free of charge, but without ANY warranty.\n"
	          "(See http://www.gnu.org/licenses/ for more details.)\n"
	          "\n"
	          "The audio I/O is based on PortAudio by Ross Bencina and Phil Burk\n");
	/*mexPrintf("Used PortAudio Version: \"%s\"\n", Pa_GetVersionText() );*/
	mexPrintf("(See http://www.portaudio.com for more information.)\n");

	/* If we're using Steinberg's ASIO interface, we have to display its
	 * copyright. */
	if( 0 <= Pa_HostApiTypeIdToHostApiIndex( paASIO ) )
	{
		mexPrintf("\n"
		          "ASIO Interface Technology by Steinberg Media Technologies GmbH.\n"
		          "(See http://www.steinberg.net for more information.)\n");
	}
	mexPrintf("\n");

	return;
}
/*------------------------------------------------------------------------*/
