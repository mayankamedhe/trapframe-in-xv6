#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_signal(void)
{
  char* handler;
  argptr(0, &handler, 32);
  myproc()->handler = handler;
  myproc()->handler_check = 1;

  return 0;
}

int sys_signal_send(void)
{
  int pid;
  int sig_type;
  argint(0, &pid);
  argint(1, &sig_type);

  // cprintf("send_signal %d \n", sig_type);
  return sigsend(pid, sig_type);
}

int sys_signal_ret(void)
{
  *myproc()->tf = *myproc()->tf_old;

  return myproc()->tf->eax;
}

int sys_setprio(void)
{
  int n;
  argint(0, &n);
  myproc()->priority = n;
  return 0; 
}

int sys_getprio(void)
{
  return myproc()->priority;
}

int sys_fork2(void)
{
  int priority;
  argint(0, &priority);

  return fork2(priority);
}