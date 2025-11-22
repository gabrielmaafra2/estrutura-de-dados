#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Struct Territorio:
 * Representa um territorio no mapa do WAR.
 * nome   -> nome do territorio
 * cor    -> cor (exercito/dono)
 * tropas -> número de tropas no territorio
 */
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/*
 * cadastraTerritorios:
 * Preenche dinamicamente o vetor de territorios usando ponteiros.
 * mapa -> ponteiro para o vetor alocado
 * n    -> quantidade de territorios
 */
void cadastrarTerritorios(Territorio *mapa, int n) {
    printf("=== Cadastro de Territorios ===\n\n");

    for (int i = 0; i < n; i++) {
        printf("Territorio %d:\n", i + 1);

        // Le nome com espacos, limitando tamanho para evitar overflow
        printf("Digite o nome do territorio: ");
        scanf(" %29[^\n]", (mapa + i)->nome);

        // Le cor (palavra unica)
        printf("Digite a cor do exercito: ");
        scanf(" %9s", (mapa + i)->cor);

        // Le tropas (inteiro)
        printf("Digite o numero de tropas: ");
        scanf("%d", &((mapa + i)->tropas));

        // Validação simples: nao aceitar tropas <= 0
        if ((mapa + i)->tropas <= 0) {
            printf("Numero invalido. Ajustando tropas para 1.\n");
            (mapa + i)->tropas = 1;
        }

        printf("\n");
    }
}

/*
 * exibirTerritorios:
 * Mostra todos os territorios e seus dados.
 * mapa -> ponteiro para o vetor
 * n    -> quantidade de territorios
 */
void exibirTerritorios(Territorio *mapa, int n) {
    printf("=== Estado Atual do Mapa ===\n\n");

    for (int i = 0; i < n; i++) {
        printf("[%d] %s | Cor: %s | Tropas: %d\n",
               i, (mapa + i)->nome, (mapa + i)->cor, (mapa + i)->tropas);
    }

    printf("\n");
}

/*
 * atacar:
 * Simula um ataque entre dois territorios usando dados (1 a 6).
 * atacante -> ponteiro para territorio atacante
 * defensor -> ponteiro para territorio defensor
 *
 * Regras do desafio:
 * - Cada lado rola 1 dado (1..6).
 * - Se atacante vencer:
 *      defensor muda de cor (vira do atacante)
 *      defensor recebe metade das tropas do atacante
 * - Se atacante perder:
 *      atacante perde 1 tropa
 */
void atacar(Territorio *atacante, Territorio *defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- ATAQUE ---\n");
    printf("Atacante: %s (%s) rolou %d\n", atacante->nome, atacante->cor, dadoAtacante);
    printf("Defensor: %s (%s) rolou %d\n", defensor->nome, defensor->cor, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        // Atacante vence
        printf("Resultado: ATACANTE VENCEU!\n");

        // Defensor muda de cor para a do atacante
        strcpy(defensor->cor, atacante->cor);

        // Transfere metade das tropas do atacante
        int tropasTransferidas = atacante->tropas / 2;

        if (tropasTransferidas < 1) {
            // Garante pelo menos 1 tropa transferida se atacante venceu
            tropasTransferidas = 1;
        }

        atacante->tropas -= tropasTransferidas;
        defensor->tropas = tropasTransferidas;

        printf("%s conquistou %s e moveu %d tropas.\n",
               atacante->nome, defensor->nome, tropasTransferidas);

    } else {
        // Atacante perde (empate conta como derrota)
        printf("Resultado: DEFENSOR VENCEU!\n");

        // Atacante perde uma tropa, mas nao pode ficar sem tropas
        if (atacante->tropas > 1) {
            atacante->tropas -= 1;
            printf("%s perdeu 1 tropa.\n", atacante->nome);
        } else {
            printf("%s nao pode perder mais tropas (minimo 1).\n", atacante->nome);
        }
    }

    printf("-------------\n\n");
}

/*
 * liberarMemoria:
 * Libera a memoria alocada dinamicamente.
 */
void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

int main() {
    srand(time(NULL)); // Garante aleatoriedade dos dados

    int n;

    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Numero invalido de territorios.\n");
        return 1;
    }

    /*
     * Alocação dinamica:
     * calloc inicializa tudo com zero.
     */
    Territorio *mapa = (Territorio *)calloc(n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);
    exibirTerritorios(mapa, n);

    // Loop de ataques
    int opcao = 1;
    while (opcao != 0) {
        int iAtacante, iDefensor;

        printf("Escolha o indice do TERRITORIO ATACANTE: ");
        scanf("%d", &iAtacante);

        printf("Escolha o indice do TERRITORIO DEFENSOR: ");
        scanf("%d", &iDefensor);

        // Valida índices
        if (iAtacante < 0 || iAtacante >= n || iDefensor < 0 || iDefensor >= n) {
            printf("Indices invalidos. Tente novamente.\n\n");
            continue;
        }

        // territorio nao pode atacar a si mesmo
        if (iAtacante == iDefensor) {
            printf("Um territorio nao pode atacar ele mesmo.\n\n");
            continue;
        }

        Territorio *atacante = (mapa + iAtacante);
        Territorio *defensor = (mapa + iDefensor);

        // Nao atacar territorio da mesma cor
        if (strcmp(atacante->cor, defensor->cor) == 0) {
            printf("Ataque invalido: voce nao pode atacar territorio da mesma cor.\n\n");
            continue;
        }

        // Atacante precisa ter tropas suficientes
        if (atacante->tropas <= 1) {
            printf("Ataque invalido: atacante precisa ter pelo menos 2 tropas.\n\n");
            continue;
        }

        atacar(atacante, defensor);
        exibirTerritorios(mapa, n);

        printf("Deseja fazer outro ataque? (1 = sim, 0 = nao): ");
        scanf("%d", &opcao);
    }

    liberarMemoria(mapa);
    printf("\nMemoria liberada. Fim do programa!\n");

    return 0;
}
