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


const long vdisable = _POSIX_VDISABLE;
/* Params
 * 		(str) name of serial port, in /dev/
 * Returns
 * 		(int) file descriptor of port
 */
int
open_port(void* port_name)
{
	int fd; // File descriptor for port
	/* O_RDWR	read/write mode
	 * O_NOCTTY	tells UNIX that this program doesn't want to be the 
	 * 		'controlling terminal' for this port. Otherwise, 
	 * 		keyboard abort signals (&c.) could affect process.
	 * O_NDELAY 	tells UNIX that the program doesn't care what state 
	 * 		DCD ('Data Carrier Detect') line is in 
	 * 		<--> in other words, what state the other end of 
	 * 		the port is in
	 */
	//fd = open(PORT, O_RDWR | O_NOCTTY | O_NDELAY);
	fd = open(port_name, O_RDWR | O_NOCTTY );
	if (fd == -1)
	{
		// Couldn't open port
		perror("open_port(): Unable to open PORT - ");
	}
	else 
	{
		/* set third argument to 0 for blocking read() calls,
		 * or set to "FNDELAY" for nonblocking */
		fcntl(fd, F_SETFL, 0);
		
		struct termios options;
		/* Get current port configuration */
		tcgetattr(fd, &options);

		/* Set baud rate to definition (I and O respectively) */
		cfsetispeed(&options, BAUDRT);
		cfsetospeed(&options, BAUDRT);		

		/* Raw output */
		options.c_oflag &= ~OPOST;

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

		/* Types of input setup:
		 * Canonical	input is line-oriented; characters are put in a buffer and only sent when
		 * 				CR or NL is received
		 * Raw		input is unprocessed. Whatever goes in, goes out.
		 */
		//options.c_lflag |= (ICANON | ECHO | ECHOE); 		// canonical
		options.c_lflag &= !(ICANON | ECHO | ECHOE | ISIG);	// raw

		/* Disabling special characters, preserving actual bytes in TX/RX
		 * TODO 
		options.c_cc[VDISCARD] 	= vdisable;
		options.c_cc[VDSUSP] 	= vdisable;
		options.c_cc[VEOF]		= vdisable;
		options.c_cc[VEOL]		= vdisable;
		options.c_cc[VEOL2]		= vdisable;
		options.c_cc[VERASE]	= vdisable;
		//options.c_cc[VERASE2] = vdisable; // BSD only
		options.c_cc[VINTR] 	= vdisable;
		options.c_cc[VKILL] 	= vdisable;
		//options.c_cc[VINTR] 	= vdisable;
		//options.c_cc[VINTR] 	= vdisable;
		//options.c_cc[VINTR] 	= vdisable;
		//options.c_cc[VINTR] 	= vdisable;
		*/

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
		 * TCSADRAIN wait until IO operations finish
		 * TCSAFLUSH flush IO buffers and make changes 
		 */
		tcsetattr(fd, TCSAFLUSH, &options);
	}

	return (fd);
}

/* Params
 * 		(int)	fd	file descriptor of port
 * 		(*void)	buffer base pointer to write to 
 * 		(size_t)count	max number of Bytes to write into buffer
 * Returns
 * 		(int)	number of Bytes read from port
 */
int 
read_port(int fd, void* buf, size_t count)
{
	int bytes_read = read(fd, buf, count);
	if (bytes_read < 0)
		perror("Error: in read_port(): -");
	return bytes_read;
}

/* Params
 *  	(int)	fd	 	file descriptor of port
 *  	(ptr) 	buf		pointer to what to write
 *  	(size_t)count	max number of bytes to write
 * Returns
 * 		(int) 	number of bytes written, or -1 if error occurred 
 */
int
write_port(int fd, void* buf, size_t count)
{
	int n = write(fd, buf, count);
	if (n < 0) 
	{
		;
		/* TODO find a better way to handle write_port errors.
		 * getting a lot of "resource busy" errors etc.
		 */
		perror("write_port(): Unable to write to PORT -");
	}

	return (n);
}

/* Params
 * 		(int)	fd	file descriptor of port
 * Returns
 * 		(int)	"0" on success, "-1" on failure
 */
int
close_port(int fd)
{
	return (close(fd));
}

