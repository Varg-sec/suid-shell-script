SCRIPT_PATH ?= /usr/local/libexec/fortrace/serveqcow2.sh

serveqcow2: suid-shell-script.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DSCRIPTPATH='"$(SCRIPT_PATH)"' $< $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -f serveqcow2
