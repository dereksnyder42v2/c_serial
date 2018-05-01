#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

/* This is version 3--incorporates sequence numbers and checksums into packets for Forward Error Correction */

/* func.c Prototypes */
int open_port(void);
int write_port(int fd, const void *buf, size_t count);
int close_port(int fd);

/* Other Prototypes */

/* TCP style. Total length is  */
struct Packet 
{ 
	short	src;		/* 2  source identifier */
	short	dst;		/* 2  destination identifier */
	int	seq;		/* 4  sequence number */
	int	ack;		/* 4  acknowledgement number */
	short	checksum;	// 2
	char	len;		// 1
	char	data[40];	// 40
				/* 55 Byte packets */
};

/* TODO
 * currently, this code attempts to open the /dev/ttyUSB0 device; funcs.c 
 * must be manually changed to use a different device. it would be useful to
 * specify the port &/or transmission Baud rate as command line args.
 */
int main(int argc, char* argv[])
{
	int port_fd = open_port();
	/* Do stuff */

	/* pointer to file we want to send over port */
	FILE *sendfile_fp = fopen(argv[1], "r");
	struct stat st;
	stat(argv[1], &st);	/* stat() returns # bytes in file */
	const int FILESIZE = st.st_size;
	printf("Sending file of size %d B\n", FILESIZE);
	
	/* descriptor of file we want to send */
	int sendfile_fd = open(argv[1], O_RDONLY);


	/* Now, to business.
	 * Write file described by "sendfile_fd", 1 Byte at a time.
	 */
	int bytes_read;
	short ck;
	int bytes_written_temp;
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
	while ( file_offset < FILESIZE )
	{
		pac.seq = file_offset / 40; 
		bytes_read = pread(sendfile_fd, &pac.data, 40, file_offset);
		ck = 0;
		printf("Calculating checksum... ");
		for (int i = 0; i < bytes_read; i++) ck += (short)*(&pac.data + i);
		pac.checksum = ck;
		printf("done (%d).\n", (int)ck);
		pac.len = bytes_read;

		/* the while block below continues to send the same Byte until "write_port()"
		 * returns 1; -1 indicates an error (e.g. resource busy, no data written) 
		 * while 0 indicates no Bytes were written.
		 * TODO figure out why so many "resource busy" errors
		 */
		printf("Sending packet...\n");
		for (int i = 0; i < 55; i++)
		{
			bytes_written_temp = 0;
			while ( bytes_written_temp != 1 )
				bytes_written_temp = write_port(port_fd, ((char*)&pac + i), 1);
		}
		printf("Packet sent. File offset %d B\n", file_offset);		
		file_offset += bytes_read;
	}
	
	/* Done with stuff */
	fclose(sendfile_fp);
	close(sendfile_fd);
	int close_success = close_port(port_fd);
	printf("close_port() returned %d\n", close_success);

	return 0;
}

