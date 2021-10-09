%% pbibtex-miktex-adapter.ch:
%% 
%% Copyright (C) 2021 Christian Schenk
%% 
%% This file is free software; you can redistribute it and/or modify it
%% under the terms of the GNU General Public License as published by the
%% Free Software Foundation; either version 2, or (at your option) any
%% later version.
%% 
%% This file is distributed in the hope that it will be useful, but
%% WITHOUT ANY WARRANTY; without even the implied warranty of
%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
%% General Public License for more details.
%% 
%% You should have received a copy of the GNU General Public License
%% along with This file; if not, write to the Free Software Foundation,
%% 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

% _____________________________________________________________________________
%
% [1.1] Introduction
% _____________________________________________________________________________

@x
@d banner=='This is BibTeX, Version 0.99d' {printed when the program starts}
@y
@d my_name=='bibtex'
@d banner=='This is BibTeX, Version 0.99d' {printed when the program starts}
@z

% _____________________________________________________________________________
%
% [2.10] The main program
% _____________________________________________________________________________

@x
  miktex_print_miktex_banner(output);
  print_newline;
@y
  miktex_print_miktex_banner(output);
  print_newline;
  print (banner);
@z

% _____________________________________________________________________________
%
% [3.27]
% _____________________________________________________________________________

@x
do_nothing;
@y
for i:=0 to @'37 do xchr[i]:=chr(i);
for i:=@'177 to @'377 do xchr[i]:=chr(i);
@z

% _____________________________________________________________________________
%
% [3.28]
% _____________________________________________________________________________

@x
miktex_initialize_char_tables;
@y
for i:=first_text_char to last_text_char do xord[xchr[i]]:=i;
@z
