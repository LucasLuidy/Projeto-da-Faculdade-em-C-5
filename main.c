#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Docente {
    int identificador;
    char nome[50];
    char area_atuacao[50];
    char departamento[50];
} Docente;

typedef struct {
    int identificador;
    char nome[50];
    char ementa[100];
    struct Docente *docente;
    int carga_horaria;
} Disciplina;

int calcularCargaHoraria(Disciplina *disciplinas, int tamanho, Docente *docente) {
    if (tamanho == 0) {
        return 0;
    }
    
    int carga = calcularCargaHoraria(disciplinas, tamanho - 1, docente);
    if (disciplinas[tamanho - 1].docente == docente) {
        carga += disciplinas[tamanho - 1].carga_horaria;
    }
    
    return carga;
}

Docente *pesquisarDocentePorNome(Docente *docentes, int tamanho, const char *nome) {
    if (tamanho == 0) {
        return NULL;
    }
    
    if (strcmp(docentes[tamanho - 1].nome, nome) == 0) {
        return &docentes[tamanho - 1];
    }
    
    return pesquisarDocentePorNome(docentes, tamanho - 1, nome);
}

void exibirDisciplinas(Disciplina *disciplinas, int numDisciplinas) {
    printf("\n-----Lista de disciplinas-----\n");
    for (int i = 0; i < numDisciplinas; i++) {
        printf("Nome: %s.\n", disciplinas[i].nome);
        printf("Ementa: %s.\n", disciplinas[i].ementa);
        printf("Carga horária: %d.\n", disciplinas[i].carga_horaria);
        printf("Docente responsável: %s.\n\n", disciplinas[i].docente->nome);
    }
}

void exibirDocentes(Docente *docentes, int numDocentes) {
    printf("\n-----Lista de docentes-----\n");
    for (int i = 0; i < numDocentes; i++) {
        printf("Docente: %d.\n", i + 1);
        printf("Nome: %s.\n", docentes[i].nome);
        printf("Área de atuação: %s.\n", docentes[i].area_atuacao);
        printf("Departamento: %s.\n\n", docentes[i].departamento);
    }
}

void gravarDados(Disciplina *disciplinas, int numDisciplinas, Docente *docentes, int numDocentes) {
    FILE *arquivo = fopen("dados.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return;
    }
    
    fprintf(arquivo, "%d\n", numDocentes);
    for (int i = 0; i < numDocentes; i++) {
        fprintf(arquivo, "%d %s %s %s\n", docentes[i].identificador, docentes[i].nome, docentes[i].area_atuacao, docentes[i].departamento);
    }
    
    fprintf(arquivo, "%d\n", numDisciplinas);
    for (int i = 0; i < numDisciplinas; i++) {
        fprintf(arquivo, "%d %s %s %d %d\n", disciplinas[i].identificador, disciplinas[i].nome, disciplinas[i].ementa,
                disciplinas[i].docente->identificador, disciplinas[i].carga_horaria);
    }

    fclose(arquivo);
}

void carregarDados(Disciplina **disciplinas, int *numDisciplinas, Docente **docentes, int *numDocentes) {
    FILE *arquivo = fopen("dados.txt", "r");
    if (arquivo == NULL) {
        printf("Arquivo de dados não encontrado.\n");
        return;
    }
    
    fscanf(arquivo, "%d", numDocentes);
    *docentes = (Docente *)malloc(*numDocentes * sizeof(Docente));
    for (int i = 0; i < *numDocentes; i++) {
        fscanf(arquivo, "%d %s %s %s", &(*docentes)[i].identificador, (*docentes)[i].nome, (*docentes)[i].area_atuacao, (*docentes)[i].departamento);
    }
    
    fscanf(arquivo, "%d", numDisciplinas);
    *disciplinas = (Disciplina *)malloc(*numDisciplinas * sizeof(Disciplina));
    for (int i = 0; i < *numDisciplinas; i++) {
        int id_docente;
        fscanf(arquivo, "%d %s %s %d %d", &(*disciplinas)[i].identificador, (*disciplinas)[i].nome, (*disciplinas)[i].ementa,
               &id_docente, &(*disciplinas)[i].carga_horaria);
        (*disciplinas)[i].docente = &(*docentes)[id_docente - 1];
    }

    fclose(arquivo);
}

