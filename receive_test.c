#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "lib_ser.h"

/* TODO
 * look into hardware flow control so that chars
 * "13d" Carriage Return (converted into "10d" Newline)
 * "17d", Device control 1 
 * & "19d" Device control 3
 * aren't gobbled up by terminal...
 */
int
main(int argc, char* argv[])
{
	/* open port */
	int port = open_port("/dev/ttyUSB0");  

	int bytes_read = 0;
	int bytes_waiting_new = 0;
	int bytes_waiting_old = 0;
	char buf;
	//int buf_convert;
	//tcflush(port, TCIFLUSH);
	while (1)
	{
		/* Store # Bytes in serial buffer in bytes_waiting_new 
		while (bytes_waiting_new == 0)
		{
			ioctl(port, FIONREAD, &bytes_waiting_new);
		}
		printf("%d B waiting. (Press RETURN)\n");
		
		getchar();
		*/
		bytes_read = read_port(port, &buf, 1);
		if (bytes_read > 0)
			printf("	read %d B\t\t%d\n", bytes_read, (int)buf);
		//bytes_waiting_new -= bytes_read;
		//printf("%d B waiting...\n", bytes_waiting_new);
	}

	/* close port */
	close_port(port);
}
