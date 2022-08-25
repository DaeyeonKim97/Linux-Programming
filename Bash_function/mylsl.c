#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_permission(struct stat statbuf)
{
	int mode = statbuf.st_mode;
	//printf("%d ", mode);
	
	if(S_ISLNK(mode))
		printf("I");
	else if(S_ISREG(mode))
		printf("-");
	else if(S_ISDIR(mode))
		printf("d");
	else if(S_ISCHR(mode))
		printf("c");
	else if(S_ISBLK(mode))
		printf("b");
	else if(S_ISFIFO(mode))
		printf("f");
	else if(S_ISSOCK(mode))
		printf("s");
	
	if(mode&S_IRUSR)
		printf("r");
	else
		printf("-");
	if(mode&S_IWUSR)
		printf("w");
	else
		printf("-");
	if(mode&S_IXUSR)
		printf("x");
	else
		printf("-");

	if(mode&S_IRGRP)
		printf("r");
	else
		printf("-");
	if(mode&S_IWGRP)
		printf("w");
	else
		printf("-");
	if(mode&S_IXGRP)
		printf("x");
	else
		printf("-");


	if(mode&S_IROTH)
		printf("r");
	else
		printf("-");
	if(mode&S_IWOTH)
		printf("w");
	else
		printf("-");
	if(mode&S_IXOTH)
		printf("x");
	else
		printf("-");


}

void print_hardlinks(struct stat statbuf)
{
	int num = statbuf.st_nlink;
	printf("%2d", num);
}

void print_owner(struct stat statbuf)
{
	struct passwd *pw = getpwuid(statbuf.st_uid);
	if(pw != 0)
	{
		printf("%-5s",pw->pw_name);
	}
}

void print_group(struct stat statbuf)
{
	struct group *gr = getgrgid(statbuf.st_gid);
	if(gr != 0)
	{
		printf("%-5s", gr->gr_name);
	}
}

void print_size(struct stat statbuf)
{
	int size = statbuf.st_size;
	printf("%6d", size);
}

void print_time(struct stat statbuf)
{
	char buff_time[32];
	strcpy(buff_time, ctime(&statbuf.st_mtime));
	buff_time[strlen(buff_time)-1] ='\0';
	printf("   %s   ",buff_time);
}

void print_name(char* name)
{
	printf("%s \n", name);
}

main()
{
	DIR	*dp;
	struct 	dirent 	*dep;
	struct 	stat	statbuf;	
	char	buff_time[32];	

	if ((dp = opendir("."))==NULL)
	{
		perror("opendir");
		exit(0);
	} // 현재 디렉토리를 열고 실패할 경우 stderr opendir 출력 후 종료

	while(dep = readdir(dp))
	{
		if(stat(dep->d_name, &statbuf) < 0)
		{
			perror("stat");
			exit(1);
		}
		
		print_permission(statbuf);
		print_hardlinks(statbuf);
		print_owner(statbuf);
		print_group(statbuf);
		print_size(statbuf);
		print_time(statbuf);
		print_name(dep->d_name);
	}

	closedir(dp);
	// 디렉토리 close
}
