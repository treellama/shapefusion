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

#include <sndfile.h>

#define FOUR_CHARS_TO_INT(a,b,c,d) (((unsigned int)(a) << 24) | ((unsigned int)(b) << 16) | ((unsigned int)(c) << 8) | (unsigned int)(d))

struct sf_adapter
{
public:
	sf_adapter(std::vector<unsigned char>& data) : data_(data), p_(data.begin()) { }

	static sf_count_t get_filelen(void *pv) {
		return ((sf_adapter*) pv)->_get_filelen();
	}

	static sf_count_t seek(sf_count_t offset, int whence, void *pv) {
		return ((sf_adapter*) pv)->_seek(offset, whence);
	}

	static sf_count_t read(void *ptr, sf_count_t count, void *pv) {
		return ((sf_adapter*) pv)->_read(ptr, count);
	}

	static sf_count_t write(const void *ptr, sf_count_t count, void *pv) {
		return ((sf_adapter*) pv)->_write(ptr, count);
	}

	static sf_count_t tell(void *pv) {
		return ((sf_adapter*) pv)->_tell();
	}

private:
	std::vector<unsigned char>& data_;
	std::vector<unsigned char>::iterator p_;

	sf_count_t _get_filelen() {
		return data_.size();
	}

	sf_count_t _seek(sf_count_t offset, int whence) {
		if (whence == SEEK_SET)
			p_ = data_.begin() + offset;
		else if (whence == SEEK_END)
			p_ = data_.end() - offset;
		else if (whence == SEEK_CUR)
			p_ += offset;

		return ((p_ >= data_.begin() && p_ <= data_.end()) ? 0 : -1);
	}

	sf_count_t _read(void *ptr, sf_count_t count) {
		if (p_ >= data_.end()) return -1;
		char *dst = reinterpret_cast<char *>(ptr);
		int i = 0;
		for (; i < count && p_ < data_.end(); ++i)
		{
			*(dst++) = *(p_++);
		}

		return i;
	}

	sf_count_t _write(const void *ptr, sf_count_t count) {
		if (p_ >= data_.end()) return -1;

		const char *src = reinterpret_cast<const char *>(ptr);
		int i = 0;
		for (; i < count && p_ < data_.end(); ++i)
		{
			*(p_++) = *(src++);
		}

		return i;
	}

	sf_count_t _tell() {
		return p_ - data_.begin();
	}

};

AppleSoundHeader::AppleSoundHeader(bool verbose): SoundsElement(verbose)
{
}

AppleSoundHeader::~AppleSoundHeader()
{
}

bool AppleSoundHeader::operator==(const AppleSoundHeader& right) const
{
	return (mSixteenBit == right.mSixteenBit &&
		mStereo == right.mStereo &&
		mSigned == right.mSigned &&
		mBytesPerFrame == right.mBytesPerFrame &&
		mSampleRate == right.mSampleRate &&
		mLoopStart == right.mLoopStart &&
		mLoopEnd == right.mLoopEnd &&
		mBaseFrequency == right.mBaseFrequency &&
		mData == right.mData);
}

BigEndianBuffer& AppleSoundHeader::LoadObject(BigEndianBuffer& buffer)
{
	unsigned char headerType = buffer.Data()[buffer.Position() + 20];
	switch (headerType) {
		case standardSoundHeader:
		{
			mBytesPerFrame = 1;
			mSigned = false;
			mStereo = false;
			mSixteenBit = false;

			buffer.ReadULong(); // sample pointer

			int frames = buffer.ReadLong();
			mSampleRate = buffer.ReadULong();
			mLoopStart = buffer.ReadLong();
			mLoopEnd = buffer.ReadLong();
			
			buffer.ReadUChar(); // type
			mBaseFrequency = buffer.ReadUChar();
			
			mData.resize(frames);
			buffer.ReadBlock(mData.size(), &mData[0]);
			break;
		}
		case extendedSoundHeader:
		case compressedSoundHeader:
		{
			buffer.ReadULong(); // samplePtr
			mStereo = (buffer.ReadLong() == 2);
			mSampleRate = buffer.ReadULong();
			mLoopStart = buffer.ReadLong();
			mLoopEnd = buffer.ReadLong();
			
			buffer.ReadUChar(); // type
			mBaseFrequency = buffer.ReadUChar();

			int frames = buffer.ReadLong();

			if (headerType == compressedSoundHeader) {
				buffer.Position(buffer.Position() + 10); // AIFF rate
				buffer.ReadULong(); // marker chunk
				unsigned int format = buffer.ReadULong();
				buffer.ReadULong(); // future use
				buffer.ReadULong(); // stateVars
				buffer.ReadULong(); // leftOverSamples
				short comp_id = buffer.ReadShort();
				if (format != FOUR_CHARS_TO_INT('t', 'w', 'o', 's') || comp_id != -1) {
					wxLogError(wxT("[AppleSoundHeader] Unsupported compression format '%.4s.'"), &format);
					mGoodData = false;
					return buffer;
				}
				mSigned = true;
				buffer.ReadShort(); // packetSize
				buffer.ReadShort(); // unused
				mSixteenBit = (buffer.ReadShort() == 16);
			} else {
				mSigned = false;
				buffer.Position(buffer.Position() + 22);
				mSixteenBit = (buffer.ReadShort() == 16);
				buffer.Position(buffer.Position() + 14);
			}
			
			mBytesPerFrame = (mSixteenBit ? 2 : 1) * (mStereo ? 2 : 1);
			mData.resize(frames * mBytesPerFrame);
			buffer.ReadBlock(mData.size(), &mData[0]);
			break;
		}
		default:
			wxLogError(wxT("[AppleSoundHeader] Unknown header type %.2x."), headerType);
			mGoodData = false;
			return buffer;
	}

	mGoodData = true;
	return buffer;
}

