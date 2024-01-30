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

// Fixed config part
extern char *cfg_device;
extern char *cfg_deviceid;
extern char *cfg_version;
extern char *cfg_token;
extern char *cfg_server;
extern char *cfg_username;
extern char *cfg_password;
extern char *gen_userid;

// Main
extern void jukebox_setup();
extern void jukebox_run();
