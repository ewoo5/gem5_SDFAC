# Copyright (c) 2018 Inria
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject

class BaseIndexingPolicy(SimObject):
    type = 'BaseIndexingPolicy'
    abstract = True
    cxx_class = 'gem5::BaseIndexingPolicy'
    cxx_header = "mem/cache/tags/indexing_policies/base.hh"

    # Get the size from the parent (cache)
    size = Param.MemorySize(Parent.size, "capacity in bytes")

    # Get the entry size from the parent (tags)
    entry_size = Param.Int(Parent.entry_size, "entry size in bytes")

    # Get the associativity
    assoc = Param.Int(Parent.assoc, "associativity")

class SetAssociative(BaseIndexingPolicy):
    type = 'SetAssociative'
    cxx_class = 'gem5::SetAssociative'
    cxx_header = "mem/cache/tags/indexing_policies/set_associative.hh"

class SkewedAssociative(BaseIndexingPolicy):
    type = 'SkewedAssociative'
    cxx_class = 'gem5::SkewedAssociative'
    cxx_header = "mem/cache/tags/indexing_policies/skewed_associative.hh"

class FlexAssociative(BaseIndexingPolicy):
    type = 'FlexAssociative'
    cxx_class = 'gem5::FlexAssociative'
    cxx_header = "mem/cache/tags/indexing_policies/flex_associative.hh"
    """
    Default Cache-Way allocation for 64-sets
    [0, 8, 16, 24,
    32, 40, 48, 56,
    64, 72, 80, 88,
    96, 104, 112, 120,
    128, 136, 144, 152,
    160, 168, 176, 184,
    192, 200, 208, 216,
    224, 232, 240, 248,
    256, 264, 272, 280,
    288, 296, 304, 312,
    320, 328, 336, 344,
    352, 360, 368, 376,
    384, 392, 400, 408,
    416, 424, 432, 440,
    448, 456, 464, 472,
    480, 488, 496, 504, 512]
    """
    index_table = VectorParam.Unsigned([0, 8, 16, 24,
                32, 40, 48, 56,
                64, 72, 80, 88,
                96, 104, 112, 120,
                128, 136, 144, 152,
                160, 168, 176, 184,
                192, 200, 208, 216,
                224, 232, 240, 248,
                256, 264, 272, 280,
                288, 296, 304, 312,
                320, 328, 336, 344,
                352, 360, 368, 376,
                384, 392, 400, 408,
                416, 424, 432, 440,
                448, 456, 464, 472,
                480, 488, 496, 504, 512], "Flex Index Table")
