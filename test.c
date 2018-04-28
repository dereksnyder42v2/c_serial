#include <stdio.h>	/* Standard I/O definitions */
#include <string.h>	/* String function definitions */
#include <unistd.h> 	/* UNIX standard function definitions */
#include <fcntl.h>	/* File control definitions */
#include <errno.h>	/* Error number definitions */
#include <termios.h>	/* POSIX terminal control definitions */

#define _PORT 	"/dev/ttyAMA0"
/* Common baud rates
 * 	  B9600
 * 	 B19200
 *	B115200
 */
#define _BAUDRT B9600

/* Returns
 * 	(int) file descriptor of port
 */
int
open_port(void)
{
	int fd; // File descriptor for port

	fd = open(_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
	/* O_RDWR	read/write mode
	 * O_NOCTTY	tells UNIX that this program doesn't want to be the 'controlling terminal' for this port. Otherwise, keyboard abort signals (&c.) could affect process.
	 * O_NDELAY 	tells UNIX that the program doesn't care what state DCD ('Data Carrier Detect') line is in <--> in other words, what state the other end of the port is in
	 */
	if (fd == -1)
	{
		// Couldn't open port
		perror("open_port: Unable to open /dev/ttyAMA0 - ");
	}
	else {
		/* set third argument to 0 for blocking read() calls,
		 * or set to "FNDELAY" for nonblocking
		 */
		fcntl(fd, F_SETFL, FNDELAY);
	}

	return (fd);
}

/* Params
 *  	(int)	fd	 file descriptor of port
 *  	(ptr) 	buf	pointer to what to write
 *  	(size_t)count	max number of bytes to write
 * Returns
 * 	(int) number of bytes written, or -1 if error occurred 
 */
int
write_port(int fd, const void *buf, size_t count)
{
	int n = write(fd, buf, count);
	if (n < 0) {
		fputs("write_port: Write to port failed!\n", stderr);
	}

	return (n);
}

/* Params
 * 	(int)	fd	file descriptor of port
 * Returns
 * 	(int)	"0" on success, "-1" on failure
 */
int
close_port(int fd)
{
	int n = close(fd);

	return (n);
}

