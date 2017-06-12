/*
 *  trng_dev.c - Create a true random number generator using entropy pool character device
 */

#include <linux/kernel.h>       
#include <linux/module.h>       
#include <linux/fs.h>
#include <asm/uaccess.h>        
#include <crypto/rng.h>
#include <crypto/internal/hash.h>

#include "trng_dev.h"


#define SUCCESS 0
#define DEVICE_NAME "trng_dev"
#define BUF_LEN 80

static int Device_Open = 0;
static char Message[BUF_LEN];
static char *Message_Ptr;


static int device_open(struct inode *inode, struct file *file)
{
#ifdef DEBUG
        printk(KERN_INFO "device_open(%p)\n", file);
#endif

    if (Device_Open)
        return -EBUSY;

    Device_Open++;
    Message_Ptr = Message;
    try_module_get(THIS_MODULE);
    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
#ifdef DEBUG
    printk(KERN_INFO "device_release(%p,%p)\n", inode, file);
#endif

    Device_Open--;

    module_put(THIS_MODULE);
    return SUCCESS;
}

static ssize_t device_read(struct file *file,   
                           char __user * buffer,       
                           size_t length,       
                           loff_t * offset)
{
    int bytes_read = 0;

#ifdef DEBUG
    printk(KERN_INFO "device_read(%p,%p,%d)\n", file, buffer, length);
#endif

    if (*Message_Ptr == 0)
        return 0;

    while (length && *Message_Ptr) {

     put_user(*(Message_Ptr++), buffer++);
     length--;
     bytes_read++;
}

#ifdef DEBUG
    printk(KERN_INFO "Read %d bytes, %d left\n", bytes_read, length);
#endif

    return bytes_read;
}

static ssize_t
device_write(struct file *file,
             const char __user * buffer, size_t length, loff_t * offset)
{
    int i;

#ifdef DEBUG
    printk(KERN_INFO "device_write(%p,%s,%d)", file, buffer, length);
#endif

    for (i = 0; i < length && i < BUF_LEN; i++)
        get_user(Message[i], buffer + i);

    Message_Ptr = Message;

    return i;
}

static int get_random_numbers(u8 *buf, unsigned int len)
{
	struct crypto_rng *rng = NULL;
	char drbg = "42_garble_warble_zoop_zoop_hitchhiker_sha_"; /* Hash DRBG with SHA-256*/
	int ret;

	if (!buf || !len) {
	        pr_debug("No output buffer provided\n");
	        return -EINVAL;
	}

	rng = crypto_alloc_rng(drbg, 0, 0);
	if (IS_ERR(rng)) {
        	pr_debug("could not allocate RNG handle for %s\n", drbg);
        	return -PTR_ERR(rng);
    	}

    	ret = crypto_rng_get_bytes(rng, buf, len);
    	if (ret < 0)
    	    pr_debug("generation of random numbers failed\n");
    	else if (ret == 0)
    	    pr_debug("RNG returned no data");
    	else
    	    pr_debug("RNG returned %d bytes of data\n", ret);

	out:
    	crypto_free_rng(rng);
    	return ret;
}

static long device_ioctl(struct file *file,             
                  unsigned int cmd,        
                  unsigned long arg)
{
    
	void __user *argp = (void __user *)arg;
	query_arg_t q;
	copy_from_user(&q,argp,sizeof(q));

  	unsigned int rand,reqd;
	
  	switch (cmd) {


    	case IOCTL_RANDOM:
			get_random_bytes(&rand, sizeof(rand));
			printk(KERN_INFO "random number: %d \n",rand);
			reqd = q.lower + (rand%(q.upper-q.lower));
			printk(KERN_INFO"reqd number: %d\n",reqd);
       			return (reqd);
        		break;
    	}
	return SUCCESS;
}

struct file_operations Fops = {
        .read = device_read,
        .write = device_write,
        .unlocked_ioctl = device_ioctl,
        .open = device_open,
        .release = device_release,      
};
int init_module()
{
    int ret_val;
    ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);

    if (ret_val < 0) {
        printk(KERN_ALERT "%s failed with %d\n",
               "Sorry, registering the character device ", ret_val);
        return ret_val;
    }

    return 0;
}

void cleanup_module()
{
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
}
