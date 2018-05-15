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

	/* send chars, 0->127 */
	for (char j = 0; j <= 127; j++)
	{
		getchar();
		printf("Sending %03d (%c)\n", (int)j, j);
		write_port(port, &j, 1);
	}

	/* close port */
	close_port(port);
}
