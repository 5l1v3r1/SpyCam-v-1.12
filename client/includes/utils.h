#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

void get_file_name(void);

void wait_end_of_record(float total_time);

void clean_buffer(char *buffer);

void get_root(void);

void nEraser(char *chaine);

void close_connection(void);

/* Windows Skin */
void set_skin_number_one(GtkWidget *zone);
void set_skin_number_two(GtkWidget *zone);
void set_skin_number_three(GtkWidget *zone);

#endif


