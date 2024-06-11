#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

// Estrutura para representar um livro
struct Livro
{
    int id;
    char titulo[100];
    char autor[100];
    int anoPublicacao;
    int classificacao;
    bool disponibilidade;
};

// Estrutura para representar um leitor
struct Leitor
{
    int id;
    int cpf;
    char nome[100];
    char endereco[100];
    int telefone;
    int idade;
};

// Estrutura para representar um empréstimo
struct Emprestimo
{
    int id_leitor;
    int id_livro;
    time_t dataEmprestimo;
};

// Função para limpar o buffer de entrada
void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Função para autenticar um usuário
bool autenticarUsuario(const char *username, const char *password)
{
    FILE *file = fopen("usuarios.txt", "r");
    if (file != NULL)
    {
        char linha[256];
        char storedUsername[100], storedPassword[100];
        while (fgets(linha, sizeof(linha), file) != NULL)
        {
            sscanf(linha, "%99[^;];%99[^\n]", storedUsername, storedPassword);
            if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0)
            {
                fclose(file);
                return true;
            }
        }
        fclose(file);
    }
    else
    {
        printf("Erro ao abrir o arquivo de usuários.\n");
    }
    return false;
}

// Função para cadastrar um novo leitor
struct Leitor cadastrarLeitor(int cpf, const char *nome, const char *endereco, int telefone, int idade)
{
    struct Leitor leitor;
    leitor.id = rand();
    leitor.cpf = cpf;
    strncpy(leitor.nome, nome, sizeof(leitor.nome) - 1);
    strncpy(leitor.endereco, endereco, sizeof(leitor.endereco) - 1);
    leitor.telefone = telefone;
    leitor.idade = idade;

    FILE *file = fopen("leitores.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%d;%d;%s;%s;%d;%d\n", leitor.id, leitor.cpf, leitor.nome, leitor.endereco, leitor.telefone, leitor.idade);
        fclose(file);
    }
    else
    {
        printf("Erro ao abrir o arquivo de leitores.\n");
    }

    return leitor;
}

// Função para cadastrar um novo livro
void cadastrarLivro(const char *titulo, const char *autor, int anoPublicacao, int classificacao)
{
    struct Livro livro;
    livro.id = rand();
    strncpy(livro.titulo, titulo, sizeof(livro.titulo) - 1);
    strncpy(livro.autor, autor, sizeof(livro.autor) - 1);
    livro.anoPublicacao = anoPublicacao;
    livro.classificacao = classificacao;
    livro.disponibilidade = true;

    FILE *file = fopen("livros.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%d;%s;%s;%d;%d;%d\n", livro.id, livro.titulo, livro.autor, livro.anoPublicacao, livro.classificacao, livro.disponibilidade);
        fclose(file);
    }
    else
    {
        printf("Erro ao abrir o arquivo de livros.\n");
    }
}

// Função para listar todos os livros cadastrados
void listarLivros()
{
    FILE *file = fopen("livros.txt", "r");
    if (file != NULL)
    {
        printf("\nLISTA DE LIVROS\n");
        printf("================\n");
        char linha[256];
        while (fgets(linha, sizeof(linha), file) != NULL)
        {
            printf("%s", linha);
        }
        fclose(file);
    }
    else
    {
        printf("Erro ao abrir o arquivo de livros.\n");
    }
}

// Função para listar todos os leitores cadastrados
void listarLeitores()
{
    FILE *file = fopen("leitores.txt", "r");
    if (file != NULL)
    {
        printf("\nLISTA DE LEITORES\n");
        printf("==================\n");
        char linha[256];
        while (fgets(linha, sizeof(linha), file) != NULL)
        {
            printf("%s", linha);
        }
        fclose(file);
    }
    else
    {
        printf("Erro ao abrir o arquivo de leitores.\n");
    }
}

