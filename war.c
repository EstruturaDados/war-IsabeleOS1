// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
#define NUM_TERRITORIOS 5
#define MAX_STRING 50
#define NUM_MISSOES 2 // Número total de missões disponíveis
#define MAX_DADOS 3   // Máximo de dados de ataque

// Códigos de Missão
#define MISSAO_DESTROIR_VERDE 1
#define MISSAO_CONQUISTAR_3 2

// --- Estrutura de Dados ---
typedef struct {
    char nome[MAX_STRING];
    char cor_exercito[MAX_STRING];
    int num_tropas;
} Territorio;

// --- Protótipos das Funções ---

// Funções de setup e gerenciamento de memória
Territorio* alocarMapa(void);
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);

// Funções de interface com o usuário
void exibirMenuPrincipal(void);
void exibirMapa(const Territorio* mapa);
void exibirMissao(int id_missao);

// Funções de lógica principal do jogo
void faseDeAtaque(Territorio* mapa, const char* cor_jogador);
void simularAtaque(Territorio* atacante, Territorio* defensor);
int sortearMissao(void);
int verificarVitoria(const Territorio* mapa, int id_missao, const char* cor_jogador);

// Função utilitária
void limparBufferEntrada(void);
int rolarDado(void);

// ============================================================================
// --- Função Principal (main) ---
// ============================================================================

int main() {
    // 1. Configuração Inicial (Setup):
    setlocale(LC_ALL, "Portuguese"); // Define o locale para português
    srand(time(NULL));               // Inicializa a semente para rand()

    Territorio* mapa = alocarMapa(); // Aloca memória para o mapa
    if (mapa == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para o mapa.\n");
        return 1;
    }

    inicializarTerritorios(mapa); // Preenche os dados iniciais

    const char* cor_jogador = "Azul"; // Cor fixa do jogador para este desafio
    int id_missao = sortearMissao();
    int escolha = -1;
    int vitoria = 0;

    printf("=== 🧠 Desafio WAR Estruturado - Nível Mestre ===\n");
    printf("Você é o exército **%s**.\n", cor_jogador);

    // 2. Laço Principal do Jogo (Game Loop):
    do {
        exibirMapa(mapa);
        exibirMissao(id_missao);
        exibirMenuPrincipal();

        printf("Sua ação (0-2): ");
        if (scanf("%d", &escolha) != 1) {
            escolha = -1; // Sinaliza erro de leitura
        }
        limparBufferEntrada(); // Limpa o buffer após a leitura

        switch (escolha) {
            case 1:
                faseDeAtaque(mapa, cor_jogador);
                break;
            case 2:
                vitoria = verificarVitoria(mapa, id_missao, cor_jogador);
                if (vitoria) {
                    printf("\n🎉🎉 **VITÓRIA!** Você cumpriu sua missão. Parabéns! 🎉🎉\n");
                } else {
                    printf("\n❌ Missão ainda não cumprida. Continue atacando!\n");
                }
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }
        
        // Pausa para que o jogador possa ler os resultados
        if (escolha != 0 && vitoria == 0) {
            printf("\nPressione ENTER para continuar a rodada...");
            getchar(); // Espera o ENTER
        }

    } while (escolha != 0 && vitoria == 0);

    // 3. Limpeza:
    liberarMemoria(mapa);

    return 0;
}

// ============================================================================
// --- Implementação das Funções ---
// ============================================================================

// Função utilitária para limpar o buffer de entrada
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Gera um número aleatório entre 1 e 6 (simulando um dado)
int rolarDado(void) {
    return (rand() % 6) + 1;
}

// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
Territorio* alocarMapa(void) {
    // calloc inicializa a memória com zeros (NULL para ponteiros, 0 para inteiros)
    return (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

// Libera a memória previamente alocada.
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemória do mapa liberada com sucesso.\n");
    }
}

// Preenche os dados iniciais dos territórios.
void inicializarTerritorios(Territorio* mapa) {
    // Vetor de nomes e cores iniciais (para garantir nomes únicos e cores variadas)
    char nomes[NUM_TERRITORIOS][MAX_STRING] = {"Alaska", "Groelandia", "Brasil", "Africa do Sul", "Siberia"};
    char cores[NUM_TERRITORIOS][MAX_STRING] = {"Vermelho", "Azul", "Verde", "Amarelo", "Vermelho"};

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor_exercito, cores[i]);
        // Tropas iniciais entre 2 e 4
        mapa[i].num_tropas = (rand() % 3) + 2; 
    }
}

