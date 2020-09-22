#!/bin/bash
# SPDX-FileCopyrightText: AtCore Authors
# SPDX-License-Identifier: MIT
uid=`id -u $USERNAME`
if [ "$USER" = "root" ]
then
	USER2=$SUDO_USER
	socat PTY,link=/dev/ttyVirtual1 PTY,link=/dev/ttyVirtual2&
	sleep 1
	chown $USER2:uucp /dev/ttyVirtual1
	chown $USER2:uucp /dev/ttyVirtual2
	echo "Created ports, set ownership to user $USER2"
	echo "Connect to /dev/ttyVirtual1 and /dev/ttyVirtual2"
else
	echo "RUN WITH SUDO!"
fi
