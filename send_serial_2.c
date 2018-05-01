#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

/* func.c Prototypes */
int open_port(void);
int write_port(int fd, const void *buf, size_t count);
int close_port(int fd);

/* Other Prototypes */

int 
main(int *argc, char* argv[])
{
	/* TODO
	 * currently, this code attempts to open the /dev/ttyUSB0 device; funcs.c 
	 * must be manually changed to use a different device. it would be useful to
	 * specify the port &/or transmission Baud rate as command line args.
	 */
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
	char* buf;
	int bytes_read;
	int bytes_written = -1;
	int offset = 0;
	while ( offset < FILESIZE )
	{
		bytes_read = pread(sendfile_fd, &buf, 1, offset);
		/* the while block below continues to send the same Byte until "write_port()"
		 * returns 1; -1 indicates an error (e.g. resource busy, no data written) 
		 * while 0 indicates no Bytes were written.
		 * TODO figure out why so many "resource busy" errors!
		 */
		while ( bytes_written != bytes_read )
		{
			bytes_written = write_port(port_fd, &buf, bytes_read);
		}
		bytes_written = 0;
		offset += bytes_read;
	}
	
	/* Done with stuff */
	fclose(sendfile_fp);
	close(sendfile_fd);
	int close_success = close_port(port_fd);
	printf("close_port() returned %d\n", close_success);

	return 0;
}

