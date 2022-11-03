#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <stddef.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include "../../lib/tlpi_hdr.h"

#define SERVER_KEY  0x1aaaaaa1


struct requestMsg {
        long mtype;
        int  clientPid;
        char pathname[PATH_MAX];
};

#define RESP_MSG_SIZE 8192

#define REQ_MSG_SIZE (offsetof(struct requestMsg, pathname) - \
                offsetof(struct requestMsg, clientPid) + PATH_MAX)

#define RESP_SIZE (offsetof(struct responseMsg, data) - \
                offsetof(struct responseMsg, stat) + numRead)


struct responseMsg {
        long mtype;
        int stat;
        char data[RESP_MSG_SIZE];
};
#define RESP_MT_FAILURE 1
#define RESP_MT_DATA    2
#define RESP_MT_END     3