int main() {
    Disciplina *disciplinas = NULL;
    Docente *docentes = NULL;
    int numDisciplinas = 0;
    int numDocentes = 0;
    int escolha;
    
    carregarDados(&disciplinas, &numDisciplinas, &docentes, &numDocentes);

    do {
        printf("\n[1] Inserir nova disciplina\n");
        printf("[2] Inserir novo docente\n");
        printf("[3] Exibir lista de disciplinas\n");
        printf("[4] Exibir lista de docentes\n");
        printf("[5] Pesquisar docente pelo nome\n");
        printf("[6] Gravar dados em arquivo\n");
        printf("[7] Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);
        
        switch (escolha) {
            case 1:
                if (numDocentes == 0) {
                    printf("Erro: Insira pelo menos um docente antes de criar uma disciplina.\n");
                } else {
                    printf("Escolha o docente responsável:\n");
                    for (int i = 0; i < numDocentes; i++) {
                        printf("[%d] %s\n", i + 1, docentes[i].nome);
                    }
                    int escolha_docente;
                    printf("Selecione um docente: ");
                    scanf("%d", &escolha_docente);
                    if (escolha_docente >= 1 && escolha_docente <= numDocentes) {
                        Disciplina nova_disciplina;
                        nova_disciplina.identificador = numDisciplinas + 1;
                        printf("Digite o nome da disciplina: ");
                        scanf("%s", nova_disciplina.nome);
                        printf("Digite a ementa da disciplina: ");
                        scanf("%s", nova_disciplina.ementa);
                        nova_disciplina.docente = &docentes[escolha_docente - 1];
                        printf("Digite a carga horária da disciplina: ");
                        scanf("%d", &nova_disciplina.carga_horaria);
                        
                        disciplinas = realloc(disciplinas, (numDisciplinas + 1) * sizeof(Disciplina));
                        disciplinas[numDisciplinas] = nova_disciplina;
                        numDisciplinas++;
                    } else {
                        printf("Escolha de docente inválida.\n");
                    }
                }
                break;
            case 2:
                Docente novo_docente;
                novo_docente.identificador = numDocentes + 1;
                printf("\nDigite o nome do docente: ");
                scanf("%s", novo_docente.nome);
                printf("Digite a área de atuação do docente: ");
                scanf("%s", novo_docente.area_atuacao);
                printf("Digite o departamento do docente: ");
                scanf("%s", novo_docente.departamento);
                
                docentes = realloc(docentes, (numDocentes + 1) * sizeof(Docente));
                docentes[numDocentes] = novo_docente;
                numDocentes++;
                break;
            case 3:
                exibirDisciplinas(disciplinas, numDisciplinas);
                break;
            case 4:
                exibirDocentes(docentes, numDocentes);
                break;
            case 5:
                printf("Digite o nome do docente para pesquisa: ");
                char nome_pesquisa[50];
                scanf("%s", nome_pesquisa);
                Docente *docente_encontrado = pesquisarDocentePorNome(docentes, numDocentes, nome_pesquisa);
                if (docente_encontrado) {
                    printf("Docente encontrado: %s\n", docente_encontrado->nome);
                    int carga_total = calcularCargaHoraria(disciplinas, numDisciplinas, docente_encontrado);
                    printf("Carga horária total: %d\n", carga_total);
                } else {
                    printf("Docente não encontrado.\n");
                }
                break;
            case 6:
                gravarDados(disciplinas, numDisciplinas, docentes, numDocentes);
                printf("Dados gravados em arquivo.\n");
                break;
            case 7:
                
                free(disciplinas);
                free(docentes);
                exit(0);
            default:
                printf("Opção inválida!\n");
        }
    } while (escolha != 7);

    return 0;
}