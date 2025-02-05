#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Filme{
    char nome[50];
    char genero[30];
    char idioma[20];
    char horario[10];
};

typedef struct Filme Filme;

void menu(int* opcao){
    printf("=====================\n");
    printf("|| C I N E M A I S ||\n");
    printf(" || MONTES-CLAROS ||\n");
    printf("||B E M - V I N D O||\n");
    printf("=====================\n\n");
    printf("O QUE GOSTARIA DE FAZER?\n\n");
    printf("1. Incluir\n");
    printf("2. Listar\n");
    printf("3. Selecionar\n");
    printf("4. Modificar\n");
    printf("5. Exportar\n");
    printf("0. Sair\n\n");
    printf("QUAL OPCAO? ");
    scanf("%d", opcao);
}

void incluirFilme(FILE* arquivo){
    Filme filme;
    printf("Nome do filme: ");
    scanf(" %[^\n]", filme.nome);

    printf("Genero do filme: ");
    scanf(" %[^\n]", filme.genero);

    printf("Idioma do filme: ");
    scanf(" %[^\n]", filme.idioma);

    printf("Horario do filme: ");
    scanf(" %[^\n]", filme.horario);

    fseek(arquivo, 0, SEEK_END);
    fwrite(&filme, sizeof(Filme), 1, arquivo);
    fflush(arquivo);

    printf("\n\nO FILME FOI INCLUIDO!\n\n");
}

void listarFilmes(FILE* arquivo){
    Filme filme;

    rewind(arquivo);

    printf("\n\nRESULTADO DA LISTAGEM:\n\n");

    while (fread(&filme, sizeof(Filme), 1, arquivo) == 1){
        if (strlen(filme.nome) > 0) {
            printf("\nNome: %s\nGenero: %s\nIdioma: %s\nHorario: %s\n", filme.nome, filme.genero, filme.idioma, filme.horario);
        }
    }
}

void selecionarFilme(FILE *arquivo){
    Filme filme;
    char nome_pesquisa[30];
    int encontrados = 0;

    printf("Nome do filme: ");
    scanf(" %[^\n]", nome_pesquisa);

    rewind(arquivo);

    printf("\n\nRESULTADO DA PESQUISA:\n\n");

    while (fread(&filme, sizeof(Filme), 1, arquivo) == 1){
        if (strcmp(filme.nome, nome_pesquisa) == 0) {
            printf("Nome: %s\n", filme.nome);
            printf("Genero: %s\n", filme.genero);
            printf("Idioma: %s\n", filme.idioma);
            printf("Horario: %s\n", filme.horario);
            printf("\n");
            encontrados++;
        }
    }

    if (encontrados == 0){
        printf("\n\nNENHUM FILME COM O NOME '%s' FOI ENCONTRADO!\n\n", nome_pesquisa);
    } else {
        printf("\n\nFILMES ENCONTRADOS: %d\n\n", encontrados);
    }
}

void modificarFilme(FILE* arquivo){
    char nome[50];
    Filme filme;
    int encontrado = 0;
    long int posicao;

    printf("\n\nQUAL O NOME DO FILME PARA MODIFICAR? ");
    scanf(" %[^\n]", nome);

    rewind(arquivo);

    while (fread(&filme, sizeof(Filme), 1, arquivo) == 1) {
        if (strcmp(filme.nome, nome) == 0){
            printf("\nVOCE QUER: \n\n");
            printf("1. Modificar o filme\n");
            printf("\nOU\n\n");
            printf("2. Deletar o filme\n");
            printf("\n\nOPCAO: \n\n");
            int opcao;
            scanf("%d", &opcao);

            if (opcao == 1) {
                printf("Novo nome do filme: ");
                scanf(" %[^\n]", filme.nome);

                printf("Novo genero do filme: ");
                scanf(" %[^\n]", filme.genero);

                printf("Novo idioma do filme: ");
                scanf(" %[^\n]", filme.idioma);

                printf("Novo horario do filme: ");
                scanf(" %[^\n]", filme.horario);

                fseek(arquivo, -sizeof(Filme), SEEK_CUR);
                fwrite(&filme, sizeof(Filme), 1, arquivo);
                fflush(arquivo);

                printf("\n\nO FILME FOI MODIFICADO!\n\n");
            } else if (opcao == 2) {
                posicao = ftell(arquivo) - sizeof(Filme);
                fseek(arquivo, posicao, SEEK_SET);
                Filme filmeVazio = {""};
                fwrite(&filmeVazio, sizeof(Filme), 1, arquivo);
                fflush(arquivo);

                printf("\n\nO FILME FOI DELETADO!\n\n");
            } else {
                printf("\n\nOPÇÃO INVÁLIDA!\n\n");
            }

            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\n\nNENHUM FILME COM ESSE NOME FOI ENCONTRADO!\n\n");
    }
}

void exportarFilmes(FILE* arquivo){
    FILE* arquivoCSV = fopen("filmes.csv", "w");
    Filme filme;

    rewind(arquivo);

    fprintf(arquivoCSV, "Nome,Genero,Idioma,Horario\n");
    while (fread(&filme, sizeof(Filme), 1, arquivo) == 1) {
        fprintf(arquivoCSV, "%s,%s,%s,%s\n", filme.nome, filme.genero, filme.idioma, filme.horario);
    }

    fclose(arquivoCSV);
    printf("\n\nO ARQUIVO FOI EXPORTADO!\n\n");
}

void executarOpcao(int opcao, FILE* arquivo){
    switch (opcao) {
        case 1: incluirFilme(arquivo); break;

        case 2: listarFilmes(arquivo); break;

        case 3: selecionarFilme(arquivo); break;

        case 4: modificarFilme(arquivo); break;

        case 5: exportarFilmes(arquivo); break;

        case 0: printf("Saindo...\n"); exit(0);

        default: printf("\n\nOPCAO INVALIDA!\n\n");
    }
}

int main(){
    FILE* arquivo;
    int opcao;
    arquivo = fopen("cinemais.bin", "rb+");

    if (arquivo == NULL){
        arquivo = fopen("cinemais.bin", "wb+");
    }
    while (1) {
        menu(&opcao);
        executarOpcao(opcao, arquivo);
    }
    fclose(arquivo);
    return 0;
}
