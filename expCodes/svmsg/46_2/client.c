#include "head.h"

int
main(int argc, char *argv[])
{
        struct requestMsg req;
        ssize_t totBytes;

        if (argc != 2 || strcmp(argv[1], "--help") == 0)
                usageErr("%s pathname", argv[0]);

        if (strlen(argv[1]) > sizeof(req.pathname) - 1)
                cmdLineErr("pathname too long (max: %ld bytes)\n",
                        (long) sizeof(req.pathname) - 1);

        int mqd;
        mqd = msgget(SERVER_KEY, S_IWUSR);
        if (mqd == -1)
                errExit("msgget");
  
        req.mtype = 1;
        req.clientPid = getpid();
        strncpy(req.pathname, argv[1], sizeof(req.pathname) - 1);
        req.pathname[sizeof(req.pathname)] = '\0';

        if (msgsnd(mqd, &req, REQ_MSG_SIZE, 0) == -1)
                errExit("msgsnd");

        struct responseMsg resp;
        ssize_t msgLen;
        msgLen = msgrcv(mqd, &resp, RESP_MSG_SIZE, getpid(), 0);
        printf("resp.stat is: %d, resp.mtype is %d, msgLen is %d\n", resp.stat, resp.mtype, msgLen);
        if (msgLen == -1)
                errExit("msgrcv");

        if (resp.stat == RESP_MT_FAILURE) {
                printf("%s \n", resp.data);
                exit(EXIT_FAILURE);
        }

        int numMsgs;
        totBytes = msgLen;
        printf("resp.stat is : %d\n", resp.stat);
        for (numMsgs = 1; resp.stat == RESP_MT_DATA; numMsgs++) {
                msgLen = msgrcv(mqd, &resp, RESP_MSG_SIZE, getpid(), 0);
                printf("resp.mtype is %ld\n",(long int) resp.mtype);
                if (msgLen == -1)
                        errExit("msgrcv");
                totBytes += msgLen;
        }

        printf("Received %ld bytes (%d messages)\n", (long) totBytes, numMsgs);

        exit(EXIT_SUCCESS);
}
