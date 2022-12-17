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
 * Declaration of a set associative indexing policy.
 */
 #ifndef __MEM_CACHE_INDEXING_POLICIES_FLEX_ASSOCIATIVE_HH__
#define __MEM_CACHE_INDEXING_POLICIES_FLEX_ASSOCIATIVE_HH__

#include <vector>

#include "base/statistics.hh"
#include "sim/cur_tick.hh"
#include "sim/stats.hh"

#include "mem/cache/tags/indexing_policies/base.hh"
#include "mem/cache/tags/indexing_policies/set_associative.hh"
#include "params/FlexAssociative.hh"

namespace gem5
{

class ReplaceableEntry;

class flexTable{
  public:
    uint32_t targetIdx[65];
    flexTable(){
      int i;
      for (i = 0; i < 65; i++){
        targetIdx[i] = i*8;
      }
    }
    flexTable(
      uint32_t idx0, uint32_t idx1, uint32_t idx2, uint32_t idx3,
      uint32_t idx4, uint32_t idx5, uint32_t idx6, uint32_t idx7,
      uint32_t idx8, uint32_t idx9, uint32_t idx10, uint32_t idx11,
      uint32_t idx12, uint32_t idx13, uint32_t idx14, uint32_t idx15,
      uint32_t idx16, uint32_t idx17, uint32_t idx18, uint32_t idx19,
      uint32_t idx20, uint32_t idx21, uint32_t idx22, uint32_t idx23,
      uint32_t idx24, uint32_t idx25, uint32_t idx26, uint32_t idx27,
      uint32_t idx28, uint32_t idx29, uint32_t idx30, uint32_t idx31,
      uint32_t idx32, uint32_t idx33, uint32_t idx34, uint32_t idx35,
      uint32_t idx36, uint32_t idx37, uint32_t idx38, uint32_t idx39,
      uint32_t idx40, uint32_t idx41, uint32_t idx42, uint32_t idx43,
      uint32_t idx44, uint32_t idx45, uint32_t idx46, uint32_t idx47,
      uint32_t idx48, uint32_t idx49, uint32_t idx50, uint32_t idx51,
      uint32_t idx52, uint32_t idx53, uint32_t idx54, uint32_t idx55,
      uint32_t idx56, uint32_t idx57, uint32_t idx58, uint32_t idx59,
      uint32_t idx60, uint32_t idx61, uint32_t idx62, uint32_t idx63,
      uint32_t idx64
    ){
      targetIdx[0] = idx0; targetIdx[1] = idx1; targetIdx[2] = idx2; targetIdx[3] = idx3;
      targetIdx[4] = idx4; targetIdx[5] = idx5; targetIdx[6] = idx6; targetIdx[7] = idx7;
      targetIdx[8] = idx8; targetIdx[9] = idx9; targetIdx[10] = idx10; targetIdx[11] = idx11;
      targetIdx[12] = idx12; targetIdx[13] = idx13; targetIdx[14] = idx14; targetIdx[15] = idx15;
      targetIdx[16] = idx16; targetIdx[17] = idx17; targetIdx[18] = idx18; targetIdx[19] = idx19;
      targetIdx[20] = idx20; targetIdx[21] = idx21; targetIdx[22] = idx22; targetIdx[23] = idx23;
      targetIdx[24] = idx24; targetIdx[25] = idx25; targetIdx[26] = idx26; targetIdx[27] = idx27;
      targetIdx[28] = idx28; targetIdx[29] = idx29; targetIdx[30] = idx30; targetIdx[31] = idx31;
      targetIdx[32] = idx32; targetIdx[33] = idx33; targetIdx[34] = idx34; targetIdx[35] = idx35;
      targetIdx[36] = idx36; targetIdx[37] = idx37; targetIdx[38] = idx38; targetIdx[39] = idx39;
      targetIdx[40] = idx40; targetIdx[41] = idx41; targetIdx[42] = idx42; targetIdx[43] = idx43;
      targetIdx[44] = idx44; targetIdx[45] = idx45; targetIdx[46] = idx46; targetIdx[47] = idx47;
      targetIdx[48] = idx48; targetIdx[49] = idx49; targetIdx[50] = idx50; targetIdx[51] = idx51;
      targetIdx[52] = idx52; targetIdx[53] = idx53; targetIdx[54] = idx54; targetIdx[55] = idx55;
      targetIdx[56] = idx56; targetIdx[57] = idx57; targetIdx[58] = idx58; targetIdx[59] = idx59;
      targetIdx[60] = idx60; targetIdx[61] = idx61; targetIdx[62] = idx62; targetIdx[63] = idx63;
      targetIdx[64] = idx64;
    }
};

class FlexAssociative: public BaseIndexingPolicy
{

  protected:

  public:

    /**
     * Convenience typedef.
     */
    typedef FlexAssociativeParams Params;

    /*
    * Table containing the new indexing policy
    * which determines how many ways are allocated
    * to each set.
    * The value at each index of the vector tells 
    * us the starting index of the ways allocated
    * to that set.
    * The table contains N + 1 elements, where
    * N is the total number of sets. The (N+1)th
    * elements tells us the upper bound, which is
    * the total number of cache lines available.
    */
    std::vector<uint32_t> indexTable;

    /**
     * Construct and initialize this policy.
     */
    FlexAssociative(const Params &p);

    /**
     * Destructor.
     */
    ~FlexAssociative() {};

    /**
     * Associate a pointer to an entry to its physical counterpart.
     *
     * @param entry The entry pointer.
     * @param index An unique index for the entry.
     */
    virtual void setEntry(ReplaceableEntry* entry, const uint64_t index) override;

    /**
     * Apply a hash function to calculate address set.
     *
     * @param addr The address to calculate the set for.
     * @return The set index for given combination of address and way.
     */
    virtual uint32_t extractSet(const Addr addr) const;

    /**
     * Find all possible entries for insertion and replacement of an address.
     * Should be called immediately before ReplacementPolicy's findVictim()
     * not to break cache resizing.
     * Returns entries in all ways belonging to the set of the address.
     *
     * @param addr The addr to a find possible entries for.
     * @return The possible entries.
     */
    std::vector<ReplaceableEntry*> getPossibleEntries(const Addr addr) override;

    /**
     * Regenerate an entry's address from its tag and assigned set and way.
     *
     * @param tag The tag bits.
     * @param entry The entry.
     * @return the entry's original addr value.
     */
    Addr regenerateAddr(const Addr tag, const ReplaceableEntry* entry) const
                                                                   override;

    struct AssocStats: public statistics::Group
    {

      //void regStats() override;

      statistics::Histogram setAccessHist;
      //statistics::Histogram tagMissHist;

      AssocStats(statistics::Group *parent, uint32_t numBins);

    } stats;

};

} // namespace gem5

#endif // __MEM_CACHE_INDEXING_POLICIES_FLEX_ASSOCIATIVE_HH__