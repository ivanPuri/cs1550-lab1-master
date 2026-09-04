# CS 1550 Lab 1

## PART 0: Windows Users
NOTE: Make sure you clone the git repo within the Linux Filesystem. If the repo is not in the Linux FS, your file may get corrupted and file compilation will take a long time.  

If you are using WSL1, switch to WSL2: https://docs.microsoft.com/en-us/windows/wsl/install

You will also need to install Ubuntu: 
1. Install Ubuntu App from Windows Store
2. Run Ubuntu App
3. Using the command line, change directory to the folder where you want to clone the repo 
``` 
> mkdir /home/<user>/cs1550/
> cd /home/<user>/cs1550/
```
4. Run git clone yourrepo.git (see instructions below)

Note: You can access the Linux FS on Windows as follows
1. Open your Ubuntu App and run `explorer.exe .`  (This should launch the FileExplorer with the current location). 
2. You can navigate to the path `\\wsl$`

## PART 1: Setting Up the XV6 Development Environment

We will use Docker container to setup the xv6 environment. Download docker [here](https://docs.docker.com/get-docker/)

1. Download cs1550-lab1-master.zip on Canvas into your local machine.
2. Unzip and change directory into the folder using `cd cs1550-lab1-master` and `cd xv6` 
4. Run the command to download the [docker xv6 image](https://hub.docker.com/repository/docker/cs1550/xv6) `docker pull cs1550/xv6:latest`
5. To run the docker image container, `docker run --rm -it -v {absolutepath_to_xv6/cs1550-lab1-master/xv6}:/home/ubuntu/xv6 cs1550/xv6`, specify the absolute path without the braces.  
6. Once inside the container, type `cd xv6; make qemu-nox` to compile and run Xv6. To exit, press `CTRL+a` _then_ `x`.
8. **It is important for all labs and project of this class that you commit frequently into your Github Classroom repository.** 



## PART 2: Adding A System Call to Xv6

You'll then add a new system call called `getcount` to Xv6, which, when passed a valid system call number (listed in the file `syscall.h`) as an argument, will return the number of times the referenced system call was invoked by the calling process.
For instance, consider the following test program (`getcount.c`) that should produce the following output (note that each character is output with a separate call to `write/printf` in Xv6):
```c
#include "types.h"
#include "user.h"
#include "syscall.h"
int  main(int argc, char *argv[])
{
    printf(1, "initial fork count %d\n", getcount(SYS_fork));
    if (fork() == 0) {
        printf(1, "child fork count %d\n", getcount(SYS_fork));
        printf(1, "child write count %d\n", getcount(SYS_write));
    } else {
        wait();
        printf(1, "parent fork count %d\n", getcount(SYS_fork));
        printf(1, "parent write count %d\n", getcount(SYS_write));
    }
    printf(1, "wait count %d\n", getcount(SYS_wait));
    exit();
}
```

Output:
```
initial fork count 0
child fork count 0
child write count 19
wait count 0
parent fork count 1
parent write count 41
wait count 1
```

## Hints
You will need to modify several files for this exercise, though the total number of lines of code you'll be adding is quite small. At a minimum, you'll need to alter `syscall.h`, `syscall.c`, `user.h`, and `usys.S` to implement your new system call.  It may be helpful to trace how some other system call is implemented (e.g., `uptime`) for clues. 
You will likely also need to update `struct proc`, located in `proc.h`, to add a syscall-count tracking data structure for each process. To re-initialize your data structure when a process terminates, you may want to look into the `exit` or `fork` functions in `proc.c`.
Chapter 3 of the Xv6 book (available in pdf in Canvas) contains details on traps and system calls (though most of the low-level details won't be necessary for you to complete this exercise).
Testing

To test your implementation, you'll run the `getcount` executable (after booting into Xv6), which is based on the program above. Because the program depends on your implementation, it isn't compiled into Xv6 by default. When you're ready to test, you should create the `getcount.c` file and add `getcount` to `UPROGS` declaration in the `Makefile`.
Note that while the getcount example only prints out counts for three different system calls, your implementation should support all the system calls listed in `syscall.h`. It is a good exercise to add tests to the test program, located in `getcount.c`, for other system calls.

## Debugging 
Here is a way to run gdb on Xv6:
1.	In the first terminal, run cs1550/xv6 and start Xv6 with the following command: `make qemu-nox-gdb`
2.	In the second terminal, use the command to print the list of running container, `docker container ls` 
```
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
2aa06ec5b33c        cs1550/xv6          "/bin/bash"         24 seconds ago      Up 23 seconds                           silly_gagarin
```
3.	Open another terminal and login to the cs1550/xv6 container using the following command. `docker exec -it <container_id> /bin/bash`
```
docker exec -it 2aa06ec5b33c /bin/bash
```
4.	Run `cd xv6` and then run gdb on the second terminal using the command:
				`gdb -iex "set auto-load safe-path ."`
4.	Feel free to use your GDB skills. For example, you can place a breakpoint at a given line in the Xv6 source code as follows:
				`(gdb) b proc.c:215`
5.	Then continue running the kernel using the command `(gdb) c`. This will start the debugging session.
6.	To debug a user program (e.g., getcount), you can add the symbol table inside the user program to the GDB session as follows (note the underscore in front of the program name)
`(gdb) add-symbol-file _getcount 0`
7.	You would then be able to add breakpoints in the user program: 
`(gdb) b getcount.c:7`
8.	Using the next command in the GDB prompt (`n`) you can trace inside kernel code and out to the user code.
9.	Here is a nice page about [gdb commands](https://visualgdb.com/gdbreference/commands/).

## Submission Instructions
Submit your Github repository to GradeScope. You should modify the following files only:
- `syscall.h`
- `syscall.c`
- `user.h`
- `usys.S`
- `proc.h`
- `proc.c`
- `sysproc.c`
- `Makefile`

Your submission will be auto-graded by compiling and running the test program more than once. 