BigEndianBuffer& AppleSoundHeader::SaveObject(BigEndianBuffer& buffer)
{
	if (mSixteenBit || mStereo || mSigned) {
		// extended or compressed sound header
		buffer.WriteULong(0); // samplePtr
		buffer.WriteLong(mStereo ? 2 : 1);
		buffer.WriteULong(mSampleRate);
		buffer.WriteLong(mLoopStart);
		buffer.WriteLong(mLoopEnd);
		
		buffer.WriteUChar((mSigned && !mSixteenBit) ? compressedSoundHeader : extendedSoundHeader);
		buffer.WriteUChar(mBaseFrequency);
		buffer.WriteLong(mData.size() / mBytesPerFrame);
		buffer.WriteZeroes(10); // AIFF rate
		buffer.WriteULong(0); // marker chunk
		if (mSigned && !mSixteenBit) {
			buffer.Write4CharCode('t', 'w', 'o', 's');
			buffer.WriteLong(0); // futureUse2
			buffer.WriteULong(0); // stateVars
			buffer.WriteULong(0); // leftOverSamples
			buffer.WriteShort(-1); // compressionID
			buffer.WriteShort(0); // packetSize
			buffer.WriteShort(0); // synthID
			buffer.WriteShort(mSixteenBit ? 16 : 8);
		} else {
			buffer.WriteULong(0); // instrument chunks
			buffer.WriteULong(0); // AESRecording
			buffer.WriteShort(mSixteenBit ? 16 : 8);
			buffer.WriteZeroes(14); // futureUse1 through futureUse4
		}
		buffer.WriteBlock(mData.size(), &mData[0]);
	} else {
		// standard sound header
		buffer.WriteULong(0); // sample ptr
		buffer.WriteLong(mData.size()); // frames
		buffer.WriteULong(mSampleRate);
		buffer.WriteLong(mLoopStart);
		buffer.WriteLong(mLoopEnd);
		buffer.WriteUChar(standardSoundHeader);
		buffer.WriteUChar(mBaseFrequency);
		buffer.WriteBlock(mData.size(), &mData[0]);
	}

	return buffer;
}

static const int kBufferSize = 8192;

// RAII for SNDFILE*
class SNDFILE_ptr {
public:
	SNDFILE_ptr(SNDFILE* file) : mFile(file) {}
	~SNDFILE_ptr() { if (mFile) sf_close(mFile); mFile = 0; }
	SNDFILE* get() { return mFile; }
private:
	SNDFILE_ptr(const SNDFILE_ptr&);
	SNDFILE_ptr& operator= (const SNDFILE_ptr&);
	SNDFILE* mFile;
};

