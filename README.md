# serveqcow2 launcher

The launcher executes one fixed QCOW2 helper script and forwards each argument
directly. The mount command also receives the original caller's UID, and the
script runs with a fixed system PATH.

`make` builds the launcher without changing privileges or installing either
file. The default script path is
`/usr/local/libexec/fortrace/serveqcow2.sh`. To build for a different absolute
path, run `make clean` and then `make SCRIPT_PATH=/absolute/path/to/script`.
Follow the [host installation instructions](../howto-setuid.md) for an elevated
installation.
