#include "fatfs.h"
#include "sd.h" // Include your SD driver
#include <string.h>
#include <stdio.h>
#include <ctype.h>

struct boot_sector *bs;
char bootSector[512];
char fat_table[8 * SECTOR_SIZE];
unsigned int root_sector;

// Implementation of fatInit
int fatInit() {
    sd_readblock(0, bootSector, 1);
    bs = (struct boot_sector *)bootSector;

    if (bs->boot_signature != 0xAA55) {
        printf("Invalid boot signature\n");
        return -1;
    }

    if (strcmp(bs->fs_type, "FAT12") != 0) {
        printf("Unsupported filesystem type\n");
        return -1;
    }

    for (int i = 0; i < bs->num_fat_tables; i++) {
        sd_readblock(bs->num_reserved_sectors + (i * bs->num_sectors_per_fat), &fat_table[i * SECTOR_SIZE], bs->num_sectors_per_fat);
    }

    root_sector = bs->num_fat_tables * bs->num_sectors_per_fat + bs->num_reserved_sectors + bs->num_hidden_sectors;

    return 0;
}

// Implementation of fatOpen
int fatOpen(const char *filename) {
    char upper_filename[12];
    strncpy(upper_filename, filename, 11);
    for (int i = 0; i < 11; i++) {
        upper_filename[i] = toupper(upper_filename[i]);
    }

    for (int i = 0; i < bs->num_root_dir_entries; i++) {
        struct directory_entry rde;
        sd_readblock(root_sector + (i / (SECTOR_SIZE / sizeof(struct directory_entry))), (char *)&rde, sizeof(struct directory_entry));

        if (rde.name[0] == 0x00) {
            break;
        }

        if (strncmp(rde.name, upper_filename, 11) == 0) {
            return rde.first_cluster;
        }
    }

    return -1;
}

// Implementation of fatRead
int fatRead(unsigned int cluster, char *buffer, size_t size) {
    size_t bytes_read = 0;
    unsigned int current_cluster = cluster;

    while (bytes_read < size) {
        int sector = (current_cluster - 2) * bs->num_sectors_per_cluster + root_sector + (bs->num_fat_tables * bs->num_sectors_per_fat);
        sd_readblock(sector, buffer + bytes_read, bs->bytes_per_sector);

        bytes_read += bs->bytes_per_sector;

        current_cluster = fat_table[current_cluster * 2];
        if (current_cluster >= 0xFFF8) {
            break;
        }
    }

    return bytes_read;
}

