#pragma once

#include "WAVE.h"

#include <libsystem/io/File.h>
#include <libsystem/io/Filesystem.h>
#include <libsystem/io/Stream.h>
#include <libutils/Path.h>

Result media::wave::open(const char *buffer, WAVE *wavefile)
{
    __cleanup(stream_cleanup) Stream *stream = stream_open(buffer, OPEN_READ);
    if (handle_has_error(stream))
    {
        return handle_get_error(stream);
    }

    size_t read;

    // get chunk id
    char *chunkid = new char[4];
    read = stream_read(stream, &chunkid, 4);
    if (read == 4)
    {
        strcpy(wavefile->riff.chunk.id, chunkid);
    }
    else
    {
        return ERR_NOT_READABLE;
    }
    delete chunkid;

    // get chunk size
    char *chunksize = new char[4];
    read = stream_read(stream, &chunksize, 4);
    if (read == 4)
    {
        wavefile->riff.chunk.size = (uint16_t)chunksize;
    }
    else
    {
        return ERR_NOT_READABLE;
    }
    delete chunksize;

    // other data from wave

    return SUCCESS;
}
