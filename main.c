#include "fatfs.h"
#include <stdio.h>
#include "fat.h"

int main() {
    if (fatInit() != 0) {
        return -1; // Initialization failed
    }

    unsigned int cluster = fatOpen("/BIN/BASH");
    if (cluster == -1) {
        printf("File not found\n");
        return -1;
    }

    char buffer[512];
    int bytes_read = fatRead(cluster, buffer, sizeof(buffer));
    printf("Read %d bytes from /BIN/BASH\n", bytes_read);

    // Process the buffer here...

    return 0;
}

