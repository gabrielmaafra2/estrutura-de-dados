#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Struct Territorio
 * Representa cada territorio no mapa do jogo WAR.
 * - nome: nome do territorio
 * - cor: exercito que controla o territorio
 * - tropas: tamanho da tropa estacionada
 */
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;


/* =====================================================================
    FUNCAO: atribuirMissao
    - destino: ponteiro para onde a missao sorteada sera copiada
    - missoes: vetor de strings contendo as missoes disponiveis
    - totalMissoes: quantidade de missoes disponiveis
    FUNCAO:
    Sorteia uma missao aleatoria e copia para o destino usando strcpy.
   ===================================================================== */
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;  // escolhe uma missao aleatoria
    strcpy(destino, missoes[sorteio]);   // copia para armazenamento do jogador
}


/* =====================================================================
    FUNCAO: verificarMissao
    - missao: string contendo a missao do jogador
    - mapa: ponteiro para o vetor de territorios
    - tamanho: quantidade total de territorios
    FUNCAO:
    Implementa uma logica simples para verificar se a missao foi cumprida.
    Exemplos de logicas implementadas:
    - "Conquistar 3 territorios" -> verifica se existem 3 territorios da mesma cor
    - "Eliminar a cor vermelha" -> verifica se vermelho ainda existe
   ===================================================================== */
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {
    // verificacao simples baseada em palavras-chave das missoes

    // Missao 1: Conquistar 3 territorios da cor azul
    if (strstr(missao, "Conquistar 3 territorios azuis") != NULL) {
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "azul") == 0)
                count++;
        }
        if (count >= 3) return 1;
    }

    // Missao 2: Eliminar a cor vermelha
    if (strstr(missao, "Eliminar todas as tropas da cor vermelha") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelha") == 0)
                return 0; // vermelho ainda existe
        }
        return 1; // missao cumprida
    }

    // Missao 3: Controlar qualquer territorio com mais de 20 tropas
    if (strstr(missao, "Controlar um territorio com mais de 20 tropas") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas > 20)
                return 1;
        }
    }

    // Missao 4: Controlar o maior numero de territorios
    if (strstr(missao, "Controlar o maior numero de territorios") != NULL) {
        int azul = 0, vermelho = 0, verde = 0;

        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "azul") == 0) azul++;
            if (strcmp(mapa[i].cor, "vermelha") == 0) vermelho++;
            if (strcmp(mapa[i].cor, "verde") == 0) verde++;
        }

        if (azul > vermelho && azul > verde) return 1;
        if (vermelho > azul && vermelho > verde) return 1;
        if (verde > azul && verde > vermelho) return 1;
    }

    // Missao 5: Conquistar 2 territorios seguidos
    if (strstr(missao, "Conquistar 2 territorios seguidos") != NULL) {
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas > 1) count++;
        }
        if (count >= 2) return 1;
    }

    return 0; // missao NAO cumprida
}


/* =====================================================================
    FUNCAO: exibirMissao
    Exibe a missao do jogador apenas uma vez no inicio.
   ===================================================================== */
void exibirMissao(char *missao) {
    printf("\n=== SUA MISSAO ===\n");
    printf("%s\n\n", missao);
}


/* =====================================================================
    FUNCAO: atacar
    Simula ataque usando um dado aleatorio (1 a 6).
    Atualiza tropas e pode alterar controle do territorio.
   ===================================================================== */
void atacar(Territorio *atacante, Territorio *defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- ATAQUE ---\n");
    printf("%s (%s) rolou %d\n", atacante->nome, atacante->cor, dadoAtacante);
    printf("%s (%s) rolou %d\n", defensor->nome, defensor->cor, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Atacante venceu!\n");

        strcpy(defensor->cor, atacante->cor);

        int tropasMovidas = atacante->tropas / 2;
        if (tropasMovidas < 1) tropasMovidas = 1;

        defensor->tropas = tropasMovidas;
        atacante->tropas -= tropasMovidas;

    } else {
        printf("Defensor venceu!\n");
        if (atacante->tropas > 1)
            atacante->tropas -= 1;
    }

    printf("------------------\n\n");
}


/* =====================================================================
    FUNCAO: exibirMapa
    Exibe todos os territorios no estado atual.
   ===================================================================== */
void exibirMapa(Territorio *mapa, int n) {
    printf("=== ESTADO ATUAL DO MAPA ===\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("\n");
}


/* =====================================================================
    FUNCAO: liberarMemoria
    Libera memoria alocada (territorios e missao do jogador).
   ===================================================================== */
void liberarMemoria(Territorio *mapa, char *missaoJogador) {
    free(mapa);
    free(missaoJogador);
}


/* =====================================================================
    FUNCAO PRINCIPAL (main)
   ===================================================================== */
int main() {
    srand(time(NULL));

    int n;
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &n);

    Territorio *mapa = (Territorio *)calloc(n, sizeof(Territorio));

    if (!mapa) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    // ==== CADASTRO DE TERRITORIOS ====
    for (int i = 0; i < n; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("Nome: ");
        scanf(" %29[^\\n]", mapa[i].nome);
        printf("Cor: ");
        scanf(" %9s", mapa[i].cor);
        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
        printf("\n");
    }

    // ==== VETOR DE MISSOES ====
    char *missoes[5] = {
        "Conquistar 3 territorios azuis",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar um territorio com mais de 20 tropas",
        "Controlar o maior numero de territorios",
        "Conquistar 2 territorios seguidos"
    };

    // ==== ALOCA MEMORIA PARA MISSAO DO JOGADOR ====
    char *missaoJogador = (char *)malloc(100 * sizeof(char));
    atribuirMissao(missaoJogador, missoes, 5);

    exibirMissao(missaoJogador);

    exibirMapa(mapa, n);

    // ==== LOOP DO JOGO ====
    int opcao = 1;
    while (opcao != 0) {
        int a, d;

        printf("Digite o indice do atacante: ");
        scanf("%d", &a);
        printf("Digite o indice do defensor: ");
        scanf("%d", &d);

        if (a < 0 || a >= n || d < 0 || d >= n || a == d) {
            printf("Movimento invalido!\n\n");
            continue;
        }

        if (strcmp(mapa[a].cor, mapa[d].cor) == 0) {
            printf("Nao pode atacar a mesma cor!\n\n");
            continue;
        }

        atacar(&mapa[a], &mapa[d]);
        exibirMapa(mapa, n);

        // ==== VERIFICA MISSAO ====
        if (verificarMissao(missaoJogador, mapa, n)) {
            printf("\n==========================\n");
            printf("MISSAO CUMPRIDA! VOCE VENCEU!\n");
            printf("Missao: %s\n", missaoJogador);
            printf("==========================\n\n");
            break;
        }

        printf("Novo turno? (1 = sim, 0 = nao): ");
        scanf("%d", &opcao);
    }

    liberarMemoria(mapa, missaoJogador);

    return 0;
}
