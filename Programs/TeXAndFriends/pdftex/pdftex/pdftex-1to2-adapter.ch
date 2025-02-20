%% pdftex-1to2-adapter.ch: pdftex-1.web to pdftex-2.web adapter
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
% [57.1499]
% _____________________________________________________________________________

@x
undump_things(font_check[null_font], font_ptr+1-null_font);
@y
param_base:=xmalloc_array(integer, font_max);

undump_things(font_check[null_font], font_ptr+1-null_font);
@z

% _____________________________________________________________________________
%
% [58.1510]
% _____________________________________________________________________________

@x
miktex_allocate_memory;
@y
miktex_allocate_memory;
REMOVE_THIS_BEGIN
  setup_bound_var (0)('hash_extra')(hash_extra);
  setup_bound_var (10000)('expand_depth')(expand_depth);

  const_chk (hash_extra);
  if error_line > ssup_error_line then error_line := ssup_error_line;

  line_stack:=xmalloc_array (integer, max_in_open);

  hyph_link :=xmalloc_array (hyph_pointer, hyph_size);
REMOVE_THIS_END
@z

% _____________________________________________________________________________
%
% [58.1515]
% _____________________________________________________________________________

@x
  font_ptr:=null_font; fmem_ptr:=7;
@y
  param_base:=xmalloc_array(integer, font_max);

  font_ptr:=null_font; fmem_ptr:=7;
@z

% _____________________________________________________________________________
%
% [61.1657]
% _____________________________________________________________________________

@x
@!eof_seen : array[1..sup_max_in_open] of boolean; {has eof been seen?}
@y
@!eof_seen : array[1..max_in_open] of boolean; {has eof been seen?}
@z

% _____________________________________________________________________________
%
% [61.1770]
% _____________________________________________________________________________

@x
@!grp_stack : array[0..sup_max_in_open] of save_pointer; {initial |cur_boundary|}
@!if_stack : array[0..sup_max_in_open] of pointer; {initial |cond_ptr|}
@y
@!grp_stack : array[0..max_in_open] of save_pointer; {initial |cur_boundary|}
@!if_stack : array[0..max_in_open] of pointer; {initial |cond_ptr|}
@z
