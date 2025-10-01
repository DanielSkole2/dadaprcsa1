#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>


#include "record.h"
#include "id_query.h"


struct index_record {
int64_t osm_id;
const struct record *record;
};


struct indexed_sorted {
struct index_record *irs;
int n;
};


static int cmp_index_record(const void *a, const void *b) {
const struct index_record *ia = a;
const struct index_record *ib = b;
if (ia->osm_id < ib->osm_id) return -1;
if (ia->osm_id > ib->osm_id) return 1;
return 0;
}


struct indexed_sorted* mk_sorted(struct record* rs, int n) {
struct indexed_sorted *d = malloc(sizeof(struct indexed_sorted));
if (!d) return NULL;
d->irs = malloc(sizeof(struct index_record) * n);
if (!d->irs) { free(d); return NULL; }
d->n = n;
for (int i = 0; i < n; i++) {
d->irs[i].osm_id = rs[i].osm_id;
d->irs[i].record = &rs[i];
}
qsort(d->irs, n, sizeof(struct index_record), cmp_index_record);
return d;
}


void free_sorted(struct indexed_sorted* data) { 
if (!data) return;
free(data->irs);
free(data);
}


const struct record* lookup_sorted(struct indexed_sorted *data, int64_t needle) {
int lo = 0, hi = data->n - 1;
while (lo <= hi) {
int mid = lo + (hi - lo) / 2;
if (data->irs[mid].osm_id == needle) return data->irs[mid].record;
if (data->irs[mid].osm_id < needle) lo = mid + 1;
else hi = mid - 1;
}
return NULL;
}


int main(int argc, char** argv) {
return id_query_loop(argc, argv,
(mk_index_fn)mk_sorted,
(free_index_fn)free_sorted,
(lookup_fn)lookup_sorted);
}