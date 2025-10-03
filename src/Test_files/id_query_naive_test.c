#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "../id_query.h"
#include "../record.h"   

struct record Test_ids[] = {
    {.osm_id = 1, .lon = -1.0, .lat = -1.0}, 
    {.osm_id = 2, .lon = 0.0, .lat = 0.0}, 
    {.osm_id = 3, .lon = 1.0, .lat = 1.0}
};

struct naive_data;
struct naive_data* mk_naive(struct record* rs, int n);
void free_naive(struct naive_data* data);
const struct record* lookup_naive(struct naive_data *data, int64_t needle);

int main() {

    struct naive_data* idT = mk_naive(Test_ids, 3);
    assert(idT != NULL);

    const struct record* T1 = lookup_naive(idT, 1);
    assert(T1 && T1->osm_id == 1);

    free_naive(idT);
    printf("id naive test passed :>\n");

    return 0;
}