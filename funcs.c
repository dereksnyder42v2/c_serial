#include <stdio.h>	/* Standard I/O definitions */
#include <string.h>	/* String function definitions */
#include <unistd.h> 	/* UNIX standard function definitions */
#include <fcntl.h>	/* File control definitions */
#include <errno.h>	/* Error number definitions */
#include <termios.h>	/* POSIX terminal control definitions */

/* Lots of credit to Michael R. Sweet for his writeup. 
 * A lot of this is directly or slightly indirectly his code, adapted for my uses.
 * http://www.cmrr.umn.edu/~strupp/serial.html#2_1
 * 
 * Derek Snyder
 * Fri 27 Apr 20:53:06 EDT 2018
 */

/* ttyAMA0 should be the port for the onboard 
 * but maybe not.
 */
//#define PORT 	"/dev/ttyAMA0"	
#define PORT	"/dev/ttyUSB0"

/* Common baud rates
 * 	  B9600
 * 	 B19200
 *	B115200
 */
#define BAUDRT 	B9600

/* Common char size and parity bit setups
 * 8N1 	No parity
 * 7E1  Even parity
 * 7O1  Odd parity
 * 7S1  Space parity/ no parity
 */
#define _8N1	0
#define _7E1	1
#define _7O1	2
#define _7S1	3

#define PARITY 	_8N1


/* Returns
 * 	(int) file descriptor of port
 */
int
open_port(void)
{
	int fd; // File descriptor for port

	fd = open(PORT, O_RDWR | O_NOCTTY | O_NDELAY);
	/* O_RDWR	read/write mode
	 * O_NOCTTY	tells UNIX that this program doesn't want to be the 
	 * 		'controlling terminal' for this port. Otherwise, 
	 * 		keyboard abort signals (&c.) could affect process.
	 * O_NDELAY 	tells UNIX that the program doesn't care what state 
	 * 		DCD ('Data Carrier Detect') line is in 
	 * 		<--> in other words, what state the other end of 
	 * 		the port is in
	 */
	if (fd == -1)
	{
		// Couldn't open port
		perror("open_port: Unable to open PORT - ");
	}
	else 
	{
		/* set third argument to 0 for blocking read() calls,
		 * or set to "FNDELAY" for nonblocking */
		fcntl(fd, F_SETFL, FNDELAY);
		
		struct termios options;
		/* Get current port configuration */
		tcgetattr(fd, &options);

		/* Set baud rate to definition (I and O respectively) */
		cfsetispeed(&options, BAUDRT);
		cfsetospeed(&options, BAUDRT);		

		/* Set control options with c_cflag member.
		 * These should be set as so pretty much always...
		 * 	CLOCAL	local line; don't change port owner
		 * 	CREAD	enable receiver
		 */
		options.c_cflag |= (CLOCAL | CREAD);

		/* Mask character size bits; gotta do this before setting */
		options.c_cflag &= ~CSIZE;
		/* Now set parity based on #def */
		if (PARITY == _8N1)
		{
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			options.c_cflag |= CS8;
		}
		else if (PARITY == _7E1)
		{
			options.c_cflag |= PARENB;
			options.c_cflag &= ~PARODD;
			options.c_cflag &= ~CSTOPB;
			options.c_cflag |= CS7;
		} 
		else if (PARITY == _7O1)
		{
			options.c_cflag |= PARENB;
			options.c_cflag |= PARODD;
			options.c_cflag &= ~CSTOPB;
			options.c_cflag |= CS7;
		} 
		else if (PARITY == _7S1)
		{
			options.c_cflag &= ~PARENB;	
			options.c_cflag &= ~CSTOPB;
			options.c_cflag |= CS8;
		}

		/* Types of input setup
		 * Canonical	input is line-oriented; characters are put in a buffer and only sent when
		 * 				CR or NL is received
		 * Raw			input is unprocessed. Whatever goes in, goes out.
		 */
		//options.c_lflag |= (ICANON | ECHO | ECHOE); 		// canonical
		options.c_lflag &= !(ICANON | ECHO | ECHOE | ISIG);	// raw

		// TODO
		/* Input parity checking
		 *
		 */

		// TODO
		/* Software flow control
		 *
		 */

		// TODO
		/* Output options
		 * 
		 */

		/* Now set new options. 
		 * TCSANOW 	means do it now, instead of waiting for input
		 * 		output operations to finish 
		 * TCSADRAIN 	wait until IO operations finish
		 * TCSAFLUSH 	flush IO buffers and make changes 
		 */
		tcsetattr(fd, TCSANOW, &options);
	}

	return (fd);
}

/* Params
 *  	(int)	fd	 	file descriptor of port
 *  	(ptr) 	buf		pointer to what to write
 *  	(size_t)count	max number of bytes to write
 * Returns
 * 	(int) number of bytes written, or -1 if error occurred 
 */
int
write_port(int fd, const void *buf, size_t count)
{
	int n = write(fd, buf, count);
	if (n < 0) 
	{
		;
		/* TODO find a better way to handle write_port errors.
		 * getting a lot of "resource busy" errors etc.
		 */
		//perror("write_port: Write to port failed -");
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

