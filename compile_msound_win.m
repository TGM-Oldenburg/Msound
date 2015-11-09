function compile_msound_win

% Msound uses the open source library PortAudio which supports various
% operating systems and audio APIs. For Windows systems the default audio
% API is Windows MultiMedia Extensions (WMME). WMME is also the default
% configuration of msound, wich should compile on almost any MATLAB system
% using the included LCC compiler. 
bWmme = true;

% In addition, msound can also compiled with support for Steinberg Audio
% Stream Input/Output (ASIO) API. But this requires a C++ compiler like
% the Microsoft Visual Studio and Steinberg's ASIO SDK 2.2. If these
% requirements are met, the following flag may be set to true.
bAsio = false; % Requires ASIO SDK 2.2

% Msound can also be compiled using Microsoft's newest audio API, Windows
% Audio Sessions (WASAPI). WASAPI provides a modern low-latency interface
% to audio hardware and is therefore favorable over MME if ASIO is not an
% option. WASAPI requires a few of Microsoft's official headers which may
% require you to use the Visual Studio compiler in MEX.
bWASAPI = false;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp('Building msound ...')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

szOptions = '';
szDefines = '';
szPaths   = '';
szFiles   = '';

szOptions = sprintf( '%s %s', szOptions, '-output msound' );

% Add main file.
szFiles = addFile( szFiles, 'msound.c' );

% Add basic PortAudio include paths and files.
szPaths = addPath( szPaths, 'portaudio/include'                        );
szPaths = addPath( szPaths, 'portaudio/src/common'                     );
szPaths = addPath( szPaths, 'portaudio/src/os/win'                     );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_allocation.c'     );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_converters.c'     );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_cpuload.c'        );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_debugprint.c'     );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_dither.c'         );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_front.c'          );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_process.c'        );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_ringbuffer.c'     );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_stream.c'         );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_trace.c'          );

% Host
szFiles = addFile( szFiles, 'portaudio/src/os/win/pa_win_hostapis.c'   );
szFiles = addFile( szFiles, 'portaudio/src/os/win/pa_win_util.c'       );
szFiles = addFile( szFiles, 'portaudio/src/os/win/pa_win_waveformat.c' );


% Add PortAudio path for: Windows MultiMedia Extensions API
if( bWmme )
    disp('    Using ''Windows MultiMedia Extensions API'' ...')
    % PortAudio
    szPaths = addPath( szPaths, 'portaudio/src/hostapi/wmme' );
    szFiles = addFile( szFiles, 'portaudio/src/hostapi/wmme/pa_win_wmme.c' );
    % Libs
    szFiles = addFile( szFiles, 'winmm.lib' );

    % Required defines to compile 'pa_win_hostapis.c'
    szDefines = addDefine( szDefines, 'PA_USE_WMME' );
end
% szFiles = addFile( szFiles, fullfile( szLccLibPath, 'winmm.lib' ) );
szFiles = addFile( szFiles, 'winmm.lib' );


% Add PortAudio path for: Steinberg Audio Stream Input/Output API
if( bAsio )
    disp('    Using ''Steinberg Audio Stream Input/Output API'' ...')
    % PortAudio
    szFiles = addFile( szFiles, 'portaudio/src/common/pa_ringbuffer.c'   );
    szFiles = addFile( szFiles, 'portaudio/src/os/win/pa_win_coinitialize.c');
    szPaths = addPath( szPaths, 'portaudio/src/hostapi/asio'             );
    szFiles = addFile( szFiles, 'portaudio/src/hostapi/asio/pa_asio.cpp' );
    % ASIO-SDK
    szPaths = addPath( szPaths, 'ASIOSDK2/common'                        );
    szPaths = addPath( szPaths, 'ASIOSDK2/host'                          );
    szPaths = addPath( szPaths, 'ASIOSDK2/host/pc'                       );
    szFiles = addFile( szFiles, 'ASIOSDK2/common/asio.cpp'               );
    szFiles = addFile( szFiles, 'ASIOSDK2/host/asiodrivers.cpp'          );
    szFiles = addFile( szFiles, 'ASIOSDK2/host/pc/asiolist.cpp'          );
    % Libs
    % Symbols: RegCloseKey, RegQueryValueExA, RegEnumKeyA, RegOpenKeyExA,
    %          RegOpenKeyA
    szFiles = addFile( szFiles, 'advapi32.lib' );
    % Symbols: CoUninitialize, CoInitialize, CLSIDFromString,
    %          CoCreateInstance
    szFiles = addFile( szFiles, 'ole32.lib' );
    % Symbols: GetDesktopWindow, CharLowerBuffA
    szFiles = addFile( szFiles, 'user32.lib' );

    % Required defines to compile 'pa_win_hostapis.c'
    szDefines = addDefine( szDefines, 'PA_USE_ASIO' );
end


% Add PortAudio path for: Windows Audio Session API
if( bWASAPI )
    disp('    Using ''Microsoft Windows Audio Session API'' ...')
    % PortAudio
    szFiles = addFile( szFiles, 'portaudio/src/os/win/pa_win_coinitialize.c'  );
    szPaths = addPath( szPaths, 'portaudio/src/hostapi/wasapi'                );
    szFiles = addFile( szFiles, 'portaudio/src/hostapi/wasapi/pa_win_wasapi.c');

    % Required defines to compile 'pa_win_hostapis.c'
    szDefines = addDefine( szDefines, 'PA_USE_WASAPI' );
end


% Build msound MEX-file using default compiler.
eval( [ 'mex', szOptions, szDefines, szPaths, szFiles ] );


function szCmd = addFile  ( szCmd, szFile )
    szCmd = sprintf( '%s %s'  , szCmd, szFile );
function szCmd = addPath  ( szCmd, szPath )
    szCmd = sprintf( '%s -I%s', szCmd, szPath );
function szCmd = addDefine( szCmd, szName )
    szCmd = sprintf( '%s -D%s'  , szCmd, szName );
