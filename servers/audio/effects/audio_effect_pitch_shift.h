/*************************************************************************/
/*  audio_effect_pitch_shift.h                                           */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef AUDIO_EFFECT_PITCH_SHIFT_H
#define AUDIO_EFFECT_PITCH_SHIFT_H

#include "servers/audio/audio_effect.h"

class SMBPitchShift {
	enum {
		MAX_FRAME_LENGTH = 8192
	};

	float gInFIFO[MAX_FRAME_LENGTH];
	float gOutFIFO[MAX_FRAME_LENGTH];
	double gFFTworksp[2 * MAX_FRAME_LENGTH];
	double gLastPhase[MAX_FRAME_LENGTH / 2 + 1];
	double gSumPhase[MAX_FRAME_LENGTH / 2 + 1];
	double gOutputAccum[2 * MAX_FRAME_LENGTH];
	double gAnaFreq[MAX_FRAME_LENGTH];
	double gAnaMagn[MAX_FRAME_LENGTH];
	double gSynFreq[MAX_FRAME_LENGTH];
	double gSynMagn[MAX_FRAME_LENGTH];
	int64_t gRover;
	float lastPitchShift;

	void smbFft(double *fftBuffer, int64_t fftFrameSize, int64_t sign);

public:
	void PitchShift(float pitchShift, int64_t numSampsToProcess, int64_t fftFrameSize, int64_t osamp, float sampleRate, float *indata, float *outdata, int stride);

	SMBPitchShift() {
		gRover = 0;
		memset(gInFIFO, 0, MAX_FRAME_LENGTH * sizeof(float));
		memset(gOutFIFO, 0, MAX_FRAME_LENGTH * sizeof(float));
		memset(gFFTworksp, 0, 2 * MAX_FRAME_LENGTH * sizeof(double));
		memset(gLastPhase, 0, (MAX_FRAME_LENGTH / 2 + 1) * sizeof(double));
		memset(gSumPhase, 0, (MAX_FRAME_LENGTH / 2 + 1) * sizeof(double));
		memset(gOutputAccum, 0, 2 * MAX_FRAME_LENGTH * sizeof(double));
		memset(gAnaFreq, 0, MAX_FRAME_LENGTH * sizeof(double));
		memset(gAnaMagn, 0, MAX_FRAME_LENGTH * sizeof(double));
		lastPitchShift = 1.0;
	}
};

class AudioEffectPitchShift;

class AudioEffectPitchShiftInstance : public AudioEffectInstance {
	GDCLASS(AudioEffectPitchShiftInstance, AudioEffectInstance);
	friend class AudioEffectPitchShift;
	Ref<AudioEffectPitchShift> base;

	int fft_size;
	SMBPitchShift shift_l;
	SMBPitchShift shift_r;

public:
	virtual void process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count);
};

class AudioEffectPitchShift : public AudioEffect {
	GDCLASS(AudioEffectPitchShift, AudioEffect);

public:
	friend class AudioEffectPitchShiftInstance;

	enum FFT_Size {
		FFT_SIZE_256,
		FFT_SIZE_512,
		FFT_SIZE_1024,
		FFT_SIZE_2048,
		FFT_SIZE_4096,
		FFT_SIZE_MAX
	};

	float pitch_scale;
	int oversampling;
	FFT_Size fft_size;

protected:
	static void _bind_methods();

public:
	Ref<AudioEffectInstance> instance();

	void set_pitch_scale(float p_pitch_scale);
	float get_pitch_scale() const;

	void set_oversampling(int p_oversampling);
	int get_oversampling() const;

	void set_fft_size(FFT_Size);
	FFT_Size get_fft_size() const;

	AudioEffectPitchShift();
};

VARIANT_ENUM_CAST(AudioEffectPitchShift::FFT_Size);

#endif // AUDIO_EFFECT_PITCH_SHIFT_H
