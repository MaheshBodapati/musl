#include <string.h>

#include <bits/alltypes.h>

struct stat {
    dev_t st_dev;
    int st_pad1[3];
    ino_t st_ino;			/* File serial number.  */
    mode_t st_mode;			/* File mode.  */
    nlink_t st_nlink;			/* Link count.  */
    uid_t st_uid;			/* User ID of the file's owner. */
    gid_t st_gid;			/* Group ID of the file's group.*/
    dev_t st_rdev;			/* Device number, if device.  */
    unsigned int st_pad2[2];
    off_t st_size;			/* Size of file, in bytes.  */
    int st_pad3;
    struct timespec st_atim;		/* Time of last access.  */
    struct timespec st_mtim;		/* Time of last modification.  */
    struct timespec st_ctim;		/* Time of last status change.  */
    blksize_t st_blksize;		/* Optimal block size for I/O.  */
    unsigned int st_pad4;
    blkcnt_t st_blocks;			/* Number of 512-byte blocks allocated.  */
    int st_pad5[14];
};

