#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/hugetlb.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
#include <linux/syscalls.h>
#include <linux/swap.h>
#include <linux/swapfile.h>
#include <linux/vmstat.h>
#include <linux/atomic.h>

//estructura pra obtener informacion
struct sysinfo info;

/**
* LEE LA INFORMACION DE LA MEMORIA Y ESCRIBE EN EL ARCHIVO DE KERNEL
**/
static int readMemori(struct seq_file *m, void *v){

    #define Convert(x) ((x) << (PAGE_SHIFT - 10))
	si_meminfo(&info); 
	seq_printf(m, "{\n");
	seq_printf(m, "\"Mem_Total\":%8lu,\n",Convert(info.totalram)/1024);
	seq_printf(m, "\"Mem_Libre\":%8lu,\n",(Convert(info.freeram))/1024);
	seq_printf(m, "\"Buffer\":%8lu,\n",(Convert(info.bufferram))/1024);
	seq_printf(m, "\"Compartida\":%8lu\n",(Convert(info.sharedram))/1024);
	seq_printf(m, "}\n");
	#undef K
	return 0;

}

/**
* ABRE EL ARCHIVO DE KERNEL
**/
static int mem_info_open(struct inode *inode, struct file *file){
	return single_open(file, readMemori, NULL);
}

static const struct file_operations mem_info_fops = {
	.owner = THIS_MODULE,
	.open = mem_info_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MODULO DE MEMORIA SO1");

/**
* INICIA EL MODULO Y ESCRIBE EN EL BUFFER
**/
static int __init initMemoryProcess(void)
{
	printk(KERN_INFO "Hola mundo, somo el grupo 22 y este es el monitor de memoria\n");
	proc_create("mem_grupo29", 0, NULL, &mem_info_fops);
	return 0;
}


/**
* FINALIZA EL MODULO Y ESCRIBE EN EL BUFFER
**/
static void __exit memoCleanup(void)
{
	remove_proc_entry("mem_grupo22", NULL);
	printk(KERN_INFO "Sayonara mundo, somo el grupo 22 y este fue el monitor de memoria\n");
}

//LLAMADA INICIALES
module_init(initMemoryProcess);
module_exit(memoCleanup);