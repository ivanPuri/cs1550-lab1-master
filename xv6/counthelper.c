#include "types.h"
#include "param.h"
#include "defs.h"
#include "x86.h"
#include "mmu.h"
#include "proc.h"

// Initialize a process's linked list.
void listinit(struct proc* p){
  p->counter_head = 0;
}

// Inserts a new node at the front of a process's list.
int listinsert(struct proc* p, int syscall_num){
  struct syscallCounter* node;

  node = (syscallCounter*) kalloc();
  if(node == 0) return -1; //failed kalloc

  node->next = 0;
  node->syscall_number = syscall_num;
  node->count = 1;

  if (p->counter_head != 0){
    node->next = p->counter_head;
  }
  p->counter_head = node;

  return 0;
}

// Main function called my other syscalls in proc.c
void addEntry(struct proc* p, int sysnum){
    struct syscallCounter* curr;

    curr = p->counter_head;

    while(curr != 0){
      if (curr->syscall_number == sysnum){
          curr->count++;
          return;
      }
    curr = curr->next;
    }

    listinsert(p, sysnum);

    return;
}

// getting a count value
int getEntry(struct proc* p, int sysnum){
    struct syscallCounter* curr;

    curr = p->counter_head;

    while(curr != 0){
      if (curr->syscall_number == sysnum){
          return curr->count;
      }
    curr = curr->next;
    }

    return 0;
}

