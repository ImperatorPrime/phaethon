/* Phaethon - A FLOSS resource explorer for BioWare's Aurora engine games
 *
 * Phaethon is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * Phaethon is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * Phaethon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Phaethon. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file common/huffman.cpp
 *  Decompressing Huffman codes.
 */

#include <cassert>

#include "common/huffman.h"
#include "common/util.h"
#include "common/error.h"
#include "common/bitstream.h"

namespace Common {

Huffman::Symbol::Symbol(uint32 c, uint32 s) : code(c), symbol(s) {
}


Huffman::Huffman(const HuffmanTable &table) {
	init(table.maxLength, table.codeCount, table.codes, table.lengths, table.symbols);
}

Huffman::Huffman(uint8 maxLength, uint32 codeCount, const uint32 *codes,
                 const uint8 *lengths, const uint32 *symbols) {

	init(maxLength, codeCount, codes, lengths, symbols);
}

void Huffman::init(uint8 maxLength, uint32 codeCount, const uint32 *codes,
                   const uint8 *lengths, const uint32 *symbols) {

	assert(codeCount > 0);

	assert(codes);
	assert(lengths);

	if (maxLength == 0)
		for (uint32 i = 0; i < codeCount; i++)
			maxLength = MAX(maxLength, lengths[i]);

	assert(maxLength <= 32);

	_codes.resize(maxLength);
	_symbols.resize(codeCount);

	for (uint32 i = 0; i < codeCount; i++) {
		// The symbol. If none were specified, just assume it's identical to the code index
		uint32 symbol = symbols ? symbols[i] : i;

		// Put the code and symbol into the correct list
		_codes[lengths[i] - 1].push_back(Symbol(codes[i], symbol));

		// And put the pointer to the symbol/code struct into the symbol list.
		_symbols[i] = &_codes[lengths[i] - 1].back();
	}
}

Huffman::~Huffman() {
}

void Huffman::setSymbols(const uint32 *symbols) {
	for (uint32 i = 0; i < _symbols.size(); i++)
		_symbols[i]->symbol = symbols ? *symbols++ : i;
}

uint32 Huffman::getSymbol(BitStream &bits) const {
	uint32 code = 0;

	for (uint32 i = 0; i < _codes.size(); i++) {
		bits.addBit(code, i);

		for (CodeList::const_iterator cCode = _codes[i].begin(); cCode != _codes[i].end(); ++cCode)
			if (code == cCode->code)
				return cCode->symbol;
	}

	throw Exception("Unknown Huffman code");
}

} // End of namespace Common
