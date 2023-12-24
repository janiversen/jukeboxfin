#include "jukeboxfin.h"

void jukebook_run_loop()
{
    printf("--> Starting\n");
    jf_API_check();
    jf_API_login();
};



void jukebox_setup(const char *config)
{
    // Setup variables
    //      g_options.server
    //      g_options.userid
    //      g_options.token
};


void jukebox_run()
{
#ifdef OLDCODE
    // SERVER NAME AND VERSION
    // this doubles up as a check for connectivity and correct login parameters
    reply = jf_net_request("/system/info", JF_REQUEST_IN_MEMORY, JF_HTTP_GET, NULL);
    if (JF_REPLY_PTR_HAS_ERROR(reply)) {
        fprintf(stderr, "FATAL: could not reach server: %s.\n", jf_reply_error_string(reply));
        jf_exit(JF_EXIT_FAILURE);
    }
    jf_json_parse_system_info_response(reply->payload);
    jf_reply_free(reply);
    //////////////
#endif // OLDCODE
}


void jukebox_playback_start()
{
#ifdef OLDCODE
    while (true) {
        switch (g_state.state) {
            case JF_STATE_STARTING:
            case JF_STATE_STARTING_FULL_CONFIG:
            case JF_STATE_STARTING_LOGIN:
                g_state.state = JF_STATE_MENU_UI;
                // no reason to break
            case JF_STATE_MENU_UI:
                jf_menu_ui();
                break;
            case JF_STATE_PLAYBACK:
            case JF_STATE_PLAYBACK_INIT:
            case JF_STATE_PLAYBACK_START_MARK:
                jf_mpv_event_dispatch(mpv_wait_event(g_mpv_ctx, -1));
                break;
            case JF_STATE_USER_QUIT:
                jf_exit(JF_EXIT_SUCCESS);
                break;
            case JF_STATE_FAIL:
                jf_exit(JF_EXIT_FAILURE);
                break;
        }
    }
#endif // OLDCODE

}


void jukebox_playback_stop()
{

}
