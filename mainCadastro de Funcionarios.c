#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "funcionarios.dat"

typedef struct {
    int id;
    char nome[50];
    char cargo[30];
    int salario;
} Funcionario;

// Prototipação das funções
void cadastrarFuncionario();
void alterarFuncionario();
void excluirFuncionario();
void consultarFuncionario();
void listarFuncionarios();
int obterNovoID();

int main() {
    int opcao;

    do {
        printf("\nMenu:\n");
        printf("1. Cadastrar um novo funcionario\n");
        printf("2. Alterar dados de um funcionario\n");
        printf("3. Excluir um funcionario pelo ID\n");
        printf("4. Consultar informacoes de um funcionario pelo ID\n");
        printf("5. Listar todos os funcionarios cadastrados\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Consumir o '\n'

        switch (opcao) {
            case 1: cadastrarFuncionario(); break;
            case 2: alterarFuncionario(); break;
            case 3: excluirFuncionario(); break;
            case 4: consultarFuncionario(); break;
            case 5: listarFuncionarios(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

void cadastrarFuncionario() {
    FILE *file = fopen(FILE_NAME, "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    Funcionario func;
    func.id = obterNovoID();

    printf("Nome: ");
    fgets(func.nome, sizeof(func.nome), stdin);
    func.nome[strcspn(func.nome, "\n")] = '\0'; // Remover '\n'

    printf("Cargo: ");
    fgets(func.cargo, sizeof(func.cargo), stdin);
    func.cargo[strcspn(func.cargo, "\n")] = '\0';

    printf("Salário: ");
    scanf("%d", &func.salario);
    getchar();

    fwrite(&func, sizeof(Funcionario), 1, file);
    fclose(file);

    printf("Funcionário cadastrado com sucesso!\n");
}

void alterarFuncionario() {
    int id, encontrado = 0;
    FILE *file = fopen(FILE_NAME, "rb+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("ID do funcionário a ser alterado: ");
    scanf("%d", &id);
    getchar();

    Funcionario func;
    while (fread(&func, sizeof(Funcionario), 1, file)) {
        if (func.id == id) {
            printf("Novo cargo: ");
            fgets(func.cargo, sizeof(func.cargo), stdin);
            func.cargo[strcspn(func.cargo, "\n")] = '\0';

            printf("Novo salário: ");
            scanf("%d", &func.salario);
            getchar();

            fseek(file, -sizeof(Funcionario), SEEK_CUR);
            fwrite(&func, sizeof(Funcionario), 1, file);

            printf("Dados do funcionário alterados com sucesso!\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Funcionário não encontrado!\n");
    }

    fclose(file);
}

void excluirFuncionario() {
    int id, encontrado = 0;
    FILE *file = fopen(FILE_NAME, "rb");
    FILE *tempFile = fopen("temp.dat", "wb");

    if (file == NULL || tempFile == NULL) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    printf("ID do funcionário a ser excluído: ");
    scanf("%d", &id);
    getchar();

    Funcionario func;
    while (fread(&func, sizeof(Funcionario), 1, file)) {
        if (func.id == id) {
            printf("Funcionário excluído com sucesso!\n");
            encontrado = 1;
        } else {
            fwrite(&func, sizeof(Funcionario), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (!encontrado) {
        printf("Funcionário não encontrado!\n");
    }
}

void consultarFuncionario() {
    int id, encontrado = 0;
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("ID do funcionário: ");
    scanf("%d", &id);
    getchar();

    Funcionario func;
    while (fread(&func, sizeof(Funcionario), 1, file)) {
        if (func.id == id) {
            printf("ID: %d\n", func.id);
            printf("Nome: %s\n", func.nome);
            printf("Cargo: %s\n", func.cargo);
            printf("Salário: %d\n", func.salario);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Funcionário não encontrado!\n");
    }

    fclose(file);
}

void listarFuncionarios() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    Funcionario func;
    printf("\nLista de funcionários:\n");
    while (fread(&func, sizeof(Funcionario), 1, file)) {
        printf("ID: %d, Nome: %s, Cargo: %s, Salário: %d\n",
               func.id, func.nome, func.cargo, func.salario);
    }

    fclose(file);
}

int obterNovoID() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        return 1; // Primeiro ID
    }

    Funcionario func;
    int maxID = 0;
    while (fread(&func, sizeof(Funcionario), 1, file)) {
        if (func.id > maxID) {
            maxID = func.id;
        }
    }

    fclose(file);
    return maxID + 1;
}
