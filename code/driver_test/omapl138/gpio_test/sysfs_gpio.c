#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int get_pin_val(int pin);
void out_pin_val(int pin, int val);
void dir_pin(int pin, char *dir);
void export_pin(int pin);

#define GPIO_2_PIN(bank, index) (bank*16 + index)
int main(int argc, char *argv[])
{
	int opt;
	int nbank = -1, index = -1;
	char dir[10];


	while ((opt = getopt(argc, argv, "b:i::h")) != -1) {
		printf("optind:%d\n",optind);
		printf("optarg:%s\n",optarg);
		printf("opt:%c\n",opt);
		switch (opt) {
		case 'h':
			printf("Usage: ./a.out -b bank -i pinIndex\n");
			exit(EXIT_FAILURE);
		case 'b':
			nbank = atoi(optarg);
			break;
		case 'i':
			index = atoi(optarg);	
			break;
		default: /* '?' */
			fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
					argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (nbank < 0 || index < 0) {
		fprintf(stderr, "wrong bank or wrong index used\n");
		exit(EXIT_FAILURE);
	}

	printf("nbank=%d; index = %d, please input direction(out or in):\n", nbank, index);

	scanf("%s", dir);
	export_pin(GPIO_2_PIN(nbank, index));

	if (strncmp(dir, "in", 2) == 0) {
		dir_pin(GPIO_2_PIN(nbank, index), "in");
		printf("pin level = %d\n", get_pin_val(GPIO_2_PIN(nbank, index)));
	
	}
	else if(strncmp(dir, "out", 3) == 0 ){
		printf("out data 0 ? 1");
		scanf("%s", dir);
		dir_pin(GPIO_2_PIN(nbank, index), "out");
		out_pin_val(GPIO_2_PIN(nbank, index), atoi(dir));
	}
	 else {
		fprintf(stderr, "direction error, program exit now!\n");
		exit(EXIT_FAILURE);
	}


	/* Other code omitted */
	exit(EXIT_SUCCESS);
}

#define PIN_PATH "/sys/class/gpio/"
#define EXPORT_FILE	PIN_PATH "export"
#define UNEXPORT_FILE	PIN_PATH "unexport"


void export_pin(int pin)
{
	int fd = -1;
	char buf[10]= {'\0'};

	if ((fd = open(EXPORT_FILE, O_WRONLY)) < 0) {
		fprintf(stderr, "open %s file error, exit now\n", EXPORT_FILE);
		exit(EXIT_FAILURE);
	}

	sprintf(buf, "%d", pin);
	if (write(fd, buf, strlen(buf)) <= 0) {
		fprintf(stderr, "write %s buf = %s error\n", EXPORT_FILE, buf);
		//exit(EXIT_FAILURE);
	}

	close(fd);
}

void dir_pin(int pin, char *dir)
{
	int fd = -1;
	char file_name[64] = {'\0'};

	sprintf(file_name, "%sgpio%d/direction", PIN_PATH, pin);

	if ((fd = open(file_name, O_WRONLY)) < 0) {
		fprintf(stderr, "open %s file error, exit now\n", file_name);
		exit(EXIT_FAILURE);
	}

	if (write(fd, dir, strlen(dir)) <= 0) {
		fprintf(stderr, "write pin dir error\n");
		exit(EXIT_FAILURE);
	}

	close(fd);
}

void out_pin_val(int pin, int val)
{
	int fd = -1;
	char file_name[64] = {'\0'};
	char buf[10] = {'\0'};

	sprintf(file_name, "%sgpio%d/value", PIN_PATH, pin);

	if ((fd = open(file_name, O_WRONLY)) < 0) {
		fprintf(stderr, "open %s file error, exit now\n", file_name);
		exit(EXIT_FAILURE);
	}

	sprintf(buf, "%d", val);
	if (write(fd, buf, 1) <= 0) {
		fprintf(stderr, "write pin dir error\n");
		exit(EXIT_FAILURE);
	}

	close(fd);
}

int get_pin_val(int pin)
{
	int fd = -1;
	char file_name[64] = {'\0'};
	char buf[10] = {'\0'};

	sprintf(file_name, "%sgpio%d/value", PIN_PATH, pin);

	if ((fd = open(file_name, O_RDONLY)) < 0) {
		fprintf(stderr, "open %s file error, exit now\n", file_name);
		exit(EXIT_FAILURE);
	}

	if (read(fd, buf, 10) <= 0) {
		fprintf(stderr, "read pin error\n");
		return -1;
	}

	close(fd);

	return atoi(buf);
}
