#include "kernel/drivers/AC97.h"
#include <libmedia/WAVE.h>
#include <libsystem/io/File.h>
#include <libsystem/io/Filesystem.h>
#include <libsystem/io/Stream.h>
#include <libutils/Path.h>
int main(int argc, char **argv)
{

    if (argc == 1)
    {
        stream_format(err_stream, "%s: Missing Audio file operand\n", argv[0]);
        return PROCESS_FAILURE;
    }
    __cleanup(stream_cleanup) Stream *streamin = stream_open(argv[1], OPEN_READ);

    if (handle_has_error(streamin))
    {
        return handle_get_error(streamin);
    }
    media::wave::WaveDecoder resampler = media::wave::WaveDecoder(argv[1]);

    __cleanup(stream_cleanup) Stream *streamout = stream_open("/Devices/sound", OPEN_WRITE | OPEN_CREATE);

    if (handle_has_error(streamout))
    {
        return handle_get_error(streamout);
    }

    size_t read;
    char buffer[2 * AC97_BDL_BUFFER_LEN];

    while ((read = stream_read(streamin, &buffer, 2 * AC97_BDL_BUFFER_LEN)) != 0)
    {
        if (handle_has_error(streamin))
        {
            return handle_get_error(streamin);
        }
        resampler.read_wave(buffer, read);

        stream_write(streamout, buffer, read);

        if (handle_has_error(streamout))
        {
            return handle_get_error(streamout);
        }
    }

    printf("Finish Playing");

    return PROCESS_SUCCESS;
}
