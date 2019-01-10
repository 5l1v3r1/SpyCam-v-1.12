#ifndef KEYLOGGER_UTILS_H_INCLUDED
#define KEYLOGGER_UTILS_H_INCLUDED

void get_kb_device_filename(const char *command);

void error(const char *expression);

void open_kb_device(char *deviceFile);

#endif