// Exibe o menu de ações do jogo.
void exibirMenuPrincipal(void) {
    printf("\n--- 🔨 Menu de Ações ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair do Jogo\n");
    printf("------------------------\n");
}

// Exibe o estado atual de todos os territórios (usa 'const').
void exibirMapa(const Territorio* mapa) {
    printf("\n--- 🗺️ Estado Atual do Mapa 🗺️ ---\n");
    printf("ID | %-20s | %-15s | %-10s\n", "Nome do Território", "Exército", "Tropas");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        // Usa o índice (i + 1) como ID para o jogador
        printf("%-2d | %-20s | %-15s | %-10d\n", 
               i + 1,
               mapa[i].nome, 
               mapa[i].cor_exercito, 
               mapa[i].num_tropas);
    }
    printf("----------------------------------------------------------\n");
}

// Exibe a descrição da missão.
void exibirMissao(int id_missao) {
    printf("\n--- 📜 Sua Missão Secreta ---\n");
    switch (id_missao) {
        case MISSAO_DESTROIR_VERDE:
            printf("**MISSÃO 1:** Destruir completamente o exército **Verde**.\n");
            break;
        case MISSAO_CONQUISTAR_3:
            printf("**MISSÃO 2:** Conquistar um total de **3 territórios**.\n");
            break;
        default:
            printf("Missão desconhecida.\n");
            break;
    }
    printf("----------------------------\n");
}

// Sorteia o ID da missão.
int sortearMissao(void) {
    // Sorteia um número entre 1 e NUM_MISSOES (1 ou 2)
    return (rand() % NUM_MISSOES) + 1;
}

// Gerencia a fase de ataque, obtendo a entrada do usuário.
void faseDeAtaque(Territorio* mapa, const char* cor_jogador) {
    int id_atacante, id_defensor;
    
    printf("\n--- ⚔️ FASE DE ATAQUE ⚔️ ---\n");
    
    printf("ID do seu território ATACANTE (1-%d): ", NUM_TERRITORIOS);
    if (scanf("%d", &id_atacante) != 1) { limparBufferEntrada(); return; }
    
    printf("ID do território DEFENSOR (1-%d): ", NUM_TERRITORIOS);
    if (scanf("%d", &id_defensor) != 1) { limparBufferEntrada(); return; }

    limparBufferEntrada(); // Garante o buffer limpo após o último scanf

    // Validações
    if (id_atacante < 1 || id_atacante > NUM_TERRITORIOS || 
        id_defensor < 1 || id_defensor > NUM_TERRITORIOS || 
        id_atacante == id_defensor) {
        printf("\nERRO: IDs de território inválidos ou iguais.\n");
        return;
    }

    // Usa (id - 1) para acessar o índice do vetor (0 a 4)
    Territorio* atacante = &mapa[id_atacante - 1];
    Territorio* defensor = &mapa[id_defensor - 1];

    if (strcmp(atacante->cor_exercito, cor_jogador) != 0) {
        printf("\nERRO: Você só pode atacar com seus próprios territórios (Cor: %s).\n", cor_jogador);
        return;
    }

    if (atacante->num_tropas <= 1) {
        printf("\nERRO: O território atacante precisa de pelo menos 2 tropas (1 para atacar, 1 de reserva).\n");
        return;
    }

    if (strcmp(defensor->cor_exercito, cor_jogador) == 0) {
        printf("\nERRO: Você não pode atacar seus próprios territórios!\n");
        return;
    }

    // Executa a lógica da batalha
    simularAtaque(atacante, defensor);
}

