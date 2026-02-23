serveqcow2: suid-shell-script.c
	gcc -DSCRIPTPATH="\"${SCRIPT_PATH}\"" suid-shell-script.c -o serveqcow2
	sed -i 's@#!/usr/bin/env bash@#!/usr/bin/bash@g' ${SCRIPT_PATH}
	chmod 544 ${SCRIPT_PATH}
	chown root ${SCRIPT_PATH}
	chown root serveqcow2
	chmod u+s serveqcow2

clean:
	rm serveqcow2
