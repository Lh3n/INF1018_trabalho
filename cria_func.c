#include <stdio.h>
#include <string.h>
#include "cria_func.h"

/*
 Dicionario de registradores de parâmetros
 1o parametro -> rdi
 2o parametro -> rsi
 3o parametro -> rdx
 */

/*
push %rbp
mov %rsp,%rbp

movl $10,%esi

movabs mult,%rax
call *%rax

leave
ret
*/

/* escreve 1 byte */
static void emit1(unsigned char codigo[], int *pos, unsigned char byte) {
    codigo[*pos] = byte;
    (*pos)++;
}

/* escreve 4 bytes */
static void emit4(unsigned char codigo[], int *pos, int valor) {
    memcpy(&codigo[*pos], &valor, 4);
    (*pos) += 4;
}

/* escreve 8 bytes */
static void emit8(unsigned char codigo[], int *pos, long valor) {
    memcpy(&codigo[*pos], &valor, 8);
    (*pos) += 8;
}

/*
mov imediato 64 bits -> registrador
movabs $imm64, %reg
*/

static void mov_imm64_reg(unsigned char codigo[], int *pos, long valor, int reg) {
    emit1(codigo, pos, 0x48);

    switch(reg) {
        case 0: emit1(codigo, pos, 0xBF); break; /* rdi */
        case 1: emit1(codigo, pos, 0xBE); break; /* rsi */
        case 2: emit1(codigo, pos, 0xBA); break; /* rdx */
        case 3: emit1(codigo, pos, 0xB8); break; /* rax */
    }

    emit8(codigo, pos, valor);
}

/*
 mov imediato 32 bits -> registrador 32 bits
*/

static void mov_imm32_reg(unsigned char codigo[], int *pos, int valor, int reg) {
    switch(reg) {
        case 0: emit1(codigo, pos, 0xBF); break; /* edi */
        case 1: emit1(codigo, pos, 0xBE); break; /* esi */
        case 2: emit1(codigo, pos, 0xBA); break; /* edx */
    }

    emit4(codigo, pos, valor);
}

/*
move registrador origem -> registrador destino
*/

static void mov_reg_reg(unsigned char codigo[], int *pos, int origem, int destino) {
    emit1(codigo, pos, 0x48);
    emit1(codigo, pos, 0x89);

    /*
    ModRM simplificado
     origem:
     0=rdi
     1=rsi
     2=rdx
     destino:
     0=rdi
     1=rsi
     2=rdx
    */

    unsigned char modrm = 0xC0;

    modrm |= (origem << 3);
    modrm |= destino;

    emit1(codigo, pos, modrm);
}

/*
carrega inteiro indiretamente
movabs endereco, %rax
mov (%rax), %reg32
*/

static void load_ind_int(unsigned char codigo[], int *pos, void *endereco, int reg) {
    /* movabs endereco -> rax */
    mov_imm64_reg(codigo, pos, (long)endereco, 3);

    emit1(codigo, pos, 0x8B);

    switch(reg) {
        case 0: emit1(codigo, pos, 0x38); break; /* edi */
        case 1: emit1(codigo, pos, 0x30); break; /* esi */
        case 2: emit1(codigo, pos, 0x10); break; /* edx */
    }
}

/*
carrega ponteiro indiretamente
movabs endereco, %rax
mov (%rax), %reg64
*/

static void load_ind_ptr(unsigned char codigo[], int *pos, void *endereco, int reg) {
    mov_imm64_reg(codigo, pos, (long)endereco, 3);
    emit1(codigo, pos, 0x48);
    emit1(codigo, pos, 0x8B);

    switch(reg) {
        case 0: emit1(codigo, pos, 0x38); break;
        case 1: emit1(codigo, pos, 0x30); break;
        case 2: emit1(codigo, pos, 0x10); break;
    }
}

void cria_func (void* f, DescParam params[], int n, unsigned char codigo[]) {
    int i;
    int pos = 0;
    int prox_param = 0;

    
    //PROLOGO

    emit1(codigo, &pos, 0x55);             /* push %rbp */

    emit1(codigo, &pos, 0x48);
    emit1(codigo, &pos, 0x89);
    emit1(codigo, &pos, 0xE5);             /* mov %rsp,%rbp */

    //processa parametros

    for(i = 0; i < n; i++) {

        //PARAM

        if(params[i].orig_val == PARAM) {

            /*
            se registrador já coincide,
            não precisa mover
            */

            if(prox_param != i) {
                mov_reg_reg(codigo, &pos,
                            prox_param, i);
            }

            prox_param++;
        }

        //FIX

        else if(params[i].orig_val == FIX) {

            if(params[i].tipo_val == INT_PAR) {

                mov_imm32_reg(codigo,
                              &pos,
                              params[i].valor.v_int,
                              i);
            }
            else {

                mov_imm64_reg(codigo,
                              &pos,
                              (long)params[i].valor.v_ptr,
                              i);
            }
        }

        //IND

        else if(params[i].orig_val == IND) {

            if(params[i].tipo_val == INT_PAR) {

                load_ind_int(codigo,
                             &pos,
                             params[i].valor.v_ptr,
                             i);
            }
            else {

                load_ind_ptr(codigo,
                             &pos,
                             params[i].valor.v_ptr,
                             i);
            }
        }
    }

    //mov endereco funcao -> rax

    mov_imm64_reg(codigo, &pos, (long)f, 3);

    //call *%rax

    emit1(codigo, &pos, 0xFF);
    emit1(codigo, &pos, 0xD0);

    //leave

    emit1(codigo, &pos, 0xC9);

    //ret

    emit1(codigo, &pos, 0xC3);
}
