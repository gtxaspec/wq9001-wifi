#include "oss.h"

void dump_bytes(char *note, uint8_t *data, int len)
{
	char hex2ch[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	int rest = len;
	uint8_t *buf = data;
	char tmp[100]; //16*3+1+16+1
	int byte_to_print, i, j;
	
	printk("%s : dump addr=%p len=%d\n", note, data, len);
	
	while (rest > 0)
	{
		byte_to_print = (rest>=16)?16:rest;
		
		for(i=0; i<16; i++)
		{
			if (i<byte_to_print)
			{
			    tmp[i*3] = hex2ch[buf[i] >> 4];
		        tmp[i*3+1] = hex2ch[buf[i] & 0xf];	
			}
            else
			{
				tmp[i*3] = ' ';
		        tmp[i*3+1] = ' ';
			}				
		    tmp[i*3+2] = (i==7)?'-':' ';
		}
		tmp[i*3] = '|';
		for(j=0; j<byte_to_print; j++)
		{
			if ((buf[j] >= ' ') && (buf[j] <= '~'))
			    tmp[i*3+1+j] = buf[j];
			else
				tmp[i*3+1+j] = '.';
		}
		tmp[i*3+1+j] = 0;
		printk("%s\n", tmp);
		
		buf += byte_to_print;
		rest -= byte_to_print;
	}
	
}

