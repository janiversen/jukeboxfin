#include "jukeboxfin.h"
#include "jf_crossover.h"
#include "orig_shared.h"
#include "orig_net.h"
#include "orig_json.h"


void jf_cross_login()
{
    char *login_post;
    jf_reply *reply;

    printf("Crossover: Get userid from server, using username/password.\n");
    login_post = jf_json_generate_login_request(cfg_username, cfg_password);
    reply = jf_net_request("/Users/authenticatebyname",
            JF_REQUEST_IN_MEMORY,
            JF_HTTP_POST,
            login_post);
    free(login_post);
    if (JF_REPLY_PTR_HAS_ERROR(reply)) {
        if (reply->state == JF_REPLY_ERROR_HTTP_401) {
            fprintf(stderr, "Error: invalid login credentials.\n");
        } else {
            fprintf(stderr,
                    "FATAL: could not login: %s\n",
                    jf_reply_error_string(reply));
        }
        jf_reply_free(reply);
        jf_exit(JF_EXIT_FAILURE);
    }
    jf_json_parse_login_response(reply->payload);
    jf_reply_free(reply);

    printf("Crossover: system info\n");
    reply = jf_net_request("/system/info", JF_REQUEST_IN_MEMORY, JF_HTTP_GET, NULL);
    if (JF_REPLY_PTR_HAS_ERROR(reply)) {
        fprintf(stderr, "FATAL: could not reach server: %s.\n", jf_reply_error_string(reply));
        jf_exit(JF_EXIT_FAILURE);
    }
    jf_json_parse_system_info_response(reply->payload);
    jf_reply_free(reply);

    printf("Crossover: Login OK\n");
};
