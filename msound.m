%MSOUND Audio playback and recording interface.
%   MSOUND is a audio interface for MATLAB that allows you to access your
%   system's sound devices via Microsoft Windows Multi Media Extension
%   (MME) or Steinberg Audio Streaming Input Output (ASIO) drivers.
%
%   Every installed audio device is identified by a unique device ID, which
%   must be specified when using such a device. Whereas these IDs vary from
%   system to system, the device ID 0 always corresponds to your system's
%   default audio device. Some devices are capable of playback, while
%   others may be used for recording, only. You may display a list of
%   supported devices by typing the command MSOUND('deviceInfo').
%
%   Before a audio device can be used, it has to be opened. MSOUND is able
%   to open devices for playback only, recording only, or duplex recoding
%   and playback. Further information on the required commands may be
%   displayed by typing MSOUND('help'). Thereafter MSOUND may be used to
%   "get" recorded audio from the device or to "put" it into the devices'
%   playback buffer.
%
%   After a device has been used by MSOUND it must be closed by calling
%   MSOUND('close'). Failure to do so may cause the used device to become
%   unavailable to any other application until reboot.
%
%   The typical MSOUND usage is [...] = MSOUND( 'command' [, ...] ).
%   MSOUND's first input argument is a string, repesenting the command to
%   be executed. Wheras some command require additional input and/or output
%   argument to be present. You may type MSOUND('help') to display a list
%   of available commands and reqired parameters.

%   Copyright 2009  S. Fischer
%     Institute of Hearing Technology and Audiology, University of Applied
%     Sciences Wilhelmshaven/Oldenburg/Elsfleth, Ofener Str. 16/19,
%     26121 Oldenburg, Germany
%     http://www.hoertechnik-audiologie.de
%   Copyright 2004-2008  S. Fischer, M. Hansen, J. Bitzer
%     Institute of Hearing Technology and Audiology, University of Applied
%     Sciences Oldenburg/Ostfriesland/Wilhelmshaven, Ofener Str. 16/19,
%     26121 Oldenburg, Germany
%
%   This library is free software; you can redistribute it and/or modify
%   it under the terms of the GNU Lesser General Public License as
%   published by the Free Software Foundation; either version 2.1 of the
%   License, or (at your option) any later version.
%
%   This library is distributed in the hope that it will be useful,
%   but WITHOUT ANY WARRANTY; without even the implied warranty of
%   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
%   Lesser General Public License for more details.
%
%   You should have received a copy of the GNU Lesser General Public
%   License along with this library; if not, write to the Free Software
%   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
%   MA 02110-1301 USA
%
%   Visit http://www.gnu.org/licenses or see provided "lgpl-2.1.txt" file
%   for more information.
%
%   $Revision: 1.1 $  $Date: 2009/11/12 0:00:00 $

%   ASIO Interface Technology by Steinberg Media Technologies GmbH.

% The following comment, MATLAB compiler pragma, is necessary to avoid
% compiling this M-file instead of linking against the MEX-file.  Don't
% remove.
%#mex

% msound is implemented as a binary MEX-File function.
error('C/C++ MEX-file not found.');
