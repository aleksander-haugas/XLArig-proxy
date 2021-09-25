/* XMRig
 * Copyright (c) 2018-2021 SChernykh   <https://github.com/SChernykh>
 * Copyright (c) 2016-2021 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "proxy/splitters/donate/DonateSplitter.h"
#include "base/io/json/Json.h"
#include "base/net/stratum/Pool.h"
#include "core/Controller.h"
#include "proxy/config/MainConfig.h"
#include "proxy/events/CloseEvent.h"
#include "proxy/events/LoginEvent.h"
#include "proxy/events/SubmitEvent.h"
#include "proxy/Miner.h"
#include "proxy/splitters/donate/DonateMapper.h"


xmrig::DonateSplitter::DonateSplitter(Controller *controller) :
    m_controller(controller)
{
}


void xmrig::DonateSplitter::onEvent(uint32_t type, IEvent *event)
{
    if (event->isRejected()) {
        return;
    }

    switch (type) {
    case CLOSE_EVENT:
        return remove(static_cast<const CloseEvent *>(event)->miner());

    case LOGIN_EVENT:
        return login(static_cast<LoginEvent *>(event));

    case SUBMIT_EVENT:
        return submit(static_cast<SubmitEvent *>(event));

    default:
        break;
    }
}


void xmrig::DonateSplitter::login(LoginEvent *event)
{
    if (!m_controller->config()->isDonateOverProxy()) {
        return;
    }

    Miner *miner = event->miner();
    miner->setExtension(Miner::EXT_CONNECT, true);

    const String url = Json::getString(event->params, "url");
    if (url.isNull()) {
        return;
    }

#   ifdef NDEBUG
    if (!url.contains("xmrig.com")) {
        return reject(event);
    }
#   endif

    Pool pool(url);
    if (!pool.isValid()) {
        return reject(event);
    }

    pool.setUser(miner->user());
    pool.setRigId(miner->rigId());
    pool.setPassword("proxy");

    auto mapper = new DonateMapper(m_sequence++, event, pool);
    m_mappers[mapper->id()] = mapper;
}


void xmrig::DonateSplitter::reject(LoginEvent *event)
{
    event->reject();
    event->miner()->replyWithError(event->loginId, Error::toString(Error::Forbidden));
    event->miner()->close();
}


void xmrig::DonateSplitter::remove(Miner *miner)
{
    const auto id = static_cast<size_t>(miner->mapperId());
    if (miner->mapperId() < 0 || miner->routeId() != 0 || m_mappers.count(id) == 0) {
        return;
    }

    remove(id);
}


void xmrig::DonateSplitter::remove(size_t id)
{
    DonateMapper *mapper = m_mappers[id];
    auto it = m_mappers.find(id);
    if (it != m_mappers.end()) {
        m_mappers.erase(it);
    }

    delete mapper;
}


void xmrig::DonateSplitter::submit(SubmitEvent *event)
{
    if (event->miner()->mapperId() < 0 || event->miner()->routeId() != 0) {
        return;
    }

    DonateMapper *mapper = m_mappers[static_cast<size_t>(event->miner()->mapperId())];
    if (mapper) {
        mapper->submit(event);
    }
}

