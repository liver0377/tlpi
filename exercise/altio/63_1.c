/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 63-2 */

#include "../lib/tlpi_hdr.h"
#include <poll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>

int main(int argc, char *argv[]) {
  int numPipes, j, ready, randPipe, numWrites;
  int(*pfds)[2]; /* File descriptors for all pipes */
  fd_set rset;
  int maxfd = -1;

  if (argc < 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s num-pipes [num-writes]\n", argv[0]);

  /* Allocate the arrays that we use. The arrays are sized according
     to the number of pipes specified on command line */

  // 创建的管道数目
  numPipes = getInt(argv[1], GN_GT_0, "num-pipes");

  pfds = calloc(numPipes, sizeof(int[2]));
  if (pfds == NULL)
    errExit("calloc");

  for (j = 0; j < numPipes; j++)
    if (pipe(pfds[j]) == -1)
      errExit("pipe %d", j);

  // 建立兴趣列表
  FD_ZERO(&rset);
  for (int i = 0; i < numPipes; i++) {
    FD_SET(pfds[i][0], &rset);
    if (pfds[i][0] > maxfd) {
      maxfd = pfds[i][0];
    }
  }

  // 写入管道的数目
  numWrites = (argc > 2) ? getInt(argv[2], GN_GT_0, "num-writes") : 1;

  // 随机向几个管道当中写数据
  srandom((int)time(NULL));
  for (j = 0; j < numWrites; j++) {
    randPipe = random() % numPipes;
    printf("Writing to fd: %3d (read fd: %3d)\n", pfds[randPipe][1],
           pfds[randPipe][0]);
    if (write(pfds[randPipe][1], "a", 1) == -1)
      errExit("write %d", pfds[randPipe][1]);
  }

  ready = select(maxfd + 1, &rset, NULL, NULL, NULL);
  if (ready == -1)
    errExit("select");

  printf("select() returned: %d\n", ready);

  for (int i = 0; i < numPipes; i++) {
    if (FD_ISSET(pfds[i][0], &rset)) {
      printf("Readable: %3d\n", pfds[i][1]);
    }
  }

  exit(EXIT_SUCCESS);
}
