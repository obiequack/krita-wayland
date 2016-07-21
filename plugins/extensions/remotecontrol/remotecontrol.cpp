/*
 *
 * Copyright (c) 2016 Sven Langkamp <sven.langkamp@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "remotecontrol.h"

#include <klocalizedstring.h>
#include <kis_debug.h>
#include <kpluginfactory.h>

#include "kis_paintop_control_object.h"
#include "kis_opcua_server.h"

K_PLUGIN_FACTORY_WITH_JSON(RemoteControlFactory, "kritaremotecontrol.json", registerPlugin<RemoteControl>();)

RemoteControl::RemoteControl(QObject *parent, const QVariantList &)
        : KisViewPlugin(parent)
{
    m_paintOpControl = new KisPaintopControlObject(m_view, this);
    m_server = new KisOpcUaServer();
    
    m_server->addObject(m_paintOpControl);
    m_server->start();
}

RemoteControl::~RemoteControl()
{
}

#include "remotecontrol.moc"
