#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char titulo[50];
    char genero[20];
    int ano;
} Filme;

void adicionarFilme();
void alterarFilme();
void excluirFilme();
void consultarFilme();
void listarFilmes();

const char *arquivo = "filmes.dat";

int main() {
    int opcao;

    do {
        printf("\nControle de Filmes\n");
        printf("1. Adicionar Filme\n");
        printf("2. Alterar Filme\n");
        printf("3. Excluir Filme\n");
        printf("4. Consultar Filme\n");
        printf("5. Listar Todos os Filmes\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Consumir o '\n' deixado pelo scanf

        switch (opcao) {
            case 1: adicionarFilme(); break;
            case 2: alterarFilme(); break;
            case 3: excluirFilme(); break;
            case 4: consultarFilme(); break;
            case 5: listarFilmes(); break;
            case 0: printf("Encerrando o programa...\n"); break;
            default: printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

void adicionarFilme() {
    FILE *fp = fopen(arquivo, "ab");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    Filme filme;
    printf("ID: ");
    scanf("%d", &filme.id);
    getchar();
    printf("Titulo: ");
    fgets(filme.titulo, sizeof(filme.titulo), stdin);
    filme.titulo[strcspn(filme.titulo, "\n")] = 0; // Remover o '\n'
    printf("Genero: ");
    fgets(filme.genero, sizeof(filme.genero), stdin);
    filme.genero[strcspn(filme.genero, "\n")] = 0;
    printf("Ano: ");
    scanf("%d", &filme.ano);

    fwrite(&filme, sizeof(Filme), 1, fp);
    fclose(fp);
    printf("Filme adicionado com sucesso!\n");
}

void alterarFilme() {
    FILE *fp = fopen(arquivo, "r+b");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int id;
    printf("Informe o ID do filme a ser alterado: ");
    scanf("%d", &id);
    getchar();

    Filme filme;
    int encontrado = 0;

    while (fread(&filme, sizeof(Filme), 1, fp)) {
        if (filme.id == id) {
            encontrado = 1;
            printf("Novo titulo: ");
            fgets(filme.titulo, sizeof(filme.titulo), stdin);
            filme.titulo[strcspn(filme.titulo, "\n")] = 0;
            printf("Novo genero: ");
            fgets(filme.genero, sizeof(filme.genero), stdin);
            filme.genero[strcspn(filme.genero, "\n")] = 0;
            printf("Novo ano: ");
            scanf("%d", &filme.ano);

            fseek(fp, -sizeof(Filme), SEEK_CUR);
            fwrite(&filme, sizeof(Filme), 1, fp);
            printf("Filme alterado com sucesso!\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Filme com ID %d não encontrado.\n", id);
    }

    fclose(fp);
}

void excluirFilme() {
    FILE *fp = fopen(arquivo, "rb");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) {
        perror("Erro ao criar arquivo temporario");
        fclose(fp);
        return;
    }

    int id;
    printf("Informe o ID do filme a ser excluido: ");
    scanf("%d", &id);

    Filme filme;
    int encontrado = 0;

    while (fread(&filme, sizeof(Filme), 1, fp)) {
        if (filme.id == id) {
            encontrado = 1;
            continue;
        }
        fwrite(&filme, sizeof(Filme), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(arquivo);
    rename("temp.dat", arquivo);

    if (encontrado) {
        printf("Filme excluído com sucesso!\n");
    } else {
        printf("Filme com ID %d não encontrado.\n", id);
    }
}

void consultarFilme() {
    FILE *fp = fopen(arquivo, "rb");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int id;
    printf("Informe o ID do filme a ser consultado: ");
    scanf("%d", &id);

    Filme filme;
    int encontrado = 0;

    while (fread(&filme, sizeof(Filme), 1, fp)) {
        if (filme.id == id) {
            printf("\nID: %d\nTitulo: %s\nGenero: %s\nAno: %d\n", filme.id, filme.titulo, filme.genero, filme.ano);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Filme com ID %d nao encontrado.\n", id);
    }

    fclose(fp);
}

void listarFilmes() {
    FILE *fp = fopen(arquivo, "rb");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    Filme filme;
    printf("\nLista de Filmes:\n");

    while (fread(&filme, sizeof(Filme), 1, fp)) {
        printf("ID: %d\nTitulo: %s\nGenero: %s\nAno: %d\n\n", filme.id, filme.titulo, filme.genero, filme.ano);
    }

    fclose(fp);
}
