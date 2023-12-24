#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "jukeboxfin.h"


// *** ************************** ***
// *** Jellyfin http client       ***
// *** ONLY for local connections ***
// *** ************************** ***
static const int HTTP_GET = 0;
static const int HTTP_POST = 1;

struct http_response
{
    char *body;
    int status_code;
};

static struct http_response *http_req(int cmd, char *url, char *custom_header)
{
    int sock;
    char buf[BUFSIZ];
    static struct http_response hresp;
    static struct sockaddr_in remote;
    static char *http_header[2];

    if (remote.sin_port == 0)
    {
        remote.sin_family = AF_INET;
        remote.sin_addr.s_addr = inet_addr(jf_get_ip());
        remote.sin_port = htons(jf_get_port());

        sprintf(buf, "%%s /%%%%s "
            "HTTP/1.1\r\n"
            "Host: %s:%d\r\n"
            "Connection: close\r\n"
            "accept: application/json\r\n"
            "User-Agent: JukeboxFin 0.1\r\n"
            "X-MediaBrowser-Token: %s\r\n"
            "%%%%s\r\n",
            jf_get_ip(),
            jf_get_port(),
            jf_get_token()
        );
        http_header[HTTP_GET] = (char *)malloc(strlen(buf) +4);
        sprintf(http_header[HTTP_GET], buf, "GET");
        http_header[HTTP_POST] = (char *)malloc(strlen(buf) +5);
        sprintf(http_header[HTTP_POST], buf, "POST");
    };

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Jukebox: Cannot create TCP socket");
        return NULL;
    }

    if (connect(sock, (struct sockaddr *)&remote, sizeof(struct sockaddr)) < 0)
    {
        fprintf(stderr, "Jukebox: Cannot connect");
        return NULL;
    }

    sprintf(buf, http_header[cmd], url, custom_header);
    char *ptr_buf = buf;
    int cnt, cnt_buf = strlen(buf);
    while (cnt_buf > 0)
    {
        if ( (cnt = send(sock, ptr_buf, cnt_buf, 0)) == -1)
        {
            fprintf(stderr, "Jukebox: Can't send headers");
            return NULL;
        }
        cnt_buf -= cnt;
        ptr_buf += cnt;
     }

    ptr_buf = (char*)malloc(0);
    cnt_buf = 0;
    while((cnt = recv(sock, buf, BUFSIZ-1, 0)) > 0)
    {
        ptr_buf = (char*)realloc(ptr_buf, cnt_buf + cnt + 1);
        strncpy(ptr_buf+cnt_buf, buf, cnt);
        cnt_buf += cnt;
    }
    if (cnt < 0)
    {
        close(sock);
        fprintf(stderr, "Jukebox: Unable to get response");
        return NULL;
    }
    ptr_buf[cnt_buf] = '\0';
    hresp.status_code = atoi(ptr_buf + 9);
    hresp.body = strstr(ptr_buf, "\r\n\r\n") + 4;

    /* Return response */
    return &hresp;
}



// *** ******************** ***
// *** Jellyfin client API ***
// *** ******************** ***


bool jf_API_check()
{
    static char *url = "system/info";
    struct http_response *hresp = http_req(0, url, "");
    printf("RESULT: status(%d) response -->%s<--", hresp->status_code, hresp->body);
};

bool jf_API_login()
{
    char *user, *passwd, *token;

    // Connect to jellyfin server
    user = jf_get_username();
    passwd = jf_get_password();
    token = jf_get_token();
};
