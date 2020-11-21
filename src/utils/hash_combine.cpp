/*
 * hash_combine.cpp
 * This file is part of ProbabilisticTraceAlignment
 *
 * Copyright (C) 2020 - Giacomo Bergami
 *
 * ProbabilisticTraceAlignment is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ProbabilisticTraceAlignment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProbabilisticTraceAlignment. If not, see <http://www.gnu.org/licenses/>.
 */

 
//
// Created by giacomo on 20/10/20.
//

#include <utils/hash_combine.h>

uint32_t distribute(const uint32_t &n) {
    uint32_t p = 0x55555555ul; // pattern of alternating 0 and 1
    uint32_t c = 3423571495ul; // random uneven integer constant;
    return c*xorshift(p*xorshift(n,16),16);
}

uint64_t hash(const uint64_t &n) {
    uint64_t p = 0x5555555555555555;     // pattern of alternating 0 and 1
    uint64_t c = 17316035218449499591ull;// random uneven integer constant;
    return c*xorshift(p*xorshift(n,32),32);
}
