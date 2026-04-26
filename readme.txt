Setup guide for WSL:

Do not run this in the Windows /mnt/c/ filesystem.
WSL experiences heavy latency and permission loss when crossing the
OS boundary. We must build this in the native Linux filesystem.

Step 1: Use the Linux filesystem
1. Open your WSL Terminal.
2. Go to the home directory:
    cd ~
3. Create the project directory:
   mkdir -p ~/f321_operating_systems
   (or whatver file structure you like)
4. Move into the directory:
   cd ~/f321_operating_systems
5. Clone the repo:
    git clone https://github.com/A-rtemi-s/lab5_f321_opsys.git
    (if you don't want to use github, go to https://github.com/A-rtemi-s/lab5_f321_opsys and
    download the files mount.sh, unmount.sh, and lab5.c, and put them in this directory)

Step 2: Make the scripts executable
Run this command ONCE to give the system permission to run our scripts:
chmod +x mount.sh unmount.sh

Step 3: Execution order
1. Build the Virtual Drives (run all scripts as root with sudo):
   sudo ./mount.sh

2. Compile the C Program:
   gcc lab5.c -o lab5
   (protip: i do `gcc lab5.c -o l` to save time)

3. Run the Virtual Machines:
   sudo ./lab5
   (protip: i do `sudo ./l` to save time)

4. Undo all effects of running ./mount.sh like this:
   sudo ./unmount.sh





Bonus (safe to ignore): How to see the effects of mount.sh and unmount.sh to make sure they work (follow these instructions in the directory with the mount.sh and unmount.sh scripts):

1. Run the mount script:
    sudo ./mount.sh

2. Run the 'disk free' command:
    df -h
    Check the bottom of the output. Do you see /dev/loop0, /dev/loop1, and /dev/loop2 attached to your /mnt/vm1 folders? Do they say they have 20MB of space? If so, success.

3. Run the 'list' command:
    ls -al
    Are the files vm1.img, vm2.img, and vm3.img present? If so, success.

4. Run the unmount script:
    sudo ./unmount.sh

5. Run the 'disk free' command again:
    df -h
    Are the loops gone? If so, success.

6. Run the 'list' command again:
    ls -al
    Are the files vm1.img, vm2.img, and vm3.img absent? If so, success.

