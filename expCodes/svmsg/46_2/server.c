#include "head.h"
  
static void
serveRequest(const struct requestMsg *req){
        int fd;
        ssize_t numRead;
        struct responseMsg resp;

        fd = open(req->pathname, O_RDONLY);


        int mqd = msgget(SERVER_KEY, S_IWUSR);
        if (fd == -1) {
                resp.mtype = req->clientPid;
                resp.stat = RESP_MT_FAILURE;
                snprintf(resp.data, sizeof(resp.data), "%s", "Couldn't open");
                msgsnd(mqd, &resp, strlen(resp.data) + 1 + sizeof(int), 0);
                printf("error, server is sending response, resp.mtype is %ld, resp.stat is %d, mqd is %d\n", resp.mtype, resp.stat, mqd);
                exit(EXIT_FAILURE);
        }

        resp.mtype = req->clientPid;
        resp.stat = RESP_MT_DATA ;
        while ((numRead = read(fd, resp.data, RESP_MSG_SIZE)) > 0) {
                printf("numRead is %d\n", numRead);
                if (msgsnd(mqd, &resp, RESP_SIZE  , 0) == -1) {
                        errExit("msgnd");
                        printf("error, server cant send response, resp.mtype is %ld, resp.stat is %d,  mqd is %d\n", resp.mtype, resp.stat, mqd);
                        break;
                }
                else
                        printf("normal, server is sending response, resp.mtype is %ld, resp.stat is %d, \n", resp.mtype, resp.stat);
        }
        resp.mtype = req->clientPid;
        resp.stat = RESP_MT_END;
        msgsnd(mqd, &resp, 0, 0);
        printf("server finished the response\n");
}

int
main(int argc, char *argv[])
{
        struct requestMsg req;
        pid_t pid;
        ssize_t msgLen;
        int mqd;

        mqd = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL |
                        S_IRUSR | S_IWUSR | S_IWGRP);
        if (mqd == -1)
                errExit("msgget");
	       for (;;) {
                msgLen = msgrcv(mqd, &req, REQ_MSG_SIZE, 0, 0);
                printf("server receive request, req.clientPid is %d, pathname is %s\n", req.clientPid, req.pathname);
                if (msgLen == -1) {
                        if (errno == EINTR)
                                continue;
                        errMsg("msgrcv");
                        break;
                }
                serveRequest(&req);
         }

        if (msgctl(mqd, IPC_RMID, NULL) == -1)
                errExit("smgctl");
        exit(EXIT_SUCCESS);
}
