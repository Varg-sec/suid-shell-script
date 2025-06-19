## suid-shell-script

This simple C program allows you to take advantage of the [setuid](https://en.wikipedia.org/wiki/Setuid) file flag and potentially the setgid file flag on scripts.

This flag is normally ignored on scripts (bash, perl, python). Using this program you can build a binary that executes a single script with arbitrary arguments. You can then set the setuid and setgid flag on that binary, which will take effect and cause the called script to inherit the permissions of the binary's owner or group.

### Warning

This is _very_ insecure and should be used very carefully. Even if you hardcode the path to the script and the binary with setuid or setgid itself cannot be edited, if the script it targets can be edited by any user then effectively that user can execute arbitrary commands as the owner of the binary (which will probably be root).

This _can_ be safe if the binary and the script have similar permissions but you need to think about this very carefully.

### Building

Since this script is dangerous it is designed to be built with a hardcoded path to a single script it will execute. To provide this script you must set the `SCRIPT_PATH` environment variable before calling make. Here is an example where the `SCRIPT_PATH` is `ls`:


    SCRIPT_PATH=/bin/ls make

### Use case

I occasionally use a program called [input-remapper](https://github.com/sezanzeb/input-remapper) that requires root privileges because the thing it does is remapping hardware inputs to other hardware inputs. However, I also want to be able to trigger changes in the mapping via a keyboard shortcut.

But if I need to use sudo (or be root) to run this program how can I map it to a keyboard shortcut? I will have no opportunity to provide a password (nor do I want to every time I hit a keybind). So I designed this program to escalated privileges in the shell script I created to configure my keyboard inputs through input-remapper so that it may make changes to the hardware. As a reference this is what that script looks like

    #!/usr/bin/env bash

    set -ex

    DEVICE_NAME="${DEVICE_NAME:-My Kool Keyboard}"
    PRESET_NAME="${PRESET_NAME:-The greatest preset you have ever seen}"

    if [ "${1}" = "start" ]; then
      input-remapper-control \
        --device "${DEVICE_NAME}" \
        --preset "${PRESET_NAME}" \
        --command start
      exit 0
    elif [ "${1}" = "stop" ]; then
      input-remapper-control \
        --device "${DEVICE_NAME}" \
        --command stop-all
      exit 0
    else
      echo "Invalid command provided"
      exit 1
    fi

This is being called from the following control script that handles toggling it back and forth

    #!/usr/bin/env bash

    set -x

    TEMP_DIRECTORY="${TMP:-/tmp}"
    MARKER_FILE="${TEMP_DIRECTORY}/.my-marker-file}"

    export SCRIPT_PATH="${HOME}/.local/bin/my-kool-script"

    if [ -e "${MARKER_FILE}" ]; then
      suid-shell-script stop
      rm ${MARKER_FILE}
      notify-send "Input remapping is disabled"
    else
      suid-shell-script start
      touch ${MARKER_FILE}
      notify-send "Input remapping is enabled"
    fi
