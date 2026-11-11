#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ACOES 100
#define TAM_TEXTO 100
#define TAM_DATA 15

// Status padronizados conforme edital
#define STATUS_PLANEJADA 1
#define STATUS_REALIZADA 2
#define STATUS_CANCELADA 3

typedef struct {
    int codigo;
    char escola[TAM_TEXTO];
    char tema[TAM_TEXTO];
    char dataPrevista[TAM_DATA];
    char publicoAlvo[TAM_TEXTO];
    char responsavel[TAM_TEXTO];
    int qtdPrevista;
    int qtdEfetiva;
    int situacao; // 1 = Planejada, 2 = Realizada, 3 = Cancelada
} AcaoPSE;

// Protótipos das funções (Modularização)
void limparBuffer();
void lerTextoObrigatorio(const char *mensagem, char *destino, int tamanho);
int lerInteiroPositivo(const char *mensagem);
int codigoJaExiste(AcaoPSE lista[], int total, int codigo);
int buscarIndicePorCodigo(AcaoPSE lista[], int total, int codigo);
int contemSubstringIgnoreCase(const char *texto, const char *busca);
const char* obterNomeSituacao(int situacao);

void exibirMenu();
void cadastrarAcao(AcaoPSE lista[], int *total);
void listarAcoes(AcaoPSE lista[], int total);
void exibirDetalhesAcao(AcaoPSE a);
void pesquisarAcoes(AcaoPSE lista[], int total);
void atualizarSituacao(AcaoPSE lista[], int total);
void gerarResumoGeral(AcaoPSE lista[], int total);

int main() {
    AcaoPSE listaAcoes[MAX_ACOES];
    int totalAcoes = 0;
    int opcao = -1;

    do {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Entrada invalida! Digite apenas numeros.\n");
            limparBuffer();
            opcao = -1;
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrarAcao(listaAcoes, &totalAcoes);
                break;
            case 2:
                listarAcoes(listaAcoes, totalAcoes);
                break;
            case 3:
                pesquisarAcoes(listaAcoes, totalAcoes);
                break;
            case 4:
                atualizarSituacao(listaAcoes, totalAcoes);
                break;
            case 5:
                gerarResumoGeral(listaAcoes, totalAcoes);
                break;
            case 0:
                printf("\n======================================================\n");
                printf(" Encerrando o Sistema IntegraPSE. Ate logo!\n");
                printf("======================================================\n");
                break;
            default:
                printf("\n[ERRO] Opcao inexistente no menu! Escolha entre 0 e 5.\n");
        }
    } while (opcao != 0);

    return 0;
}

// Funções Auxiliares e de Validação
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lerTextoObrigatorio(const char *mensagem, char *destino, int tamanho) {
    int valido = 0;
    do {
        printf("%s", mensagem);
        if (fgets(destino, tamanho, stdin) != NULL) {
            size_t len = strlen(destino);
            if (len > 0 && destino[len - 1] == '\n') {
                destino[len - 1] = '\0';
            } else {
                limparBuffer();
            }
            // Verifica se não está vazio ou só com espaços
            valido = 0;
            for (size_t i = 0; i < strlen(destino); i++) {
                if (!isspace((unsigned char)destino[i])) {
                    valido = 1;
                    break;
                }
            }
            if (!valido) {
                printf("[ERRO] Este campo e obrigatorio e nao pode ficar vazio!\n");
            }
        }
    } while (!valido);
}

int lerInteiroPositivo(const char *mensagem) {
    int valor;
    int valido = 0;
    do {
        printf("%s", mensagem);
        if (scanf("%d", &valor) != 1) {
            printf("[ERRO] Digite um numero inteiro valido!\n");
            limparBuffer();
        } else {
            limparBuffer();
            if (valor <= 0) {
                printf("[ERRO] O valor deve ser maior que zero (nao sao aceitas quantidades negativas ou nulas)!\n");
            } else {
                valido = 1;
            }
        }
    } while (!valido);
    return valor;
}

