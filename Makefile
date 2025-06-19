suid-shell-script: suid-shell-script.c
	gcc -DSCRIPTPATH="\"${SCRIPT_PATH}\"" suid-shell-script.c -o suid-shell-script
	chmod u+s suid-shell-script
	sudo chown root suid-shell-script

clean:
	rm suid-shell-script
