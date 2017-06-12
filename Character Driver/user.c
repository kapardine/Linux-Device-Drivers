
#include "trng_dev.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>              /* open */
#include <unistd.h>             /* exit */
#include <sys/ioctl.h>          /* ioctl */



int ioctl_get_random_number(int file_desc)
{
   	
    int c;
    query_arg_t q;
	
     printf("Enter upper value\n");
    scanf("%d",&(q.upper));
    
    printf("Enter lower value\n");
    scanf("%d",&(q.lower));

    c = ioctl(file_desc,IOCTL_RANDOM,&q);

    printf("Random Number= %d \n\n",c);
    return 0;
}

int main()
{
    int file_desc, ret_val;
    char *msg = "Message passed by ioctl\n";

    file_desc = open(DEVICE_FILE_NAME, 0);
    if (file_desc < 0) {
        printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
        exit(-1);
    }

    ioctl_get_random_number(file_desc);
    close(file_desc);
    return 0;
}
