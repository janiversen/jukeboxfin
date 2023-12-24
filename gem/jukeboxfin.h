// Jellyfin jukebox device with airport support (using shairport-sync)
//
// This device allows jellyfin server to utilize speakers connected directly
// to the server (like e.g. DigiAmp+ from IQAudio)
//
// It is made as an extension to shairport-sync, so it continues to allow
// music being played with airsync from apple devices.
//
// This device is accompaned by a jellyfin plugin, allowing it to be
// controlled from "normal" jellyfin clients.
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Main
void jukebox_setup();
void jukebox_run();

// Config part
char *jf_get_url();
int jf_get_port();
char *jf_get_user();
char *jf_get_password();
char *jf_get_token();
