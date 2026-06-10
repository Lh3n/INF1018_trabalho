# Gerador Dinâmico de Funções em C

## Sobre o Projeto

Este projeto foi desenvolvido para a disciplina de Software Básico da graduação em Ciência da Computação.

O objetivo é implementar uma função capaz de gerar código de máquina dinamicamente em tempo de execução, criando novas versões de funções existentes a partir da fixação ou redirecionamento de parâmetros.

A implementação trabalha diretamente com instruções da arquitetura x86-64, escrevendo bytes de código executável em memória.

## Problema Proposto

A função principal do projeto, `cria_func`, recebe:

* O endereço de uma função original;
* Uma descrição dos parâmetros desejados;
* Uma região de memória para armazenar o código gerado.

A partir dessas informações, o programa constrói dinamicamente uma nova função capaz de:

* Repassar parâmetros recebidos;
* Fixar parâmetros com valores constantes;
* Carregar valores indiretamente através de ponteiros;
* Invocar a função original utilizando a convenção de chamada da arquitetura.

## Conceitos Utilizados

* Linguagem C
* Arquitetura x86-64
* Convenção de chamada System V ABI
* Manipulação de memória
* Geração dinâmica de código
* Ponteiros para função
* Instruções de máquina

## Funcionalidades Implementadas

* Escrita de instruções diretamente em memória;
* Geração de instruções `mov`;
* Movimentação entre registradores;
* Carregamento indireto de valores;
* Fixação de parâmetros em tempo de geração;
* Construção automática de prólogo e epílogo de função;
* Chamada indireta para funções através de ponteiros.

## Estrutura do Projeto

```text
cria_func.c
cria_func.h
teste.c
README.md
```

## Exemplo Conceitual

Considere uma função original:

```c
int soma(int a, int b);
```

É possível gerar dinamicamente uma nova função equivalente a:

```c
int soma10(int b) {
    return soma(10, b);
}
```

sem escrever manualmente uma nova implementação.

O código correspondente é construído em tempo de execução pela função `cria_func`.

## Aprendizados

Este projeto proporcionou experiência prática com:

* Representação de instruções em código de máquina;
* Registradores da arquitetura x86-64;
* Convenções de chamada utilizadas por compiladores;
* Manipulação avançada de ponteiros;
* Geração dinâmica de código executável.

## Autor

Luiz Henrique Machado da Silva

Graduando em Ciência da Computação – PUC-Rio

