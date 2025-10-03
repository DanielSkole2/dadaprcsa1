#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "../record.h"
#include "../coord_query.h"

struct record Test_ids[] = {
    {.osm_id = 1, .lon = 0.0, .lat = 0.0}, 
    {.osm_id = 2, .lon = 1.0, .lat = 1.0}, 
    {.osm_id = 3, .lon = -1.0, .lat = -1.0}
};

struct kd_data;
struct kd_data* mk_kdtree(struct record *rs, int n);
const struct record* lookup_kdtree(struct kd_data *data, double lon, double lat);
void free_kdtree(struct kd_data* data);

int main() {

    struct kd_data* idT = mk_kdtree(Test_ids, 3);
    assert(idT != NULL);

    const struct record* T1 = lookup_kdtree(idT, -0.7, -0.8);
    assert(T1 && T1->osm_id == 3);

    free_kdtree(idT);
    printf("kdtree test passed :>\n");
    return 0;
}