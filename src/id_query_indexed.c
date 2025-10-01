#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "id_query.h"
#include "record.h"   



struct index_record {
int64_t osm_id;
const struct record *record;
};


struct indexed_data {
struct index_record *irs;
int n;
};


struct indexed_data* mk_indexed(struct record* rs, int n) {
struct indexed_data *d = malloc(sizeof(struct indexed_data));
if (!d) return NULL;
d->irs = malloc(sizeof(struct index_record) * n);
if (!d->irs) { free(d); return NULL; }
d->n = n;
for (int i = 0; i < n; i++) {
d->irs[i].osm_id = rs[i].osm_id;
d->irs[i].record = &rs[i];
}
return d;
}


void free_indexed(struct indexed_data* data) {
if (!data) return;
free(data->irs);
free(data);
}


const struct record* lookup_indexed(struct indexed_data *data, int64_t needle) {
for (int i = 0; i < data->n; i++) {
if (data->irs[i].osm_id == needle) return data->irs[i].record;
}
return NULL;
}


int main(int argc, char** argv) { 
return id_query_loop(argc, argv,
(mk_index_fn)mk_indexed,
(free_index_fn)free_indexed,
(lookup_fn)lookup_indexed);
}