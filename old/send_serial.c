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
	char* buf[100];
	volatile char* err_buf_base;
	volatile char* err_buf_ptr;
	//char* buf_err[100];
	int bytes_read = 0;
	int bytes_written = 100;
	int tmp_bytes_written;
	int offset = 0;
	while ( offset < FILESIZE )
	{
		bytes_read = pread(sendfile_fd, &buf, 100, offset);
		bytes_written = write_port(port_fd, &buf, bytes_read);
		while ( bytes_written != bytes_read )
		{
			if ( bytes_written == -1 )
			{
				bytes_written = 0;
			}
			else
			{
				/* ohshit 
				 * allocate new array, and try to write that. however much is written, add 
				 * that to bytes_written */
				err_buf_base = (char*)malloc(bytes_read - bytes_written);
				err_buf_ptr = err_buf_base;
				for (int i = 0; i < (bytes_read - bytes_written); i++)
				{
					*err_buf_ptr = (char)*(&buf + i + bytes_written);					
					err_buf_ptr += sizeof(char);
				}
				tmp_bytes_written = write_port(port_fd, &err_buf_base, bytes_read - bytes_written);
				tmp_bytes_written = (tmp_bytes_written >= 0)? tmp_bytes_written : 0 ;
				bytes_written += tmp_bytes_written;

				//free(err_buf_base);
				//free(err_buf_ptr);
			}			
		}
		offset += bytes_read;
		printf("\tread %d B, wrote %d B\n", bytes_read, bytes_written);
	}
	
	/* Done with stuff */
	fclose(sendfile_fp);
	close(sendfile_fd);
	int close_success = close_port(port_fd);
	printf("close_port() returned %d\n", close_success);
	return 0;
}


