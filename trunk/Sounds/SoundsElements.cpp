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

#include "SoundsElements.h"

SoundsDefinition::SoundsDefinition(bool verbose) : SoundsElement(verbose) {}

SoundsDefinition::~SoundsDefinition() {}

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
	
	wxLogDebug("Invalid chance %d", mChance);
	return -1;
}

void SoundsDefinition::SetChance(short chance)
{
	if (chance < 0 || chance > 10)
		wxLogDebug("Invalid chance %d", mChance);
	
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
		wxLogError("[SoundsDefinition] incorrect Behavior/Chance");
		return buffer;
	}
	
	wxInt32		low_pitch_fixed, high_pitch_fixed;
	
	low_pitch_fixed = buffer.ReadLong();
	high_pitch_fixed = buffer.ReadLong();
	
	mLowPitch = ((low_pitch_fixed >> 16) & 0xffff) + (float)(low_pitch_fixed & 0xffff) / 65536.0;	// convert fixed point [0,1] to float
	mHighPitch = ((high_pitch_fixed >> 16) & 0xffff) + (float)(high_pitch_fixed & 0xffff) / 65536.0;	// convert fixed point [0,1] to float

	short permutations = buffer.ReadShort();
	
	if (permutations < 0 || permutations > MAXIMUM_PERMUTATIONS_PER_SOUND) {
		wxLogError("[SoundsDefinition] incorrect permutation count");
		return buffer;
	}
	
	mPermutationsPlayed = buffer.ReadUShort();
	int groupOffset = buffer.ReadLong();
	int singleLength = buffer.ReadLong();
	int totalLength = buffer.ReadLong();
	
	if ((unsigned int)(groupOffset + totalLength) > buffer.Size()) {
		wxLogError("[SoundsDefinition] incorrect group offset / total length");
		return buffer;
	}
	
	std::vector<long> soundOffsets;
	
	soundOffsets.resize(MAXIMUM_PERMUTATIONS_PER_SOUND);
	for (unsigned int i = 0; i < MAXIMUM_PERMUTATIONS_PER_SOUND; i++) {
		soundOffsets[i] = buffer.ReadLong();
	}
	
	mLastPlayed = buffer.ReadULong();
	
	if (IsVerbose()) {
		wxLogDebug("[SoundsDefinition] Sound Code:			%d", mSoundCode);
		wxLogDebug("[SoundsDefinition] Behavior Index:		%d", mBehaviorIndex);
		wxLogDebug("[SoundsDefinition] Flags:				%d", mFlags);
		wxLogDebug("[SoundsDefinition] Chance:				%d", mChance);
		wxLogDebug("[SoundsDefinition] Low Pitch:			%f", mLowPitch);
		wxLogDebug("[SoundsDefinition] High Pitch:			%f", mHighPitch);
		wxLogDebug("[SoundsDefinition] Permutations:		%d", permutations);
		wxLogDebug("[SoundsDefinition] Permutations Played:	%d", mPermutationsPlayed);
		wxLogDebug("[SoundsDefinition] Group Offset:		%d", groupOffset);
		wxLogDebug("[SoundsDefinition] Single Length:		%d", singleLength);
		wxLogDebug("[SoundsDefinition] Total Length:		%d", totalLength);
		wxLogDebug("[SoundsDefinition] Last Played:			%d", mLastPlayed);
	}
	
	// Now we load actual sound data
	unsigned int oldpos = buffer.Position();
	
	for (short i = 0; i < permutations; i++) {
	
		unsigned int size = 0;
		if (permutations == 1)
			size = singleLength;
		else if (i == permutations - 1)
			size = totalLength - soundOffsets[i];
		else
			size = soundOffsets[i + 1] - soundOffsets[i];
		
		BigEndianBuffer *sndbuffer = new BigEndianBuffer(size);
		
		buffer.Position(groupOffset + soundOffsets[i]);
		buffer.ReadBlock(sndbuffer->Size(), sndbuffer->Data());
		
		mSounds.push_back(sndbuffer);
	}

	buffer.Position(oldpos);
	
	mGoodData = true;
	return buffer;
}

BigEndianBuffer* SoundsDefinition::GetPermutation(unsigned int permutation_index)
{
	if (permutation_index > mSounds.size())
		return NULL;
	
	return mSounds[permutation_index];
}