#!/bin/bash

# mount.sh
# Partner Names: Miko Lemecha, Artyom Kesler
# Due April 29, 2026
#
# CS F321 Spring 2026
# For PA5 (Lab 5: Virtualization)


# First, ensuring this script can only be run as root (same code exists in unmount.sh)

#### Anti-script-kiddie notes ####
## For `[ "$EUID" -ne 0 ]` ##
# `[` is a built-in Bash command equivalent to the `test` command: it evaluates whatever is inside.
# `$EUID` means "Effective User ID". Whenever you run a terminal, the OS assigns you a number.
# `-ne` means "Not Equal". (In Bash, you use -eq, -ne, -gt, -lt -ge -le for numbers. You only use == or != for strings.)
# `0` is hardcoded into Unix/Linux as root. Normal users usually start at UID 1000 while UID 1-999 are System Daemons.
#
## For the rest ##
# `exit 1` means "General Error". There are other exit codes i.e. `exit 0` => "Graceful Exit", `exit 127` => "Command not found"
# `fi` closes the block by mirroring the opening word (like how `case` closes with `esac`)
if [ "$EUID" -ne 0 ]
then
  printf "ERROR: This script modifies system mounts and can only be run using: sudo %s\n" "$0"
  exit 1
fi

# 1. Allocate space
dd if=/dev/zero of=vm1.img bs=1M count=20
dd if=/dev/zero of=vm2.img bs=1M count=20
dd if=/dev/zero of=vm3.img bs=1M count=20

# 2. Format the drives
mkfs.ext3 vm1.img
mkfs.ext3 vm2.img
mkfs.ext3 vm3.img

# 3. Create the mount points
mkdir -p /mnt/vm1
mkdir -p /mnt/vm2
mkdir -p /mnt/vm3

# 4. Attach the drives
# `-o loop` explicitly forces the OS to mount this file as a virtual block device a loopback interface in case WSL's auto-detection for loop devices fails (as it often does compared to native Linux)
mount -o loop vm1.img /mnt/vm1
mount -o loop vm2.img /mnt/vm2
mount -o loop vm3.img /mnt/vm3

echo "virtual disks mounted and ready to rip :DDDD"

