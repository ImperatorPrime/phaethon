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

/** @file sound/decoders/wave.cpp
 *  Decoding RIFF WAVE (Resource Interchange File Format Waveform).
 */

#include "common/error.h"
#include "common/util.h"
#include "common/stream.h"

#include "sound/audiostream.h"
#include "sound/decoders/wave.h"
#include "sound/decoders/adpcm.h"
#include "sound/decoders/pcm.h"
#include "sound/decoders/wave_types.h"

namespace Sound {

RewindableAudioStream *makeWAVStream(Common::SeekableReadStream *stream, bool disposeAfterUse) {
	if (stream->readUint32BE() != MKTAG('R', 'I', 'F', 'F'))
		throw Common::Exception("makeWAVStream(): No 'RIFF' header");

	/* uint32 fileSize = */ stream->readUint32LE();

	if (stream->readUint32BE() != MKTAG('W', 'A', 'V', 'E'))
		throw Common::Exception("makeWAVStream(): No 'WAVE' RIFF type");

	if (stream->readUint32BE() != MKTAG('f', 'm', 't', ' '))
		throw Common::Exception("makeWAVStream(): No 'fmt ' chunk");

	uint32 fmtLength = stream->readUint32LE();
	if (fmtLength < 16) // A valid fmt chunk always contains at least 16 bytes
		throw Common::Exception("makeWAVStream(): Invalid wave format size %d", fmtLength);

	// Now parse the WAVEFORMAT(EX) structure
	uint16 compression = stream->readUint16LE();
	uint16 channels = stream->readUint16LE();
	uint32 sampleRate = stream->readUint32LE();
	/* uint32 avgBytesPerSecond = */ stream->readUint32LE();
	uint16 blockAlign = stream->readUint16LE();
	uint16 bitsPerSample = stream->readUint16LE();

	// Skip over the rest of the fmt chunk.
	stream->skip(fmtLength - 16);

	// Skip over all chunks until we hit the data
	for (;;) {
		if (stream->readUint32BE() == MKTAG('d', 'a', 't', 'a'))
			break;

		if (stream->eos())
			throw Common::Exception("makeWAVStream(): Unexpected eos");

		stream->skip(stream->readUint32LE());
	}

	uint32 size = stream->readUint32LE();
	Common::SeekableSubReadStream *subStream = new Common::SeekableSubReadStream(stream, stream->pos(), stream->pos() + size, disposeAfterUse);

	// Return the decoder we need
	switch (compression) {
	case kWavePCM: {
		byte flags = 0;

		// 8 bit data is unsigned, 16 bit data signed
		if (bitsPerSample == 8)
			flags |= FLAG_UNSIGNED;
		else if (bitsPerSample == 16)
			flags |= (FLAG_16BITS | FLAG_LITTLE_ENDIAN);
		else
			throw Common::Exception("makeWAVStream(): Unsupported PCM bits per sample %d", bitsPerSample);

		return makePCMStream(subStream, sampleRate, flags, channels, true);
	}
	case kWaveMSIMAADPCM:
	case kWaveMSIMAADPCM2:
		return makeADPCMStream(subStream, true, size, kADPCMMSIma, sampleRate, channels, blockAlign);
	case kWaveMSADPCM:
		return makeADPCMStream(subStream, true, size, kADPCMMS, sampleRate, channels, blockAlign);
	}

	throw Common::Exception("makeWAVStream(): Unhandled wave type 0x%04x", compression);
	return 0;
}

} // End of namespace Sound
