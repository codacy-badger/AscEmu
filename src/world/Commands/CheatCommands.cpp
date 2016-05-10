/*
Copyright (c) 2014-2016 AscEmu Team <http://www.ascemu.org/>
This file is released under the MIT license. See README-MIT for more information.
*/

#include "StdAfx.h"

//.cheat list
bool ChatHandler::HandleCheatListCommand(const char* /*args*/, WorldSession* m_session)
{
    auto player_target = GetSelectedPlayer(m_session, true, true);
    if (player_target == nullptr)
        return true;

    if (player_target->CooldownCheat || player_target->CastTimeCheat ||
        player_target->GodModeCheat || player_target->PowerCheat ||
        player_target->FlyCheat || player_target->AuraStackCheat ||
        player_target->ItemStackCheat || player_target->TriggerpassCheat ||
        player_target->m_isGmInvisible || player_target->bInvincible
        )
    {
        auto player_name = player_target->GetName();
        SystemMessage(m_session, "Player %s has the following cheats activated:", player_name);


        if (player_target->CooldownCheat)
            SystemMessage(m_session, "-- Cooldown is active.", player_name);
        if (player_target->CastTimeCheat)
            SystemMessage(m_session, "-- CastTime is active.", player_name);
        if (player_target->GodModeCheat)
            SystemMessage(m_session, "-- GodMode is active.", player_name);
        if (player_target->PowerCheat)
            SystemMessage(m_session, "-- PowerCheat is active.", player_name);
        if (player_target->FlyCheat)
            SystemMessage(m_session, "-- FlyCheat is active.", player_name);
        if (player_target->AuraStackCheat)
            SystemMessage(m_session, "-- AuraStack is active.", player_name);
        if (player_target->ItemStackCheat)
            SystemMessage(m_session, "-- ItemStack is active.", player_name);
        if (player_target->TriggerpassCheat)
            SystemMessage(m_session, "-- TriggerPass is active.", player_name);
        if (player_target->m_isGmInvisible)
            SystemMessage(m_session, "-- Invisibility is active.", player_name);
        if (player_target->bInvincible)
            SystemMessage(m_session, "-- Invincibility is active.", player_name);
    }
    else
    {
        SystemMessage(m_session, "Player %s has no active cheats!", player_target->GetName());
    }

    return true;
}