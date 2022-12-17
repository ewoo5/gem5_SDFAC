/*
 * Copyright (c) 2012-2014 ARM Limited
 * All rights reserved.
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2003-2005,2014 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * Definitions of a conventional tag store.
 */

#include "mem/cache/tags/monitored_base_set_assoc.hh"

#include <string>

#include "base/intmath.hh"

#include "base/types.hh"
#include "mem/cache/replacement_policies/replaceable_entry.hh"
#include "mem/cache/tags/indexing_policies/base.hh"
#include "mem/cache/tags/indexing_policies/set_associative.hh"
#include "mem/request.hh"
#include "sim/core.hh"
#include "sim/sim_exit.hh"
#include "sim/system.hh"

#include "sim/cur_tick.hh"

namespace gem5
{

MonitoredBaseSetAssoc::MonitoredBaseSetAssoc(const Params &p)
    :BaseSetAssoc(p)
{
  // nothing extra
}

/*
*
* |___TAG___|___SET___|__BYTE_IDX__|
* TAG: Address >> (6 + 6)bits
* SET: 32kB/(64B * 8assoc) = 64 sets --> 6 bits
* BYTE_IDX: 64 bytes --> 6 bits
*
*/

CacheBlk*
MonitoredBaseSetAssoc::findBlock(Addr addr, bool is_secure)
{
    // Extract block tag
    Addr tag = extractTag(addr);

    printf("Object: %s \t curTick: %ld \t Set: %d \t", this->name().c_str(), curTick(), indexingPolicy->extractSet(addr));

    // Find possible entries that may contain the given address
    const std::vector<ReplaceableEntry*> entries =
        indexingPolicy->getPossibleEntries(addr);

    // Search for block
    for (const auto& location : entries) {
        CacheBlk* blk = static_cast<CacheBlk*>(location);
        if (blk->matchTag(tag, is_secure)) {
            printf("Hit\n");
            return blk;
        }
    }

    // Did not find block
    stats.set_misses_dist.sample(indexingPolicy->extractSet(addr), 1);
    printf("Miss\n");
    return nullptr;
}

}