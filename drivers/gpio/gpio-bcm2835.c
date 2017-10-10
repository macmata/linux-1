#include <linux/init.h>
#include <linux/gpio.h>
#define ERROR 0

static int Major;
static const int RANGE = 0x40;
static const int PORT = 0x20200000;

static ssize_t device_read(struct file *filp,  char *buffer,  size_t length, loff_t * offset)
{
	uint8_t *addr = ioremap(PORT, RANGE);
	unsigned int memread = ioread32(addr);
	if (sizeof(memread) >= length){
		put_user(memread, buffer);
		return length;
	} else {
		return ERROR;
	}
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	printk(KERN_ALERT "Sorry not available\n");
	return -EINVAL;
}

static int device_open(struct inode *inode, struct file *file)
{
	if(try_module_get(THIS_MODULE)) {
	} else {
		return 0;
	}
}

static int device_release(struct inode *inode, struct file *file)
{
	unregister_chrdev(Major, DEVICE_NAME);
	module_put(THIS_MODULE);
	release_mem_region(PORT, RANGE);
}

void init_module(void)
{
	Major = register_chrdev(0, DEVICE_NAME &fops);
	if (Major < 0) {
		printk(KERNEL_ALERT "Register char devide error %d\n", Major);
	}
	if (check_mem_region(PORT, RANGE)) {
		request_mem_region(PORT,RANGE, DEVICE_NAME);
	}
}

static const struct file_operations bcm2832_gpio_operations = {
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release  = device_release,
}

MODULE_AUTHOR("Alexandre Leblanc");
MODULE_DESCRIPTION("Driver for Broadcom BRC2835 GPIO");
MODULE_LICENSE("GPL2");
