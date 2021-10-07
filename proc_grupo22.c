#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sysinfo.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/swap.h>

/*

 Modulos de Kernel para Obtener estadisticas de los procesos del sistema

*/

    static int meminfo_proc_show(struct seq_file *m, void *v){
        int contador_general = 0;
        int contador_run = 0;
        int contador_sleep = 0;
        int contador_stop = 0;
        int contador_zombie = 0;

        struct task_struct *task;

        for_each_process(task){
			seq_printf(m,"{\"PROC_ID\":%d,\"PROC_NAME\":\"%s\",\"USER_ID\":%d,\"MEMORY\":%d,\"PARENT_ID\":%d,\"STATE\":",task->pid,task->comm,task->cred->uid, task->parent->pid,task->usage);

                switch(task->state){
                                case 0: seq_printf(m,"\"R\","); contador_run = contador_run + 1;
                                break;

                                case 1: seq_printf(m,"\"S\","); contador_sleep = contador_sleep + 1;
                                break;

                                case 2: seq_printf(m,"\"S\","); contador_sleep = contador_sleep + 1;
                                break;

                                case 4: seq_printf(m,"\"T\","); contador_stop = contador_stop + 1;
                                break;

                                case 8: seq_printf(m,"\"S\","); contador_sleep = contador_sleep + 1;
                                break;

                                case 32: seq_printf(m,"\"Z\","); contador_zombie = contador_zombie + 1;
                                break;

                                default: seq_printf(m,"\"S\","); contador_sleep = contador_sleep + 1;
                }

                                if(task->mm)
                                {
                                seq_printf(m,"\"MEMORY_USED\":%8lu},",task->mm->mmap->vm_end - task->mm->mmap->vm_start);
                                }
                                else
                                {
                                 seq_printf(m,"\"MEMORY_USED\":0},");
                                }
                seq_printf(m,"\n");
                contador_general = contador_general +1;

        }

        return 0;
    }

    static void __exit final(void)
    {
         printk(KERN_INFO "FIN ++++ SOMOS EL GRUPO #22 Y ESTE FUE EL MONITOR DE PROCESOS.\r\n");
         remove_proc_entry("proc_grupo22", NULL);
    }

    static int meminfo_proc_open(struct inode *inode, struct file *file)
    {
        return single_open(file, meminfo_proc_show, NULL);
    }

    static const struct file_operations meminfo_proc_fops = {
        .owner      = THIS_MODULE,
        .open       = meminfo_proc_open,
        .read       = seq_read,
        .llseek     = seq_lseek,
        .release    = single_release,
    };

    static int __init inicio(void) //Escribe archivo en /proc
    {
        printk(KERN_INFO "INICIO ++++ SOMOS EL GRUPO # 22 Y ESTE ES EL MONITOR DE PROCESOS.\r\n");
        proc_create("proc_grupo22", 0777, NULL, &meminfo_proc_fops);
        return 0;
    }


    module_init(inicio);
    module_exit(final);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Grupo 222 , Sistemas Operativos 1 - 2do Semestre 2021");
MODULE_DESCRIPTION("Monitor de Procesos");
MODULE_VERSION("1.0");