bool AppleSoundHeader::LoadFromFile(wxString path)
{
	SF_INFO inputInfo;
	SNDFILE_ptr infile(sf_open(path.fn_str(), SFM_READ, &inputInfo));
	if (!infile.get()) {
		wxLogError(wxT("[AppleSoundHeader] libsndfile could not open file."));
		return false;
	}

	mSixteenBit = !(inputInfo.format & (SF_FORMAT_PCM_S8 | SF_FORMAT_PCM_U8));
	if (inputInfo.samplerate <= 44100) {
		mSampleRate = inputInfo.samplerate << 16;
	} else {
		mSampleRate = 44100 << 16;
	}
	mStereo = (inputInfo.channels >= 2);
	mSigned = false;
	mBytesPerFrame = (mSixteenBit ? 2 : 1) * (mStereo ? 2 : 1);
	mLoopStart = mLoopEnd = 0;
	mBaseFrequency = 60;
	
	SF_INFO outputInfo;
	outputInfo.samplerate = mSampleRate >> 16;
	outputInfo.channels = mStereo ? 2 : 1;
	if (mSixteenBit) {
		outputInfo.format = SF_FORMAT_PCM_16 | SF_FORMAT_RAW | SF_ENDIAN_BIG;
	} else {
		outputInfo.format = SF_FORMAT_PCM_U8 | SF_FORMAT_RAW | SF_ENDIAN_BIG;
	}

	SF_VIRTUAL_IO virtual_io = {
		&sf_adapter::get_filelen,
		&sf_adapter::seek,
		&sf_adapter::read,
		&sf_adapter::write,
		&sf_adapter::tell };
	
	mData.resize(inputInfo.frames * mBytesPerFrame);
	sf_adapter adapter(mData);

	SNDFILE_ptr outfile(sf_open_virtual(&virtual_io, SFM_WRITE, &outputInfo, &adapter));
	if (!outfile.get()) {
		wxLogError(wxT("[AppleSoundHeader] libsndfile write error."));
		return false;
	}

	int frames_remaining = inputInfo.frames;
	while (frames_remaining > 0) {
		int buf[kBufferSize * 2];
		int frames = std::min(kBufferSize, frames_remaining);
		
		if (sf_readf_int(infile.get(), buf, frames) != frames) {
			wxLogError(wxT("[AppleSoundHeader] libsndfile read error."));
			return false;
		}
		if (sf_writef_int(outfile.get(), buf, frames) != frames) {
			wxLogError(wxT("[AppleSoundHeader] libsndfile write error."));
			return false;
		}

		frames_remaining -= frames;
	}

	return true;
}

bool AppleSoundHeader::SaveToWaveOrAiff(wxString path, bool aiff)
{
	int inputFormat;
	int outputFormat;

	if (mSixteenBit) {
		inputFormat = outputFormat = SF_FORMAT_PCM_16;
	} else if (mSigned) {
		inputFormat = SF_FORMAT_PCM_S8;
		outputFormat = SF_FORMAT_PCM_U8;
	} else {
		inputFormat = outputFormat = SF_FORMAT_PCM_U8;
	}
	
	SF_INFO outputInfo;
	outputInfo.samplerate = mSampleRate >> 16;
	outputInfo.channels = mStereo ? 2 : 1;
	if (aiff) {
		outputInfo.format = SF_FORMAT_AIFF | outputFormat;
	} else {
		outputInfo.format = SF_FORMAT_WAV | outputFormat;
	}
	
	SNDFILE* outfile = sf_open(path.fn_str(), SFM_WRITE, &outputInfo);

	SF_VIRTUAL_IO virtual_io = {
		&sf_adapter::get_filelen,
		&sf_adapter::seek,
		&sf_adapter::read,
		&sf_adapter::write,
		&sf_adapter::tell };

	sf_adapter adapter(mData);

	SF_INFO inputInfo;
	inputInfo.samplerate = mSampleRate >> 16;
	inputInfo.channels = mStereo ? 2 : 1;
	inputInfo.format = SF_FORMAT_RAW | inputFormat | SF_ENDIAN_BIG;

	SNDFILE* infile = sf_open_virtual(&virtual_io, SFM_READ, &inputInfo, &adapter);

	int frames_remaining = mData.size() / mBytesPerFrame;
	while (frames_remaining) {
		int buf[kBufferSize * 2];
		int frames = std::min(kBufferSize, frames_remaining);
		if (sf_readf_int(infile, buf, frames) != frames) {
			wxLogError(wxT("[AppleSoundHeader] libsndfile read error"));
			sf_close(infile);
			sf_close(outfile);
			return false;
		}
		if (sf_writef_int(outfile, buf, frames) != frames) {
			wxLogError(wxT("[AppleSoundHeader] libsndfile write error"));
			sf_close(infile);
			sf_close(outfile);
			return false;
		}

		frames_remaining -= frames;
	}
	
	sf_close(infile);
	sf_close(outfile);

	return true;
}

void AppleSoundHeader::PlaySound(void)
{
	wxString	tempfile = wxFileName::CreateTempFileName(wxT("sf")) + wxString(wxT(".wav"));
	
	wxBeginBusyCursor();
	SaveToWave(tempfile);
	wxSound(tempfile).Play(wxSOUND_SYNC);
	wxRemoveFile(tempfile);
	wxEndBusyCursor();
}

unsigned int AppleSoundHeader::Size(void)
{
	if (mSixteenBit || mStereo || mSigned) {
		// compressed or extended header
		return mData.size() + 64;
	} else {
		return mData.size() + 22;
	}
}

