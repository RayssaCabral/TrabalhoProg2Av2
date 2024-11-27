#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura do veículo
typedef struct {
    int id;
    char modelo[50];
    char placa[10];
    int ano;
} Veiculo;

// Nome do arquivo de armazenamento
#define ARQUIVO "veiculos.dat"

// Funções do programa
void adicionarVeiculo();
void alterarVeiculo();
void excluirVeiculo();
void consultarVeiculo();
void listarVeiculos();

int main() {
    int opcao;

    do {
        printf("\n--- Sistema de Registro de Veículos ---\n");
        printf("1. Adicionar Veiculo\n");
        printf("2. Alterar Veiculo\n");
        printf("3. Excluir Veiculo\n");
        printf("4. Consultar Veiculo\n");
        printf("5. Listar Veiculos\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: adicionarVeiculo(); break;
            case 2: alterarVeiculo(); break;
            case 3: excluirVeiculo(); break;
            case 4: consultarVeiculo(); break;
            case 5: listarVeiculos(); break;
            case 0: printf("Encerrando o programa...\n"); break;
            default: printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

void adicionarVeiculo() {
    Veiculo veiculo;
    FILE *arquivo = fopen(ARQUIVO, "ab");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    printf("\n--- Adicionar Veiculo ---\n");
    printf("ID: ");
    scanf("%d", &veiculo.id);
    printf("Modelo: ");
    getchar();  // Limpar buffer
    fgets(veiculo.modelo, sizeof(veiculo.modelo), stdin);
    veiculo.modelo[strcspn(veiculo.modelo, "\n")] = '\0';  // Remover \n
    printf("Placa: ");
    scanf("%s", veiculo.placa);
    printf("Ano: ");
    scanf("%d", &veiculo.ano);

    fwrite(&veiculo, sizeof(Veiculo), 1, arquivo);
    fclose(arquivo);

    printf("Veiculo adicionado com sucesso!\n");
}

void alterarVeiculo() {
    int id, encontrado = 0;
    Veiculo veiculo;
    FILE *arquivo = fopen(ARQUIVO, "rb+");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    printf("\n--- Alterar Veiculo ---\n");
    printf("ID do veiculo a alterar: ");
    scanf("%d", &id);

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo)) {
        if (veiculo.id == id) {
            encontrado = 1;
            printf("Novo Modelo: ");
            getchar();  // Limpar buffer
            fgets(veiculo.modelo, sizeof(veiculo.modelo), stdin);
            veiculo.modelo[strcspn(veiculo.modelo, "\n")] = '\0';
            printf("Nova Placa: ");
            scanf("%s", veiculo.placa);
            printf("Novo Ano: ");
            scanf("%d", &veiculo.ano);

            fseek(arquivo, -sizeof(Veiculo), SEEK_CUR);
            fwrite(&veiculo, sizeof(Veiculo), 1, arquivo);
            printf("Vecculo alterado com sucesso!\n");
            break;
        }
    }

    if (!encontrado)
        printf("Veiculo com ID %d não encontrado.\n");

    fclose(arquivo);
}

void excluirVeiculo() {
    int id, encontrado = 0;
    Veiculo veiculo;
    FILE *arquivo = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!arquivo || !temp) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    printf("\n--- Excluir Veiculo ---\n");
    printf("ID do veiculo a excluir: ");
    scanf("%d", &id);

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo)) {
        if (veiculo.id == id) {
            encontrado = 1;
        } else {
            fwrite(&veiculo, sizeof(Veiculo), 1, temp);
        }
    }

    fclose(arquivo);
    fclose(temp);

    remove(ARQUIVO);
    rename("temp.dat", ARQUIVO);

    if (encontrado)
        printf("Veiculo excluído com sucesso!\n");
    else
        printf("Veiculo com ID %d não encontrado.\n");
}

void consultarVeiculo() {
    char placa[10];
    int encontrado = 0;
    Veiculo veiculo;
    FILE *arquivo = fopen(ARQUIVO, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    printf("\n--- Consultar Veiculo ---\n");
    printf("Placa do veiculo: ");
    scanf("%s", placa);

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo)) {
        if (strcmp(veiculo.placa, placa) == 0) {
            printf("\nID: %d\n", veiculo.id);
            printf("Modelo: %s\n", veiculo.modelo);
            printf("Placa: %s\n", veiculo.placa);
            printf("Ano: %d\n", veiculo.ano);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
        printf("Veiculo com placa %s não encontrado.\n", placa);

    fclose(arquivo);
}

void listarVeiculos() {
    Veiculo veiculo;
    FILE *arquivo = fopen(ARQUIVO, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    printf("\n--- Lista de Veiculos ---\n");

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo)) {
        printf("\nID: %d\n", veiculo.id);
        printf("Modelo: %s\n", veiculo.modelo);
        printf("Placa: %s\n", veiculo.placa);
        printf("Ano: %d\n", veiculo.ano);
    }

    fclose(arquivo);
}
