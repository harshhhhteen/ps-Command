#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define MAJOR(dev)	((dev)>>8)
#define MINOR(dev)	((dev) & 0xff)
#define MKDEV(ma,mi)	((ma)<<8 | (mi))

#define BUFF_SIZE 3000

int ifProcess(char *s)
{
	int i;
	for(i=0;s[i]!='\0';i++)
	{
		if(!isdigit(s[i]))
		   return 0;
	}
	return 1;
}
		   
int main()
{
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	DIR *proc;
	FILE *fp;
	char path[BUFF_SIZE], comm[BUFF_SIZE], state, temp_string[BUFF_SIZE], c, cmdline[BUFF_SIZE];
	int pid, session, tty_nr, tpgid, temp_int, major, minor, device, sec, min, hour, day, count, width;
	long unsigned int utime, stime, cutime, cstime, temp_luint, time;
	long int nice, num_threads, temp_lint;
	unsigned int temp_uint, VmLck;
	proc = opendir("/proc/");
	if(proc == NULL)
	{
		perror("Cannot open /proc directory\n");
		return 0;
	}
	struct dirent *process;
	puts("PID\tTTY\tSTAT\tTIME\tCOMMAND");
	errno = 0;
	while((process = readdir(proc)) != NULL && errno == 0)
	{
		if(ifProcess(process->d_name))
		{
			width = w.ws_col - 32;
			VmLck = 0;
			count = 0;
			strcpy(path,"/proc/");
			strcat(path,process->d_name);
			strcat(path,"/stat");
			fp = fopen(path,"r");
			if(fp == NULL)
				continue;
			fscanf(fp,"%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %lu %lu %ld %ld %ld",&pid,comm,&state,&temp_int,&temp_int,&session,&tty_nr,&tpgid,&temp_uint,&temp_luint,&temp_luint,&temp_luint,&temp_luint,&utime,&stime,&cutime,&cstime,&temp_lint,&nice,&num_threads);
			fclose(fp);
			strcpy(path,"/proc/");
			strcat(path,process->d_name);
			strcat(path,"/status");
			fp = fopen(path,"r");
			if(fp == NULL)
				continue;
			do
				fscanf(fp,"%s ",temp_string);
			while(strcmp(temp_string,"VmLck:")!=0 && !feof(fp));
			fscanf(fp," %u",&VmLck);
			fclose(fp);
			major = MAJOR(tty_nr);
			minor = MINOR(tty_nr);
			device = MKDEV(major,minor);
			if(device != tty_nr)
			{
				printf("tty error\n");
				return 0;
			}
			strcpy(path,"/proc/");
			strcat(path,process->d_name);
			strcat(path,"/cmdline");
			fopen(path,"r");
			if(fp == NULL)
				continue;
			printf("%d",pid);
			if(major == 0)
				printf("\t?");
			else if(major == 4)
				printf("\ttty%d",minor);
			else if(major == 136)
				printf("\tpts/%d",minor);
			else
			{
				puts("other major value");
				return 0;
			}
			printf("\t%c",state);
			if(nice < 0)
				printf("<");
			else if(nice > 0)
				printf("N");
			if(VmLck > 0)
				printf("L");
			if(session == pid)
				printf("s");
			if(num_threads > 1)
				printf("l");
			if(tpgid > 0)
				printf("+");
			time = utime + stime;
			time/=(unsigned long int)sysconf(_SC_CLK_TCK);
			min = time/60;
			sec = time % 60;
			hour = min/60;
			min = min % 60;
			day = hour / 24;
			hour = hour % 24;
			if(day > 0)
				printf("\t[%.2d-]%.2d:%.2d:%.2d",day,hour,min,sec);
			else if(hour > 0)
				printf("\t%.2d:%.2d:%.2d",hour,min,sec);
			else
				printf("\t%.2d:%.2d",min,sec);
			printf("\t");
			while((c = fgetc(fp)) != EOF && width)
			{
				count++;
				printf("%c",c);
				width--;
			}
			if(count == 0)
				printf("[%s\b]",comm+1);
			fclose(fp);
			puts("");
		}
		errno = 0;
	}
	if(closedir(proc)==-1)
		puts("/proc directory closing error");
	return 0;
}