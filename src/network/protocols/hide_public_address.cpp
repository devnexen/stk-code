//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2013 SuperTuxKart-Team
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "network/protocols/hide_public_address.hpp"

#include "online/http_connector.hpp"
#include "online/current_online_user.hpp"
#include "config/user_config.hpp"
#include "utils/log.hpp"

HidePublicAddress::HidePublicAddress() : Protocol(NULL, PROTOCOL_SILENT)
{
}

HidePublicAddress::~HidePublicAddress()
{
}

void HidePublicAddress::notifyEvent(Event* event)
{
}

void HidePublicAddress::setup()
{
    m_state = NONE;
}

void HidePublicAddress::update()
{
    if (m_state == NONE)
    {
        HTTPConnector * connector = new HTTPConnector((std::string)UserConfigParams::m_server_multiplayer + "address-management.php");
        connector->setParameter("id",CurrentOnlineUser::get()->getUserID());
        connector->setParameter("token",CurrentOnlineUser::get()->getToken());
        connector->setParameter("action","unset");

        const XMLNode * result = connector->getXMLFromPage();
        std::string rec_success;

        if(result->get("success", &rec_success))
        {
            if(rec_success == "yes")
            {
                Log::debug("ShowPublicAddress", "Address hidden successfully.");
            }
            else
            {
                Log::error("ShowPublicAddress", "Fail to hide address.");
            }
        }
        else
        {
            Log::error("ShowPublicAddress", "Fail to hide address.");
        }
        m_state = DONE;
    }
    else if (m_state == DONE)
    {
        m_listener->requestTerminate(this);
    }
}
