/*
 * FIPS 180-2 SHA-224/256/384/512 implementation
 * Last update: 02/02/2007
 * Issue date:  04/30/2005
 *
 * Copyright (C) 2013, Con Kolivas <kernel@kolivas.org>
 * Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include <cstdint>
#include <array>

constexpr std::size_t SHA256_DIGEST_SIZE = 256 / 8;
constexpr std::size_t SHA256_BLOCK_SIZE = 512 / 8;

constexpr uint32_t SHFR(uint32_t x, int n) {
    return (x >> n);
}

constexpr uint32_t ROTR(uint32_t x, int n) {
    return ((x >> n) | (x << ((sizeof(x) << 3) - n)));
}

constexpr uint32_t CH(uint32_t x, uint32_t y, uint32_t z) {
    return ((x & y) ^ (~x & z));
}

constexpr uint32_t MAJ(uint32_t x, uint32_t y, uint32_t z) {
    return ((x & y) ^ (x & z) ^ (y & z));
}

constexpr uint32_t SHA256_F1(uint32_t x) {
    return (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22));
}

constexpr uint32_t SHA256_F2(uint32_t x) {
    return (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25));
}

constexpr uint32_t SHA256_F3(uint32_t x) {
    return (ROTR(x, 7) ^ ROTR(x, 18) ^ SHFR(x, 3));
}

constexpr uint32_t SHA256_F4(uint32_t x) {
    return (ROTR(x, 17) ^ ROTR(x, 19) ^ SHFR(x, 10));
}

struct SHA256Context {
    unsigned int tot_len;
    unsigned int len;
    std::array<unsigned char, 2 * SHA256_BLOCK_SIZE> block;
    std::array<uint32_t, 8> h;
};

extern std::array<uint32_t, 64> sha256_k;

void sha256_init(SHA256Context& ctx);
void sha256_update(SHA256Context& ctx, const unsigned char* message, unsigned int len);
void sha256_final(SHA256Context& ctx, unsigned char* digest);
void sha256(const unsigned char* message, unsigned int len, unsigned char* digest);
