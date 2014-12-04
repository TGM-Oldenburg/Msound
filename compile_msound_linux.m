function compile_msound_linux

% The more recent versions of MATLAB (Version 7.3.0.267 (R2006b) and later)
% use a slightly modified external interfaces API, which is not compatible
% with older MATLAB versions. If the following flag evaluates true, msound
% will be compiled using the old API. So it may possibly work with an older
% MATLAB version. But when it comes to MEX-Files different MATLAB versions
% may not always be compatible.
bOldMexApi = false;

% Msound uses the open source library PortAudio which supports various
% operating systems and audio APIs. For Linux systems the default audio
% API is Open Sound System (OSS) API. OSS is the default configuration of
% msound, wich should compile on almost any MATLAB system using the
% installed GCC compiler.
bOss = true;

% In addition, msound can also compiled with support for Advanced Linux
% Sound Architecture (ALSA) API. But this requires the libasound-dev
% packages to be installed on your system. ALSA support will be enabled
% if the following flag is set to true. ALSA seems kinda bumpy on default
% latency settings, but runs smooth for reasonable user defined latency
% settings.
bAlsa = true;

% Another commonly used audio interface is Jack Audio Connection Kit (JACK)
% API. Using JACK requires the libjack-dev packages to be installed and
% the following flag to evaluate true. But JACK didn't work at all on the
% author's test system (ACER eMachines EL1600, Ubuntu Karmic Koala, Matlab
% 7.5 (R2007b.) Someone with more expertise would possibly be able to get
% it working.
bJack = false;


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
szPaths = addPath( szPaths, '/usr/include' );
szLibs  = addLib(  szLibs,  'portaudio'    );


% Add PortAudio path for: Open Sound System
if( bOss )
    disp('    Using ''Open Sound System API'' ...')
    % Required defines to compile with OSS support.
    szDefines = addDefine( szDefines, 'PA_USE_OSS'           );
    szDefines = addDefine( szDefines, 'HAVE_SYS_SOUNDCARD_H' );
end


% Add PortAudio path for: Advanced Linux Sound Architecture API
if( bAlsa )
    disp('    Using ''Advanced Linux Sound Architecture API'' ...')
    % Required defines to compile with ALSA support.
    szDefines = addDefine( szDefines, 'PA_USE_ALSA' );
    % Libs
    szLibs  = addLib ( szLibs , 'asound' );
end


% Add PortAudio path for: Jack Audio Connection Kit API
if( bJack )
    disp('    Using ''Jack Audio Connection Kit API'' ...')
    % Required defines to compile with JACK support.
    szDefines = addDefine( szDefines, 'PA_USE_JACK' );
    % Libs
    szLibs  = addLib ( szLibs , 'jack'    );
    % Do we need these Libs, too?
%   szLibs  = addLib ( szLibs , 'asound'  );
%   szLibs  = addLib ( szLibs , 'pthread' );
%   szLibs  = addLib ( szLibs , 'rt'      );
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
