#include <libsystem/io/File.h>
#include <libsystem/io/Filesystem.h>
#include <libsystem/io/Stream.h>
#include <libsystem/json/Json.h>
#include <libutils/Path.h>

#include "WAVE.h"
#include "kernel/drivers/AC97.h"

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
        wavemetadata.riff.chunk.size = (uintptr_t)chunk_size;
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
        wavemetadata.fmt.chunk.size = (uintptr_t)subchunk1_size;
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
        wavemetadata.fmt.audio_format = (uintptr_t)audio_format;
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
        wavemetadata.fmt.num_channel = (uintptr_t)num_channels;
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
        wavemetadata.fmt.sample_rate = (uintptr_t)sample_rate;
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
        wavemetadata.fmt.byte_rate = (uintptr_t)byte_rate;
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
        wavemetadata.fmt.byte_per_block = (uintptr_t)block_align;
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

        wavemetadata.fmt.bits_per_sample = (uintptr_t)bits_per_sample;
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
        wavemetadata.data.chunk.size = (uintptr_t)subchunk2_size;
    }

    return SUCCESS;
}

Result media::wave::WaveDecoder::read_wave(char buffer[], size_t size)
{
    // char *buffer_two;
    // int i, j, two_seconds;

    le_uint32_t sample_rate = wavemetadata.fmt.sample_rate;
    le_uint16_t bits_per_sample = wavemetadata.fmt.bits_per_sample;

    // two_seconds = sample_rate() * bits_per_sample() * 2;
    char inputbuffer[AC97_BDL_BUFFER_LEN];
    stream_read(wavedata, &inputbuffer, size * (sample_rate() * bits_per_sample() / 96000));

    if (handle_has_error(wavedata))
    {
        return handle_get_error(wavedata);
    }
    if (sample_rate() < 48000)
    {
        upsample_wave(inputbuffer, buffer);
    }
    else
    {
        downsample_wave(inputbuffer, buffer);
    }
    // for (i = 0; i < (size * 8); i = i + two_seconds)
    // {
    //     for (j = 0; j < two_seconds; j++)
    //     {
    //         buffer_two[j] = buffer[i];
    //         if (i % two_seconds == 0)
    //         {
    //             if (sample_rate() < 48000)
    //             {
    //                 upsample_wave(buffer_two, buffer);
    //             }
    //             else
    //             {
    //                 downsample_wave(buffer_two, buffer);
    //             }
    //         }
    //     }
    // }
    return SUCCESS;
}

// Result media::wave::WaveDecoder::seek_wave(int time_seconds)
// {
//     int seekwave;
//     le_uint32_t sample_rate = wavemetadata.fmt.sample_rate;
//     seekwave = stream_seek(wavedata, time_seconds * sample_rate(), WHENCE_HERE);
//     if (handle_has_error(wavedata))
//     {
//         return handle_get_error(wavedata);
//     }
//     return SUCCESS;
// }

Result media::wave::WaveDecoder::upsample_wave(char buffer_in[], char buffer_out[])
{
    int i = 1;
    int j = 1;
    le_uint32_t sample_rate = wavemetadata.fmt.sample_rate;
    //le_uint16_t bits_per_sample = wavemetadata.fmt.bits_per_sample;

    int resampling_factor;

    // resampling_factor = 96000 / (sample_rate() * bits_per_sample());
    resampling_factor = (48000 / sample_rate()) * 2;

    buffer_out[0] = buffer_in[0];

    while (i < 2 * AC97_BDL_BUFFER_LEN)
    {
        if (i % resampling_factor == 0)
        {
            buffer_out[j + 1] = buffer_in[i] & 0x00FF;
            buffer_out[j + 1] = buffer_out[j + 1] << 8;
            buffer_out[j + 1] = buffer_in[i + 1] & 0xFF00;
            j++;
        }

        buffer_out[j] = buffer_in[i];
        i++;
        j++;
    }
    return SUCCESS;
}

Result media::wave::WaveDecoder::downsample_wave(char buffer_in[], char buffer_out[])
{
    int i = 1;
    int j = 1;
    le_uint32_t sample_rate = wavemetadata.fmt.sample_rate;
    int resampling_factor;
    resampling_factor = (48000 / sample_rate()) * 2;
    buffer_out[0] = buffer_in[0];

    while (i < 2 * AC97_BDL_BUFFER_LEN - 1)
    {
        if (i % resampling_factor == 0)
        {
            i++;
            buffer_out[j] = buffer_in[i];
        }
        else
        {
            buffer_out[j] = buffer_in[i];
        }
        j++;
    }
    return SUCCESS;
}
