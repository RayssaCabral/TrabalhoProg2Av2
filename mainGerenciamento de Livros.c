#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "catalogo_livros.dat"

// Estrutura para armazenar as informações de cada livro
typedef struct {
    int codigo;
    char titulo[50];
    char autor[50];
    int ano;
} Livro;

// Funções do programa
void adicionarLivro();
void alterarLivro();
void excluirLivro();
void consultarLivro();
void listarLivros();

int main() {
    int opcao;
    do {
        printf("\n--- Gerenciamento de Livros ---\n");
        printf("1. Adicionar livro\n");
        printf("2. Alterar livro\n");
        printf("3. Excluir livro\n");
        printf("4. Consultar livro\n");
        printf("5. Listar todos os livros\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Consumir o '\n' deixado pelo scanf

        switch (opcao) {
            case 1: adicionarLivro(); break;
            case 2: alterarLivro(); break;
            case 3: excluirLivro(); break;
            case 4: consultarLivro(); break;
            case 5: listarLivros(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

// Adicionar um novo livro ao catálogo
void adicionarLivro() {
    Livro livro;
    FILE *arquivo = fopen(ARQUIVO, "ab");

    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    printf("Codigo: ");
    scanf("%d", &livro.codigo);
    getchar(); // Consumir o '\n'
    printf("Titulo: ");
    fgets(livro.titulo, sizeof(livro.titulo), stdin);
    livro.titulo[strcspn(livro.titulo, "\n")] = '\0'; // Remover o '\n'
    printf("Autor: ");
    fgets(livro.autor, sizeof(livro.autor), stdin);
    livro.autor[strcspn(livro.autor, "\n")] = '\0';
    printf("Ano: ");
    scanf("%d", &livro.ano);

    fwrite(&livro, sizeof(Livro), 1, arquivo);
    fclose(arquivo);
    printf("Livro adicionado com sucesso!\n");
}

// Alterar os dados de um livro
void alterarLivro() {
    int codigo;
    Livro livro;
    FILE *arquivo = fopen(ARQUIVO, "r+b");
    int encontrado = 0;

    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    printf("Informe o codigo do livro a alterar: ");
    scanf("%d", &codigo);
    getchar();

    while (fread(&livro, sizeof(Livro), 1, arquivo)) {
        if (livro.codigo == codigo) {
            encontrado = 1;
            printf("Novo titulo: ");
            fgets(livro.titulo, sizeof(livro.titulo), stdin);
            livro.titulo[strcspn(livro.titulo, "\n")] = '\0';
            printf("Novo autor: ");
            fgets(livro.autor, sizeof(livro.autor), stdin);
            livro.autor[strcspn(livro.autor, "\n")] = '\0';
            printf("Novo ano: ");
            scanf("%d", &livro.ano);

            fseek(arquivo, -sizeof(Livro), SEEK_CUR);
            fwrite(&livro, sizeof(Livro), 1, arquivo);
            printf("Livro atualizado com sucesso!\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Livro com codigo %d nao encontrado.\n", codigo);
    }

    fclose(arquivo);
}

// Excluir um livro pelo código
void excluirLivro() {
    int codigo;
    Livro livro;
    FILE *arquivo = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    int encontrado = 0;

    if (!arquivo || !temp) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    printf("Informe o codigo do livro a excluir: ");
    scanf("%d", &codigo);

    while (fread(&livro, sizeof(Livro), 1, arquivo)) {
        if (livro.codigo == codigo) {
            encontrado = 1;
        } else {
            fwrite(&livro, sizeof(Livro), 1, temp);
        }
    }

    fclose(arquivo);
    fclose(temp);

    remove(ARQUIVO);
    rename("temp.dat", ARQUIVO);

    if (encontrado) {
        printf("Livro excluido com sucesso!\n");
    } else {
        printf("Livro com codigo %d nao encontrado.\n", codigo);
    }
}

// Consultar informações de um livro pelo código
void consultarLivro() {
    int codigo;
    Livro livro;
    FILE *arquivo = fopen(ARQUIVO, "rb");
    int encontrado = 0;

    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    printf("Informe o codigo do livro a consultar: ");
    scanf("%d", &codigo);

    while (fread(&livro, sizeof(Livro), 1, arquivo)) {
        if (livro.codigo == codigo) {
            encontrado = 1;
            printf("\n--- Livro encontrado ---\n");
            printf("Codigo: %d\n", livro.codigo);
            printf("Titulo: %s\n", livro.titulo);
            printf("Autor: %s\n", livro.autor);
            printf("Ano: %d\n", livro.ano);
            break;
        }
    }

    if (!encontrado) {
        printf("Livro com codigo %d nao encontrado.\n", codigo);
    }

    fclose(arquivo);
}

// Listar todos os livros cadastrados
void listarLivros() {
    Livro livro;
    FILE *arquivo = fopen(ARQUIVO, "rb");

    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    printf("\n--- Livros cadastrados ---\n");
    while (fread(&livro, sizeof(Livro), 1, arquivo)) {
        printf("\nCodigo: %d\n", livro.codigo);
        printf("Titulo: %s\n", livro.titulo);
        printf("Autor: %s\n", livro.autor);
        printf("Ano: %d\n", livro.ano);
    }

    fclose(arquivo);
}
