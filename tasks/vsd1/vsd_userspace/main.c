#include "../vsd_driver/vsd_ioctl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

const char* size_set = "size_set";
const char* size_get = "size_get";
const char* dev_path = "/dev/vsd";

static void perr_with_usages(const char* mesg)
{
    if (mesg)
        fprintf(stderr, "[ERROR]:%s\n", mesg);
    fprintf(stderr, "\tusage: vsd_userspace size_get\n");
    fprintf(stderr, "\tusage: vsd_userspace size_set SIZE_IN_BYTES\n");
}

#define PARSE_ERROR -1
#define PARSE_OK     0
static int parse_size(size_t* nsize_p, const char* str)
{
    char* endptr;

    endptr = 0;
    *nsize_p = strtoul(str, &endptr, 10);
    if (*str == '\0' || *endptr != '\0') {
        return PARSE_ERROR;
    }
    return PARSE_OK;
}

static int process_size_get()
{
    int fd;
    int ret_val;
    vsd_ioctl_get_size_arg_t res;

    fd = open(dev_path, 0);
    if (fd < 0) {
        fprintf(stderr, "can't open file: %s\n", dev_path);
        return EXIT_FAILURE;
    }

    ret_val = ioctl(fd, VSD_IOCTL_GET_SIZE, &res);
    if (ret_val < 0) {
        printf("vsd_ioctl_get_size failed: %d\n", ret_val);
        close(fd);
        return EXIT_FAILURE;
    }

    printf("vsd_ioctl_get_size returned: %zu\n", res.size);
    return EXIT_SUCCESS;
}

static int process_size_set(size_t new_size)
{
    int fd;
    int ret_val;
    vsd_ioctl_set_size_arg_t arg;

    fd = open(dev_path, 0);
    if (fd < 0) {
        fprintf(stderr, "can't open file: %s\n", dev_path);
        return EXIT_FAILURE;
    }

    arg.size = new_size;
    ret_val = ioctl(fd, VSD_IOCTL_SET_SIZE, &arg);
    if (ret_val < 0) {
        printf("vsd_ioctl_set_size failed: %d\n", ret_val);
        close(fd);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3) {
        perr_with_usages("incorrect number of arguments");
        return EXIT_FAILURE;
    }

    if (argc == 2) {
        if (strcmp(size_get, argv[1]) == 0) {
            return process_size_get();
        }
    } else if (argc == 3) {
        if (strcmp(size_set, argv[1]) == 0) {

            size_t new_size;
            int rc = parse_size(&new_size, argv[2]);
            if (rc != PARSE_OK) {
                perr_with_usages("incorrect SIZE_IN_BYTES argument");
                return EXIT_FAILURE;
            }
            return process_size_set(new_size);
        }
    }

    perr_with_usages("incorrect command format");
    return EXIT_FAILURE;
}
