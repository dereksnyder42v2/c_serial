#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib_ser.h"

struct Packet 
{ 
	short	src;		/* 2  source identifier */
	short	dst;		/* 2  destination identifier */
	int		seq;		/* 4  sequence number */
	int		ack;		/* 4  acknowledgement number */
	short	checksum;	// 2
	char	len;		// 1
	char	data[40];	// 40
						/* 55 Byte packets */
};

void print_packet(struct Packet pac)
{
	printf("\nsrc\n\t%d\n", (int)pac.src);
	printf("dst\n\t%d\n", (int)pac.dst);
	printf("seq\n\t%d\n", pac.seq);
	printf("ack\n\t%d\n", pac.ack);
	printf("checksum\n\t%d\n", pac.checksum);
	printf("len\n\t%d\n", pac.len);
	for (int j = 0; j < 40; j++)
	{
		if (j % 8 == 0) printf("\n"); 
		printf("%02x ", (char)pac.data[j]);
	}
	printf("\n");
	
	return;
}

int 
main(int argc, char* argv[])
{
	int port_fd = open_port(argv[2]);
	if (port_fd == -1) printf("open_port() returned %d\n", port_fd);
	/* Do stuff */

	/* descriptor of file we want to write results to. */
	int rcvfile_fd = open(argv[1], O_WRONLY);

	/* Now, to business.
	 * Write to file described by "rcvfile_fd", 1 Byte at a time.
	 */
	int bytes_read = 0;
	short ck;
	int file_offset = 0; 	/* 'how far in' to the file we are sending */
	
	struct Packet pac;
	/* Not using these fields for now...*/
	pac.src = 	(short)0;
	pac.dst = 	(short)0;
	/* ... */
	pac.seq = 	(int)0;
	pac.ack = 	(int)0;
	pac.checksum = 	(short)0;
	pac.len = 	(char)0;
	printf("Listening to %s\n", argv[2]);

	char c;
	char buf[55];
	while (1)
	{
		bytes_read += read_port(port_fd, &c, 1);
		buf[bytes_read] = c;

		if ( bytes_read >= 55 )
		{
			bytes_read = 0;
			/* Write buffer to pac */
			char* pac_ptr = &pac;
			for (int i = 0; i < 55; i++)
			{
				*pac_ptr = buf[i];
				printf("%c", (char)*pac_ptr);
				pac_ptr += sizeof(char);	
			}
			bytes_read = 0;
			print_packet(pac);
		}
	}
	print_packet(pac);
			
	/* Done with stuff */
	close(rcvfile_fd);
	int close_success = close_port(port_fd);
	if ( close_success != 0) printf("\nclose_port() returned %d\n", close_success);

	return 0;
}


