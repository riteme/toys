#include <cstdio>

#include "verilated.h"
#include "VCacheTop.h"

#define CACHE_T 24
#define CACHE_E 2
#define CACHE_B 6
#define SET_NUM (1 << 2)
#define LINE_SIZE (1 << ((CACHE_B) - 2))

int main(int argc, char *argv[]) {
    Verilated::commandArgs(argc, argv);
    VCacheTop *dev = new VCacheTop;

    printf("cache.enabled=%d\n", dev->enabled);
    printf("cache.now=%d, cache.count=%d\n", dev->now, dev->count);
    printf("cache.state=%d\n", dev->state);
    printf("cache.saved_tag=%x\n", dev->saved_tag);

    for (int i = 0; i < SET_NUM; i++) {
        printf("\n# set %d\n", i);
        printf("req_tag[%d]=%d, dirty=%d\n", i, dev->req_tag[i], dev->req_dirty[i]);
        for (int j = 0; j < CACHE_E; j++) {
            printf("tag=%x, dirty=%d, tick=%d\n",
                dev->tag[i][j], dev->dirty[i][j], dev->tick[i][j]);
            for (int k = 0; k < LINE_SIZE; k += 8) {
                printf("\t");
                for (int l = 0; l < 8; l++)
                    printf(l == 3 ? "%08x  " : "%08x " , dev->line[i][j][k + l]);
                puts("");
            }
        }
    }

    return 0;
}