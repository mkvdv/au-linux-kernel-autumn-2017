#include "vsd_device.h"
#include "../vsd_driver/vsd_ioctl.h"
#include <fcntl.h>
#include <stdio.h>

#define ERR -1
#define OK   0
static const char* dev_path = "/dev/vsd";
static int fd = -1;


int vsd_init()
{
    fd = open(dev_path, O_RDWR);
    if (fd < 0) {
        return ERR;
    }
    return OK;
}

int vsd_deinit()
{
    if (fd < 0) {
        return ERR;
    }

    close(fd);
    fd = -1;
    return OK;
}

int vsd_get_size(size_t *out_size)
{
    if (fd < 0) {
        return ERR;
    }

    int ret_val;
    vsd_ioctl_get_size_arg_t res;
    ret_val = ioctl(fd, VSD_IOCTL_GET_SIZE, &res);
    if (ret_val < 0) {
        return ERR;
    }
    *out_size = res.size;
    return OK;
}

int vsd_set_size(size_t size)
{
    if (fd < 0) {
        return ERR;
    }

    int ret_val;
    vsd_ioctl_set_size_arg_t arg;

    arg.size = size;
    ret_val = ioctl(fd, VSD_IOCTL_SET_SIZE, &arg);
    if (ret_val < 0) {
        return ERR;
    }
    return OK;
}

ssize_t vsd_read(char* dst, off_t offset, size_t size)
{
    if (fd < 0) {
        return ERR;
    }

    if (lseek(fd, offset, SEEK_SET) < 0) {
        return ERR;
    }

    return read(fd, dst, size);
}

ssize_t vsd_write(const char* src, off_t offset, size_t size)
{
    if (fd < 0) {
        return ERR;
    }

    if (lseek(fd, offset, SEEK_SET) < 0) {
        return ERR;
    }

    return write(fd, src, size);
}

void* vsd_mmap(size_t offset)
{
    if (fd < 0) {
        return NULL;
    }

    size_t cur_size;
    int err = vsd_get_size(&cur_size);
    if (err || offset > cur_size) {
       return NULL;
    }

    size_t length = cur_size - offset;
    void* addr = mmap(NULL, length, PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, offset);
    if (addr == MAP_FAILED) {
        return NULL;
    }
    return addr;
}

int vsd_munmap(void* addr, size_t offset)
{
    if (fd < 0) {
        return ERR;
    }

    size_t cur_size;

    int rc = vsd_get_size(&cur_size);
    if ((!rc) || offset >= cur_size) {
        return ERR;
    }

    return munmap(addr, cur_size - offset);
}
