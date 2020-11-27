#pragma once

#include <libsystem/io/File.h>
#include <libsystem/io/Filesystem.h>
#include <libsystem/io/Stream.h>
#include <libutils/Endian.h>

namespace media::wave
{

struct RIFFChunk
{
    char id[4];
    le_uint32_t size;
};

struct RIFF
{
    RIFFChunk chunk;

    char format[4];
};

struct FMT
{
    RIFFChunk chunk;

    le_uint16_t audio_format;
    le_uint16_t num_channel;
    le_uint32_t sample_rate;
    le_uint32_t byte_rate;
    le_uint16_t byte_per_block;
    le_uint16_t bits_per_sample;
};

struct DATA
{
    RIFFChunk chunk;
};

struct WAVE
{
    RIFF riff;
    FMT fmt;
    DATA data;
};

class WaveDecoder
{

private:
    char path[PATH_LENGTH];
    WAVE wavemetadata;
    Stream *wavedata;
    Result open_wave(const char *path);

public:
    WaveDecoder(const char *path);
    ~WaveDecoder();
    Result read_wave(const char *buffer, size_t size);
    // decide what to use to seek : time or byte offset
    Result seek_wave(int time_seconds);
};

// Result open_wave(const char *path, WAVE *wavefile);
// Result read_wave(const char *buffer, size_t size, size_t offset, const char *path, WAVE *wavefile);

} // namespace media::wave
