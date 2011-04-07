#include <stdio.h>
#include <string.h>

static int test_pressure(char *sysfs_dir)
{
	char sysfs_name[64];
	char buf[8];
	FILE *fp;

	sprintf(sysfs_name, "%s/pressure0_input", sysfs_dir);
	fp = fopen(sysfs_name, "r+");
	if (fp) /* No rw! This should fail! */
		return -1;

	fp = fopen(sysfs_name, "r");
	if (!fp)
		return -1;

	if (fread(buf, 1, 8, fp) == 0)
		goto error_pressure;
	rewind(fp);

	fclose(fp);
	return 0;

error_pressure:
	//perror("error_pressure");
	fclose(fp);
	return -1;
}

static int test_temperature(char *sysfs_dir)
{
	char sysfs_name[64];
	char buf[8];
	FILE *fp;

	sprintf(sysfs_name, "%s/temp0_input", sysfs_dir);
	fp = fopen(sysfs_name, "r+");
	if (fp) /* No rw! This should fail! */
		return -1;

	fp = fopen(sysfs_name, "r");
	if (!fp)
		return -1;

	if (fread(buf, 1, 8, fp) == 0)
		goto error_temperature;
	rewind(fp);

	fclose(fp);
	return 0;

error_temperature:
	//perror("error_temperature");
	fclose(fp);
	return -1;
}

static int test_oversampling(char *sysfs_dir)
{
	char sysfs_name[64];
	char buf[8];
	int oversampling;
	FILE *fp;

	sprintf(sysfs_name, "%s/oversampling", sysfs_dir);
	fp = fopen(sysfs_name, "r+");
	if (!fp)
		return -1;

	if (fread(buf, 1, 8, fp) == 0) {
		printf("Error: %d\n", ferror(fp));
		goto error_oversampling;
	}
	rewind(fp);

	oversampling = atoi(buf);
	sprintf(buf, "%u", oversampling+1);
	if (fwrite(buf, 1, strlen(buf), fp) != strlen(buf))
		goto error_oversampling;
	rewind(fp);

	if (fread(buf, 1, 8, fp) == 0)
		goto error_oversampling;
	rewind(fp);

	if (atoi(buf) != oversampling + 1)
		goto error_oversampling;

	fclose(fp);
	return 0;

error_oversampling:
	//perror("error_oversampling");
	fclose(fp);
	return -1;
}

int main(int argc, char **argv)
{
	char *sysfs_dir;

	printf("Testing bmp18x sysfs interfaces.\n");
	if (argc != 2) {
		fprintf(stderr, "usage: %s <sysfs path>\n", argv[0]);
		goto error;
	}
	sysfs_dir = argv[1];

	printf("Test oversampling... ");
	if (test_oversampling(sysfs_dir) < 0) {
		printf("FAILED!\n");
		goto error;
	}
	printf("OK\n");

	printf("Test temperature... ");
	if (test_temperature(sysfs_dir) < 0) {
		printf("FAILED!\n");
		goto error;
	}
	printf("OK\n");

	printf("Test pressure... ");
	if (test_pressure(sysfs_dir) < 0) {
		printf("FAILED!\n");
		goto error;
	}
	printf("OK\n");

	printf("Testing was successful!\n");
	return 0;

error:
	printf("Testing FAILED!\n");
	return 1;
}
