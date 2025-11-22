#include <stdio.h>
#include <string.h>

/*
 * Struct Territorio:
 * Agrupa informacoes relacionadas a um territorio do jogo/sistema.
 * - nome: nome do territorio (ate 29 caracteres + '\0')
 * - cor: cor do exercito que domina o territorio (ate 9 caracteres + '\0')
 * - tropas: quantidade de tropas no territorio
 */
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    // Vetor de structs para armazenar somente 5 territorios
    Territorio territorios[5];

    printf("=== Cadastro de Territorios ===\n\n");

    // Laco para preencher os dados dos 5 territorios
    for (int i = 0; i < 5; i++) {
        printf("Territorio %d:\n", i + 1);

        /*
         * Leitura do nome:
         * scanf(" %29[^\n]", ...) para colocar espacos no nome
         * e limitar o tamanho.
         */
        printf("Digite o nome do territorio: ");
        scanf(" %29[^\n]", territorios[i].nome);

        /*
         * Leitura da cor:
         * scanf("%9s", ...) porque cor usa uma palavra so.
         */
        printf("Digite a cor do exercito: ");
        scanf(" %9s", territorios[i].cor);

        /*
         * Leitura do numero de tropas:
         * Inteiro simples com scanf.
         */
        printf("Digite o numero de tropas: ");
        scanf("%d", &territorios[i].tropas);

        printf("\n");
    }

    printf("=== Territorios Cadastrados ===\n\n");

    // Laco para exibir os dados cadastrados
    for (int i = 0; i < 5; i++) {
        printf("Territorio %d\n", i + 1);
        printf("Nome  : %s\n", territorios[i].nome);
        printf("Cor   : %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
        printf("-------------------------\n");
    }

    return 0;
}
