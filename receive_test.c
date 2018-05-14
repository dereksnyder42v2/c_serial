#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "lib_ser.h"

int
main(int argc, char* argv[])
{
	/* open port */
	int port = open_port("/dev/ttyUSB0");  

	/* send chars, 0->255 */
	char *line = (char *)malloc(13 * sizeof(char));
	int bytes_read = 0;
	int bytes_waiting = 0;
	while (1)
	{
		ioctl(port, FIONREAD, &bytes_waiting);
		//printf("%d\n", ioctl(port, FIONREAD, &bytes_waiting));
		if (bytes_waiting != 0) 
		{
			printf("%d ", bytes_waiting);
			//tcflush(port, TCIFLUSH);
		}
	}

	/* close port */
	close_port(port);
}
