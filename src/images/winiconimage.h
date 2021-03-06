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

/** @file images/winiconimage.h
 *  Decoding Windows icon and cursor files (.ICO and .CUR).
 */

#ifndef IMAGES_WINICONIMAGE_H
#define IMAGES_WINICONIMAGE_H

#include "images/decoder.h"

namespace Common {
	class SeekableReadStream;
}

namespace Images {

/** Windows cursor. */
class WinIconImage : public Decoder {
public:
	WinIconImage(Common::SeekableReadStream &cur);
	~WinIconImage();

	int getHotspotX() const;
	int getHotspotY() const;

private:
	uint16 _imageCount;
	uint16 _iconType;

	int _hotspotX;
	int _hotspotY;

	// Loading helpers
	void load(Common::SeekableReadStream &cur);
	void readHeader(Common::SeekableReadStream &cur);
	void readData(Common::SeekableReadStream &cur);
};

} // End of namespace Images

#endif // IMAGES_WINICONIMAGE_H
