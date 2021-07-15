# Syncing Files with Rsync

The `rsync` command in linux allows us to sync files both locally as well as remotely across different directories and machines.

# Why `rsync`?

The important thing to understand here is that `rsync` only copies (synchronises) what's different (somewhat like git). This means that every time something is updated in a directory at a particular location, only the *changed* portions since the last backup will get synced, instead of again copying the entire directory.

Another scenario would be when you lose your network connection in the middle of taking a backup. Even in this case `rsync` will know where it left off, and continue from there.


# References - 
1. https://www.youtube.com/watch?v=qE77MbDnljA
2. https://www.youtube.com/watch?v=2PnAohLS-Q4

a. https://stackoverflow.com/questions/818255/in-the-shell-what-does-21-mean
b. https://serverfault.com/questions/835104/what-does-connecttimeout-in-ssh-config-mean
c. https://www.perturb.org/display/1208_SSH_Batch_mode_for_SSH_SCP_scripting.html 