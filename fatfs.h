#ifndef FATFS_H
#define FATFS_H

#include <stdint.h>

// Boot sector structure
struct boot_sector {
    char code[3];
    char oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t num_sectors_per_cluster;
    uint16_t num_reserved_sectors;
    uint8_t num_fat_tables;
    uint16_t num_root_dir_entries;
    uint16_t total_sectors;
    uint8_t media_descriptor;
    uint16_t num_sectors_per_fat;
    uint16_t num_sectors_per_track;
    uint16_t num_heads;
    uint32_t num_hidden_sectors;
    uint32_t total_sectors_in_fs;
    uint8_t logical_drive_num;
    uint8_t reserved;
    uint8_t extended_signature;
    uint32_t serial_number;
    char volume_label[11];
    char fs_type[8];
    char boot_code[448];
    uint16_t boot_signature;
} __attribute__((packed));

// Function prototypes
int fatInit();
int fatOpen(const char *filename);
int fatRead(unsigned int cluster, char *buffer, size_t size);

#endif // FATFS_H

