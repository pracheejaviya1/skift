#pragma once

#include <libsystem/io/File.h>
#include <libsystem/io/Filesystem.h>
#include <libsystem/io/Stream.h>
#include <libutils/Path.h>

#include "WAVE.h"

media::wave::WaveDecoder::WaveDecoder(const char *path)
{
    open_wave(path);
}

media::wave::WaveDecoder::~WaveDecoder()
{
    stream_close(wavedata);
}

Result media::wave::WaveDecoder::open_wave(const char *buffer)
{
    wavedata = stream_open(buffer, OPEN_READ);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }

    size_t read;

    // get chunk id
    char chunk_id[4];
    read = stream_read(wavedata, &chunk_id, 4);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (read == 4)
    {
        strcpy(wavemetadata.riff.chunk.id, chunk_id);
    }

    // get chunk size
    char chunk_size[4];
    read = stream_read(wavedata, &chunk_size, 4);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (read == 4)
    {
        wavemetadata.riff.chunk.size = (uint16_t)chunk_size;
    }

    // get format
    char format[4];
    read = stream_read(wavedata, &format, 4);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (read == 4)
    {
        strcpy(wavemetadata.riff.format, format);
    }

    //  get subchunk1 id
    char subchunk1_id[4];
    read = stream_read(wavedata, &subchunk1_id, 4);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (read == 4)
    {
        strcpy(wavemetadata.fmt.chunk.id, subchunk1_id);
    }

    //  get subchunk1 size
    char subchunk1_size[4];
    read = stream_read(wavedata, &subchunk1_size, 4);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (read == 4)
    {
        wavemetadata.fmt.chunk.size = (uint16_t)subchunk1_size;
    }

    //  get format
    char audio_format[2];
    read = stream_read(wavedata, &audio_format, 2);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (read == 2)
    {
        wavemetadata.fmt.audio_format = (uint16_t)audio_format;
    }

    //  get channels
    char num_channels[2];
    read = stream_read(wavedata, &num_channels, 2);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (read == 2)
    {
        wavemetadata.fmt.num_channel = (uint16_t)num_channels;
    }

    //  get sample rate
    char sample_rate[4];
    read = stream_read(wavedata, &sample_rate, 4);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (read == 4)
    {
        wavemetadata.fmt.sample_rate = (uint16_t)sample_rate;
    }

    //  get byte rate
    char byte_rate[4];
    read = stream_read(wavedata, &byte_rate, 4);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (read == 4)
    {
        wavemetadata.fmt.byte_rate = (uint16_t)byte_rate;
    }

    //  block align
    char block_align[2];
    read = stream_read(wavedata, &block_align, 2);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (read == 2)
    {
        wavemetadata.fmt.byte_per_block = (uint16_t)block_align;
    }
    //  bits per sample
    char bits_per_sample[2];
    read = stream_read(wavedata, &bits_per_sample, 2);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (read == 2)
    {
        wavemetadata.fmt.bits_per_sample = (uint16_t)bits_per_sample;
    }
    //  subchunk2 ID
    char subchunk2_id[4];
    read = stream_read(wavedata, &subchunk2_id, 4);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (read == 4)
    {
        strcpy(wavemetadata.data.chunk.id, subchunk2_id);
    }

    //  subchunk2 size
    char subchunk2_size[4];
    read = stream_read(wavedata, &subchunk2_size, 4);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (read == 4)
    {
        wavemetadata.data.chunk.size = (uint16_t)subchunk2_size;
    }

    return SUCCESS;
}

Result media::wave::WaveDecoder::read_wave(const char *buffer, size_t size)
{
    size_t read;
    read = stream_read(wavedata, &buffer, size);
    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    return SUCCESS;
}

Result media::wave::WaveDecoder::seek_wave(int time_seconds)
{
    le_uint32_t sample_rate = wavemetadata.fmt.sample_rate;
    stream_seek(wavedata, time_seconds * sample_rate(), WHENCE_HERE);
}
