#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

/*
** index is an index in a node array
*/

struct vector{
    size_t size, capacity;
    void **arr;
};

struct vector *vector_init(size_t capacity);

void vector_destroy(struct vector *v);

int vector_append(struct vector *v, void *elt);

void *vector_pop_tail(struct vector *v);

void *vector_peek_tail(struct vector *v);

void *vector_peek_head(struct vector *v);

void *vector_get(struct vector *v, size_t pos);

int vector_isempty(struct vector *v);

int vector_is_in(struct vector *v, void *elt);

void vector_reverse(struct vector *v);

#endif /* ! VECTOR_H */

