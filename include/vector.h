#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef void * vtype;

struct vector{
    size_t size, capacity;
    vtype *arr;
};

struct vector *vector_init(size_t capacity);

void vector_destroy(struct vector *v);

int vector_append(struct vector *v, vtype elt);

vtype vector_pop_tail(struct vector *v);

vtype vector_peek_tail(struct vector *v);

vtype vector_peek_head(struct vector *v);

vtype vector_get(struct vector *v, size_t pos);

void vector_replace(struct vector *v, size_t pos, vtype elt);

int vector_is_empty(struct vector *v);

int vector_is_in(struct vector *v, vtype elt);

void vector_reverse(struct vector *v);

#endif /* ! VECTOR_H */