// Executa a lógica de uma batalha entre dois territórios (passagem por referência).
void simularAtaque(Territorio* atacante, Territorio* defensor) {
    printf("\n--- Detalhes da Batalha: %s vs %s ---\n", atacante->nome, defensor->nome);
    
    // O atacante pode usar no máximo 3 dados, limitado pelo número de tropas - 1
    int num_dados_ataque = (atacante->num_tropas - 1 < MAX_DADOS) ? atacante->num_tropas - 1 : MAX_DADOS;
    
    // O defensor pode usar no máximo 2 dados, limitado pelo número de tropas
    int num_dados_defesa = (defensor->num_tropas < 2) ? defensor->num_tropas : 2;

    int dados_ataque[MAX_DADOS];
    int dados_defesa[2]; // Defensor usa no máximo 2 dados

    // Rola os dados de ataque
    printf("Atacante (%s - %d dados):", atacante->cor_exercito, num_dados_ataque);
    for (int i = 0; i < num_dados_ataque; i++) {
        dados_ataque[i] = rolarDado();
        printf(" %d", dados_ataque[i]);
    }

    // Rola os dados de defesa
    printf("\nDefensor (%s - %d dados):", defensor->cor_exercito, num_dados_defesa);
    for (int i = 0; i < num_dados_defesa; i++) {
        dados_defesa[i] = rolarDado();
        printf(" %d", dados_defesa[i]);
    }
    printf("\n");

    // Lógica da comparação dos dados (apenas para o maior par neste desafio simplificado)
    int dano_atacante = 0;
    int dano_defensor = 0;
    
    // Simplificação: Compara o maior dado de ataque com o maior dado de defesa
    int maior_ataque = 0;
    for (int i = 0; i < num_dados_ataque; i++) {
        if (dados_ataque[i] > maior_ataque) maior_ataque = dados_ataque[i];
    }

    int maior_defesa = 0;
    for (int i = 0; i < num_dados_defesa; i++) {
        if (dados_defesa[i] > maior_defesa) maior_defesa = dados_defesa[i];
    }
    
    printf("-> Comparação: Ataque Máximo (%d) vs Defesa Máxima (%d)\n", maior_ataque, maior_defesa);
    
    if (maior_ataque > maior_defesa) {
        dano_defensor = 1;
        printf("🏆 O Atacante venceu esta comparação! Defensor perde 1 tropa.\n");
    } else { // Empate ou Defesa maior
        dano_atacante = 1;
        printf("🛡️ O Defensor venceu/empatou esta comparação! Atacante perde 1 tropa.\n");
    }

    // Aplica os danos
    atacante->num_tropas -= dano_atacante;
    defensor->num_tropas -= dano_defensor;
    
    printf("Resultado: %s agora tem %d tropas. %s agora tem %d tropas.\n", 
           atacante->nome, atacante->num_tropas, defensor->nome, defensor->num_tropas);

    // Checa por conquista
    if (defensor->num_tropas <= 0) {
        printf("\n🌟 **CONQUISTA!** %s conquistou o território %s!\n", atacante->cor_exercito, defensor->nome);
        
        // Conquista: defensor perde a cor e atacante move 1 tropa para lá.
        strcpy(defensor->cor_exercito, atacante->cor_exercito);
        
        // Move 1 tropa (a tropa de ataque obrigatória)
        defensor->num_tropas = 1;
        atacante->num_tropas -= 1; // A tropa de ataque é a primeira a ser movida
    }
}

// Verifica se a missão foi cumprida (usa 'const' para o mapa e cor_jogador).
int verificarVitoria(const Territorio* mapa, int id_missao, const char* cor_jogador) {
    int territorios_conquistados = 0;
    int verde_existe = 0; // Flag para a missão de destruição

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].cor_exercito, cor_jogador) == 0) {
            territorios_conquistados++;
        }
        if (strcmp(mapa[i].cor_exercito, "Verde") == 0) {
            verde_existe = 1;
        }
    }

    switch (id_missao) {
        case MISSAO_DESTROIR_VERDE:
            // Missão: Destruir o exército Verde (se o exército Verde não existir e o jogador tiver tomado algum território dele, ou seja, se verde_existe for 0).
            return !verde_existe;
            
        case MISSAO_CONQUISTAR_3:
            // Missão: Conquistar 3 territórios.
            printf("\nProgresso da Missão 2: %d/%d territórios conquistados.\n", territorios_conquistados, 3);
            return (territorios_conquistados >= 3);
            
        default:
            return 0; // Missão desconhecida
    }
}