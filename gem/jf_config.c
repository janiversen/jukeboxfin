#include "jukeboxfin.h"


char *jf_get_url()
{
    return "http://127.0.0.1:8096";
};

char *jf_get_user()
{
    // Get user name name from shairport config
    return "musica";
};

char *jf_get_password()
{
    // Get user password from shairport config
    return "musica";
};

char *jf_get_token()
{
    // Get client token from shairport config
    return "6b8ffe460f0848df91e34f922f7f3224";
};
