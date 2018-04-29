#include <stdio.h>
#include <stdlib.h>

/* Params
 *		const char* input_buf
 * 			buffer we want to read from
 * 		const int 	start_pos
 *			index in input_buf to start reading at
 *		const int 	end_pos
 * 			index in input_buf to stop reading at
 *  Returns
 * 		pointer to first char of sliced array
 */
char*
slice_buffer(char* input_buf, const int start_pos, const int end_pos) {

	char *sliced_buf;
	volatile char *buf_ptr; // pointer 	

	sliced_buf = (char *)malloc(end_pos - start_pos + 1);

	if (sliced_buf == NULL) printf("woops\n");

	for (int i = 0; i <= (end_pos - start_pos); i++)
	{
		buf_ptr = sliced_buf + i*sizeof(char);
		*buf_ptr = (char)(input_buf + start_pos*sizeof(char) + i*sizeof(char));
		/*
		printf("buf_ptr = %p, value at = %d\n", buf_ptr, (int)*buf_ptr);
		printf("index = %d, buf[index] = %d\n", i + start_pos, (int)input_buf[i + start_pos]);
		*/
	}
	
	return sliced_buf;
}
/*
int
main(void)
{
	char* in_buf = {0, 1, 2, 3, 4};

	char* out_buf = slice(in_buf, 2, 4);
	//return 0;
	for (int i = 0; i < 3; i++)
	{
		printf("%d %d\n", i, (int)*(out_buf + i));
	}

	return 0;
}
*/