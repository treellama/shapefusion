/*
 * This file is part of ShapeFusion (Copyright 2000 Tito Dal Canton)
 *
 * ShapeFusion is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ShapeFusion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ShapeFusion; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if wxUSE_STD_IOSTREAM
    #include "wx/ioswrap.h"
#else
    #include "wx/txtstrm.h"
#endif
#include "wx/filename.h"
#include "wx/sound.h"

#include "SoundsElements.h"
#include "../LittleEndianBuffer.h"

AppleSoundHeader::AppleSoundHeader(unsigned int sndSize, bool verbose) : SoundsElement(verbose)
{
	mData = new BigEndianBuffer(sndSize);
}

AppleSoundHeader::~AppleSoundHeader()
{
	if (mData)
		delete mData;
}

bool AppleSoundHeader::SaveToWave(wxString path)
{
	unsigned int	wavChannels, wavSampleRate, wavBitsPerSample, wavFrames;
	unsigned long	sampleRate, loopStart, loopEnd;
	unsigned char	baseFrequency;
	
	mData->Position(20);
	unsigned char	encode = mData->ReadUChar();
	switch (encode) {
		case standardSoundHeader:
			// standard sound header
			mData->Position(4);
			wavFrames = mData->ReadULong();
			sampleRate = mData->ReadULong();
			loopStart = mData->ReadULong();
			loopEnd = mData->ReadULong();
			mData->Position(21);
			baseFrequency = mData->ReadUChar();
			wavChannels = 1;
			wavSampleRate = sampleRate >> 16;
			wavBitsPerSample = 8;
			break;
		case extendedSoundHeader:
			// extended sound header
			mData->Position(4);
			wavChannels = mData->ReadULong();
			sampleRate = mData->ReadULong();
			loopStart = mData->ReadULong();
			loopEnd = mData->ReadULong();
			mData->Position(21);
			baseFrequency = mData->ReadUChar();
			wavFrames = mData->ReadULong();
			mData->Position(48);
			wavBitsPerSample = mData->ReadUShort();
			wavSampleRate = sampleRate >> 16;
			mData->Position(64);
			break;
		case compressedSoundHeader:
			// compressed sound header
			wxLogError(wxT("[AppleSoundHeader] Compressed sounds not yet supported, can not export this sound."));
			return false;
			break;
		default:
			wxLogError(wxT("[AppleSoundHeader] Unknown sound encoding %.2x, can not export this sound."), encode);
			return false;
			break;
	}

#if 0
	// check alignment of loop* fields
	// Apple docs say they are to be interpreted in byte values,
	// so they should be frame-aligned. Marathon sound values are not
	// like that, they seem to be in units of frames instead. This
	// triggers the following error dialog almost always. A1 interprets
	// those fields as byte values, and this causes problems with at
	// least one sound from rubicon. WTF? FIXME!
	if ((loopStart % (wavBitsPerSample / 8)) != 0 ||
		(loopEnd % (wavBitsPerSample / 8)) != 0)
		wxLogError(wxT("[AppleSoundHeader] This sound has broken loop values, it could cause problems when playing the game. loopStart=%lu, loopEnd=%lu, frameCount=%lu, bitsPerSample=%u"),
					loopStart, loopEnd, wavFrames, wavBitsPerSample);
#endif

	// write the WAVE file
	LittleEndianBuffer	riffHeader(12),
						fmtChunk(24),
						dataChunk(8 + wavFrames * wavBitsPerSample / 8);
	std::ofstream		stream(path.fn_str(), std::ios::binary);
	
	if (!stream.good()) {
		wxLogError(wxT("[AppleSoundHeader] Error opening stream"));
		return false;
	}
	// RIFF chunk
	riffHeader.WriteULong('FFIR');				// RIFF signature
	riffHeader.WriteULong(riffHeader.Size() + fmtChunk.Size() + dataChunk.Size() - 8);	// total file size
	riffHeader.WriteULong('EVAW');				// WAVE signature
	// format chunk
	fmtChunk.WriteULong(' tmf');				// fmt signature
	fmtChunk.WriteULong(fmtChunk.Size() - 8);	// chunk size
	fmtChunk.WriteUShort(1);					// PCM data
	fmtChunk.WriteUShort(wavChannels);
	fmtChunk.WriteULong(wavSampleRate);
	fmtChunk.WriteULong(wavSampleRate * wavChannels * wavBitsPerSample / 8);	// byte rate
	fmtChunk.WriteUShort(wavChannels * wavBitsPerSample / 8);	// block align
	fmtChunk.WriteUShort(wavBitsPerSample);
	// data chunk
	dataChunk.WriteULong('atad');				// data signature
	dataChunk.WriteULong(dataChunk.Size() - 8);	// chunk size
	if (wavBitsPerSample == 8) {
		dataChunk.WriteBlock(wavFrames * wavChannels, mData->Data() + mData->Position());
	} else if (wavBitsPerSample == 16) {
		// need to flip endian order
		for (unsigned int i = 0; i < wavFrames * wavChannels; i++)
			dataChunk.WriteShort(mData->ReadShort());
	}
	// actual write
	stream.write((const char *)riffHeader.Data(), riffHeader.Size());
	stream.write((const char *)fmtChunk.Data(), fmtChunk.Size());
	stream.write((const char *)dataChunk.Data(), dataChunk.Size());
	stream.close();
	return true;
}

bool AppleSoundHeader::SaveToAiff(wxString path)
{
	unsigned int	aiffChannels, aiffSampleRate, aiffBitsPerSample, aiffFrames;
	long			length, sampleRate, numChannels, numFrames;
	short			sampleSize;
	unsigned char	baseFrequency;
	
	mData->Position(20);
	unsigned char	encode = mData->ReadUChar();
	switch (encode) {
		case standardSoundHeader:
		// standard sound header
		mData->Position(4);
		length = mData->ReadLong();
		sampleRate = mData->ReadLong();
		mData->Position(21);
		baseFrequency = mData->ReadUChar();
		// <length> 8-bit signed samples follow
		aiffChannels = 1;
		aiffSampleRate = (unsigned int)(sampleRate >> 16);
		aiffBitsPerSample = 8;
		aiffFrames = (unsigned int)length;
		break;
	case extendedSoundHeader:
		// extended sound header
		mData->Position(4);
		numChannels = mData->ReadLong();
		sampleRate = mData->ReadLong();
		mData->Position(21);
		baseFrequency = mData->ReadUChar();
		numFrames = mData->ReadLong();
		mData->Position(48);
		sampleSize = mData->ReadShort();
		aiffChannels = (unsigned int)numChannels;
		aiffSampleRate = (unsigned int)(sampleRate >> 16);
		aiffBitsPerSample = (unsigned int)sampleSize;
		aiffFrames = (unsigned int)numFrames;
		mData->Position(64);
		break;
	case compressedSoundHeader:
		// compressed sound header
		wxLogError(wxT("[AppleSoundHeader] Compressed sounds not yet supported, can not export this sound."));
		return false;
		break;
	default:
		wxLogError(wxT("[AppleSoundHeader] Unknown sound encoding %.2x, can not export this sound."), encode);
		return false;
		break;
	}
	
	// write the AIFF file
	BigEndianBuffer		formHeader(12),
						commChunk(24),
						ssndChunk(16 + aiffFrames * aiffBitsPerSample / 8);
	std::ofstream		stream(path.fn_str(), std::ios::binary);
	
	if (!stream.good()) {
		wxLogError(wxT("[AppleSoundHeader] Error opening stream"));
		return false;
	}
	// FORM AIFF chunk
	formHeader.WriteULong('FORM');				// groupID
	formHeader.WriteULong(formHeader.Size() + commChunk.Size() + ssndChunk.Size() - 8);
												// fileSize
	formHeader.WriteULong('AIFF');				// typeID
	
	// Common chunk
	commChunk.WriteULong('COMM');				// chunkID
	commChunk.WriteULong(commChunk.Size() - 8);	// chunkSize
	commChunk.WriteShort(aiffChannels);			// numChannels
	commChunk.WriteULong(aiffFrames);			// numSampleFrames
	commChunk.WriteShort(aiffBitsPerSample);	// sampleSize
	// FIXME this is actually an 80 bit extended floating point value.
	// See MPlayer/libavutil/intfloat_readwrite.* for code about this.
	commChunk.WriteLong(aiffSampleRate);		// sampleRate
	
	// Sound Data chunk
	ssndChunk.WriteULong('SSND');				// chunkID
	ssndChunk.WriteULong(ssndChunk.Size() - 8);	// chunkSize
	ssndChunk.WriteULong(0);					// offset
	ssndChunk.WriteULong(0);					// blockSize
	// FIXME?
	ssndChunk.WriteBlock(aiffFrames * aiffChannels, mData->Data() + mData->Position());

	// actual write
	stream.write((const char *)formHeader.Data(), formHeader.Size());
	stream.write((const char *)commChunk.Data(), commChunk.Size());
	stream.write((const char *)ssndChunk.Data(), ssndChunk.Size());
	stream.close();
	return true;
}

void AppleSoundHeader::PlaySound(void)
{
	wxString	tempfile = wxFileName::CreateTempFileName(wxT("sf"));

	wxBeginBusyCursor();
	SaveToWave(tempfile);
	wxSound(tempfile).Play(wxSOUND_SYNC);
	wxRemoveFile(tempfile);
	wxEndBusyCursor();
}

unsigned int AppleSoundHeader::Size(void)
{
	return mData->Size();
}

unsigned char* AppleSoundHeader::Data(void)
{
	return mData->Data();
}

SoundsDefinition::SoundsDefinition(bool verbose) : SoundsElement(verbose)
{
	mSoundCode = -1;
	mBehaviorIndex = _sound_is_quiet;
	mFlags = 0;
	mChance = _always;
	mSounds.resize(0);
}

SoundsDefinition::~SoundsDefinition() {}

bool SoundsDefinition::HaveSameAttributesAs(const SoundsDefinition& right) const
{
	return (
			(mSoundCode == right.mSoundCode) &&
			(mBehaviorIndex == right.mBehaviorIndex) &&
			(mFlags == right.mFlags) &&
			(mChance == right.mChance) &&
			(mLowPitch == right.mLowPitch) &&
			(mHighPitch == right.mHighPitch)
		   );
}

bool SoundsDefinition::HaveSameSoundsAs(const SoundsDefinition& right) const
{
	bool isSame = ((&right) != this) && (mSounds.size() == right.mSounds.size());
	
	unsigned int i = 0;
	while (isSame && (i < mSounds.size())) {
		isSame = isSame && mSounds[i] == right.mSounds[i];
	}
	
	return isSame;
}

bool SoundsDefinition::operator== (const SoundsDefinition&	right) const
{
	return (HaveSameAttributesAs(right) &&
			HaveSameSoundsAs(right));
}


bool SoundsDefinition::operator!=(const SoundsDefinition& right) const
{
	return (!HaveSameAttributesAs(right) ||
			!HaveSameSoundsAs(right));
}

unsigned int SoundsDefinition::GetSizeInFile(void)
{
	unsigned int size = SIZEOF_sound_definition;
	
	for (unsigned int i = 0; i < mSounds.size(); i++)
		size += mSounds[i]->Size();
	
	return size;
}

short SoundsDefinition::GetChance(void) const
{
	for (int i = 0; i < 10; i++)
		if (mChance == 32768*i/10) return i;
	
	wxLogDebug(wxT("Invalid chance %d"), mChance);
	return -1;
}

void SoundsDefinition::SetChance(short chance)
{
	if (chance < 0 || chance > 10)
		wxLogDebug(wxT("Invalid chance %d"), mChance);
	
	mChance = 32768*chance/10;
}

BigEndianBuffer& SoundsDefinition::SaveObject(BigEndianBuffer& buffer, unsigned int& offset)
{
	unsigned int oldpos = buffer.Position();
	
	// We write sound_definition header
	buffer.WriteShort(mSoundCode);
	
	buffer.WriteShort(mBehaviorIndex);
	buffer.WriteUShort(mFlags);
	
	buffer.WriteUShort(mChance);
	
	float			low_pitch_integer, low_pitch_fractional,
					high_pitch_integer, high_pitch_fractional;
	long			low_pitch = 0, high_pitch = 0;
	
	// float to fixed
	low_pitch_fractional = modff(mLowPitch, &low_pitch_integer);
	low_pitch |= (((short)low_pitch_integer) << 16) & 0xffff0000;
	low_pitch |= (short)roundf(low_pitch_fractional * 0xffff) & 0x0000ffff;
	
	high_pitch_fractional = modff(mHighPitch, &high_pitch_integer);
	high_pitch |= (((short)high_pitch_integer) << 16) & 0xffff0000;
	high_pitch |= (short)roundf(high_pitch_fractional * 0xffff) & 0x0000ffff;
	
	buffer.WriteLong(low_pitch);
	buffer.WriteLong(high_pitch);
	
	buffer.WriteShort(mSounds.size());
	buffer.WriteUShort(mPermutationsPlayed);
	
	// We need to recalculate those fields...
	unsigned long single_length = (mSounds.size() >= 1 ? mSounds[0]->Size() : 0);
	unsigned long total_length = single_length;
	std::vector<long> soundOffsets;
	soundOffsets.push_back(0);
	
	// ... and the corresponding offsets ...
	for (unsigned int i = 1; i < mSounds.size(); i++) {
		soundOffsets.push_back(total_length);
		total_length += mSounds[i]->Size();
	}
	
	// ... and write everything
	buffer.WriteLong(offset);
	buffer.WriteLong(single_length);
	buffer.WriteLong(total_length);
	
	// We have to pad with zeroes, as engine always expect MAXIMUM_PERMUTATIONS_PER_SOUND sound offsets...
	for (unsigned int i = 0; i < MAXIMUM_PERMUTATIONS_PER_SOUND; i++) {
		if (i < soundOffsets.size())
			buffer.WriteLong(soundOffsets[i]);
		else
			buffer.WriteLong(0);
	}
	
	buffer.WriteULong(mLastPlayed);
	
	// Now, we write actual sound data where it belongs...
	buffer.Position(offset);
	
	for (unsigned int i = 0; i < mSounds.size(); i++) {
		buffer.WriteBlock(mSounds[i]->Size(), mSounds[i]->Data());
	}
	
	// We put back position to the end of the written sound_definition...
	buffer.Position(oldpos + SIZEOF_sound_definition);
	// ... and add our total_length to the offset, so that next invocation
	// writes its sound data at the correct place.
	offset += total_length;
	
	return buffer;
}

BigEndianBuffer& SoundsDefinition::LoadObject(BigEndianBuffer& buffer)
{
	mSoundCode = buffer.ReadShort();

	mBehaviorIndex = buffer.ReadShort();
	mFlags = buffer.ReadUShort();
	
	mChance = buffer.ReadUShort();
	
	if ((mBehaviorIndex > NUMBER_OF_SOUND_BEHAVIOR_DEFINITIONS) || (mChance > _ten_percent)) {
		wxLogError(wxT("[SoundsDefinition] incorrect Behavior/Chance"));
		return buffer;
	}
	
	wxInt32		low_pitch_fixed, high_pitch_fixed;
	
	low_pitch_fixed = buffer.ReadLong();
	high_pitch_fixed = buffer.ReadLong();
	
	mLowPitch = ((low_pitch_fixed >> 16) & 0xffff) + (float)(low_pitch_fixed & 0xffff) / 65536.0;	// convert fixed point [0,1] to float
	mHighPitch = ((high_pitch_fixed >> 16) & 0xffff) + (float)(high_pitch_fixed & 0xffff) / 65536.0;	// convert fixed point [0,1] to float

	short permutations = buffer.ReadShort();
	
	if (permutations < 0 || permutations > MAXIMUM_PERMUTATIONS_PER_SOUND) {
		wxLogError(wxT("[SoundsDefinition] incorrect permutation count"));
		return buffer;
	}
	
	mPermutationsPlayed = buffer.ReadUShort();
	int groupOffset = buffer.ReadULong();
	int singleLength = buffer.ReadULong();
	int totalLength = buffer.ReadULong();
	
	// Bug fix for RED Sounds : When groupOffset is out of bounds, consider sound empty.
	if (groupOffset < 0)
		permutations = 0;
	
	if (permutations != 0 && (unsigned int)(groupOffset + totalLength) > buffer.Size()) {
		wxLogError(wxT("[SoundsDefinition] incorrect group offset / total length (%d/%d)"), groupOffset, totalLength);
		return buffer;
	}
	
	std::vector<long> soundOffsets;
	
	soundOffsets.resize(MAXIMUM_PERMUTATIONS_PER_SOUND);
	for (unsigned int i = 0; i < MAXIMUM_PERMUTATIONS_PER_SOUND; i++) {
		soundOffsets[i] = buffer.ReadLong();
	}
	
	mLastPlayed = buffer.ReadULong();
	
	if (IsVerbose()) {
		wxLogDebug(wxT("[SoundsDefinition] Sound Code:			%d"), mSoundCode);
		wxLogDebug(wxT("[SoundsDefinition] Behavior Index:		%d"), mBehaviorIndex);
		wxLogDebug(wxT("[SoundsDefinition] Flags:				%d"), mFlags);
		wxLogDebug(wxT("[SoundsDefinition] Chance:				%d"), mChance);
		wxLogDebug(wxT("[SoundsDefinition] Low Pitch:			%f"), mLowPitch);
		wxLogDebug(wxT("[SoundsDefinition] High Pitch:			%f"), mHighPitch);
		wxLogDebug(wxT("[SoundsDefinition] Permutations:		%d"), permutations);
		wxLogDebug(wxT("[SoundsDefinition] Permutations Played:	%d"), mPermutationsPlayed);
		wxLogDebug(wxT("[SoundsDefinition] Group Offset:		%d"), groupOffset);
		wxLogDebug(wxT("[SoundsDefinition] Single Length:		%d"), singleLength);
		wxLogDebug(wxT("[SoundsDefinition] Total Length:		%d"), totalLength);
		wxLogDebug(wxT("[SoundsDefinition] Last Played:			%d"), mLastPlayed);
	}
	
	// Now we load actual sound data
	// We save our current position, 'coz we need to restore it at the end
	unsigned int oldpos = buffer.Position();

	for (short i = 0; i < permutations; i++) {
		unsigned int size = 0;
		if (permutations == 1)
			size = singleLength;
		else if (i == permutations - 1)
			size = totalLength - soundOffsets[i];
		else
			size = soundOffsets[i + 1] - soundOffsets[i];
		
		AppleSoundHeader *sndbuffer = new AppleSoundHeader(size, IsVerbose());

		buffer.Position(groupOffset + soundOffsets[i]);
		buffer.ReadBlock(sndbuffer->Size(), sndbuffer->Data());
		mSounds.push_back(sndbuffer);
	}

	buffer.Position(oldpos);
	
	mGoodData = true;
	return buffer;
}

AppleSoundHeader* SoundsDefinition::GetPermutation(unsigned int permutation_index)
{
	if (permutation_index >= mSounds.size())
		return NULL;
	
	return mSounds[permutation_index];
}