// Função para buscar um livro pelo título
void buscarLivro(const char *titulo)
{
    FILE *file = fopen("livros.txt", "r");
    if (file != NULL)
    {
        printf("\nRESULTADO DA BUSCA\n");
        printf("==================\n");
        struct Livro livro;
        bool encontrado = false;
        while (fscanf(file, "%d;%99[^;];%99[^;];%d;%d;%d\n", &livro.id, livro.titulo, livro.autor, &livro.anoPublicacao, &livro.classificacao, &livro.disponibilidade) == 6)
        {
            if (strcmp(livro.titulo, titulo) == 0)
            {
                encontrado = true;
                printf("ID: %d\n", livro.id);
                printf("Titulo: %s\n", livro.titulo);
                printf("Autor: %s\n", livro.autor);
                printf("Ano de publicacao: %d\n", livro.anoPublicacao);
                printf("Classificacao: %d\n", livro.classificacao);
                printf("Disponibilidade: %s\n", livro.disponibilidade ? "Disponivel" : "Indisponivel");
                printf("\n");
            }
        }
        if (!encontrado)
        {
            printf("Livro nao encontrado.\n");
        }
        fclose(file);
    }
    else
    {
        printf("Erro ao abrir o arquivo de livros.\n");
    }
}

// Função para emprestar um livro para um leitor
void emprestarLivro(int id_livro, int id_cliente)
{
    FILE *livrosFile = fopen("livros.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    FILE *emprestimosFile = fopen("emprestimos.txt", "a");
    if (livrosFile == NULL || tempFile == NULL || emprestimosFile == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        if (livrosFile)
            fclose(livrosFile);
        if (tempFile)
            fclose(tempFile);
        if (emprestimosFile)
            fclose(emprestimosFile);
        return;
    }

    struct Livro livro;
    bool livroEncontrado = false;
    while (fscanf(livrosFile, "%d;%99[^;];%99[^;];%d;%d;%d\n", &livro.id, livro.titulo, livro.autor, &livro.anoPublicacao, &livro.classificacao, &livro.disponibilidade) == 6)
    {
        if (livro.id == id_livro)
        {
            livroEncontrado = true;
            if (!livro.disponibilidade)
            {
                printf("Livro não está disponível para empréstimo.\n");
                fclose(livrosFile);
                fclose(tempFile);
                fclose(emprestimosFile);
                remove("temp.txt");
                return;
            }
            livro.disponibilidade = false;
        }
        fprintf(tempFile, "%d;%s;%s;%d;%d;%d\n", livro.id, livro.titulo, livro.autor, livro.anoPublicacao, livro.classificacao, livro.disponibilidade);
    }

    fclose(livrosFile);
    fclose(tempFile);

    if (!livroEncontrado)
    {
        printf("Livro não encontrado.\n");
        fclose(emprestimosFile);
        remove("temp.txt");
        return;
    }

    struct Leitor leitor;
    FILE *leitoresFile = fopen("leitores.txt", "r");
    if (leitoresFile == NULL)
    {
        printf("Erro ao abrir o arquivo de leitores.\n");
        fclose(emprestimosFile);
        remove("temp.txt");
        return;
    }

    bool leitorEncontrado = false;
    while (fscanf(leitoresFile, "%d;%d;%99[^;];%99[^;];%d;%d\n", &leitor.id, &leitor.cpf, leitor.nome, leitor.endereco, &leitor.telefone, &leitor.idade) == 6)
    {
        if (leitor.id == id_cliente)
        {
            leitorEncontrado = true;
            break;
        }
    }

    fclose(leitoresFile);

    if (!leitorEncontrado)
    {
        printf("Leitor não encontrado.\n");
        fclose(emprestimosFile);
        remove("temp.txt");
        return;
    }

    if (leitor.idade < livro.classificacao)
    {
        printf("Leitor não tem idade suficiente para pegar este livro emprestado.\n");
        fclose(emprestimosFile);
        remove("temp.txt");
        return;
    }

    struct Emprestimo emprestimo;
    emprestimo.id_leitor = id_cliente;
    emprestimo.id_livro = id_livro;
    emprestimo.dataEmprestimo = time(NULL);
    fprintf(emprestimosFile, "%d;%d;%ld\n", emprestimo.id_leitor, emprestimo.id_livro, emprestimo.dataEmprestimo);

    fclose(emprestimosFile);

    // Renomear o arquivo temporário para substituir o original
    remove("livros.txt");
    rename("temp.txt", "livros.txt");

    printf("Empréstimo realizado com sucesso!\n");
}

// Função para calcular a diferença em dias entre duas datas
int diferencaDias(time_t dataInicio, time_t dataFim)
{
    return (int)difftime(dataFim, dataInicio) / (60 * 60 * 24);
}

// Função para central de avisos
void centralAvisos()
{
    FILE *emprestimosFile = fopen("emprestimos.txt", "r");
    if (emprestimosFile == NULL)
    {
        printf("Erro ao abrir o arquivo de empréstimos.\n");
        return;
    }

    time_t hoje = time(NULL);
    struct Emprestimo emprestimo;
    while (fscanf(emprestimosFile, "%d;%d;%ld\n", &emprestimo.id_leitor, &emprestimo.id_livro, &emprestimo.dataEmprestimo) == 3)
    {
        int diasEmprestimo = diferencaDias(emprestimo.dataEmprestimo, hoje);
        if (diasEmprestimo > 90)
        {
            int diasAtraso = diasEmprestimo - 90;
            double multa = diasAtraso * 0.25;
            printf("Leitor ID %d está com o livro ID %d atrasado %d dias. Multa: R$%.2f\n", emprestimo.id_leitor, emprestimo.id_livro, diasAtraso, multa);
        }
        else if (diasEmprestimo >= 80)
        {
            printf("Leitor ID %d está com o livro ID %d próximo do prazo de devolução (faltam %d dias).\n", emprestimo.id_leitor, emprestimo.id_livro, 90 - diasEmprestimo);
        }
    }

    fclose(emprestimosFile);
}

int main()
{
    srand(time(NULL)); // Inicializar o gerador de números aleatórios

    char username[100], password[100];
    int i = 0;
    printf("Username: ");
    scanf("%s", username);
    limparBuffer();

    printf("Password: ");
    while (1)
    {
        char c = getch();
        if (c == '\r') // Enter
        {
            password[i] = '\0';
            break;
        }
        else if (c == '\b') // Backspace
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            password[i++] = c;
            printf("*");
        }
    }
    printf("\n");

    if (!autenticarUsuario(username, password))
    {
        printf("Username ou senha incorretos.\n");
        return 1;
    }

    printf("Login realizado com sucesso!\n");

    int opcao;
    do
    {
        centralAvisos();

        printf("\nBIBLIOTECA VIRTUAL\n");
        printf("1 - Cadastrar livro\n");
        printf("2 - Cadastrar leitor\n");
        printf("3 - Listar livros\n");
        printf("4 - Listar leitores\n");
        printf("5 - Emprestar Livro\n");
        printf("6 - Buscar Livro\n");
        printf("7 - Sair\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao)
        {
        case 1:
        {
            char titulo[100];
            char autor[100];
            int anoPublicacao;
            int classificacao;

            printf("Titulo do livro: ");
            fgets(titulo, sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = '\0';
            printf("Autor do livro: ");
            fgets(autor, sizeof(autor), stdin);
            autor[strcspn(autor, "\n")] = '\0';
            printf("Ano de publicacao: ");
            scanf("%d", &anoPublicacao);
            printf("Classificacao: ");
            scanf("%d", &classificacao);
            limparBuffer();

            cadastrarLivro(titulo, autor, anoPublicacao, classificacao);
            break;
        }
        case 2:
        {
            int cpf, telefone, idade;
            char nome[100], endereco[100];

            printf("CPF do cliente: ");
            scanf("%d", &cpf);
            limparBuffer();
            printf("Nome do cliente: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = '\0';
            printf("Endereco do cliente: ");
            fgets(endereco, sizeof(endereco), stdin);
            endereco[strcspn(endereco, "\n")] = '\0';
            printf("Telefone do cliente: ");
            scanf("%d", &telefone);
            limparBuffer();
            printf("Idade do cliente: ");
            scanf("%d", &idade);
            limparBuffer();

            cadastrarLeitor(cpf, nome, endereco, telefone, idade);
            printf("Leitor cadastrado com sucesso!\n");
            break;
        }
        case 3:
        {
            listarLivros();
            break;
        }
        case 4:
        {
            listarLeitores();
            break;
        }
        case 5:
        {
            int id_cliente, id_livro;
            printf("ID do cliente: ");
            scanf("%d", &id_cliente);
            limparBuffer();
            printf("ID do livro a ser emprestado: ");
            scanf("%d", &id_livro);
            limparBuffer();
            emprestarLivro(id_livro, id_cliente);
            break;
        }
        case 6:
        {
            char titulo[100];
            printf("Digite o titulo do livro: ");
            fgets(titulo, sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = '\0';
            buscarLivro(titulo);
            break;
        }
        case 7:
        {
            printf("Saindo da biblioteca virtual. Ate logo!\n");
            break;
        }
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    } while (opcao != 7);

    return 0;
}
