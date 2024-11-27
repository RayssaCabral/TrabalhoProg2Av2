#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "cidades.txt"

typedef struct {
    int codigo;
    char nome[50];
    char estado[30];
    int populacao;
} Cidade;

void incluirCidade() {
    FILE *arquivo = fopen(ARQUIVO, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Cidade cidade;
    printf("Digite o código da cidade: ");
    scanf("%d", &cidade.codigo);
    printf("Digite o nome da cidade: ");
    scanf(" %[^\n]", cidade.nome);
    printf("Digite o estado da cidade: ");
    scanf(" %[^\n]", cidade.estado);
    printf("Digite a populacao da cidade: ");
    scanf("%d", &cidade.populacao);

    fwrite(&cidade, sizeof(Cidade), 1, arquivo);
    fclose(arquivo);
    printf("Cidade incluída com sucesso!\n");
}

void alterarCidade() {
    FILE *arquivo = fopen(ARQUIVO, "r+b");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int codigo;
    printf("Digite o codigo da cidade que deseja alterar: ");
    scanf("%d", &codigo);

    Cidade cidade;
    int encontrado = 0;

    while (fread(&cidade, sizeof(Cidade), 1, arquivo)) {
        if (cidade.codigo == codigo) {
            encontrado = 1;

            printf("Digite o novo nome da cidade: ");
            scanf(" %[^\n]", cidade.nome);
            printf("Digite o novo estado da cidade: ");
            scanf(" %[^\n]", cidade.estado);
            printf("Digite a nova populacao da cidade: ");
            scanf("%d", &cidade.populacao);

            fseek(arquivo, -sizeof(Cidade), SEEK_CUR);
            fwrite(&cidade, sizeof(Cidade), 1, arquivo);
            printf("Cidade alterada com sucesso!\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Cidade com codigo %d não encontrada.\n", codigo);
    }

    fclose(arquivo);
}

void excluirCidade() {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "wb");
    if (temp == NULL) {
        printf("Erro ao criar o arquivo temporario.\n");
        fclose(arquivo);
        return;
    }

    int codigo;
    printf("Digite o codigo da cidade que deseja excluir: ");
    scanf("%d", &codigo);

    Cidade cidade;
    int encontrado = 0;

    while (fread(&cidade, sizeof(Cidade), 1, arquivo)) {
        if (cidade.codigo == codigo) {
            encontrado = 1;
            continue;
        }
        fwrite(&cidade, sizeof(Cidade), 1, temp);
    }

    fclose(arquivo);
    fclose(temp);

    remove(ARQUIVO);
    rename("temp.txt", ARQUIVO);

    if (encontrado) {
        printf("Cidade excluida com sucesso!\n");
    } else {
        printf("Cidade com codigo %d não encontrada.\n", codigo);
    }
}

void consultarCidade() {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int codigo;
    printf("Digite o codigo da cidade que deseja consultar: ");
    scanf("%d", &codigo);

    Cidade cidade;
    int encontrado = 0;

    while (fread(&cidade, sizeof(Cidade), 1, arquivo)) {
        if (cidade.codigo == codigo) {
            encontrado = 1;
            printf("Codigo: %d\n", cidade.codigo);
            printf("Nome: %s\n", cidade.nome);
            printf("Estado: %s\n", cidade.estado);
            printf("Populacao: %d\n", cidade.populacao);
            break;
        }
    }

    if (!encontrado) {
        printf("Cidade com codigo %d não encontrada.\n", codigo);
    }

    fclose(arquivo);
}

void listarCidades() {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Cidade cidade;
    printf("Cidades cadastradas:\n");
    while (fread(&cidade, sizeof(Cidade), 1, arquivo)) {
        printf("Codigo: %d, Nome: %s, Estado: %s, Populacao: %d\n",
               cidade.codigo, cidade.nome, cidade.estado, cidade.populacao);
    }

    fclose(arquivo);
}

int main() {
    int opcao;

    do {
        printf("\n--- Cadastro de Cidades ---\n");
        printf("1. Incluir cidade\n");
        printf("2. Alterar dados\n");
        printf("3. Excluir cidade\n");
        printf("4. Consultar cidade\n");
        printf("5. Listar cidades\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                incluirCidade();
                break;
            case 2:
                alterarCidade();
                break;
            case 3:
                excluirCidade();
                break;
            case 4:
                consultarCidade();
                break;
            case 5:
                listarCidades();
                break;
            case 0:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
