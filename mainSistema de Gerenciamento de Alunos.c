#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int matricula;
    char nome[50];
    char curso[30];
    int idade;
} Aluno;

void incluirAluno(FILE *arquivo) {
    Aluno aluno;

    printf("Digite a matrícula: ");
    scanf("%d", &aluno.matricula);
    printf("Digite o nome: ");
    getchar();  // Limpar o buffer
    fgets(aluno.nome, sizeof(aluno.nome), stdin);
    aluno.nome[strcspn(aluno.nome, "\n")] = 0;  // Remover o '\n'
    printf("Digite o curso: ");
    fgets(aluno.curso, sizeof(aluno.curso), stdin);
    aluno.curso[strcspn(aluno.curso, "\n")] = 0;  // Remover o '\n'
    printf("Digite a idade: ");
    scanf("%d", &aluno.idade);

    fseek(arquivo, 0, SEEK_END);
    fwrite(&aluno, sizeof(Aluno), 1, arquivo);
    printf("Aluno incluído com sucesso!\n");
}

void alterarAluno(FILE *arquivo) {
    int matricula, encontrado = 0;
    Aluno aluno;

    printf("Digite a matrícula do aluno que deseja alterar: ");
    scanf("%d", &matricula);

    rewind(arquivo);
    while (fread(&aluno, sizeof(Aluno), 1, arquivo)) {
        if (aluno.matricula == matricula) {
            encontrado = 1;
            printf("Aluno encontrado!\n");
            printf("Digite o novo nome: ");
            getchar();  // Limpar o buffer
            fgets(aluno.nome, sizeof(aluno.nome), stdin);
            aluno.nome[strcspn(aluno.nome, "\n")] = 0;  // Remover o '\n'
            printf("Digite o novo curso: ");
            fgets(aluno.curso, sizeof(aluno.curso), stdin);
            aluno.curso[strcspn(aluno.curso, "\n")] = 0;  // Remover o '\n'
            printf("Digite a nova idade: ");
            scanf("%d", &aluno.idade);

            fseek(arquivo, -sizeof(Aluno), SEEK_CUR);
            fwrite(&aluno, sizeof(Aluno), 1, arquivo);
            printf("Informações do aluno alteradas com sucesso!\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Aluno não encontrado!\n");
    }
}

void excluirAluno(FILE *arquivo) {
    int matricula, encontrado = 0;
    FILE *temp = fopen("temp.dat", "wb");
    Aluno aluno;

    if (!temp) {
        printf("Erro ao criar arquivo temporário!\n");
        return;
    }

    printf("Digite a matrícula do aluno que deseja excluir: ");
    scanf("%d", &matricula);

    rewind(arquivo);
    while (fread(&aluno, sizeof(Aluno), 1, arquivo)) {
        if (aluno.matricula == matricula) {
            encontrado = 1;
            printf("Aluno excluído com sucesso!\n");
        } else {
            fwrite(&aluno, sizeof(Aluno), 1, temp);
        }
    }

    fclose(arquivo);
    fclose(temp);

    remove("alunos.dat");
    rename("temp.dat", "alunos.dat");

    if (!encontrado) {
        printf("Aluno não encontrado!\n");
    }
}

void consultarAluno(FILE *arquivo) {
    int matricula, encontrado = 0;
    Aluno aluno;

    printf("Digite a matrícula do aluno que deseja consultar: ");
    scanf("%d", &matricula);

    rewind(arquivo);
    while (fread(&aluno, sizeof(Aluno), 1, arquivo)) {
        if (aluno.matricula == matricula) {
            encontrado = 1;
            printf("\nMatrícula: %d\nNome: %s\nCurso: %s\nIdade: %d\n",
                   aluno.matricula, aluno.nome, aluno.curso, aluno.idade);
            break;
        }
    }

    if (!encontrado) {
        printf("Aluno não encontrado!\n");
    }
}

void listarAlunos(FILE *arquivo) {
    Aluno aluno;
    int contador = 0;

    rewind(arquivo);
    printf("\n--- Lista de Alunos ---\n");
    while (fread(&aluno, sizeof(Aluno), 1, arquivo)) {
        contador++;
        printf("\nMatrícula: %d\nNome: %s\nCurso: %s\nIdade: %d\n",
               aluno.matricula, aluno.nome, aluno.curso, aluno.idade);
    }

    if (contador == 0) {
        printf("Nenhum aluno cadastrado!\n");
    }
}

int main() {
    FILE *arquivo = fopen("alunos.dat", "rb+");
    int opcao;

    if (!arquivo) {
        arquivo = fopen("alunos.dat", "wb+");
        if (!arquivo) {
            printf("Erro ao abrir o arquivo!\n");
            return 1;
        }
    }

    do {
        printf("\n--- Sistema de Gerenciamento de Alunos ---\n");
        printf("1. Incluir aluno\n");
        printf("2. Alterar informacoes de aluno\n");
        printf("3. Excluir aluno\n");
        printf("4. Consultar informacoes de aluno\n");
        printf("5. Listar todos os alunos\n");
        printf("0. Sair\n");
        printf("Digite sua opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                incluirAluno(arquivo);
                break;
            case 2:
                alterarAluno(arquivo);
                break;
            case 3:
                excluirAluno(arquivo);
                arquivo = fopen("alunos.dat", "rb+");
                break;
            case 4:
                consultarAluno(arquivo);
                break;
            case 5:
                listarAlunos(arquivo);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    fclose(arquivo);
    return 0;
}
