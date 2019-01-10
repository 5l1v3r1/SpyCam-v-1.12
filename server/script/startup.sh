#!/bin/bash

### BEGIN INIT INFO
# Provides:          SpyCam_Server
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: SpyCam server
# Description:       Main SpyCam server
### END INIT INFO

DESC="Programme:"
DAEMON=/usr/bin/SpyCam_Server

update-rc.d daemon_server.sh start 30 2 3 4 5 . stop 10 0 1 6 .

echo -e "\nSPyCam v 1.34 added to startup routine.\n"

