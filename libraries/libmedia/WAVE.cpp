#pragma once

#include <libsystem/io/File.h>
#include <libsystem/io/Filesystem.h>
#include <libsystem/io/Stream.h>
#include <libutils/Path.h>

#include "WAVE.h"

Result media::wave::open_wave(const char *buffer, WAVE *wavefile)
{
    __cleanup(stream_cleanup) Stream *stream = stream_open(buffer, OPEN_READ);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }

    size_t read;

    // get chunk id
    char chunk_id[4];
    read = stream_read(stream, &chunk_id, 4);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }
    if (read == 4)
    {
        strcpy(wavefile->riff.chunk.id, chunk_id);
    }

    // get chunk size
    char chunk_size[4];
    read = stream_read(stream, &chunk_size, 4);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }
    if (read == 4)
    {
        wavefile->riff.chunk.size = (uint16_t)chunk_size;
    }

    // get format
    char format[4];
    read = stream_read(stream, &format, 4);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }
    if (read == 4)
    {
        strcpy(wavefile->riff.format, format);
    }

    //  get subchunk1 id
    char subchunk1_id[4];
    read = stream_read(stream, &subchunk1_id, 4);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }
    if (read == 4)
    {
        strcpy(wavefile->fmt.chunk.id, subchunk1_id);
    }

    //  get subchunk1 size
    char subchunk1_size[4];
    read = stream_read(stream, &subchunk1_size, 4);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }
    if (read == 4)
    {
        wavefile->fmt.chunk.size = (uint16_t)subchunk1_size;
    }

    //  get format
    char audio_format[2];
    read = stream_read(stream, &audio_format, 2);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }
    if (read == 2)
    {
        wavefile->fmt.audio_format = (uint16_t)audio_format;
    }

    //  get channels
    char num_channels[2];
    read = stream_read(stream, &num_channels, 2);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }
    if (read == 2)
    {
        wavefile->fmt.num_channel = (uint16_t)num_channels;
    }

    //  get sample rate
    char sample_rate[4];
    read = stream_read(stream, &sample_rate, 4);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }
    if (read == 4)
    {
        wavefile->fmt.sample_rate = (uint16_t)sample_rate;
    }

    //  get byte rate
    char byte_rate[4];
    read = stream_read(stream, &byte_rate, 4);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }
    if (read == 4)
    {
        wavefile->fmt.byte_rate = (uint16_t)byte_rate;
    }

    //  block align
    char block_align[2];
    read = stream_read(stream, &block_align, 2);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }
    if (read == 2)
    {
        wavefile->fmt.byte_per_block = (uint16_t)block_align;
    }
    //  bits per sample
    char bits_per_sample[2];
    read = stream_read(stream, &bits_per_sample, 2);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }
    if (read == 2)
    {
        wavefile->fmt.bits_per_sample = (uint16_t)bits_per_sample;
    }
    //  subchunk2 ID
    char subchunk2_id[4];
    read = stream_read(stream, &subchunk2_id, 4);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }
    if (read == 4)
    {
        strcpy(wavefile->data.chunk.id, subchunk2_id);
    }

    //  subchunk2 size
    char subchunk2_size[4];
    read = stream_read(stream, &subchunk2_size, 4);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }
    if (read == 4)
    {
        wavefile->data.chunk.size = (uint16_t)subchunk2_size;
    }

    return SUCCESS;
}
