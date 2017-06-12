#include <linux/module.h>
#include <linux/init.h>

#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>
#include <linux/types.h>
#include <linux/string.h>

#include "partition.h"

static int major_num = 0;
static int logical_block_size = 512;
static int no_sectors = 1024; /* How big the drive is */

#define KERNEL_SECTOR_SIZE 512

static struct ram_device {
	unsigned long size;
	spinlock_t lock;
	u8 *data;
	struct gendisk *gd;
} ram;

static struct request_queue *Queue;

static void ram_transfer(struct ram_device *dev, sector_t sector,
		unsigned long nsect, char *buffer, int write)
{
	unsigned long offset = sector * logical_block_size;
	unsigned long nbytes = nsect * logical_block_size;

	if ((offset + nbytes) > dev->size) {
		printk (KERN_NOTICE "DiskonRAM: Beyond-end write (%ld %ld)\n", offset, nbytes);
		return;
	}
	if (write)
		memcpy(dev->data + offset, buffer, nbytes);
	else
		memcpy(buffer, dev->data + offset, nbytes);
}

static int ram_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	geo->heads = 1;
	geo->cylinders = 32;
	geo->sectors = 32;
	geo->start = 0;
	return 0;
}

static void ram_request(struct request_queue *q) {
	struct request *req;

	req = blk_fetch_request(q);
	while (req != NULL) {
		// blk_fs_request() was removed in 2.6.36 - many thanks to
		// Christian Paro for the heads up and fix...
		//if (!blk_fs_request(req)) {
		if (req == NULL || (req->cmd_type != REQ_TYPE_FS)) {
			printk (KERN_NOTICE "Skip non-CMD request\n");
			__blk_end_request_all(req, -EIO);
			continue;
		}
		ram_transfer(&ram, blk_rq_pos(req), blk_rq_cur_sectors(req),
				bio_data(req->bio) , rq_data_dir(req));
		if ( ! __blk_end_request_cur(req, 0) ) {
			req = blk_fetch_request(q);
		}
	}
}
// 
static struct block_device_operations ram_ops = {
		.owner  = THIS_MODULE,
//		.open = ram_open,
//		.release = ram_close,
		.getgeo = ram_getgeo,
};

static int __init ram_init(void)
{
	ram.size = no_sectors * logical_block_size;
	spin_lock_init(&ram.lock);
	ram.data = vmalloc(ram.size);
	if (ram.data == NULL)
		return -ENOMEM;

	else
		printk(KERN_NOTICE "Kuch assign toh hua hai\n");
	copy_mbr_n_br(ram.data);
	/*Register the device*/

	major_num = register_blkdev(major_num, "DiskonRAM");
	if (major_num < 0) 
		printk(KERN_WARNING "DiskonRAM: unable to get major number\n");
		//goto out;

	/*Get a request queue*/
	
	Queue = blk_init_queue(ram_request, &ram.lock);
	/*if (Queue == NULL)
		goto out;*/
	blk_queue_logical_block_size(Queue, logical_block_size);
	
	/*Add the gendisk structure*/
	/*
	 * And the gendisk structure.
	 */
	ram.gd = alloc_disk(8);
	/*if (!ram.gd)
		goto out_unregister;*/
	ram.gd->major = major_num;
	ram.gd->first_minor = 0;
	ram.gd->fops = &ram_ops;
	ram.gd->private_data = &ram;
	strcpy(ram.gd->disk_name, "DiskonRAM");
	set_capacity(ram.gd, no_sectors);
	ram.gd->queue = Queue;
	add_disk(ram.gd);

	return 0;
}

static void __exit ram_exit(void)
{
	vfree(ram.data);
	unregister_blkdev(major_num, "DiskonRAM");
	del_gendisk(ram.gd);
	put_disk(ram.gd);
	unregister_blkdev(major_num, "DiskonRAM");
	blk_cleanup_queue(Queue);
	printk (KERN_NOTICE "Leaving Kernel\n");
	
}

module_init(ram_init);
module_exit(ram_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shivi Mishra");
MODULE_DESCRIPTION("Ram Block Driver");
