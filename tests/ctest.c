#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


static void kill(int pid, int sig) {
	register int    syscall_no  asm("rax") = 62;
	register int    arg1        asm("rdi") = pid;
	register int    arg2        asm("rsi") = sig;
	asm("syscall");
}

static void exit_host(int r){
	kill(0, 9);
	return;
}

int vfork1()
{
	int a = 0;
	int ret = 0xfafafafa;
	ret = vfork();
	return ret;
}

int vfork2()
{
	return vfork();
}

void test_files();

char *argv[3] = {"/ctest", "noop", NULL};
char *env[2] = {"PATH=/", NULL};

int test_vfork()
{
	int i, ret;
	int status;

	ret = vfork();
	if (ret == 0) {
		printf("child: ret=%d\r\n", ret);
		//printf("got %c\n", getchar());
		ret = execve("./ctest", argv, env);
		if (ret != 0) {
			printf("child: exec ret=%d\r\n", ret);
			while(1);
			_exit(0);
		}

		// should not be here
		test_files();
		_exit(0);
	} else if (ret < 0) {
		printf("error %d\r\n", ret);
		while(1);
		_exit(0);
	} else {
		wait(&status);
		//while(1);
		printf("parent (%s): fork status=%d child_pid=%d\r\n", __FUNCTION__, status, ret);
	}
	return ret;
}

void test_files()
{
	printf("opening\r\n");
	char *tmpname = "/files/mifile";
	char buf[128];
	int fd = open("/files/mifile2", O_RDONLY, 0);
	if (fd < 1) {
		printf("could not open %d\n", fd);
		return 1;
	}
	//write(fd, "_*_", 3);
	fsync(fd);
	close(fd);
	printf("opening\r\n");
	fd = open("/files/mifile2", O_RDONLY, 0);
	if (fd < 1) {
		printf("could not open %d\n", fd);
		return 1;
	}

	memset(buf, 0, 128);
	read(fd, buf, 64);
	printf("%s\r\n", buf);
}

int test_vfork_only()
{
	int i, ret;
	int status;

	ret = vfork1();
	if (ret == 0) {
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		printf("child %s\n", __FUNCTION__);
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		sleep(1);
		_exit(0);
	} else if (ret < 0) {
		printf("error %d\r\n", ret);
		while(1);
		_exit(0);
	} else {
		sleep(1);
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		printf("parent (%s): fork status=%d child_pid=%d\r\n", __FUNCTION__, status, ret);
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
	}
	return ret;
}



int test_vfork_sleeping()
{
	int i, ret;
	int status;

	ret = vfork();
	if (ret == 0) {
		ret = execve("./ctest", argv, env);
		_exit(0);
	} else if (ret < 0) {
		printf("error %d\r\n", ret);
		while(1);
		_exit(0);
	} else {
		printf("parent (%s): fork status=%d child_pid=%d\r\n", __FUNCTION__, status, ret);
	}
	return ret;
}


int test_vfork_in_vfork()
{
	int i, ret;
	int status;

	ret = vfork();
	if (ret == 0) {
		printf("child 1\n");
		ret = vfork();
		if (ret == 0) {
			printf("child 2\n");
			_exit(0);
		} else if (ret < 0) {
			printf("error %d\r\n", ret);
			while(1);
			_exit(0);
		} else {
			printf("parent: fork status=%d child_pid=%d\r\n", status, ret);
		}
		_exit(0);
	} else if (ret < 0) {
		printf("error %d\r\n", ret);
		while(1);
		_exit(0);
	} else {
		printf("parent (%s): fork status=%d child_pid=%d\r\n", __FUNCTION__, status, ret);
	}
	return ret;
}

int test_exec_with_vfork_in_vfork()
{
	int i, ret;
	int status;

	char *argv[3] = {"/ctest", "fork", NULL};
	char *env[2] = {"PATH=/", NULL};

	ret = vfork();
	if (ret == 0) {
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		printf("child %s\n", __FUNCTION__);
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		ret = execve("./ctest", argv, env);
		while(1); // should not be reached
	} else if (ret < 0) {
		printf("error %d\r\n", ret);
		while(1);
		_exit(0);
	} else {
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		printf("parent (%s): fork status=%d child_pid=%d\r\n", __FUNCTION__, status, ret);
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
	}
	return ret;
}


int main(int argc, char **argv)
{
	int i, ret;
	int iterations;

	if (argc < 2) {
		iterations = 10;
	} else if (strcmp(argv[1], "files") == 0) {
		test_files();
		exit(0);
	} else if (strcmp(argv[1], "loop") == 0) {
		printf("looping forever\n");
		//printf("got %c\n", getchar());
		while(1);
		exit(0);
	} else if (strcmp(argv[1], "fork") == 0) {
		//test_vfork_only();
		test_vfork();
		exit(0);
	} else if (strcmp(argv[1], "exec") == 0) {
		test_exec_with_vfork_in_vfork();
		test_files();
		exit(0);
	} else if (strcmp(argv[1], "noop") == 0) {
		printf("noop\n");
		exit(0);
	} else if (strcmp(argv[1], "exit") == 0) {
		printf("exit");
		exit_host(0);
	} else if (strcmp(argv[1], "time") == 0) {
		struct timeval current_time;
		gettimeofday(&current_time, NULL);
		printf("seconds : %ld\nmicro seconds : %ld\n",
				current_time.tv_sec, current_time.tv_usec);
		exit(0);
	} else {
		printf("default arg\n");
	}

	test_exec_with_vfork_in_vfork();

	test_vfork();
	test_vfork();
	test_vfork();
	test_vfork();
	test_vfork_sleeping();

	test_files();

	printf("got %c\n", getchar());

	//while(1);
	exit(0);
}
