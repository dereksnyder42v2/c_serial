#include <stdio.h>
#include <stdlib.h>

int
main(void) {

	int buf[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	for (int i = 0; i < 10; i++)
	{
		printf("%d,", buf[i]);
	}
	printf("\n");

	int *sliced_buf;
	const int start = 5; 	//index of slice beginning
	const int end = 9;		//index of slice end
	//char *start_address = sliced_buf + start*sizeof(char);
	
	sliced_buf = (int *)calloc(end - start + 1, sizeof(int));
	volatile int *buf_ptr; // pointer 
	for (int i = 0; i <= (end - start); i++)
	{
		buf_ptr = sliced_buf + i*sizeof(int);
		*buf_ptr = buf[i + start];
		printf("buf_ptr=%p, value at=%d\n", buf_ptr, *buf_ptr);
		printf("index=%d, buf[index]=%d, ", i + start, buf[i + start]);
		//printf("*val=%c\n", *val);
		printf("\n");
	}
	printf("\n");

	free (sliced_buf);

	return 0;
}