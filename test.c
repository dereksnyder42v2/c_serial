#include <stdio.h>
#include <sys/stat.h>

#define PICTURE	""

/* Prototypes */
int open_port(void);
int write_port(int fd, const void *buf, size_t count);
int close_port(int fd);

int 
main(void)
{
	int port_fd = open_port();
	/* Do stuff */

	/* file we want to send over port */
	FILE *picture_fp = fopen(PICTURE, "r");
	struct stat st;
	stat(PICTURE, &st);	/* stat() returns # bytes in file */
	int filesize = st.st_size;
	printf("filesize of picture is %d\n", filesize);

	int write_success = write_port(port_fd, "hello", 5);
	printf("write_port() returned %d\n", write_success);

	/* Done with stuff */
	fclose(picture_fp);
	int close_success = close_port(port_fd);
	printf("close_port() returned %d\n", close_success);
	return 0;
}
