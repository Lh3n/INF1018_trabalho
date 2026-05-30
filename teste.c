#include <stdio.h>
#include "cria_func.h"

typedef int (*func_ptr)(void);

int mult(int x, int y) {
    return x * y;
}

int main(void) {
    DescParam params[2];
    unsigned char codigo[500];

    int i;

    func_ptr f_mult;

    params[0].tipo_val = INT_PAR;
    params[0].orig_val = IND;
    params[0].valor.v_ptr = &i;

    params[1].tipo_val = INT_PAR;
    params[1].orig_val = FIX;
    params[1].valor.v_int = 10;

    cria_func(mult, params, 2, codigo);

    f_mult = (func_ptr) codigo;

    for (i = 1; i <= 5; i++) {
        printf("%d\n", f_mult());
    }

    return 0;
}