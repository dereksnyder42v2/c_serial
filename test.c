#include <stdio.h>
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
char* slice_buffer(char *input_buf, int start_pos, int end_pos);

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

	/*
	printf("Writing test string 'hello' to port\n");
	int write_success = write_port(port_fd, "hello", 5);
	printf("write_port() returned %d\n", write_success);
	*/
	
	/* Now, to business.
	 * Write file described by "sendfile_fd", 100 Bytes at a time.
	 * On each pass, print # Bytes succesfully written/ returned from "read()" call
	 */	
	char* buf[100];
	//char* buf_err[100];
	int bytes_read = 0;
	int bytes_written = 100;
	int offset = 0;
	while ( offset < FILESIZE )
	{
		bytes_read = pread(sendfile_fd, &buf, 100, offset);
		bytes_written = write_port(port_fd, &buf, bytes_read);
		while ( bytes_written != bytes_read )
		{
			printf("\tSomething went wrong...read %d, wrote %d\n", bytes_read, bytes_written);
			if ( bytes_written == -1 ) /* error occurred */
			{
				bytes_written = write_port(port_fd, &buf, bytes_read);
			}
			else
			{

				//bytes_written += write_port(port_fd, 
			}
		}
		offset += bytes_read;
		printf("\tread %d B, wrote %d B\n", bytes_read, bytes_written);
	}
	
// 	for (int head = 0; head < FILESIZE; head += num_bytes_read)
// 	{
// 		//if ( head % 4800 == 0 ) sleep(1);
// 		
// 		num_bytes_read = read(sendfile_fd, &buf, 100); /* read up to 100 B */
// 		num_bytes_wrote = write_port(port_fd, &buf, num_bytes_read);
// 		if ( num_bytes_wrote < 0 )
// 		{
// 			printf("write_port failed in neighborhood of head+num_bytes_read=%d\n",
// 					(head + num_bytes_read));
// 			//break;
// 		}
// 		printf("\tread %d B, wrote %d B\n", num_bytes_read, num_bytes_wrote);	
// 	}

		

	/* Done with stuff */
	fclose(sendfile_fp);
	close(sendfile_fd);
	int close_success = close_port(port_fd);
	printf("close_port() returned %d\n", close_success);
	return 0;
}

void* 
whats_left(	const void *input_buf, 
		int offset)
{
	for (int i = offset; i < sizeof(input_buf); i++)
	{
		
	}	
}
