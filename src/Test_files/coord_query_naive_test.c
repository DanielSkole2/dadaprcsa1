#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include "../record.h"
#include "../coord_query.h"

struct record Test_ids[] = {
    {.osm_id = 1, .lon = 0.0, .lat = 0.0}, 
    {.osm_id = 2, .lon = 1.0, .lat = 1.0}, 
    {.osm_id = 3, .lon = -1.0, .lat = -1.0}
};

struct naive_data_coord;
struct naive_data_coord* mk_naive(struct record* rs, int n);
void free_naive(struct naive_data_coord* data);
const struct record* lookup_naive(struct naive_data_coord *data, double lon, double lat);

int main() {
    struct naive_data_coord* idT = mk_naive(Test_ids, 3);
    assert(idT != NULL);

    const struct record* T1 = lookup_naive(idT, 0.1, 0.1);
    assert(T1 && T1->osm_id == 1);

    free_naive(idT);
    printf("coord naive test passed :>\n");

    return 0;
}
    