unsigned char* AppleSoundHeader::Data(void)
{
	return &mData[0];
}

SoundsDefinition::SoundsDefinition(bool verbose): SoundsElement(verbose)
{
	mSoundCode = -1;
	mBehaviorIndex = _sound_is_quiet;
	mFlags = 0;
	mChance = _always;
	mSounds.clear();
}

SoundsDefinition::~SoundsDefinition()
{
}

bool SoundsDefinition::HaveSameAttributesAs(const SoundsDefinition& right) const
{
	return ((mSoundCode == right.mSoundCode) &&
			(mBehaviorIndex == right.mBehaviorIndex) &&
			(mFlags == right.mFlags) &&
			(mChance == right.mChance) &&
			(mLowPitch == right.mLowPitch) &&
			(mHighPitch == right.mHighPitch));
}

bool SoundsDefinition::HaveSameSoundsAs(const SoundsDefinition& right) const
{
	return (mSounds == right.mSounds);
}

bool SoundsDefinition::operator== (const SoundsDefinition& right) const
{
	return (HaveSameAttributesAs(right) && HaveSameSoundsAs(right));
}

bool SoundsDefinition::operator!=(const SoundsDefinition& right) const
{
	return (!HaveSameAttributesAs(right) || !HaveSameSoundsAs(right));
}

unsigned int SoundsDefinition::GetSizeInFile(void)
{
	unsigned int	size = SIZEOF_sound_definition;

	for (unsigned int i = 0; i < mSounds.size(); i++)
		size += mSounds[i].Size();
	return size;
}

short SoundsDefinition::GetChance(void) const
{
	for (int i = 0; i < 10; i++) {
		if (mChance == 32768*i/10)
			return i;
	}
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
	
	float	low_pitch_integer, low_pitch_fractional,
			high_pitch_integer, high_pitch_fractional;
	long	low_pitch = 0, high_pitch = 0;
	
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
	unsigned long single_length = (mSounds.size() >= 1 ? mSounds[0].Size() : 0);
	unsigned long total_length = single_length;
	std::vector<long> soundOffsets;
	soundOffsets.push_back(0);
	
	// ... and the corresponding offsets ...
	for (unsigned int i = 1; i < mSounds.size(); i++) {
		soundOffsets.push_back(total_length);
		total_length += mSounds[i].Size();
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
		mSounds[i].SaveObject(buffer);
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
		wxLogError(wxT("[SoundsDefinition] incorrect Behavior/Chance (%d/%d)"), mBehaviorIndex, mChance);
		return buffer;
	}
	
	wxInt32		low_pitch_fixed, high_pitch_fixed;
	
	low_pitch_fixed = buffer.ReadLong();
	high_pitch_fixed = buffer.ReadLong();
	
	mLowPitch = ((low_pitch_fixed >> 16) & 0xffff) + (float)(low_pitch_fixed & 0xffff) / 65536.0;	// convert fixed point [0,1] to float
	mHighPitch = ((high_pitch_fixed >> 16) & 0xffff) + (float)(high_pitch_fixed & 0xffff) / 65536.0;	// convert fixed point [0,1] to float

	short permutations = buffer.ReadShort();
	
	if (permutations < 0 || permutations > MAXIMUM_PERMUTATIONS_PER_SOUND) {
		wxLogError(wxT("[SoundsDefinition] incorrect permutation count : %d"), permutations);
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
		
		AppleSoundHeader sndbuffer(IsVerbose());

		buffer.Position(groupOffset + soundOffsets[i]);
		sndbuffer.LoadObject(buffer);
		if (sndbuffer.IsGood()) {
			mSounds.push_back(sndbuffer);
		} else {
			mGoodData = false;
			return buffer;
		}
	}

	buffer.Position(oldpos);
	
	mGoodData = true;
	return buffer;
}

AppleSoundHeader* SoundsDefinition::GetPermutation(unsigned int permutation_index)
{
	if (permutation_index >= mSounds.size())
		return NULL;
	return &mSounds[permutation_index];
}

void SoundsDefinition::DeletePermutation(unsigned int permutation_index)
{
	mSounds.erase(mSounds.begin() + permutation_index);
}

AppleSoundHeader* SoundsDefinition::NewPermutation(wxString path)
{
	if (mSounds.size() >= MAXIMUM_PERMUTATIONS_PER_SOUND) {
		return NULL;
	}

	AppleSoundHeader header(IsVerbose());
	if (header.LoadFromFile(path)) {
		mSounds.push_back(header);
		return &mSounds.back();
	} else {
		return NULL;
	}
}
