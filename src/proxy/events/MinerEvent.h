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

#ifndef XMRIG_MINEREVENT_H
#define XMRIG_MINEREVENT_H


#include "proxy/events/ProxyEvent.h"


namespace xmrig {


class Miner;


class MinerEvent : public Event
{
public:
    XMRIG_DISABLE_COPY_MOVE_DEFAULT(MinerEvent)

    ~MinerEvent() override  = default;

    inline Miner *miner() const     { return m_miner; }

    const char *ip() const;

protected:
    inline MinerEvent(Miner *miner) : m_miner(miner) {}

    int32_t route() const override;

private:
    Miner *m_miner;
};


} // namespace xmrig


#endif // XMRIG_MINEREVENT_H
