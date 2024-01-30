#include "jukeboxfin.h"
#include "jf_crossover.h"

// Fixed config part
char *cfg_device = "domotica";
char *cfg_deviceid = "6500007";
char *cfg_version = "0.1.0";
char *cfg_token = "6b8ffe460f0848df91e34f922f7f3224";
char *cfg_server = "http://127.0.0.1:8096";
char *cfg_username = "musica";
char *cfg_password = "musica";
char *gen_userid;


void jukebox_setup()
{
    jf_cross_login();
};

void jukebox_run()
{
};
