#!/bin/bash

# unmount.sh
# Partner Names: Miko Lemecha, Artyom Kesler
# Due April 29, 2026
#
# For CS F321 Spring 2026
# For PA5 (Lab 5: Virtualization)

# Running this script will effectively undo the effects of having run mount.sh
# This script satisfies 'Part 6: Cleanup'


# First, ensuring this script can only be run as root (same code exists in mount.sh)
if [ "$EUID" -ne 0 ]
then
  printf "ERROR: This script modifies system mounts and can only be run using: sudo %s\n" "$0"
  exit 1
fi


# 1. Unmount the filesystems from the mount points
umount /mnt/vm1
umount /mnt/vm2
umount /mnt/vm3

printf "mount points successfully unlinked! :DDDD\n"

printf "attempting to remove mount points: /mnt/vm1, /mnt/vm2, /mnt/vm3...\n"
printf "(this will safely fail if any data remains inside them!)\n"

# 2. Remove the mount points.
# Using the absolute binary path to `rm` so that we
# dodge rogue aliases (e.g. .bashrc for Linux, .zshrc for MacOS, etc.)
/bin/rm -d /mnt/vm1 /mnt/vm2 /mnt/vm3

# 3. Remove the local virtual block device files (the .img files)
/bin/rm vm1.img vm2.img vm3.img

printf "all evidence destroyed. We were never here. :DDDD\n"
