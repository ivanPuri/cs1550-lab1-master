#ifndef COUNTHELPER_H
#define COUNTHELPER_H


void listinit(struct proc *p);

int listinsert(struct proc *p, int syscall_num);

void addEntry(struct proc *p, int sysnum);

void freelist(struct proc *p);

int getEntry(struct proc *p, int sysnum);

#endif
