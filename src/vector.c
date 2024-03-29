#include <stdlib.h>
#include <stddef.h>

#include "vector.h"

static int vector_double_capacity(struct vector *v);

struct vector *vector_init(size_t capacity)
{
    struct vector *v = malloc(sizeof(struct vector));
    if (!v)
        return NULL;
    v->arr = malloc(capacity * sizeof(void *));
    if (!v->arr)
    {
        free(v);
        return NULL;
    }
    v->size = 0;
    v->capacity = capacity;
    return v;
}

void vector_destroy(struct vector *v)
{
    free(v->arr);
    free(v);
}

int vector_append(struct vector *v, vtype elt)
{
    if (v->size >= v->capacity)
    {
        if (vector_double_capacity(v) == -1)
            return -1;
    }
    v->arr[v->size] = elt;
    v->size += 1;
    return 1;
}

static int vector_double_capacity(struct vector *v)
{
    vtype *tmp = realloc(v->arr, v->capacity * 2 * sizeof(vtype ));
    if (!tmp)
        return -1;
    v->arr = tmp;
    v->capacity *= 2;
    return 1;
}

vtype vector_get(struct vector *v, size_t pos)
{
    if (v->size <= 0)
        return NULL;
    if (pos > v->size)
        return v->arr[v->size - 1];
    return v->arr[pos];
}

void vector_replace(struct vector *v, size_t pos, vtype elt)
{
    if (v->size <= 0)
        return;
    if (pos > v->size)
        pos = v->size - 1;
    v->arr[pos] = elt;
}

vtype vector_peek_head(struct vector *v)
{
    return vector_get(v, 0);
}

vtype vector_peek_tail(struct vector *v)
{
    return vector_get(v, v->size - 1);
}

vtype vector_pop_tail(struct vector *v)
{
    vtype ret = vector_peek_tail(v);
    if (ret == NULL)
        return NULL;
    v->size -= 1;
    return ret;
}

int vector_is_empty(struct vector *v)
{
    return (v->size == 0);
}

int vector_is_in(struct vector *v, vtype elt)
{
    for (size_t i = 0; i < v->size; i++)
    {
        if (v->arr[i] == elt)
            return 1;
    }
    return 0;
}

static void swap(vtype *a, vtype *b)
{
    vtype tmp = *a;
    *a = *b;
    *b = tmp;
}

void vector_reverse(struct vector *v)
{
    int i = 0;
    int j = v->size - 1;
    while (i < j)
    {
        swap(&v->arr[i], &v->arr[j]);
        i++;
        j--;
    }
}

