#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/hugetlb.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
#include <linux/swap.h>
#include <linux/swapfile.h>
#include <linux/vmstat.h>
#include <linux/atomic.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

/*
*
* Modulos de Kernel para Obtener la Memoria RAM Disponible del Sistema

*/

static int meminfo_proc_show(struct seq_file *m, void *v)
{
        struct sysinfo i;
        si_meminfo(&i);
        // datos que se escriben en el Json
        seq_printf(m,"{\"MemoriaTotal\":%8lu,\n",i.totalram);
        seq_printf(m,"\"MemoriaLibre\":%8lu,\n",i.freeram);
        seq_printf(m,"\"PorcentajeConsumo\":%lu}",((i.totalram-i.freeram)*100)/i.totalram);
        return 0;
}

static int meminfo_proc_open(struct inode *inode, struct file *file)
{
        return single_open(file, meminfo_proc_show, NULL);
}

static const struct file_operations meminfo_proc_fops = {
        .owner          = THIS_MODULE,
        .open           = meminfo_proc_open,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};

static int __init proc_meminfo_init(void)
{
        printk(KERN_INFO "Hola mundo, somos el GRUPO #22 y este es el monitor de memoria.\r\n");
        proc_create("mem_grupo22", 0777, NULL, &meminfo_proc_fops);
        return 0;
}

static void __exit proc_meminfo_end(void){
        printk(KERN_INFO "Sayonara mundo, somos el grupo #22 y este fue el monitor de memoria.\r\n");
        remove_proc_entry("mem_grupo22", NULL);

}

module_init(proc_meminfo_init);
module_exit(proc_meminfo_end);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Grupo 22 , Sistemas Operativos 2 - 2do Semestre 2021");
MODULE_DESCRIPTION("Monitor de Memoria");
MODULE_VERSION("1.0");
