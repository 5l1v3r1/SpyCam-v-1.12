#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#include <errno.h>  //errno
#include <string.h> //strerror()
#include <stdlib.h> //printf()

#include <gst/gst.h>

#define error(expression, function)     printf("ERROR : %s\nFunction : %s\nError Number : %d\nError Message : %s\n", expression, function, errno, strerror(errno));

#define PORT    4444
#define MAXDATASIZE 70000

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close (s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

typedef struct _CustomData
{
    gboolean is_live;
    GstElement *pipeline;
    GMainLoop *loop;

}CustomData;

void dispatch_modules(char *argv[]);
void recv_upload(void);

void *get_remote_screen_resolution(void);

void execute_watch_cmd(void);
void cb_message(GstBus *bus, GstMessage *msg, CustomData *data);

void *execute_record_cmd(void);
void *upload_screenshot_file(void);
void *download_multi_screenshot_files(void);

void *start_remote_shell(char *argv[]);

void daemonize(void);

#endif




