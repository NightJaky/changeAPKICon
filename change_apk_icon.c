#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define SHELL_COMMAND_LENGTH		1024
static char* unzip_temp_path_name	"unziptemp";

enum CUSTOM_FILE_TYPE
{
	TYPE_FILE = 1,
	TYPE_DIR
};




int check_arg(char* argc, enum CUSTOM_FILE_TYPE type)
{
	struct stat stat_file;
	int iret = -1;

	if (-1 == (iret = stat(argc, &stat_file)))
	{
		fprintf(stderr, "get file stat failed, filepath=%s, errno=%d, strerror(errno)=%s\n", \
				argc, errno, strerror(errno));
		return iret;
	}

	if (type == TYPE_FILE)
	{
		if (!(stat_file.st_mode & S_IFREG))
		{
			fprintf(stderr, "file type error, really type is:%x\n", stat_file.st_mode);
			return -1;
		}
	}
	else
	{
		if (!(stat_file.st_mode & S_IFDIR))
		{
			fprintf(stderr, "file type not dir, really type is:%x\n", stat_file.st_mode);
			return -1;
		}
	}
	
	return 0;
}

int unzip_apk(char* apk_path_name)
{
	
}

int main(int argc, char** argv)
{
	int iret = -1;
	FILE* fp = NULL;
	char buffer[20] = {0};
	char buff_shell[SHELL_COMMAND_LENGTH] = {0};

	if (argc != 3)
	{
		fprintf(stderr, "argc must be 3.\n");
		return -1;
	}

	if (!check_arg(argv[1], TYPE_FILE))
	{
		if (!check_arg(argv[2], TYPE_DIR))
		{
			printf("start to unzip file\n");

			

			fp = popen("ls -l", "r");
			if (!fp)
			{
				fprintf(stderr, "popen failed, errno=%d, strerror(errno)=%s\n", errno, strerror(errno));
				return -1;
			}
			
			while (!feof(fp))
			{
				memset(buffer, 0, 20);
				iret = fread(buffer, 1, 20, fp);
				printf("%s", buffer);
				if (iret < 20 && errno != 0)
				{
					fprintf(stderr, "iret = %d, errno=%d, strerror(errno)=%s\n", iret, errno, strerror(errno));
				}
			}
			printf("\n");

		}
	}



	return 0;
}
