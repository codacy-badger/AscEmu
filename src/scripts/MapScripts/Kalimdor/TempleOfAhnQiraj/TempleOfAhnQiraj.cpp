/*
Copyright (c) 2014-2018 AscEmu Team <http://www.ascemu.org>
This file is released under the MIT license. See README-MIT for more information.
*/

#include "Setup.h"
#include "TempleOfAhnQiraj.h"


class TempleOfAhnQiraj : public InstanceScript
{
public:

    TempleOfAhnQiraj(MapMgr* pMapMgr) : InstanceScript(pMapMgr)
    {
    }

    static InstanceScript* Create(MapMgr* pMapMgr) { return new TempleOfAhnQiraj(pMapMgr); }
};


void TempleOfAhnQirajScripts(ScriptMgr* scriptMgr)
{
    scriptMgr->register_instance_script(531, &TempleOfAhnQiraj::Create);
}

