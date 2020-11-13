#include "RPN.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SAFE(call)                  \
    do {                            \
        if ((flag = call) != 0) {   \
            stack_finalize(&stack); \
            return flag;            \
        }                           \
    } while (0)

int RPN_compute(RPN *notation, void *res, size_t res_size, Var_table *vars) {
    Stack stack;
    int flag;
    struct input_data {
        Size_elem size;
        Calculate_elem f;
    };
    if ((flag = stack_init(&stack, notation->data_size)) != 0) return flag;
    for (size_t cur_size = 0; cur_size < notation->data_size;) {
        void *elem = &(((char *)notation->data)[cur_size]);
        struct input_data in_dat = *((struct input_data *)elem);
        Calculation_data dat =
            (Calculation_data){.elem = &((char*)notation->data)[cur_size + sizeof(struct input_data)],
                               .size = in_dat.size - sizeof(Calculate_elem),
                               .stack = &stack,
                               .v_tab = vars};
        SAFE(in_dat.f(&dat));
        cur_size += sizeof(struct input_data) + in_dat.size - sizeof(Calculate_elem);
    }
    SAFE(stack_pop(&stack, res, res_size));
    stack_finalize(&stack);
    return 0;
}

int RPN_init(RPN *notation, size_t size) {
    if ((notation->data = malloc(size)) == NULL) return E_MEM_ALLOC;
    notation->data_size = size;
    notation->occupied = 0;
    return 0;
}

void RPN_finalize(RPN *notation) { free(notation->data); }
