#include <stdio.h>

#include "jukeboxfin.h"

extern int orig_main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    jukebox_setup();
    return orig_main(argc,argv);
}
