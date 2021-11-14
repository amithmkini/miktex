/* miktex-updvitype.h:

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

#include "miktex-updvitype-config.h"

#include <iostream>

#include <miktex/TeXAndFriends/WebApp>

#include <miktex/texjp/common.h>

#define OPT_KANJI 1000
#define OPT_MAX_PAGES 1001
#define OPT_NEW_MAG 1002
#define OPT_OUT_MODE 1003
#define OPT_RESOLUTION 1004
#define OPT_START_THERE 1005

extern UPDVITYPEPROGCLASS UPDVITYPEPROG;

class UPDVITYPEAPPCLASS :
    public MiKTeX::TeXAndFriends::WebApp
{
public:
    void AddOptions() override
    {
        AddOption("kanji", MIKTEXTEXT("Set Japanese encoding (ENC=euc|jis|sjis|utf8)."), OPT_KANJI, POPT_ARG_STRING, "ENC");
        AddOption("max-pages", MIKTEXTEXT("Set maximum number of pages."), OPT_MAX_PAGES, POPT_ARG_STRING, "N");
        AddOption("new-mag", MIKTEXTEXT("Set new magnification."), OPT_NEW_MAG, POPT_ARG_STRING, "MAG");
        AddOption("out-mode", MIKTEXTEXT("Set output mode."), OPT_OUT_MODE, POPT_ARG_STRING, "MODE");
        AddOption("resolution", MIKTEXTEXT("Set desired resolution."), OPT_RESOLUTION, POPT_ARG_STRING, MIKTEXTEXT("NUM/DEN"));
        AddOption("start-there", MIKTEXTEXT("Set starting page."), OPT_START_THERE, POPT_ARG_STRING, "PAGESPEC");
        WebApp::AddOptions();
    }

public:
    std::string GetUsage() const override
    {
        return MIKTEXTEXT("[OPTION...] INPUTFILE");
    }

public:
    void Init(std::vector<char*>& args) override
    {
        WebApp::Init(args);
        UPDVITYPEPROG.maxpages = 1000000;
        UPDVITYPEPROG.newmag = 0;
        UPDVITYPEPROG.outmode = 4;
        UPDVITYPEPROG.resolution = 300.0;
        UPDVITYPEPROG.startthere[0] = false;
        UPDVITYPEPROG.startvals = 0;
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
        case OPT_MAX_PAGES:
            if (optArg.empty() || !isdigit(optArg[0]))
            {
                BadUsage();
            }
            UPDVITYPEPROG.maxpages = std::stoi(optArg);
            break;
        case OPT_NEW_MAG:
            if (optArg.empty() || !isdigit(optArg[0]))
            {
                BadUsage();
            }
            UPDVITYPEPROG.newmag = std::stoi(optArg);
            break;
        case OPT_OUT_MODE:
            if (!(optArg == "0" || optArg == "1" || optArg == "2" || optArg == "3" || optArg == "4"))
            {
                BadUsage();
            }
            UPDVITYPEPROG.outmode = std::stoi(optArg);
            break;
        case OPT_RESOLUTION:
        {
            int num, den;
#if defined(MIKTEX_WINDOWS)
            if (sscanf_s(optArg.c_str(), "%d/%d", &num, &den) != 2 || (num < 0))
            {
                BadUsage();
            }
#else
            if (sscanf(optArg.c_str(), "%d/%d", &num, &den) != 2 || (num < 0))
            {
                BadUsage();
            }
#endif
            UPDVITYPEPROG.resolution = static_cast<float>(num) / den;
            break;
        }
        case OPT_START_THERE:
        {
            const char* lpsz = optArg.c_str();
            size_t k = 0;
            do
            {
                if (*lpsz == MIKTEXTEXT('*'))
                {
                    UPDVITYPEPROG.startthere[k] = false;
                    ++lpsz;
                }
                else if (!(isdigit(*lpsz) || (*lpsz == MIKTEXTEXT('-') && isdigit(lpsz[1]))))
                {
                    BadUsage();
                }
                else
                {
                    UPDVITYPEPROG.startthere[k] = true;
                    char* lpsz2 = 0;
                    UPDVITYPEPROG.startcount[k] = strtol(lpsz, &lpsz2, 10);
                    lpsz = const_cast<const char*>(lpsz2);
                }
                if (k < 9 && *lpsz == '.')
                {
                    ++k;
                    ++lpsz;
                }
                else if (*lpsz == 0)
                {
                    UPDVITYPEPROG.startvals = static_cast<C4P::C4P_signed8>(k);
                }
                else
                {
                    BadUsage();
                }
            } while (UPDVITYPEPROG.startvals != static_cast<C4P::C4P_signed8>(k));
            break;
        }
        default:
            done = WebApp::ProcessOption(opt, optArg);
            break;
        }
        return done;
    }

public:
    void ProcessCommandLineOptions() override
    {
        WebApp::ProcessCommandLineOptions();
        if (GetProgram()->GetArgC() != 2)
        {
            BadUsage();
        }
    }
};
