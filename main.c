#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para representar um livro
struct Livro
{
    int id_livro;
    char titulo[100];
    char autor[100];
    int anoPublicacao;
    int classificacao;
    bool disponibilidade;
};

// Estrutura para representar um leitor
struct Leitor
{
    int id_cliente;
    int cpf;
    char nome[100];
    char endereco[100];
    int telefone;
    int idade;
};

// Função para ler o próximo ID de um arquivo
int lerProximoId(const char *nomeArquivo)
{
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL)
    {
        return 1; // Se o arquivo não existe, começamos com 1
    }
    int id;
    fscanf(file, "%d", &id);
    fclose(file);
    return id + 1;
}

// Função para salvar o próximo ID em um arquivo
void salvarProximoId(const char *nomeArquivo, int id)
{
    FILE *file = fopen(nomeArquivo, "w");
    if (file != NULL)
    {
        fprintf(file, "%d", id);
        fclose(file);
    }
}

// Função para cadastrar um novo leitor
struct Leitor cadastrarLeitor(int cpf, const char *nome,
                              const char *endereco, int telefone, int idade)
{
    int id = lerProximoId("ultimo_id_cliente.txt");
    struct Leitor leitor;
    leitor.id_cliente = id;
    leitor.cpf = cpf;
    strncpy(leitor.nome, nome, sizeof(leitor.nome) - 1);
    strncpy(leitor.endereco, endereco, sizeof(leitor.endereco) - 1);
    leitor.telefone = telefone;
    leitor.idade = idade;

    FILE *file = fopen("leitores.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%d;%d;%s;%s;%d;%d\n", leitor.id_cliente, leitor.cpf,
                leitor.nome, leitor.endereco, leitor.telefone, leitor.idade);
        fclose(file);
    }
    else
    {
        printf("Erro ao abrir o arquivo de leitores.\n");
    }

    salvarProximoId("ultimo_id_cliente.txt", id);
    return leitor;
}

// Função para cadastrar um novo livro
void cadastrarLivro(const char *titulo, const char *autor, int anoPublicacao,
                    int classificacao)
{
    int id = lerProximoId("ultimo_id_livro.txt");
    struct Livro livro;
    livro.id_livro = id;
    strncpy(livro.titulo, titulo, sizeof(livro.titulo) - 1);
    strncpy(livro.autor, autor, sizeof(livro.autor) - 1);
    livro.anoPublicacao = anoPublicacao;
    livro.classificacao = classificacao;
    livro.disponibilidade = true;

    FILE *file = fopen("livros.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%d;%s;%s;%d;%d;%d\n", livro.id_livro, livro.titulo, livro.autor,
                livro.anoPublicacao, livro.classificacao, livro.disponibilidade);
        fclose(file);
    }
    else
    {
        printf("Erro ao abrir o arquivo de livros.\n");
    }

    salvarProximoId("ultimo_id_livro.txt", id);
}

// Função para limpar o buffer de entrada
void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
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
        while (fscanf(file, "%d;%99[^;];%99[^;];%d;%d;%d\n", &livro.id_livro, livro.titulo, livro.autor,
                      &livro.anoPublicacao, &livro.classificacao,
                      &livro.disponibilidade) == 6)
        {
            if (strcmp(livro.titulo, titulo) == 0)
            {
                encontrado = true;
                printf("ID: %d\n", livro.id_livro);
                printf("Titulo: %s\n", livro.titulo);
                printf("Autor: %s\n", livro.autor);
                printf("Ano de publicacao: %d\n", livro.anoPublicacao);
                printf("Classificacao: %d\n", livro.classificacao);
                printf("Disponibilidade: %s\n",
                       livro.disponibilidade ? "Disponivel" : "Indisponivel");
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
    // Abrir arquivo de livros para leitura e escrita
    FILE *file = fopen("livros.txt", "r+");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo de livros.\n");
        return;
    }

    // Abrir arquivo de empréstimos para escrita
    FILE *emprestimos_file = fopen("emprestimos.txt", "a");
    if (emprestimos_file == NULL)
    {
        printf("Erro ao abrir o arquivo de empréstimos.\n");
        fclose(file);
        return;
    }

    // Procurar pelo livro e verificar a disponibilidade
    struct Livro livro;
    long pos;
    bool encontrado = false;
    while (fscanf(file, "%d;%99[^;];%99[^;];%d;%d;%d\n", &livro.id_livro, livro.titulo, livro.autor,
                  &livro.anoPublicacao, &livro.classificacao, &livro.disponibilidade) == 6)
    {
        if (livro.id_livro == id_livro)
        {
            encontrado = true;
            if (livro.disponibilidade)
            {
                livro.disponibilidade = false;
                pos = ftell(file) - (long)strlen(livro.titulo) - (long)strlen(livro.autor) - sizeof(int) * 4 - sizeof(bool) * 1 - 5; // -5 para os caracteres de separação
                fseek(file, pos, SEEK_SET);
                fprintf(file, "%d;%s;%s;%d;%d;%d\n", livro.id_livro, livro.titulo, livro.autor,
                        livro.anoPublicacao, livro.classificacao, livro.disponibilidade);

                // Registrar o empréstimo
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                fprintf(emprestimos_file, "%d;%d;%d-%02d-%02d\n", id_cliente, id_livro, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
                printf("Livro emprestado com sucesso!\n");
            }
            else
            {
                printf("Livro indisponivel para emprestimo.\n");
            }
            break;
        }
    }
    if (!encontrado)
    {
        printf("Livro nao encontrado.\n");
    }

    fclose(emprestimos_file);
    fclose(file);
}

// Função principal
int main()
{
    int opcao;

    do
    {
        printf("\nBIBLIOTECA VIRTUAL\n");
        printf("1 - Cadastrar livro\n");
        printf("2 - Cadastrar leitor\n");
        printf("3 - Listar livros\n");
        printf("4 - Listar leitores\n");
        printf("5 - Emprestar Livro\n");
        printf("6 - Buscar Livro\n");
        printf("7 - Sair\n");
        printf("Escolha uma opcao: ");

        // Ler a opção do usuário
        scanf("%d", &opcao);
        limparBuffer(); // Limpar o buffer de entrada

        switch (opcao)
        {
        case 1:
        {
            char titulo[100];
            char autor[100];
            int anoPublicacao;
            int classificacao;

            // Solicitar informações do livro ao usuário
            printf("Titulo do livro: ");
            fgets(titulo, sizeof(titulo), stdin);
            printf("Autor do livro: ");
            fgets(autor, sizeof(autor), stdin);
            printf("Ano de publicacao: ");
            scanf("%d", &anoPublicacao);
            printf("Classificacao: ");
            scanf("%d", &classificacao);
            limparBuffer(); // Limpar o buffer de entrada

            // Cadastrar o livro
            cadastrarLivro(titulo, autor, anoPublicacao, classificacao);
            break;
        }
        case 2:
        {
            int cpf, telefone, idade;
            char nome[100], endereco[100];

            // Solicitar informações do leitor ao usuário
            printf("CPF do cliente: ");
            scanf("%d", &cpf);
            limparBuffer();
            printf("Nome do cliente: ");
            fgets(nome, sizeof(nome), stdin);
            printf("Endereco do cliente: ");
            fgets(endereco, sizeof(endereco), stdin);
            printf("Telefone do cliente: ");
            scanf("%d", &telefone);
            limparBuffer();
            printf("Idade do cliente: ");
            scanf("%d", &idade);
            limparBuffer();

            // Cadastrar o leitor
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
            int id_cliente, id_livro; // Declarar id_cliente e id_livro dentro do escopo correto
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
