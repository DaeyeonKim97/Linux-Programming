#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main (int argc, char *argv[])
{
	struct stat	statbuf;
	char		*mode;
	int			i;

	for (i=1; i<argc; i++) //입력으로 주어진 파일갯수만큼 반복
	{
		printf("%s", argv[i]);
		if(lstat(argv[i], &statbuf)<0) //파일정보를 받아오지 못했을경우 stderr lstat을 출력하고 jump
		{
			perror("lstat");
			continue;
		}
		if(S_ISREG(statbuf.st_mode)) //regular file일 경우 regular 출력
				mode = "regular";
		else if(S_ISDIR(statbuf.st_mode)) //directory file일 경우 directory 출력
				mode="directory";
		else if(S_ISCHR(statbuf.st_mode)) //character special file일 경우 character special 출력
				mode="character special";
		else if(S_ISBLK(statbuf.st_mode)) //block special file일 경우 block special 출력
				mode="block special";
		else if(S_ISFIFO(statbuf.st_mode)) //pipe or FIFO일 경우 FIFO 출력
				mode="FIFO";
		else if(S_ISLNK(statbuf.st_mode)) //symbolic link일 경우 symbolic link 출력
				mode="symbolic link";
		else if(S_ISSOCK(statbuf.st_mode)) //socket일 경우 socket 출력
				mode="socket";

		printf("%s\n",mode);
		printf("\tst_mode = %d\n", statbuf.st_mode); //file type & mode
		printf("\tst_ino = %d\n", statbuf.st_ino); //i-node number
		printf("\tst_dev = %d\n", statbuf.st_dev); //device number
		printf("\tst_rdev = %d\n", statbuf.st_rdev); //device number for special files
		printf("\tst_nlink = %d\n", statbuf.st_nlink); //number of links
		printf("\tst_uid = %d\n", statbuf.st_uid); //user ID of owner
		printf("\tst_gid = %d\n", statbuf.st_gid); //group ID of owner
		printf("\tst_size = %d\n", statbuf.st_size); //size in bytes, for regular files
		printf("\tst_atime = %d\n", statbuf.st_atime); //time of last access
		printf("\tst_mtime = %d\n", statbuf.st_mtime); //time of modification
		printf("\tst_ctime = %d\n", statbuf.st_ctime); //time of last file status change
		printf("\tst_blksize = %d\n", statbuf.st_blksize); //best I/O block size
		printf("\tst_blocks = %d\n", statbuf.st_blocks); // no. of 512-byte blocks allocated
	}
}
