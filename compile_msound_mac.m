function compile_msound_mac

% The more recent versions of MATLAB (Version 7.3.0.267 (R2006b) and later)
% use a slightly modified external interfaces API, which is not compatible
% with older MATLAB versions. If the following flag evaluates true, msound
% will be compiled using the old API. So it may possibly work with an older
% MATLAB version. But when it comes to MEX-Files different MATLAB versions
% may not always be compatible.
bOldMexApi = false;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp('Building msound ...')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

szOptions = '';
szDefines = '';
szPaths   = '';
szFiles   = '';
szLibs    = '';

szOptions = sprintf( '%s %s', szOptions, '-output msound' );

% Use the old MATLAB C API on newer MATLAB versions.
if( ~verLessThan('matlab','7.3') )
    if( bOldMexApi )
        % Use backward compatible old MATLAB C API
        szOptions = sprintf( '%s %s', szOptions, '-compatibleArrayDims' );
    else
        szOptions = sprintf( '%s %s', szOptions, '-largeArrayDims'      );
    end
end

% Add main file.
szFiles = addFile( szFiles, 'msound.c' );

% Add basic PortAudio include paths and files.
szPaths = addPath( szPaths, 'portaudio/include'                        );
szPaths = addPath( szPaths, 'portaudio/src/common'                     );
szPaths = addPath( szPaths, 'portaudio/src/os/unix'                    );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_allocation.c'     );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_converters.c'     );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_cpuload.c'        );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_debugprint.c'     );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_dither.c'         );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_front.c'          );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_process.c'        );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_ringbuffer.c'     );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_stream.c'       );
szFiles = addFile( szFiles, 'portaudio/src/common/pa_trace.c'         );

% Host
szFiles = addFile( szFiles, 'portaudio/src/os/unix/pa_unix_hostapis.c' );
szFiles = addFile( szFiles, 'portaudio/src/os/unix/pa_unix_util.c'     );

% Add PortAudio path for: CoreAudio
if( bCoreAudio )
    disp('    Using ''CoreAudio'' ...')
    % Required defines to compile with CoreAudio support.
    szDefines = addDefine( szDefines, 'PA_USE_COREAUDIO' );
    % Trick the compiler into accepting the portaudio source code
    szDefines = addDefine( szDefines, 'char16_t=UINT16_T');
    % PortAudio
    szPaths = addPath( szPaths, 'portaudio/src/hostapi/coreaudio'                 );
    szFiles = addFile( szFiles, 'portaudio/src/hostapi/coreaudio/pa_mac_core_utilities.c' );
    szFiles = addFile( szFiles, 'portaudio/src/hostapi/coreaudio/pa_mac_core_blocking.c' );
    szFiles = addFile( szFiles, 'portaudio/src/hostapi/coreaudio/pa_mac_core.c' );
    % Libs
    szOptions = sprintf('%s %s', szOptions, 'LDFLAGS=''\$LDFLAGS -framework CoreFoundation -framework CoreServices -framework CoreAudio -framework AudioToolBox -framework AudioUnit''');
end

% Build msound MEX-file using default compiler.
eval( [ 'mex', szOptions, szDefines, szPaths, szFiles, szLibs ] );


% Call msound to see it doesn't crash... ;-)
% msound



function szCmd = addFile  ( szCmd, szFile )
    szCmd = sprintf( '%s "%s"'  , szCmd, szFile );
function szCmd = addPath  ( szCmd, szPath )
    szCmd = sprintf( '%s "-I%s"', szCmd, szPath );
function szCmd = addDefine( szCmd, szName )
    szCmd = sprintf( '%s "-D%s"', szCmd, szName );
function szCmd = addLib   ( szCmd, szLib  )
    szCmd = sprintf( '%s "-l%s"', szCmd, szLib  );
function szCmd = addLibPath( szCmd, szPath )
    szCmd = sprintf( '%s "-L%s"', szCmd, szPath );
