#!/bin/bash

### BEGIN INIT INFO
# Provides:          Mint_Serv_SpyCam
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Mint Webcam control tool
# Description:       Description
### END INIT INFO

DESC="Programme:"
DAEMON=/usr/bin/Mint_Serv_SpyCam

update-rc.d daemon_server.sh start 30 2 3 4 5 . stop 10 0 1 6 .

echo -e "\nMint_Serv_SpyCam 1.0 added to startup routine.\n"

