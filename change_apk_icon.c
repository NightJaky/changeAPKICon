#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define SHELL_COMMAND_LENGTH		1024
static char* unzip_temp_path_name =	"unziptemp";

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

int check_file_is_exists(char* file_path)
{
	if (!strncmp(file_path, "/", 1) || !strncmp(file_path, "~", 1)) {
		if (!access(file_path, R_OK)) {
			return 0;
		}
	}

	return -1;
}

int strrfind(char* source, char* match)
{
	int i;
	int lm = strlen(match);
	int ls = strlen(source);
	source = source + strlen(source) - 1;
	printf("[strrfind]\n");
	for (i = 0; i < ls; ++i) {
		printf("source context:%s\n", source);
		if (!strncmp(source, match, lm)) {
			return (ls - i);
		}
		--source;
	}

	return -1;
}


int change_currnet_work_dir(char* def_work_dir, char* current_exe_path)
{
	char* full_file_path = NULL;
	char* work_dir = NULL;
	int pos = -1;
	printf("[chage_current_work_dir]\n");
	//test current_exe_path is absolute path
	if (!strncmp(current_exe_path, "/", 1) || !strncmp(current_exe_path, "~", 1)) {
		//absolute path
		pos = strrfind(current_exe_path, "/");
		printf("[change_currnet_work_dir]pos=%d, line=%d\n", pos, __LINE__);
		work_dir = (char*)malloc(strlen(current_exe_path));
		strncpy(work_dir, current_exe_path, pos);
		printf("[change_currnet_work_dir]work_dir=%s, line=%d\n", work_dir, __LINE__);
	}
	else {
		full_file_path = (char*)malloc(strlen(def_work_dir) + strlen(current_exe_path) + 10);
		if (!full_file_path) {
			fprintf(stderr, "[change_currnet_work_dir]");
			return -1;
		}
		memset(full_file_path, 0, strlen(def_work_dir) + strlen(current_exe_path) + 10);
		strcpy(full_file_path, def_work_dir);
		strcat(full_file_path, "/");
		strcat(full_file_path, current_exe_path);
		printf("[change_currnet_work_dir]full_file_path=%s, line=%d\n", full_file_path, __LINE__);
		pos = strrfind(full_file_path, "/");
		printf("[change_currnet_work_dir]full_file_path=%s, line=%d\n", full_file_path, __LINE__);
		work_dir = (char*)malloc(strlen(full_file_path));
		strncpy(work_dir, full_file_path, pos);
		printf("[change_currnet_work_dir]work_dir=%s, line=%d\n", work_dir, __LINE__);
	}

	if (-1 == chdir(work_dir)) {
		fprintf(stderr, "[change_currnet_work_dir]chdir failed, errno=%d, strerror(errno)=%s, line=%d\n", errno, strerror(errno), __LINE__);
	}

	if (work_dir) {
		free(work_dir);
		work_dir = NULL;
	}

	if (full_file_path) {
		free(full_file_path);
		full_file_path = NULL;
	}


	return 0;
}


int main(int argc, char** argv)
{
	int iret = -1;
	FILE* fp = NULL;
	char buffer[20] = {0};
	char buff_shell[SHELL_COMMAND_LENGTH] = {0};
	char buff_current_work_dir[1024] = {0};
	char* pp;
	if (!(pp = getcwd(buff_current_work_dir, sizeof(buff_current_work_dir)))) {
		fprintf(stderr, "getcwd failed, errno=%d, strerror(errno)=%s\n", errno, strerror(errno));
	}

	printf("pp=%p, pp context=%s, buff_current_work_dir=%p, buff_current_work_dir context=%s\n", pp, pp, buff_current_work_dir, buff_current_work_dir);
	
	change_currnet_work_dir(buff_current_work_dir, argv[0]);


	if (argc != 3)
	{
		fprintf(stderr, "argc must be 3.\n");
		printf("usage\nargv1 is apk file path,argv2 is resource path\n");
		return -1;
	}

	if (check_file_is_exists(argv[1])) {
		fprintf(stderr, "dest file 1 \"%s\" not found or not a full path\n", argv[1]);
		return -1;
	}

	if (check_file_is_exists(argv[2])) {
		fprintf(stderr, "dest file 2 \"%s\" not found or not a full path\n", argv[2]);
		return -1;
	}


	if (!check_arg(argv[1], TYPE_FILE))
	{
		if (!check_arg(argv[2], TYPE_DIR))
		{
			printf("start to unzip file\n");			

			//fp = popen("ls -l", "r");
			strcpy(buff_shell, "./unzip -d unzip_temp_folder ");
			strcat(buff_shell, argv[1]);
			printf("buff_shell=%s\n", buff_shell);
			fp = popen(buff_shell, "r");
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
