#pragma once

#include "types.h"
#include "riscv.h"

struct buf;
struct context;
struct file;
struct inode;
struct mbuf;
struct pipe;
struct proc;
struct socket;
struct spinlock;
struct sleeplock;
struct stat;
struct superblock;

// bio.c
void            binit(void);
struct buf*     bread(uint, uint);
void            brelse(struct buf*);
void            bwrite(struct buf*);
void            bpin(struct buf*);
void            bunpin(struct buf*);

// console.c
void            consoleinit(void);
void            consoleintr(int);
void            consputc(int);

// e1000.c
void            e1000_init(uint32*);
int             e1000_send(struct mbuf*);

// exec.c
int             exec(char*, char**);

// file.c
struct file*    filealloc(void);
void            fileclose(struct file*);
struct file*    filedup(struct file*);
void            fileinit(void);
int             fileread(struct file*, uint32, int n);
int             filestat(struct file*, uint32 addr);
int             filewrite(struct file*, uint32, int n);

// fs.c
void            fsinit(int);
int             dirlink(struct inode*, char*, uint);
struct inode*   dirlookup(struct inode*, char*, uint*);
struct inode*   ialloc(uint, short);
struct inode*   idup(struct inode*);
void            iinit();
void            ilock(struct inode*);
void            iput(struct inode*);
void            iunlock(struct inode*);
void            iunlockput(struct inode*);
void            iupdate(struct inode*);
int             namecmp(const char*, const char*);
struct inode*   namei(char*);
struct inode*   nameiparent(char*, char*);
int             readi(struct inode*, int, uint32, uint, uint);
void            stati(struct inode*, struct stat*);
int             writei(struct inode*, int, uint32, uint, uint);
void            itrunc(struct inode*);

// ramdisk.c
void            ramdiskinit(void);
void            ramdiskintr(void);
void            ramdiskrw(struct buf*);

// kalloc.c
void*           kalloc(void);
void            kfree(void *);
void            kinit(void);

// log.c
void            initlog(int, struct superblock*);
void            log_write(struct buf*);
void            begin_op(void);
void            end_op(void);

// pci.c
void            pci_init();

// pipe.c
int             pipealloc(struct file**, struct file**);
void            pipeclose(struct pipe*, int);
int             piperead(struct pipe*, uint32, int);
int             pipewrite(struct pipe*, uint32, int);

// printf.c
void            printf(char*, ...);
void            panic(char*) __attribute__((noreturn));
void            printfinit(void);

// proc.c
int             cpuid(void);
void            exit(int);
int             fork(void);
int             growproc(int);
void            proc_mapstacks(pagetable_t);
pagetable_t     proc_pagetable(struct proc *);
void            proc_freepagetable(pagetable_t, uint32);
int             kill(int);
struct cpu*     mycpu(void);
struct cpu*     getmycpu(void);
struct proc*    myproc();
void            procinit(void);
void            scheduler(void) __attribute__((noreturn));
void            sched(void);
void            setproc(struct proc*);
void            sleep(void*, struct spinlock*);
void            userinit(void);
int             wait(uint32);
void            wakeup(void*);
void            yield(void);
int             either_copyout(int user_dst, uint32 dst, void *src, uint32 len);
int             either_copyin(void *dst, int user_src, uint32 src, uint32 len);
void            procdump(void);

// swtch.S
void            swtch(struct context*, struct context*);

// spinlock.c
void            acquire(struct spinlock*);
int             holding(struct spinlock*);
void            initlock(struct spinlock*, char*);
void            release(struct spinlock*);
void            push_off(void);
void            pop_off(void);

// sleeplock.c
void            acquiresleep(struct sleeplock*);
void            releasesleep(struct sleeplock*);
int             holdingsleep(struct sleeplock*);
void            initsleeplock(struct sleeplock*, char*);

// string.c
int             memcmp(const void*, const void*, uint);
void*           memmove(void*, const void*, uint);
void*           memset(void*, int, uint);
char*           safestrcpy(char*, const char*, int);
int             strlen(const char*);
int             strncmp(const char*, const char*, uint);
char*           strncpy(char*, const char*, int);

// syscall.c
int             argint(int, int*);
int             argstr(int, char*, int);
int             argaddr(int, uint32 *);
int             fetchstr(uint32, char*, int);
int             fetchaddr(uint32, uint32*);
void            syscall();

// socket.c
void            sock_recv_udp(struct mbuf*, uint32, uint16, uint16);

// trap.c
extern uint     ticks;
void            trapinit(void);
void            trapinithart(void);
extern struct spinlock tickslock;
void            usertrapret(void);

// uart.c
void            uartinit(void);
void            uartintr(void);
void            uartputc(int);
void            uartputc_sync(int);
int             uartgetc(void);

// vm.c
void            kvminit(void);
void            kvminithart(void);
void            kvmmap(pagetable_t, uint32, uint32, uint32, int);
int             mappages(pagetable_t, uint32, uint32, uint32, int);
pagetable_t     uvmcreate(void);
void            uvminit(pagetable_t, uchar *, uint);
uint32          uvmalloc(pagetable_t, uint32, uint32);
uint32          uvmdealloc(pagetable_t, uint32, uint32);
int             uvmcopy(pagetable_t, pagetable_t, uint32);
void            uvmfree(pagetable_t, uint32);
void            uvmunmap(pagetable_t, uint32, uint32, int);
void            uvmclear(pagetable_t, uint32);
uint32          walkaddr(pagetable_t, uint32);
int             copyout(pagetable_t, uint32, char *, uint32);
int             copyin(pagetable_t, char *, uint32, uint32);
int             copyinstr(pagetable_t, char *, uint32, uint32);

// plic.c
void            plicinit(void);
void            plicinithart(void);
int             plic_claim(void);
void            plic_complete(int);

// net.c
void            net_rx(struct mbuf*);

// virtio_disk.c
void            virtio_disk_init(void);
void            virtio_disk_rw(struct buf *, int);
void            virtio_disk_intr(void);

// number of elements in fixed-size array
#define NELEM(x) (sizeof(x)/sizeof((x)[0]))
