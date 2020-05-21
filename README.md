# ps Command
C implementation of Linux command ps -ax

Aim---

Developing Linux system call for ps -ax command.
(This command displays the information about process. For more details visit man ps)
------------------------------------------------------------------------------------

Work Done---

Implemented ps -ax command in C (instead of system call).
---------------------------------------------------------

Failure Reasons---

Lack of proper information and books on kernel mode functions.
--------------------------------------------------------------

Procedure Followed---

1. Read about basic hello word system call on web and implemented as directed.
2. Tried finding source code for ps-ax command, so that it can be modified to work in kernel mode. Source code available but without proper documentation, so hard to understand.
3. Decided to write my own code, analysed output of ps-ax command, read the man page of ps, how ps is implemented, from where it takes input, what are outputs, significance of metrics and data in output, everything is available in man page of ps.
4. ps read proc virtual file system and takes all the information about each process from this file system. Read the man page of proc, which describes what are the entities in the file system, how they can be used to know about the process completely.
5. The whole program is written by just properly reading man pages of proc and ps mainly, without any sort of external help. All the variable names used are mentioned in man pages.
6. Then tried running same program in kernel mode by adding it to kernel source code.
7. Came to know that user mode function like printf, fopen etc, are not supported in kernel mode.
8. Tried replacing each of user mode function with kernel mode function doing same task, but failed in doing so. (Reason mentioned above)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

What more needs to be done---

Somehow modify the code to support in kernel mode by finding appropriate functions or may be some other way.
------------------------------------------------------------------------------------------------------------

Requirements for current code---

1. gcc compiler
2. Linux environment
3. C libraries used in code
-------------------------------------

Compilation---

gcc my_ps-ax.c
-------------------------

Running---

./a.out
-----------------------
