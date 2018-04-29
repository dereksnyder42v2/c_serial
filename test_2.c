#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define FILENAME	"qty5.jpg"
//#define FILENAME	"haiku.txt"

/* func.c Prototypes */
int open_port(void);
int write_port(int fd, const void *buf, size_t count);
int close_port(int fd);

/* Other Prototypes */
//char* slice_buffer(char *input_buf, int start_pos, int end_pos);

/* TODO change main to write whatever file is passed from cmd line
 * TODO add retry when # bytes written < 100
 */
int 
main(void)
{
	int port_fd = open_port();
	/* Do stuff */

	/* pointer to file we want to send over port */
	FILE *sendfile_fp = fopen(FILENAME, "r");
	struct stat st;
	stat(FILENAME, &st);	/* stat() returns # bytes in file */
	const int FILESIZE = st.st_size;
	printf("Sending file of size %d B\n", FILESIZE);
	/* descriptor of file we want to send */
	int sendfile_fd = open(FILENAME, O_RDONLY);

	/* Now, to business.
	 * Write file described by "sendfile_fd", 100 Bytes at a time.
	 * On each pass, print # Bytes succesfully written/ returned from "read()" call
	 */	
	char* buf;
	int bytes_read;
	int bytes_written = -1;
	int offset = 0;
	while ( offset < FILESIZE )
	{
		bytes_read = pread(sendfile_fd, &buf, 1, offset);
		while ( bytes_written != bytes_read )
		{
			bytes_written = write_port(port_fd, &buf, bytes_read);
			bytes_written = (bytes_written < 0)? 0 : bytes_written ;
		}
		offset += bytes_read;
	}
	
	/* Done with stuff */
	fclose(sendfile_fp);
	close(sendfile_fd);
	int close_success = close_port(port_fd);
	printf("close_port() returned %d\n", close_success);
	return 0;
}


