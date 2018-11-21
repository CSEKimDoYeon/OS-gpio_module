#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include "gpio_device_driver.h"

static int gpio = 4;
module_param(gpio, int, 0);

static int gpio2 = 3; // new gpio
module_param(gpio2, int, 0); // new parm

static int gpio_open(struct inode *inode, struct file *filp){
	if(gpio_request(gpio, "led1") == 0)
		printk(KERN_INFO "GPIO 4 OPEN COMPLETE\n");
	else if(gpio_request(gpio2, "led2") == 0)
		printk(KERN_INFO "GPIO 3 OPEN COMPLETE\n"); // new

	gpio_direction_output(gpio, 1);
	gpio_direction_output(gpio2,1); // new 
	return 0;
}

static int gpio_release(struct inode *inode, struct file *filp){
	return 0;
}

static ssize_t gpio_read(struct file *filp, char *buf, size_t count, loff_t *f_pos){
	char tmp_buf;
	copy_to_user(buf, &tmp_buf, count);
	return count;
}

static ssize_t gpio_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos){ // have to change
	char tmp_buf;
	copy_from_user(&tmp_buf, buf, count);
	if(tmp_buf){
		gpio_set_value(gpio,1);
		gpio_set_value(gpio2,0); // new
	}
	else{
		gpio_set_value(gpio,0);
		gpio_set_value(gpio2,1); // new
	}
	return 0;
}

static struct file_operations gpio_fops = {
	.owner		= THIS_MODULE,
	.read		= gpio_read,
	.open		= gpio_open,
	.release	= gpio_release,
	.write		= gpio_write,
};

static int gpio_init(void){
	printk(KERN_INFO "GPIO_INIT() CALLED/n");
	if(!register_chrdev(DEV_GPIO_MAJOR_NUMBER, DEV_GPIO_NAME, &gpio_fops)){
		printk("REGISTER DEVICE %s COMPLETE\n",DEV_GPIO_NAME);
	}
	else{
		printk("ERROR TO REGISTER DEVICE, CANNOT GET MAJOR %d FOR %s", DEV_GPIO_MAJOR_NUMBER, DEV_GPIO_NAME);
		return -EBUSY;
	}
	return 0;
}

static void gpio_exit(void){
	printk(KERN_INFO "BYE\n");
	unregister_chrdev(DEV_GPIO_MAJOR_NUMBER, DEV_GPIO_NAME);
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("Dual BSD/GPL");

