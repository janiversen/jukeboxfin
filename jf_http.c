
#pragma GCC diagnostic ignored "-Wwrite-strings"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include <errno.h>
#include "stringx.h";
#include "urlparser.h"



/*
    Makes a HTTP request and returns the response
*/

/*
    Makes a HTTP GET request to the given url
*/
struct http_response* http_get(char *url, char *custom_headers)
{
    /* Parse url */
    struct parsed_url *purl = parse_url(url);
    if(purl == NULL)
    {
        printf("Unable to parse url");
        return NULL;
    }

    /* Declare variable */
    char *http_headers = (char*)malloc(1024);

    /* Build query/headers */
    if(purl->path != NULL)
    {
        if(purl->query != NULL)
        {
            sprintf(http_headers, "GET /%s?%s HTTP/1.1\r\nHost:%s\r\nConnection:close\r\n", purl->path, purl->query, purl->host);
        }
        else
        {
            sprintf(http_headers, "GET /%s HTTP/1.1\r\nHost:%s\r\nConnection:close\r\n", purl->path, purl->host);
        }
    }
    else
    {
        if(purl->query != NULL)
        {
            sprintf(http_headers, "GET /?%s HTTP/1.1\r\nHost:%s\r\nConnection:close\r\n", purl->query, purl->host);
        }
        else
        {
            sprintf(http_headers, "GET / HTTP/1.1\r\nHost:%s\r\nConnection:close\r\n", purl->host);
        }
    }

    /* Handle authorisation if needed */
    if(purl->username != NULL)
    {
        /* Format username:password pair */
        char *upwd = (char*)malloc(1024);
        sprintf(upwd, "%s:%s", purl->username, purl->password);
        upwd = (char*)realloc(upwd, strlen(upwd) + 1);

        /* Base64 encode */
        char *base64 = base64_encode(upwd);

        /* Form header */
        char *auth_header = (char*)malloc(1024);
        sprintf(auth_header, "Authorization: Basic %s\r\n", base64);
        auth_header = (char*)realloc(auth_header, strlen(auth_header) + 1);

        /* Add to header */
        http_headers = (char*)realloc(http_headers, strlen(http_headers) + strlen(auth_header) + 2);
        sprintf(http_headers, "%s%s", http_headers, auth_header);
    }

    /* Add custom headers, and close */
    if(custom_headers != NULL)
    {
        sprintf(http_headers, "%s%s\r\n", http_headers, custom_headers);
    }
    else
    {
        sprintf(http_headers, "%s\r\n", http_headers);
    }
    http_headers = (char*)realloc(http_headers, strlen(http_headers) + 1);

    /* Make request and return response */
    struct http_response *hresp = http_req(http_headers, purl);

    /* Handle redirect */
    return handle_redirect_get(hresp, custom_headers);
}

/*
    Makes a HTTP POST request to the given url
*/
struct http_response* http_post(char *url, char *custom_headers, char *post_data)
{
    /* Parse url */
    struct parsed_url *purl = parse_url(url);
    if(purl == NULL)
    {
        printf("Unable to parse url");
        return NULL;
    }

    /* Declare variable */
    char *http_headers = (char*)malloc(1024);

    /* Build query/headers */
    if(purl->path != NULL)
    {
        if(purl->query != NULL)
        {
            sprintf(http_headers, "POST /%s?%s HTTP/1.1\r\nHost:%s\r\nConnection:close\r\nContent-Length:%zu\r\nContent-Type:application/x-www-form-urlencoded\r\n", purl->path, purl->query, purl->host, strlen(post_data));
        }
        else
        {
            sprintf(http_headers, "POST /%s HTTP/1.1\r\nHost:%s\r\nConnection:close\r\nContent-Length:%zu\r\nContent-Type:application/x-www-form-urlencoded\r\n", purl->path, purl->host, strlen(post_data));
        }
    }
    else
    {
        if(purl->query != NULL)
        {
            sprintf(http_headers, "POST /?%s HTTP/1.1\r\nHost:%s\r\nConnection:close\r\nContent-Length:%zu\r\nContent-Type:application/x-www-form-urlencoded\r\n", purl->query, purl->host, strlen(post_data));
        }
        else
        {
            sprintf(http_headers, "POST / HTTP/1.1\r\nHost:%s\r\nConnection:close\r\nContent-Length:%zu\r\nContent-Type:application/x-www-form-urlencoded\r\n", purl->host, strlen(post_data));
        }
    }

    /* Handle authorisation if needed */
    if(purl->username != NULL)
    {
        /* Format username:password pair */
        char *upwd = (char*)malloc(1024);
        sprintf(upwd, "%s:%s", purl->username, purl->password);
        upwd = (char*)realloc(upwd, strlen(upwd) + 1);

        /* Base64 encode */
        char *base64 = base64_encode(upwd);

        /* Form header */
        char *auth_header = (char*)malloc(1024);
        sprintf(auth_header, "Authorization: Basic %s\r\n", base64);
        auth_header = (char*)realloc(auth_header, strlen(auth_header) + 1);

        /* Add to header */
        http_headers = (char*)realloc(http_headers, strlen(http_headers) + strlen(auth_header) + 2);
        sprintf(http_headers, "%s%s", http_headers, auth_header);
    }

    if(custom_headers != NULL)
    {
        sprintf(http_headers, "%s%s\r\n", http_headers, custom_headers);
        sprintf(http_headers, "%s\r\n%s", http_headers, post_data);
    }
    else
    {
        sprintf(http_headers, "%s\r\n%s", http_headers, post_data);
    }
    http_headers = (char*)realloc(http_headers, strlen(http_headers) + 1);

    /* Make request and return response */
    struct http_response *hresp = http_req(http_headers, purl);

    /* Handle redirect */
    return handle_redirect_post(hresp, custom_headers, post_data);
}



/*
    Free memory of http_response
*/
void http_response_free(struct http_response *hresp)
{
    if(hresp != NULL)
    {
        if(hresp->request_uri != NULL) parsed_url_free(hresp->request_uri);
        if(hresp->body != NULL) free(hresp->body);
        if(hresp->status_code != NULL) free(hresp->status_code);
        if(hresp->status_text != NULL) free(hresp->status_text);
        if(hresp->request_headers != NULL) free(hresp->request_headers);
        if(hresp->response_headers != NULL) free(hresp->response_headers);
        free(hresp);
    }
}
