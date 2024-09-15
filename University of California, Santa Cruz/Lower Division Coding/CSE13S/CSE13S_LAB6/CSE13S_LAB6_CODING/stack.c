/*
struct stack;
typedef struct stack Stack;

Stack *stack_create(uint32_t capacity);

void stack_free(Stack **sp);

bool stack_push(Stack *s, uint32_t val);

bool stack_pop(Stack *s, uint32_t *val);

bool stack_peek(const Stack *s, uint32_t *val);

bool stack_empty(const Stack *s);

bool stack_full(const Stack *s);

uint32_t stack_size(const Stack *s);

void stack_copy(Stack *dst, const Stack *src);

void stack_print(const Stack *s, FILE *f, char *vals[]);
*/

#include "stack.h"

#include <assert.h>
#include <stdlib.h>
typedef struct stack {
    uint32_t capacity;
    uint32_t top;
    uint32_t *items;
} Stack;

Stack *stack_create(uint32_t capacity) {
    // Attempt to allocate memory for a stack
    // Cast it to a stack pointer too!
    Stack *s = (Stack *) malloc(sizeof(Stack));
    s->capacity = capacity;
    s->top = 0;
    // We need enough memory for <capacity> numbers
    s->items = calloc(s->capacity, sizeof(uint32_t));
    // We created our stack, return it!
    return s;
}

void stack_free(Stack **sp) {
    // sp is a double pointer, so we have to check if it,
    // or the pointer it points to is null.
    if (sp != NULL && *sp != NULL) {
        // Of course, we have to remember to free the
        // memory for the array of items first,
        // as that was also dynamically allocated!
        // If we freed the Stack first then we would
        // not be able to access the array to free it.
        if ((*sp)->items) {
            free((*sp)->items);
            (*sp)->items = NULL;
        }
        // Free memory allocated for the stack
        free(*sp);
    }
    if (sp != NULL) {
        // Set the pointer to null! This ensures we dont ever do a double free!
        *sp = NULL;
    }
}

bool stack_push(Stack *s, uint32_t val) {
    // If the stack is full, return false;
    if (stack_full(s)) {
        return false;
    }
    // Set val
    s->items[s->top] = val;
    // Move the top of the stack
    s->top++;
    return true;
}

bool stack_pop(Stack *s, uint32_t *val) {
    if (s == NULL || stack_empty(s) || val == NULL)
        return false;
    *val = s->items[(s->top) - 1];
    s->top--;
    return true;
}

bool stack_peek(const Stack *s, uint32_t *val) {
    if (s == NULL || stack_empty(s) || val == NULL)
        return false;
    *val = s->items[(s->top) - 1];
    return true;
}

bool stack_empty(const Stack *s) {
    return s->top > 0 ? false : true;
}

bool stack_full(const Stack *s) {
    return s->top >= s->capacity ? true : false;
}

uint32_t stack_size(const Stack *s) {
    return s->top;
}

void stack_copy(Stack *dst, const Stack *src) {
    assert(dst->capacity >= src->top);
    for (uint32_t i = 0; i < src->top; ++i)
        dst->items[i] = src->items[i];
    dst->top = src->top;
    dst->capacity = src->capacity;
}

void stack_print(const Stack *s, FILE *f, char *vals[]) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(f, "%s\n", vals[s->items[i]]);
    }
}
