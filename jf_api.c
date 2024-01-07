#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "jukeboxfin.h"


// *** ************************** ***
// *** Jellyfin http client       ***
// *** ONLY for local connections ***
// *** ************************** ***
static const char *HTTP_GET = "GET";
static const char *HTTP_POST = "POST";

struct http_response
{
    char *body;
    int status_code;
    char *reason;
};

static struct http_response *http_req(const char *cmd, const char *url, char *data)
{
    int sock;
    char buf[BUFSIZ];
    static struct http_response hresp;
    static struct sockaddr_in remote;

    printf("REQUEST: %s url(%s), data(%s)", cmd, url, data);
    sprintf(buf,
        "%s /%s "
        "HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "X-MediaBrowser-Token: %s\r\n"
        "User-Agent: JukeboxFin 0.1\r\n"
        "Connection: close\r\n"
        "Content-type: application/json\r\n"
        "Content-Length: %d\r\n"
        "accept: application/json\r\n"
        "\r\n%s",
        cmd,
        url,
        jf_get_ip(),
        jf_get_port(),
        jf_get_token(),
        strlen(data),
        data
    );


    if (remote.sin_port == 0)
    {
        remote.sin_family = AF_INET;
        remote.sin_addr.s_addr = inet_addr(jf_get_ip());
        remote.sin_port = htons(jf_get_port());
    }

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

    char *ptr_buf = buf;
    int cnt, cnt_buf = strlen(buf);
    while (cnt_buf > 0)
    {
        if ( (cnt = send(sock, ptr_buf, cnt_buf, 0)) == -1)
        {
            fprintf(stderr, "Jukebox: Can't send message");
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
    printf("RESULT: status(%d) response -->%s<--", hresp.status_code, hresp.body);
    return &hresp;
}



// *** ******************** ***
// *** Jellyfin client API ***
// *** ******************** ***


bool jf_API_check()
{
    static char *url = "system/info";

    struct http_response *hresp = http_req(HTTP_GET, url, "");
    return (hresp->status_code == 200);
};

bool jf_API_login()
{
    static char *url = "Users/authenticatebyname";
    char buf[1024];

    sprintf(buf,"{\"Username\":\"%s\",\"Pw\":\"%s\"}",
        jf_get_username(),
        jf_get_password()
    );
    struct http_response *hresp = http_req(HTTP_POST, url, buf);
};
