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

struct indexed_data;
struct indexed_data* mk_indexed(struct record* rs, int n);
void free_indexed(struct indexed_data* data);
const struct record* lookup_indexed(struct indexed_data *data, int64_t needle);

int main() {

    struct indexed_data* idT = mk_indexed(Test_ids, 3);
    assert(idT != NULL);

    const struct record* T1 = lookup_indexed(idT, 1);
    assert(T1 && T1->osm_id == 1);

    free_indexed(idT);
    printf("id indexed test passed :>\n");
    return 0;
}