int codigoJaExiste(AcaoPSE lista[], int total, int codigo) {
    for (int i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return 1;
        }
    }
    return 0;
}

int buscarIndicePorCodigo(AcaoPSE lista[], int total, int codigo) {
    for (int i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

int contemSubstringIgnoreCase(const char *texto, const char *busca) {
    char t[TAM_TEXTO], b[TAM_TEXTO];
    strncpy(t, texto, TAM_TEXTO - 1); t[TAM_TEXTO - 1] = '\0';
    strncpy(b, busca, TAM_TEXTO - 1); b[TAM_TEXTO - 1] = '\0';

    for (int i = 0; t[i]; i++) t[i] = tolower((unsigned char)t[i]);
    for (int i = 0; b[i]; i++) b[i] = tolower((unsigned char)b[i]);

    return (strstr(t, b) != NULL);
}

const char* obterNomeSituacao(int situacao) {
    switch (situacao) {
        case STATUS_PLANEJADA: return "Planejada";
        case STATUS_REALIZADA: return "Realizada";
        case STATUS_CANCELADA: return "Cancelada";
        default: return "Desconhecida";
    }
}

void exibirMenu() {
    printf("\n======================================================\n");
    printf("   PSE EM ACAO - SISTEMA DE GESTAO INTEGRAPSE\n");
    printf("======================================================\n");
    printf(" 1 - Cadastrar acao do PSE\n");
    printf(" 2 - Listar todas as acoes cadastradas\n");
    printf(" 3 - Pesquisar acoes (por Codigo, Escola ou Tema)\n");
    printf(" 4 - Atualizar situacao de uma acao\n");
    printf(" 5 - Gerar resumo geral e estatisticas\n");
    printf(" 0 - Encerrar o programa\n");
    printf("======================================================\n");
    printf(" Escolha uma opcao: ");
}

// 1. Funcionalidade: Cadastrar Ação
void cadastrarAcao(AcaoPSE lista[], int *total) {
    if (*total >= MAX_ACOES) {
        printf("\n[ALERTA] Limite maximo de armazenamento (%d acoes) atingido!\n", MAX_ACOES);
        return;
    }

    AcaoPSE nova;
    printf("\n--- 1. CADASTRO DE NOVA ACAO DO PSE ---\n");

    int codigoValido = 0;
    do {
        nova.codigo = lerInteiroPositivo("Digite o codigo da acao (ex: 101): ");
        if (codigoJaExiste(lista, *total, nova.codigo)) {
            printf("[ERRO] Ja existe uma acao cadastrada com o codigo %d! Informe outro.\n", nova.codigo);
        } else {
            codigoValido = 1;
        }
    } while (!codigoValido);

    lerTextoObrigatorio("Nome da Escola: ", nova.escola, TAM_TEXTO);
    lerTextoObrigatorio("Tema da Acao (ex: Saude Bucal, Vacinacao, Saude Mental): ", nova.tema, TAM_TEXTO);
    lerTextoObrigatorio("Data Prevista (DD/MM/AAAA): ", nova.dataPrevista, TAM_DATA);
    lerTextoObrigatorio("Publico-Alvo (ex: 5o Ano B / Ensino Fundamental): ", nova.publicoAlvo, TAM_TEXTO);
    lerTextoObrigatorio("Profissional/Equipe Responsavel: ", nova.responsavel, TAM_TEXTO);
    nova.qtdPrevista = lerInteiroPositivo("Quantidade prevista de participantes: ");

    printf("Situacao inicial (1 - Planejada | 2 - Realizada | 3 - Cancelada): ");
    int sit;
    while (scanf("%d", &sit) != 1 || sit < 1 || sit > 3) {
        printf("[ERRO] Opcao invalida! Digite 1 (Planejada), 2 (Realizada) ou 3 (Cancelada): ");
        limparBuffer();
    }
    limparBuffer();
    nova.situacao = sit;

    if (nova.situacao == STATUS_REALIZADA) {
        int qtdEf;
        do {
            printf("Digite a quantidade efetiva de participantes (>= 0): ");
            if (scanf("%d", &qtdEf) != 1 || qtdEf < 0) {
                printf("[ERRO] Digite uma quantidade valida (maior ou igual a zero)!\n");
                limparBuffer();
                qtdEf = -1;
            } else {
                limparBuffer();
            }
        } while (qtdEf < 0);
        nova.qtdEfetiva = qtdEf;
    } else {
        nova.qtdEfetiva = 0;
    }

    lista[*total] = nova;
    (*total)++;

    printf("\n[SUCESSO] Acao codigo %d cadastrada com sucesso!\n", nova.codigo);
}

void exibirDetalhesAcao(AcaoPSE a) {
    printf("------------------------------------------------------\n");
    printf(" Codigo: %d | Situacao: %s\n", a.codigo, obterNomeSituacao(a.situacao));
    printf(" Escola: %s\n", a.escola);
    printf(" Tema: %s | Data Prevista: %s\n", a.tema, a.dataPrevista);
    printf(" Publico-Alvo: %s\n", a.publicoAlvo);
    printf(" Responsavel: %s\n", a.responsavel);
    printf(" Participantes Previstos: %d", a.qtdPrevista);
    if (a.situacao == STATUS_REALIZADA) {
        printf(" | Participantes Efetivos: %d\n", a.qtdEfetiva);
    } else {
        printf(" | Participantes Efetivos: N/A (Acao nao realizada)\n");
    }
}

// 2. Funcionalidade: Listar Ações
void listarAcoes(AcaoPSE lista[], int total) {
    printf("\n--- 2. LISTA DE TODAS AS ACOES CADASTRADAS ---\n");
    if (total == 0) {
        printf("[INFO] Nenhuma acao cadastrada no sistema ate o momento.\n");
        return;
    }
    for (int i = 0; i < total; i++) {
        exibirDetalhesAcao(lista[i]);
    }
    printf("------------------------------------------------------\n");
    printf(" Total de registros listados: %d\n", total);
}

// 3. Funcionalidade: Pesquisar Ações
void pesquisarAcoes(AcaoPSE lista[], int total) {
    printf("\n--- 3. PESQUISAR ACOES DO PSE ---\n");
    if (total == 0) {
        printf("[INFO] Nao ha acoes cadastradas para pesquisar.\n");
        return;
    }

    printf("Pesquisar por:\n");
    printf(" 1 - Codigo\n");
    printf(" 2 - Escola\n");
    printf(" 3 - Tema\n");
    printf("Escolha o criterio: ");

    int criterio;
    if (scanf("%d", &criterio) != 1 || criterio < 1 || criterio > 3) {
        printf("[ERRO] Criterio de pesquisa invalido!\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    int encontrados = 0;
    if (criterio == 1) {
        int cod = lerInteiroPositivo("Digite o codigo a pesquisar: ");
        int idx = buscarIndicePorCodigo(lista, total, cod);
        if (idx != -1) {
            exibirDetalhesAcao(lista[idx]);
            encontrados = 1;
        }
    } else if (criterio == 2) {
        char termo[TAM_TEXTO];
        lerTextoObrigatorio("Digite o nome (ou parte do nome) da Escola: ", termo, TAM_TEXTO);
        for (int i = 0; i < total; i++) {
            if (contemSubstringIgnoreCase(lista[i].escola, termo)) {
                exibirDetalhesAcao(lista[i]);
                encontrados++;
            }
        }
    } else if (criterio == 3) {
        char termo[TAM_TEXTO];
        lerTextoObrigatorio("Digite o Tema (ex: Vacinacao, Saude Bucal): ", termo, TAM_TEXTO);
        for (int i = 0; i < total; i++) {
            if (contemSubstringIgnoreCase(lista[i].tema, termo)) {
                exibirDetalhesAcao(lista[i]);
                encontrados++;
            }
        }
    }

    if (encontrados == 0) {
        printf("\n[INFO] Nenhum registro encontrado para o criterio informado.\n");
    } else {
        printf("------------------------------------------------------\n");
        printf(" Total de registros encontrados: %d\n", encontrados);
    }
}

// 4. Funcionalidade: Atualizar Situação
void atualizarSituacao(AcaoPSE lista[], int total) {
    printf("\n--- 4. ATUALIZAR SITUACAO DE UMA ACAO ---\n");
    if (total == 0) {
        printf("[INFO] Nao ha acoes cadastradas no sistema.\n");
        return;
    }

    int cod = lerInteiroPositivo("Informe o codigo da acao que deseja atualizar: ");
    int idx = buscarIndicePorCodigo(lista, total, cod);

    if (idx == -1) {
        printf("[ERRO] Acao com codigo %d nao encontrada!\n", cod);
        return;
    }

    printf("\nRegistro localizado:\n");
    exibirDetalhesAcao(lista[idx]);

    printf("\nEscolha a nova situacao:\n");
    printf(" 1 - Planejada\n");
    printf(" 2 - Realizada\n");
    printf(" 3 - Cancelada\n");
    printf("Nova situacao: ");

    int novaSit;
    while (scanf("%d", &novaSit) != 1 || novaSit < 1 || novaSit > 3) {
        printf("[ERRO] Opcao invalida! Digite 1, 2 ou 3: ");
        limparBuffer();
    }
    limparBuffer();

    lista[idx].situacao = novaSit;

    if (novaSit == STATUS_REALIZADA) {
        int qtdEf;
        do {
            printf("Informe a quantidade efetiva de participantes atendidos (>= 0): ");
            if (scanf("%d", &qtdEf) != 1 || qtdEf < 0) {
                printf("[ERRO] Quantidade invalida! Digite um numero maior ou igual a zero.\n");
                limparBuffer();
                qtdEf = -1;
            } else {
                limparBuffer();
            }
        } while (qtdEf < 0);
        lista[idx].qtdEfetiva = qtdEf;
    } else {
        lista[idx].qtdEfetiva = 0;
    }

    printf("\n[SUCESSO] Situacao da acao %d atualizada para '%s'!\n", cod, obterNomeSituacao(novaSit));
}

// 5. Funcionalidade: Gerar Resumo Geral
void gerarResumoGeral(AcaoPSE lista[], int total) {
    printf("\n--- 5. RESUMO GERAL DAS ACOES DO PSE ---\n");
    if (total == 0) {
        printf("[INFO] Nenhuma acao cadastrada para gerar estatisticas.\n");
        return;
    }

    int qtdPlanejadas = 0, qtdRealizadas = 0, qtdCanceladas = 0;
    int totalPrevistosGeral = 0;
    int totalEfetivosRealizadas = 0;

    for (int i = 0; i < total; i++) {
        totalPrevistosGeral += lista[i].qtdPrevista;

        if (lista[i].situacao == STATUS_PLANEJADA) {
            qtdPlanejadas++;
        } else if (lista[i].situacao == STATUS_REALIZADA) {
            qtdRealizadas++;
            totalEfetivosRealizadas += lista[i].qtdEfetiva;
        } else if (lista[i].situacao == STATUS_CANCELADA) {
            qtdCanceladas++;
        }
    }

    double percentualParticipacao = 0.0;
    if (totalPrevistosGeral > 0) {
        percentualParticipacao = ((double)totalEfetivosRealizadas / totalPrevistosGeral) * 100.0;
    }

    printf("======================================================\n");
    printf(" Total de Acoes Cadastradas : %d\n", total);
    printf("  - Acoes Planejadas        : %d\n", qtdPlanejadas);
    printf("  - Acoes Realizadas        : %d\n", qtdRealizadas);
    printf("  - Acoes Canceladas        : %d\n", qtdCanceladas);
    printf("------------------------------------------------------\n");
    printf(" Total de Participantes Previstos : %d\n", totalPrevistosGeral);
    printf(" Total Efetivo de Participantes   : %d\n", totalEfetivosRealizadas);
    printf(" Percentual de Participacao       : %.2f%%\n", percentualParticipacao);
    printf("======================================================\n");
}
