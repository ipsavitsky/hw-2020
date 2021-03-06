#include "stack.h"

/**
 * struct of an RPN
 */
typedef struct {
    void *data;        ///< input data of rpn
    size_t data_size;  ///< size of data_size
    size_t occupied;   ///< tracking of the last written element
} RPN;

/**
 * struct of a variable table
 */
typedef struct {
    /**
     * struct of a single variable
     */
    struct Var_data {
        char name[7];  ///< name of the varibale, null-terminated(constrained by
                       ///< +1(\0) by the task)
        void *data;    ///< data written to the variable
        size_t data_size;  ///< size of the data in the variable
    } * vars;              ///< array of variables
    size_t var_num;        ///< amount of variables in a table
} Var_table;

/**
 * struct of a data passed to calculation functions
 */
typedef struct {
    const void *elem;  ///< pointer to the data
    Size_elem size;    ///< size of elem
    Stack *stack;      ///< stack for the stack machine
    Var_table *v_tab;  ///< variable table
} Calculation_data;

typedef int (*Calculate_elem)(Calculation_data *data);

/**
 * compute the RPN
 * \param notation notation to compute
 * \param res result of the computation
 * \param res_size size of res
 * \param vars variable table for calculation
 * \exception may throw an exception thrown to it by a Calculate_elem function
 * \return error code
 */
int RPN_compute(RPN *notation, void *res, size_t res_size, Var_table *vars);

/**
 * initialize an RPN
 * \param notation RPN to initialize
 * \param size size of the notation
 * \exception E_MEM_ALLOC Thrown in case of memory allocation error
 * \return error code
 */
int RPN_init(RPN *notation, size_t size);

/**
 * finalize an RPN
 * \param notation RPN to finalize
 */
void RPN_finalize(RPN *notation);
