#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include "lib_ser.h"

int
main(int argc, char* argv[])
{
	/* open port */
	int port = open_port("/dev/ttyS0");  

	/* send chars, 0->255 */
	char *str = (char *)malloc(14 * sizeof(char));
	for (int j = 33; j <= 126; j++)
	{
		sprintf(str, "Sending %03d %c\n", j, j);
		//str = "Sending 000 00"
		printf("%s", str);
		getchar();
		write_port(port, str, 14);
	}

	/* close port */
	close_port(port);
}
