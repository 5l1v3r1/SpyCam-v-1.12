// Note: not sure if "WINDOWS" or "WIN32" or something else is defined on Windows
#ifdef WINDOWS
    #include <window.h>
    #include <winsock2.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>  //gethostbyaddr()

    #include <stdio.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <unistd.h>
    #include <string.h>

    #include <gtk-2.0/gtk/gtk.h>

#endif

#include "../includes/constants.h"

GtkWidget *log_entry;
const gchar *chemin;

const gchar *server_ip;
const char *server_port;
int port;

GtkWidget *text_view;

void upload()
{
    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    long weight = 0;
    int err = 0;
    size_t flag = 1;

    long dataSend = 0;
    long dataRead = 0;
    long totalSend = 0;
    char buffer[2048] = "";
    char ffmpeg_result[64] = "";

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;

    const char *log_name = NULL;
    size_t len_log_name = 0;

    FILE *log = NULL;

    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "upload()");
        exit(-1);
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    if(err == SOCKET_ERROR)
    {
        error("connect()", "upload()");
        exit(-1);
    }

    if(send(sock, (char*)&flag, sizeof(flag), 0) == SOCKET_ERROR)
    {
        error("send() flag", "upload()");
        exit(-1);
    }

    log = fopen(chemin, "rb");
    if(log == NULL)
    {
        error("fopen() log", "upload()");
        exit(-1);
    }

    /* weight of the app file */
    fseek(log, 0, SEEK_END);
    weight = ftell(log);
    rewind(log);

    if(send(sock, (char*)&weight, sizeof(weight), 0) == SOCKET_ERROR)
    {
        error("send() file weight", "upload()");
        exit(-1);
    }

    printf("Envoie du poid reussis : %ld\n\nSome checking will be done plus ffmpeg and gstreamer will be installed, so it can take more than one minute be patient please ....\n\n", weight);

    log_name = gtk_entry_get_text(GTK_ENTRY(log_entry));

    len_log_name = strlen(log_name) + 1;

    if(send(sock, (char*)&len_log_name, sizeof(len_log_name), 0) == SOCKET_ERROR)
    {
        error("send() len_log_name", "upload()");
        exit(-1);
    }

    if(send(sock, log_name, len_log_name, 0) == SOCKET_ERROR)
    {
        error("send() log_name", "upload()");
        exit(-1);
    }

    do
    {
        dataRead = fread(buffer, sizeof(char), weight, log);
        if(dataRead < 0)
        {
            error("send() dataRead", "upload()");
            exit(-1);
        }

        dataSend = send(sock, buffer, weight, 0);
        if(dataSend == 0)
        {
            error("send() dataSend", "upload()");
            exit(-1);
        }

        totalSend += dataSend;

    }while(totalSend < weight);

    if(recv(sock, ffmpeg_result, sizeof(ffmpeg_result), 0) == SOCKET_ERROR)
    {
        error("recv() ffmpeg_result", "upload()");
        exit(-1);
    }

    printf("%s\n", ffmpeg_result);

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "ffmpeg and gstreamer have been installed ...", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    fclose(log);

    close(sock);

    return;
}
