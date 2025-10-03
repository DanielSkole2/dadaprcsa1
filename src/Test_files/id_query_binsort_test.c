#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "../record.h"
#include "../id_query.h"

struct record Test_ids[] = {
    {.osm_id = 1, .lon = -1.0, .lat = -1.0}, 
    {.osm_id = 2, .lon = 0.0, .lat = 0.0}, 
    {.osm_id = 3, .lon = 1.0, .lat = 1.0}
};

struct indexed_sorted;
struct indexed_sorted* mk_sorted(struct record* rs, int n);
const struct record* lookup_sorted(struct indexed_sorted *data, int64_t needle);
void free_sorted(struct indexed_sorted* data);

int main() {

    struct indexed_sorted* idT = mk_sorted(Test_ids, 3);
    assert(idT != NULL);

    const struct record* T1 = lookup_sorted(idT, 2);
    assert(T1 && T1->osm_id==2);

    free_sorted(idT);
    printf("id binsort test passed :> \n");
    return 0;
}
