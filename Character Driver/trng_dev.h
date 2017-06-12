
#ifndef TRNG_DEV_H
#define TRNG_DEV_H

#include <linux/ioctl.h>

#define MAJOR_NUM 100

#define IOCTL_RANDOM _IOWR(MAJOR_NUM, 2, int)

#define DEVICE_FILE_NAME "/dev/trng_dev"

typedef struct
{
	unsigned int upper,lower;
} query_arg_t;

int MurmurHash64A (void * key, int len, unsigned int seed )
{
	 int m = 0xc6a4a7935bd1e995;
	 int r = 47;

	int h = seed ^ (len * m);

	int * data = (int *)key;
	int * end = data + (len/8);

	while(data != end)
	{
		int k = *data++;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h ^= k;
		h *= m; 
	}

	unsigned char * data2 = (unsigned char*)data;

	switch(len & 7)
	{
	case 7: h ^= (data2[6]) << 48;
	case 6: h ^= (data2[5]) << 40;
	case 5: h ^= (data2[4]) << 32;
	case 4: h ^= (data2[3]) << 24;
	case 3: h ^= (data2[2]) << 16;
	case 2: h ^= (data2[1]) << 8;
	case 1: h ^= (data2[0]);
	        h *= m;
	};
 
	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
} 

#endif
