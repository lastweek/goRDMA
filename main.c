/*
 * Copyright (c) 2021 Yizhou Shan syzwhat@gmail.com
 */

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <infiniband/verbs.h>

void test_print(void)
{
	printf("Hello from C\n");
}

/*
 * Given the @ibvdev name, we will try to fill the @ndev.
 * This is Linux-specific and relys on /sys files.
 */
int ibdev2netdev(const char *ibdev, char *ndev, size_t ndev_buf_size)
{
	char dev_dir[128];
	DIR *dp;
	struct dirent *dirp;
	int ret;

	snprintf(dev_dir, 128, "/sys/class/infiniband/%s/device/net", ibdev);

	dp = opendir(dev_dir);
	if (!dp)
		return errno;

	ret = -1;
	while (1) {
		dirp = readdir(dp);
		if (!dirp)
			break;

		if (!strcmp(dirp->d_name, "."))
			continue;
		if (!strcmp(dirp->d_name, ".."))
			continue;

		strncpy(ndev, dirp->d_name, ndev_buf_size);
		ret = 0;
		break;
	}
	closedir(dp);

	return ret;
}

struct ibv_device *goRDMA_get_ibv_device(char *name)
{
	struct ibv_device **dev_list;
	int i, num_devices;
	struct ibv_device *ib_dev;
	int ret;

	dev_list = ibv_get_device_list(&num_devices);
	if (!dev_list) {
		perror("Failed to get devices list");
		return NULL;
	}

	for (i = 0; i < num_devices; i++) {
		ib_dev = dev_list[i];
		if (!ib_dev) {
			fprintf(stderr, "IB device [%d] not valid\n", i);
			return NULL;
		}

		if (!strncmp(ibv_get_device_name(ib_dev), name, 32)) {
			return ib_dev;
		}
	}
	return NULL;
}

/*
 * Dump all IBV devices and their corresponding network device name.
 * This function calls the original ibv_get_device_list().
 */
int goRDMA_get_device_list(void)
{
	struct ibv_device **dev_list;
	int i, num_devices;
	struct ibv_device *ib_dev;
	int ret;
	char ndev[32];

	dev_list = ibv_get_device_list(&num_devices);
	if (!dev_list) {
		perror("Failed to get devices list");
		return -ENODEV;
	}

	printf("Total number of IB devices: %d\n", num_devices);
	for (i = 0; i < num_devices; i++) {
		ib_dev = dev_list[i];
		if (!ib_dev) {
			fprintf(stderr, "IB device [%d] not valid\n", i);
			return -ENODEV;
		}

		ret = ibdev2netdev(ibv_get_device_name(ib_dev), ndev, sizeof(ndev));
		if (ret) {
			fprintf(stderr, "fail to do ibdev2netdev %d\n", 0);
			return ret;
		}
		printf("   ib_dev[%d]: %s -> %s\n",
			i, ibv_get_device_name(ib_dev), ndev);
	}
	return 0;


	/* ib_context = ibv_open_device(ib_dev); */
	/* if (!ib_context) { */
	/*         fprintf(stderr, "Couldn't get ib_context for %s\n", */
	/*                 ibv_get_device_name(ib_dev)); */
	/*         return -ENODEV; */
	/* } */
        /*  */
	/* ib_pd = ibv_alloc_pd(ib_context); */
	/* if (!ib_pd) { */
	/*         fprintf(stderr, "Couldn't allocate PD\n"); */
	/*         return -ENOMEM; */
	/* } */
}
