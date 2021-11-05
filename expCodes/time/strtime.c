#define _XOPEN_SOURCE
#include<time.h>
#include<locale.h>
#include"../lib/tlpi_hdr.h"

#define SBUF_SIZE 1000

int main(int argc, char **argv){
	struct tm tm;
	char sbuf[SBUF_SIZE];
	char *ofmt;

	if(argc < 3 || strcmp(argv[1], "--help") == 0)
		usageErr("%s input-date-time in-format [out-format]\n", argv[0]);

	if(setlocale(LC_ALL, "") == NULL)
		errExit("setlocale");

	memset(&tm, 0, sizeof(struct tm));					//初始化结构体
	if(strptime(argv[1], argv[2], &tm) == NULL)
		fatal("strptime");

	tm.tm_isdst = -1;							//strptime不会设置该字段，需要自己手动设置

	printf("calendar time (seconds since Epoch): %ld\n", (long) mktime(&tm));

	ofmt = (argc > 3) ? argv[3] : "%H:%M:%S %A, %d %B %Y %Z";  		//默认输出格式
	if(strftime(sbuf, SBUF_SIZE, ofmt, &tm) == 0)
		fatal("strtime() yields: %s\n", sbuf);

	exit(EXIT_SUCCESS);
}



