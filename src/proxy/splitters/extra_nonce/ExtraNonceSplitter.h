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

#ifndef XMRIG_EXTRANONCESPLITTER_H
#define XMRIG_EXTRANONCESPLITTER_H


#include "proxy/splitters/Splitter.h"


namespace xmrig {


class Controller;
class LoginEvent;
class Miner;
class ExtraNonceMapper;
class Stats;
class SubmitEvent;


class ExtraNonceSplitter : public Splitter
{
public:
    XMRIG_DISABLE_COPY_MOVE_DEFAULT(ExtraNonceSplitter)

    static std::shared_ptr<ExtraNonceSplitter> create(Controller *controller);
    ~ExtraNonceSplitter() override = default;

protected:
    ExtraNonceSplitter(Controller* controller);

    Upstreams upstreams() const override;
    void connect() override;
    void gc() override;
    void printConnections() override;
    void tick(uint64_t ticks, uint64_t now) override;

#   ifdef APP_DEVEL
    void printState() override;
#   endif

    void onEvent(uint32_t type, IEvent *event) override;

private:
    void login(const LoginEvent *event);
    void remove(Miner *miner);
    void submit(SubmitEvent *event);

    std::shared_ptr<ExtraNonceMapper> m_upstream;
};


} // namespace xmrig


#endif // XMRIG_EXTRANONCESPLITTER_H
