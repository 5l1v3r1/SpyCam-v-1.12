#!/bin/bash

### BEGIN INIT INFO
# Provides:          Ubuntu_Serv_SpyCam
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Ubuntu webcam control tool
# Description:       description
### END INIT INFO

DESC="Programme:"
DAEMON=/usr/sbin/Ubuntu_Serv_SpyCam

PATH=/bin:/usr/bin:/sbin:/usr/sbin

# Optionnel : définit le nom qui s'affichera

# lors du démarrage ou de l'arrêt du du démon

NAME=Ubuntu_Serv_SpyCam

# Emplacement du binaire du démon

DAEMON=/usr/bin/Ubuntu_Serv_SpyCam

# On vérifie que le programme du démon

# est exécutable, sinon fin du script.

test -x $DAEMON || exit 0

case "$1" in

    start)

# On vérifie si le démon

        # n'est pas déjà lancé.

        if [ -z "$(ps -A | grep $NAME)" ]

then

            #echo "$NAME : lancement du démon."

            $NAME & >& /dev/null

        fi

;;

esac

exit 1
