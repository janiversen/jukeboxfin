#include <stdio.h>

#include "jukeboxfin.h"


int main(int argc, char *argv[])
{
    printf("Starting jukeboxfin.\n");

    jukebook_run_loop();


    jukebox_setup(NULL);
    jukebox_run();
    jukebox_playback_start();
    jukebox_playback_stop();
}
