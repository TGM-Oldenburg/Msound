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

szSourceFile    = 'msound.c';
szLibName       = 'portaudio';
szIncludeDir    = '/usr/local/include';
szArrayDims     = '';

% Use the old MATLAB C API on newer MATLAB versions.
if( ~verLessThan('matlab','7.3') )
    if( bOldMexApi )
        % Use backward compatible old MATLAB C API
        szArrayDims = '-compatibleArrayDims';
    else
        szArrayDims = '-largeArrayDims';
end
end

mex(szArrayDims, ['-I' szIncludeDir], ['-l' szLibName], szSourceFile);