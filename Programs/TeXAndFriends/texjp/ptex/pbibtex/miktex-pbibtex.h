/* miktex-pbibtex.h:

   Copyright (C) 2021 Christian Schenk

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

#include "miktex-pbibtex-config.h"

#include <iostream>

#define IMPLEMENT_TCX 1

#include <miktex/Configuration/ConfigNames>
#include <miktex/Core/FileType>
#include <miktex/TeXAndFriends/CharacterConverterImpl>
#include <miktex/TeXAndFriends/InitFinalizeImpl>
#include <miktex/TeXAndFriends/InputOutputImpl>
#include <miktex/TeXAndFriends/WebAppInputLine>

#include "pbibtex.h"

namespace bibtex {
#include <miktex/bibtex.defaults.h>
}

#include <miktex/texjp/common.h>

extern PBIBTEXPROGCLASS PBIBTEXPROG;

class PBIBTEXAPPCLASS :
    public MiKTeX::TeXAndFriends::WebAppInputLine
{
public:
    template<typename T> void Reallocate(T*& p, size_t n)
    {
        size_t amount = n * sizeof(T);
        void* p2 = realloc(p, amount);
        if (p2 == nullptr && amount > 0)
        {
            FatalError(MIKTEXTEXT("Virtual memory exhausted."));
        }
        p = reinterpret_cast<T*>(p2);
    }
  
public:
    template<typename T> void PascalReallocate(T*& p, size_t n)
    {
        return Reallocate(p, n + 1);
    }

public:
    template<typename T> void Allocate(T*& p, size_t n)
    {
        p = nullptr;
        Reallocate(p, n);
    }

public:
    template<typename T> void PascalAllocate(T*& p, size_t n)
    {
        Allocate(p, n + 1);
    }

public:
    template<typename T> void Free(T*& p)
    {
        free(p);
        p = nullptr;
    }

private:
    std::shared_ptr<MiKTeX::Core::Session> session;
  
private:
    MiKTeX::TeXAndFriends::CharacterConverterImpl<PBIBTEXPROGCLASS> charConv{ PBIBTEXPROG };

private:
    MiKTeX::TeXAndFriends::InitFinalizeImpl<PBIBTEXPROGCLASS> initFinalize{ PBIBTEXPROG };

private:
    MiKTeX::TeXAndFriends::InputOutputImpl<PBIBTEXPROGCLASS> inputOutput{ PBIBTEXPROG };

public:
    void Init(std::vector<char*>& args) override
    {
        SetCharacterConverter(&charConv);
        SetInitFinalize(&initFinalize);
        SetInputOutput(&inputOutput);
        WebAppInputLine::Init(args);
        session = GetSession();
#if defined(IMPLEMENT_TCX)
        EnableFeature(MiKTeX::TeXAndFriends::Feature::TCX);
#endif
        PBIBTEXPROG.entstrsize = session->GetConfigValue(MIKTEX_CONFIG_SECTION_BIBTEX, "ent_str_size", MiKTeX::Configuration::ConfigValue(bibtex::bibtex::ent_str_size())).GetInt();
        PBIBTEXPROG.globstrsize = session->GetConfigValue(MIKTEX_CONFIG_SECTION_BIBTEX, "glob_str_size", MiKTeX::Configuration::ConfigValue(bibtex::bibtex::glob_str_size())).GetInt();
        PBIBTEXPROG.maxstrings = session->GetConfigValue(MIKTEX_CONFIG_SECTION_BIBTEX, "max_strings", MiKTeX::Configuration::ConfigValue(bibtex::bibtex::max_strings())).GetInt();
        PBIBTEXPROG.mincrossrefs = session->GetConfigValue(MIKTEX_CONFIG_SECTION_BIBTEX, "min_crossrefs", MiKTeX::Configuration::ConfigValue(bibtex::bibtex::min_crossrefs())).GetInt();
        PBIBTEXPROG.hashsize = PBIBTEXPROG.maxstrings;
        const int HASH_SIZE_MIN = 5000;
        if (PBIBTEXPROG.hashsize < HASH_SIZE_MIN)
        {
            PBIBTEXPROG.hashsize = HASH_SIZE_MIN;
        }
        PBIBTEXPROG.hashmax = PBIBTEXPROG.hashsize + PBIBTEXPROG.hashbase - 1;
        PBIBTEXPROG.endofdef = PBIBTEXPROG.hashmax + 1;
        PBIBTEXPROG.undefined = PBIBTEXPROG.hashmax + 1;
        PBIBTEXPROG.bufsize = PBIBTEXPROG.bufsizedef;
        PBIBTEXPROG.litstksize = PBIBTEXPROG.litstksizedef;
        PBIBTEXPROG.maxbibfiles = PBIBTEXPROG.maxbibfilesdef;
        PBIBTEXPROG.maxglobstrs = PBIBTEXPROG.maxglobstrsdef;
        PBIBTEXPROG.maxcites = PBIBTEXPROG.maxcitesdef;
        PBIBTEXPROG.maxentints = PBIBTEXPROG.maxentintsdef;
        PBIBTEXPROG.maxentstrs = PBIBTEXPROG.maxentstrsdef;
        PBIBTEXPROG.maxfields = PBIBTEXPROG.maxfieldsdef;
        PBIBTEXPROG.poolsize = PBIBTEXPROG.poolsizedef;
        PBIBTEXPROG.singlefnspace = PBIBTEXPROG.singlefnspacedef;
        PBIBTEXPROG.wizfnspace = PBIBTEXPROG.wizfnspacedef;
        PBIBTEXPROG.entryints = nullptr;
        PBIBTEXPROG.entrystrs = nullptr;
        PascalAllocate(PBIBTEXPROG.bibfile, PBIBTEXPROG.maxbibfiles);
        PascalAllocate(PBIBTEXPROG.biblist, PBIBTEXPROG.maxbibfiles);
        PascalAllocate(PBIBTEXPROG.buffer, PBIBTEXPROG.bufsize);
        PascalAllocate(PBIBTEXPROG.citeinfo, PBIBTEXPROG.maxcites);
        PascalAllocate(PBIBTEXPROG.citelist, PBIBTEXPROG.maxcites);
        PascalAllocate(PBIBTEXPROG.entryexists, PBIBTEXPROG.maxcites);
        PascalAllocate(PBIBTEXPROG.exbuf, PBIBTEXPROG.bufsize);
        PascalAllocate(PBIBTEXPROG.fieldinfo, PBIBTEXPROG.maxfields);
        PascalAllocate(PBIBTEXPROG.fntype, PBIBTEXPROG.hashmax);
        PascalAllocate(PBIBTEXPROG.glbstrend, PBIBTEXPROG.maxglobstrs);
        PascalAllocate(PBIBTEXPROG.glbstrptr, PBIBTEXPROG.maxglobstrs);
        PascalAllocate(PBIBTEXPROG.globalstrs, static_cast<size_t>(PBIBTEXPROG.maxglobstrs) * (static_cast<size_t>(PBIBTEXPROG.globstrsize) + 1));
        PascalAllocate(PBIBTEXPROG.hashilk, PBIBTEXPROG.hashmax);
        PascalAllocate(PBIBTEXPROG.hashnext, PBIBTEXPROG.hashmax);
        PascalAllocate(PBIBTEXPROG.hashtext, PBIBTEXPROG.hashmax);
        PascalAllocate(PBIBTEXPROG.ilkinfo, PBIBTEXPROG.hashmax);
        PascalAllocate(PBIBTEXPROG.litstack, PBIBTEXPROG.litstksize);
        PascalAllocate(PBIBTEXPROG.litstktype, PBIBTEXPROG.litstksize);
        PascalAllocate(PBIBTEXPROG.namesepchar, PBIBTEXPROG.bufsize);
        PascalAllocate(PBIBTEXPROG.nametok, PBIBTEXPROG.bufsize);
        PascalAllocate(PBIBTEXPROG.outbuf, PBIBTEXPROG.bufsize);
        PascalAllocate(PBIBTEXPROG.spreamble, PBIBTEXPROG.maxbibfiles);
        PascalAllocate(PBIBTEXPROG.strpool, PBIBTEXPROG.poolsize);
        PascalAllocate(PBIBTEXPROG.strstart, PBIBTEXPROG.maxstrings);
        PascalAllocate(PBIBTEXPROG.svbuffer, PBIBTEXPROG.bufsize);
        PascalAllocate(PBIBTEXPROG.typelist, PBIBTEXPROG.maxcites);
        PascalAllocate(PBIBTEXPROG.wizfunctions, PBIBTEXPROG.wizfnspace);
        PBIBTEXPROG.computehashprime();
    }
  
public:
    void Finalize() override
    {
        Free(PBIBTEXPROG.bibfile);
        Free(PBIBTEXPROG.biblist);
        Free(PBIBTEXPROG.buffer);
        Free(PBIBTEXPROG.citeinfo);
        Free(PBIBTEXPROG.citelist);
        Free(PBIBTEXPROG.entryexists);
        Free(PBIBTEXPROG.entryints);
        Free(PBIBTEXPROG.entrystrs);
        Free(PBIBTEXPROG.exbuf);
        Free(PBIBTEXPROG.fieldinfo);
        Free(PBIBTEXPROG.fntype);
        Free(PBIBTEXPROG.glbstrend);
        Free(PBIBTEXPROG.glbstrptr);
        Free(PBIBTEXPROG.globalstrs);
        Free(PBIBTEXPROG.hashilk);
        Free(PBIBTEXPROG.hashnext);
        Free(PBIBTEXPROG.hashtext);
        Free(PBIBTEXPROG.ilkinfo);
        Free(PBIBTEXPROG.litstack);
        Free(PBIBTEXPROG.litstktype);
        Free(PBIBTEXPROG.namesepchar);
        Free(PBIBTEXPROG.nametok);
        Free(PBIBTEXPROG.outbuf);
        Free(PBIBTEXPROG.spreamble);
        Free(PBIBTEXPROG.strpool);
        Free(PBIBTEXPROG.strstart);
        Free(PBIBTEXPROG.svbuffer);
        Free(PBIBTEXPROG.typelist);
        Free(PBIBTEXPROG.wizfunctions);
        WebAppInputLine::Finalize();
    }

#define OPT_KANJI 1000
#define OPT_MIN_CROSSREFS 1001
#define OPT_QUIET 1002

public:
    void AddOptions() override
    {
        WebAppInputLine::AddOptions();
        AddOption("kanji", MIKTEXTEXT("Set Japanese encoding (ENC=euc|jis|sjis|utf8)."), OPT_KANJI, POPT_ARG_STRING, "ENC");
        AddOption(MIKTEXTEXT("quiet\0Suppress all output (except errors)."), OPT_QUIET, POPT_ARG_NONE);
        AddOption("silent", "quiet");
        AddOption("terse", "quiet");
    }
  
public:
    MiKTeX::Core::FileType GetInputFileType() const override
    {
        return MiKTeX::Core::FileType::BIB;
    }

public:
    std::string MIKTEXTHISCALL GetUsage() const override
    {
        return MIKTEXTEXT("[OPTION...] AUXFILE");
    }

public:
    bool ProcessOption(int opt, const std::string& optArg) override
    {
        bool done = true;
        switch (opt)
        {
        case OPT_KANJI:
            set_prior_file_enc();
            if (!set_enc_string(optArg.c_str(), optArg.c_str()))
            {
                std::cerr << MIKTEXTEXT("Unknown encoding: ") << optArg << std::endl;
                throw 1;
            }
            break;
        case OPT_MIN_CROSSREFS:
            PBIBTEXPROG.mincrossrefs = std::stoi(optArg);
            break;
        case OPT_QUIET:
            SetQuietFlag(true);
            break;
        default:
            done = WebAppInputLine::ProcessOption(opt, optArg);
            break;
        }
        return done;
    }
  
public:
    std::string MIKTEXTHISCALL TheNameOfTheGame() const override
    {
        return "BibTeX";
    }

public:
    void BufferSizeExceeded() const override
    {
        PBIBTEXPROG.bufferoverflow();
    }

public:
    void SetNameOfFile(const MiKTeX::Util::PathName& fileName) override
    {
        MiKTeX::TeXAndFriends::IInputOutput* inputOutput = GetInputOutput();
        Reallocate(inputOutput->nameoffile(), fileName.GetLength() + 1);
        MiKTeX::Util::StringUtil::CopyString(inputOutput->nameoffile(), fileName.GetLength() + 1, fileName.GetData());
        inputOutput->namelength() = static_cast<C4P::C4P_signed32>(fileName.GetLength());
    }

public:
    template<class T> bool OpenBstFile(T& f) const
    {
        const char* fileName = GetInputOutput()->nameoffile();
        MIKTEX_ASSERT_STRING(fileName);
        MiKTeX::Util::PathName bstFileName(fileName);
        if (!bstFileName.HasExtension())
        {
            bstFileName.SetExtension(".bst");
        }
        MiKTeX::Util::PathName path;
        if (!session->FindFile(bstFileName.ToString(), MiKTeX::Core::FileType::BST, path))
        {
            return false;
        }
        FILE* file = session->OpenFile(path, MiKTeX::Core::FileMode::Open, MiKTeX::Core::FileAccess::Read, true);
        f.Attach(file, true);
#ifdef PASCAL_TEXT_IO
        get(f);
#endif
        return true;
    }
};

extern PBIBTEXAPPCLASS PBIBTEXAPP;

template<class T> inline void miktexbibtexalloc(T*& p, size_t n)
{
    PBIBTEXAPP.PascalAllocate(p, n);
}

template<class T> inline void miktexbibtexrealloc(const char* varName, T*& p, size_t n)
{
    if (PBIBTEXPROG.logfile != nullptr)
    {
        fprintf(PBIBTEXPROG.logfile, "Reallocating '%s' (item size: %d) to %d items.\n",
            varName, static_cast<int>(sizeof(T)), static_cast<int>(n));
    }
    PBIBTEXAPP.PascalReallocate(p, n);
}

template<class T> inline void miktexbibtexfree(T*& p)
{
    PBIBTEXAPP.Free(p);
}

template<class T> inline bool miktexopenbstfile(T& f)
{
    return PBIBTEXAPP.OpenBstFile(f);
}

inline bool miktexhasextension(const char* fileName, const char* extension)
{
    return MiKTeX::Util::PathName(fileName).HasExtension(extension);
}
