#include "RPN.h"

#include <stdlib.h>
#include <string.h>

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
    if ((flag = stack_init(&stack, notation->data_size)) != 0) return flag;
    for (size_t cur_size = 0; cur_size < notation->data_size;
         cur_size += sizeof(Size_elem) + ((unsigned char *)notation->data)[cur_size]) {
        void *elem = &(((char *)notation->data)[cur_size + sizeof(Size_elem)]);
        Calculate_elem func;  // = *((Calculate_elem *)elem);
        // this is slow but easy and reliable :)
        memcpy(&func, (Calculate_elem *)elem, sizeof(func));
        Calculation_data dat =
            (Calculation_data){.elem = elem,
                               .size = ((char *)notation->data)[cur_size],
                               .stack = &stack,
                               .v_tab = vars};
        SAFE(func(&dat));
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
