#!/bin/bash

### BEGIN INIT INFO
# Provides:          Serv_SpyCam
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: main server
# Description:       Main SpyCam server
### END INIT INFO


cd /usr/bin/

sudo rm -rf Serv_SpyCam

cd /usr/sbin/

sudo rm -rf Serv_SpyCam 

cd /etc/init.d/

sudo rm -rf daemon_server.sh

cd /etc/rc2.d/

sudo rm -rf 'S02daemon_server.sh'

sudo rm -rf 'S88daemon_server.sh'

cd /etc/rc3.d/

sudo rm -rf 'S02daemon_server.sh'

cd /etc/rc4.d/

sudo rm -rf 'S02daemon_server.sh'

cd /etc/rc5.d/

sudo rm -rf 'S02daemon_server.sh'

echo "SpyCam have been removed from startup routine."
