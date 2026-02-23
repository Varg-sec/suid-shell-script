## suid-shell-script

This simple C program allows you to take advantage of
the [setuid](https://en.wikipedia.org/wiki/Setuid) file flag and potentially the setgid
file flag on scripts.

This flag is normally ignored on scripts (bash, perl, python). Using this program you
can build a binary that executes a single script with arbitrary arguments. You can then
set the setuid and setgid flag on that binary, which will take effect and cause the
called script to inherit the permissions of the binary's owner or group.

### Warning

This is _very_ insecure and should be used very carefully. Even if you hardcode the path
to the script and the binary with setuid or setgid itself cannot be edited, if the
script it targets can be edited by any user then effectively that user can execute
arbitrary commands as the owner of the binary (which will probably be root).

This _can_ be safe if the binary and the script have similar permissions but you need to
think about this very carefully.

### Building

Since this script is dangerous it is designed to be built with a hardcoded path to a
single script it will execute. To provide this script you must set the `SCRIPT_PATH`
environment variable before calling make. Here is an example where the `SCRIPT_PATH` is
`ls`:

    SCRIPT_PATH=/bin/ls make

To build the qcow2-utils executable make sure
the [repository](https://github.com/Varg-sec/qcow2-utils) is located on the same level
as this one. Follow the instructions there to build the executable script with bashly.
This results in a `serveqcow2` executable. Execute the following commands:

```bash
export SCRIPT_PATH=$(realpath ../qcow2-utils/serveqcow2)     
sudo --preserve-env=SCRIPT_PATH make
```

The script's owner will be changed to root and all write permissions are removed to
hinder subsequent editing. The ownership of the suid executable is also root, without
any write permissions. Of course this does not increase security by much, but avoids a
straight forward privilege escalation. 
