/*
 * Copyright (c) 2018 Inria
 * Copyright (c) 2012-2014,2017 ARM Limited
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
 * Definitions of a set associative indexing policy.
 */

#include "mem/cache/tags/indexing_policies/flex_associative.hh"

#include "mem/cache/replacement_policies/replaceable_entry.hh"

namespace gem5
{

FlexAssociative::FlexAssociative(const Params &p)
    : BaseIndexingPolicy(p),
    //stats(this, get_numSets())
    stats(this, 64),
    indexTable(p.index_table)
{
  // Make space for the entries

  for (uint32_t i = 0; i < numSets; ++i) {
    sets[i].resize(indexTable[i+1] - indexTable[i]);
  }

}

void 
FlexAssociative::setEntry(ReplaceableEntry* entry, const uint64_t index){

  // Calculate set and way from entry index
  // const std::lldiv_t div_result = std::div((long long)index, assoc);
  // const uint32_t set = div_result.quot;
  // const uint32_t way = div_result.rem;

  // Sanity check
  // assert(set < numSets);

  // Assign a free pointer
  uint32_t set, way;
  uint32_t stride = 65/2;
  int i = stride;

  // binary search for which set it belongs to
  while (index < indexTable[i] || index >= indexTable[i+1]){
    stride = (stride+1)/2;
    if (index < indexTable[i]){
      i -= stride;
    }else{
      i += stride;
    }
  }
  set = i;
  way = index - indexTable[i];
  // Sanity check
  assert(set < numSets && set >= 0);
  sets[set][way] = entry;

  // set = index/assoc;
  // way = index - set * assoc;

  printf("Index %d has been assigned to SET %d and WAY %d\n", index, set, way);

  // Inform the entry its position
  entry->setPosition(set, way);

}

uint32_t
FlexAssociative::extractSet(const Addr addr) const
{
    return (addr >> setShift) & setMask;
}

Addr
FlexAssociative::regenerateAddr(const Addr tag, const ReplaceableEntry* entry)
                                                                        const
{
    return (tag << tagShift) | (entry->getSet() << setShift);
}

std::vector<ReplaceableEntry*>
FlexAssociative::getPossibleEntries(const Addr addr)
{

    stats.setAccessHist.sample(extractSet(addr), 1);
    //printf("TimeTick: %ld, AccessedTag: %d\n", curTick(), extractSet(addr));

    return sets[extractSet(addr)];
}

FlexAssociative::AssocStats::AssocStats(statistics::Group *parent, uint32_t numBins)
    : statistics::Group(parent),
    ADD_STAT(setAccessHist, statistics::units::Count::get(),
        "Histogram of set associative cache access per set")
    // ADD_STAT(tagMissHist, statistics::units::Count::get(),
    //     "Histogram of set associative cache access misses per tag"),
    {
        using namespace statistics;

        #define LAST_TICK 8616812895

        setAccessHist
            .init(numBins)
            .flags(nonan);

        // tagMissHist
        //     .init(num_bins)
        //     .flags(nonan);

    }

} // namespace gem5