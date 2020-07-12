#ifndef __IOCTL_REMAP_H__
#define __IOCTL_REMAP_H__

#include <linux/ioctl.h>
#include <linux/types.h>

#define EXT4_IOC_WAL_TX_WRITE _IOW('f', 34, struct wal_tx_write_info)
#define EXT4_IOC_CP_WRITE _IOW('f', 37, struct cp_write_info)
#define EXT4_IOC_REMAP_CKPT _IOW('f', 35, struct remap_ckpt_info)
#define EXT4_IOC_REMAP_COPY _IOW('f', 36, struct remap_copy_info)

#define WAL_WRITE  100
#define CP_WRITE 102
#define REMAP_CKPT 2
#define REMAP_COPY 3
#define REMAP_MOVE 4

// transactional SSD for wal
struct wal_tx_write_info{
    unsigned long pbuf;       // 用户空间数据buffer地址
    unsigned long count;             // 写wal文件字符数（一次处理一个页，count<=4096）
    long long int pos;               // 写wal文件偏移
    int db_fd;                // db文件fd
    unsigned int db_lblk;     // 数据页在db文件中的偏移
    // unsigned int tid;      // sqlite事务id
    unsigned int flag;     // 数据页在tx中的位置
};

struct cp_write_info{
    unsigned long pbuf;       // 用户空间数据buffer地址
    unsigned long count;             // 写wal文件字符数（一次处理一个页，count<=4096）
    long long int pos;               // 写wal文件偏移
    unsigned int flag;     // 数据写不计算时间
};

// REMAP SSD：checkpoint write（操作db文件fd，update的数据，分别获取两个文件LPN，下发REMAP命令）
struct remap_ckpt_info{
    int wal_fd;               // wal文件的fd
    unsigned int wal_lblk;    // 执行REMAP的文件block在wal文件偏移
    unsigned int db_lblk;     // 执行REMAP的文件block在db文件偏移
    unsigned int len;         // 执行REMAP的连续的文件block数量
};

// REMAP SSD：copy write（操作复制数据的目的文件fd，insert的数据，分配目的LPN，下发REMAP命令）
struct remap_copy_info{
    int src_fd;                 // 源文件的fd
    unsigned int src_lblk;      // 执行REMAP的文件block在源文件偏移
    unsigned int dst_lblk;      // 执行REMAP的文件block在目的文件偏移
    unsigned int len;           // 执行REMAP的连续的文件block数量
    unsigned long count;               // copy的字符数
};

extern int nvme_issue_remap(unsigned long src_lpn, unsigned long dst_lpn, unsigned int len, unsigned int ope);

#endif
