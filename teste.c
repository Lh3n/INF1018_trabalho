#include <stdio.h>
#include "cria_func.h"

typedef int (*func_ptr)(int);

int mult(int x, int y) {
    return x * y;
}

int main(void) {
    DescParam params[2];
    unsigned char codigo[500];

    func_ptr f_mult;

    params[0].tipo_val = INT_PAR;
    params[0].orig_val = PARAM;

    params[1].tipo_val = INT_PAR;
    params[1].orig_val = FIX;
    params[1].valor.v_int = 10;

    cria_func(mult, params, 2, codigo);

    f_mult = (func_ptr) codigo;

    printf("5 * 10 = %d\n", f_mult(5));
    printf("7 * 10 = %d\n", f_mult(7));
    printf("12 * 10 = %d\n", f_mult(12));

    return 0;
}