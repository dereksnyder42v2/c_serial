#include <stdio.h>

/* Prototypes */
int open_port(void);
int write_port(int fd, const void *buf, size_t count);
int close_port(int fd);

int 
main(void)
{
	int port_fd = open_port();
	/* Do stuff */
	
	int write_success = write_port(port_fd, "hello", 5);
	printf("write_port() returned %d\n", write_success);

	/* Done with stuff */
	int close_success = close_port(port_fd);

	return 0;
}
