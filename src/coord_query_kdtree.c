#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "coord_query.h"


struct kd_node {
    const struct record *pt; 
    int axis;
    struct kd_node *left, *right;
};


struct kd_data {
    const struct record **buf;
    struct kd_node *root;
    int n;
};


static int cmp_lon(const void *a, const void *b) {
    const struct record *ra = *(const struct record **)a;
    const struct record *rb = *(const struct record **)b;
    if (ra->lon < rb->lon) return -1;
    if (ra->lon > rb->lon) return 1;
    return 0;
}

static int cmp_lat(const void *a, const void *b) {
    const struct record *ra = *(const struct record **)a;
    const struct record *rb = *(const struct record **)b;
    if (ra->lat < rb->lat) return -1;
    if (ra->lat > rb->lat) return 1;
    return 0;
}


static struct kd_node* build_kd(struct kd_node *node,
                                const struct record **pts, int l, int r, int depth) {
    if (l > r) return NULL;
    int axis = depth % 2;
    int m = (l + r) / 2;

    
    if (axis == 0) {
        qsort(pts + l, r - l + 1, sizeof(*pts), cmp_lon);
    } else {
        qsort(pts + l, r - l + 1, sizeof(*pts), cmp_lat);
    }

    node = malloc(sizeof(*node));
    node->pt = pts[m];
    node->axis = axis;
    node->left = build_kd(node->left, pts, l, m - 1, depth + 1);
    node->right = build_kd(node->right, pts, m + 1, r, depth + 1);
    return node;
}


struct kd_data* mk_kdtree(struct record *rs, int n) {
    struct kd_data *d = malloc(sizeof(*d));
    d->buf = malloc(n * sizeof(*d->buf));
    d->n = n;

    for (int i = 0; i < n; i++) d->buf[i] = &rs[i];
    d->root = build_kd(NULL, d->buf, 0, n - 1, 0);
    return d;
}

static double dist2rec(double lon1, double lat1, const struct record *r) {
    double dx = lon1 - r->lon;
    double dy = lat1 - r->lat;
    return dx*dx + dy*dy;
}

static void lookup_rec(const struct record **closest, double *best_d2,
                       const struct kd_node *node, double qlon, double qlat) {
    if (!node) return;
    double d2 = dist2rec(qlon, qlat, node->pt);
    if (d2 < *best_d2) {
        *best_d2 = d2;
        *closest = node->pt;
    }
    int axis = node->axis;
    double diff = (axis == 0) ? (node->pt->lon - qlon) : (node->pt->lat - qlat);
    const struct kd_node *first = (diff >= 0) ? node->left : node->right;
    const struct kd_node *second = (diff >= 0) ? node->right : node->left;
    if (first) lookup_rec(closest, best_d2, first, qlon, qlat);
    if (second && (*best_d2 > diff*diff)) lookup_rec(closest, best_d2, second, qlon, qlat);
}

const struct record* lookup_kdtree(struct kd_data *data, double lon, double lat) {
    if (data->n == 0) return NULL;
    const struct record *closest = data->buf[0];
    double best_d2 = dist2rec(lon, lat, closest);
    lookup_rec(&closest, &best_d2, data->root, lon, lat);
    return closest;
}

static void free_node(struct kd_node *n) {
    if (!n) return;
    free_node(n->left);
    free_node(n->right);
    free(n);
}

void free_kdtree(struct kd_data* data) {
    if (!data) return;
    free_node(data->root);
    free(data->buf);
    free(data);
}

#ifndef COORD_QUERY_NO_MAIN
int main(int argc, char** argv) {
    return coord_query_loop(argc, argv,
                            (mk_index_fn)mk_kdtree,
                            (free_index_fn)free_kdtree,
                            (lookup_fn)lookup_kdtree);
}
#endif
