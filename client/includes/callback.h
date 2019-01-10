#ifndef CALLBACK_H_INCLUDED
#define CALLBACK_H_INCLUDED

/* Upload/Install part */
void get_server_ip(GtkButton *button, gpointer user_data);
void get_server_port(GtkButton *button, gpointer user_data);

void open_file(GtkButton *button, gpointer user_data);
void get_path(GtkWidget *bouton, GtkWidget *file_selection);

void get_log_name(void);

/* Ubuntu tab Command */
void cb_ubuntu_watch(GtkWidget *toggle, gpointer data);
void cb_ubuntu_record(GtkWidget *toggle, gpointer data);
void cb_ubuntu_screenshot(GtkWidget *toggle, gpointer data);
void cb_ubuntu_mutli_screenshot(GtkWidget *toggle, gpointer data);

/* Mint tab Command */
void cb_mint_watch(GtkWidget *toggle, gpointer data);
void cb_mint_record(GtkWidget *toggle, gpointer data);
void cb_mint_screenshot(GtkWidget *toggle, gpointer data);
void cb_mint_mutli_screenshot(GtkWidget *toggle, gpointer data);

/* debian tab Command */
void cb_debian_watch(GtkWidget *toggle, gpointer data);
void cb_debian_record(GtkWidget *toggle, gpointer data);
void cb_debian_screenshot(GtkWidget *toggle, gpointer data);
void cb_debian_mutli_screenshot(GtkWidget *toggle, gpointer data);

/* Kali tab Command */
void cb_kali_watch(GtkWidget *toggle, gpointer data);
void cb_kali_record(GtkWidget *toggle, gpointer data);
void cb_kali_screenshot(GtkWidget *toggle, gpointer data);
void cb_kali_mutli_screenshot(GtkWidget *toggle, gpointer data);

/* Closing server connection */
void cb_close_connection(GtkButton *button, gpointer user_data);

//void mint_close_pipe_line(GtkButton *button, gpointer user_data);
//void debian_close_pipe_line(GtkButton *button, gpointer user_data);
//void kali_close_pipe_line(GtkButton *button, gpointer user_data);

/* Remote Shell */
void start_remote_shell(GtkButton *button, gpointer user_data);
void send_cmd(GtkWidget *widget, gpointer user_data);

/* Keylogger */
void ubuntu_run_the_keylogger(GtkButton *button, gpointer user_data);
void mint_run_the_keylogger(GtkButton *button, gpointer user_data);
void debian_run_the_keylogger(GtkButton *button, gpointer user_data);
void kali_run_the_keylogger(GtkButton *button, gpointer user_data);

void open_log_file(GtkButton *button, gpointer user_data);

/* Windows Skins */
void cb_set_skin_1(GtkButton *button, gpointer user_data);
void cb_set_skin_2(GtkButton *button, gpointer user_data);
void cb_set_skin_3(GtkButton *button, gpointer user_data);

#endif


