/* ptex-miktex.h:

   Copyright (C) 1991-2021 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#pragma once

#include "ptex-miktex-config.h"

#include "ptex-version.h"

#include <miktex/TeXAndFriends/CharacterConverterImpl>
#include <miktex/TeXAndFriends/ErrorHandlerImpl>
#include <miktex/TeXAndFriends/FormatHandlerImpl>
#include <miktex/TeXAndFriends/InitFinalizeImpl>
#include <miktex/TeXAndFriends/InputOutputImpl>
#include <miktex/TeXAndFriends/StringHandlerImpl>
#include <miktex/TeXAndFriends/TeXApp>
#include <miktex/TeXAndFriends/TeXMemoryHandlerImpl>
#include <miktex/W2C/Emulation>

#include "ptex.h"

#if defined(MIKTEX_WINDOWS)
#include "ptex.rc"
#endif

#if !defined(MIKTEXHELP_PTEX)
#include <miktex/Core/Help>
#endif

extern PTEXPROGCLASS PTEXPROG;

class PTEXAPPCLASS :
    public MiKTeX::TeXAndFriends::TeXApp
{
private:
    MiKTeX::TeXAndFriends::CharacterConverterImpl<PTEXPROGCLASS> charConv{ PTEXPROG };

private:
    MiKTeX::TeXAndFriends::ErrorHandlerImpl<PTEXPROGCLASS> errorHandler{ PTEXPROG };

private:
    MiKTeX::TeXAndFriends::FormatHandlerImpl<PTEXPROGCLASS> formatHandler{ PTEXPROG };

private:
    MiKTeX::TeXAndFriends::InitFinalizeImpl<PTEXPROGCLASS> initFinalize{ PTEXPROG };

private:
    MiKTeX::TeXAndFriends::InputOutputImpl<PTEXPROGCLASS> inputOutput{ PTEXPROG };

private:
    MiKTeX::TeXAndFriends::StringHandlerImpl<PTEXPROGCLASS> stringHandler{ PTEXPROG };

private:
    MiKTeX::TeXAndFriends::TeXMemoryHandlerImpl<PTEXPROGCLASS> memoryHandler{ PTEXPROG, *this };

public:
    void Init(std::vector<char*>& args) override
    {
        SetCharacterConverter(&charConv);
        SetErrorHandler(&errorHandler);
        SetFormatHandler(&formatHandler);
        SetInitFinalize(&initFinalize);
        SetInputOutput(&inputOutput);
        SetStringHandler(&stringHandler);
        SetTeXMFMemoryHandler(&memoryHandler);
        TeXApp::Init(args);
#if defined(IMPLEMENT_TCX)
        EnableFeature(MiKTeX::TeXAndFriends::Feature::EightBitChars);
        EnableFeature(MiKTeX::TeXAndFriends::Feature::TCX);
#endif
    }

public:
    MiKTeX::Util::PathName GetMemoryDumpFileName() const override
    {
        return MiKTeX::Util::PathName("ptex.fmt");
    }

public:
    std::string GetInitProgramName() const override
    {
        return "iniptex";
    }

public:
    std::string GetVirginProgramName() const override
    {
        return "virptex";
    }

public:
    std::string TheNameOfTheGame() const override
    {
        return "pTeX";
    }
};

int miktexloadpoolstrings(int size);

inline int loadpoolstrings(int size)
{
    return miktexloadpoolstrings(size);
}

extern PTEXAPPCLASS PTEXAPP;

inline void miktexreallocatenameoffile(size_t n)
{  
    PTEXPROG.nameoffile = reinterpret_cast<char*>(PTEXAPP.GetTeXMFMemoryHandler()->ReallocateArray("name_of_file", PTEXPROG.nameoffile, sizeof(*PTEXPROG.nameoffile), n, MIKTEX_SOURCE_LOCATION()));
}
