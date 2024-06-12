#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

// Definir códigos de cores ANSI
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"
#define BLUE_COLOR "\033[34m"
#define MAGENTA_COLOR "\033[35m"
#define CYAN_COLOR "\033[36m"

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
    int id_cliente;
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

// Função para exibir mensagens coloridas
void exibirMensagem(const char *mensagem, const char *cor)
{
    printf("%s%s%s\n", cor, mensagem, RESET_COLOR);
}

// Função para gerar um novo ID de leitor
int gerarNovoIdLeitor()
{
    FILE *file = fopen("leitores.txt", "r");
    if (file == NULL)
    {
        return 1; // Primeiro ID
    }

    int ultimoId = 0;
    char linha[256];
    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        int id;
        sscanf(linha, "%d", &id);
        if (id > ultimoId)
        {
            ultimoId = id;
        }
    }

    fclose(file);
    return ultimoId + 1;
}

// Função para gerar um novo ID de livro
int gerarNovoIdLivro()
{
    FILE *file = fopen("livros.txt", "r");
    if (file == NULL)
    {
        return 1; // Primeiro ID
    }

    int ultimoId = 0;
    char linha[256];
    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        int id;
        sscanf(linha, "%d", &id);
        if (id > ultimoId)
        {
            ultimoId = id;
        }
    }

    fclose(file);
    return ultimoId + 1;
}

// Função para cadastrar um novo leitor
struct Leitor cadastrarLeitor(int cpf, const char *nome, const char *endereco, int telefone, int idade)
{
    struct Leitor leitor;
    leitor.id_cliente = gerarNovoIdLeitor();
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
        exibirMensagem("Erro ao abrir o arquivo de leitores.", RED_COLOR);
    }

    return leitor;
}

// Função para cadastrar um novo livro
void cadastrarLivro(const char *titulo, const char *autor, int anoPublicacao, int classificacao)
{
    struct Livro livro;
    livro.id = gerarNovoIdLivro();
    strncpy(livro.titulo, titulo, sizeof(livro.titulo) - 1);
    strncpy(livro.autor, autor, sizeof(livro.autor) - 1);
    livro.anoPublicacao = anoPublicacao;
    livro.classificacao = classificacao;
    livro.disponibilidade = true;

    FILE *file = fopen("livros.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%d;%s;%s;%d;%d;%d\n", livro.id, livro.titulo, livro.autor,
                livro.anoPublicacao, livro.classificacao, livro.disponibilidade);
        fclose(file);
    }
    else
    {
        exibirMensagem("Erro ao abrir o arquivo de livros.", RED_COLOR);
    }
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
        exibirMensagem("Erro ao abrir o arquivo de livros.", RED_COLOR);
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
        exibirMensagem("Erro ao abrir o arquivo de leitores.", RED_COLOR);
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
        char linha[256];
        while (fgets(linha, sizeof(linha), file) != NULL)
        {
            sscanf(linha, "%d;%99[^;];%99[^;];%d;%d;%d\n", &livro.id, livro.titulo, livro.autor,
                   &livro.anoPublicacao, &livro.classificacao, &livro.disponibilidade);
            if (strcmp(livro.titulo, titulo) == 0)
            {
                encontrado = true;
                printf("ID: %d\n", livro.id);
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
            exibirMensagem("Livro não encontrado.", RED_COLOR);
        }
        fclose(file);
    }
    else
    {
        exibirMensagem("Erro ao abrir o arquivo de livros.", RED_COLOR);
    }
}

// Função para emprestar um livro para um leitor
void emprestarLivro(int id_livro, int id_cliente)
{
    // Abrir arquivo de livros para leitura e escrita
    FILE *file = fopen("livros.txt", "r+");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de livros.", RED_COLOR);
        return;
    }

    // Abrir arquivo de empréstimos para escrita
    FILE *emprestimos_file = fopen("emprestimos.txt", "a");
    if (emprestimos_file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de empréstimos.", RED_COLOR);
        fclose(file);
        return;
    }

    struct Livro livro;
    char linha[256];
    bool livroEncontrado = false;
    bool livroDisponivel = false;
    long posicao = 0;

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        sscanf(linha, "%d;%99[^;];%99[^;];%d;%d;%d\n", &livro.id, livro.titulo, livro.autor,
               &livro.anoPublicacao, &livro.classificacao, &livro.disponibilidade);
        if (livro.id == id_livro)
        {
            livroEncontrado = true;
            if (livro.disponibilidade)
            {
                livroDisponivel = true;
                livro.disponibilidade = false;
                fseek(file, posicao, SEEK_SET);
                fprintf(file, "%d;%s;%s;%d;%d;%d\n", livro.id, livro.titulo, livro.autor,
                        livro.anoPublicacao, livro.classificacao, livro.disponibilidade);
                break;
            }
        }
        posicao = ftell(file);
    }

    if (!livroEncontrado)
    {
        exibirMensagem("Livro não encontrado.", RED_COLOR);
        fclose(file);
        fclose(emprestimos_file);
        return;
    }

    if (!livroDisponivel)
    {
        exibirMensagem("Livro indisponível.", RED_COLOR);
        fclose(file);
        fclose(emprestimos_file);
        return;
    }

    struct Emprestimo emprestimo;
    emprestimo.id_leitor = id_cliente;
    emprestimo.id_livro = id_livro;
    emprestimo.dataEmprestimo = time(NULL);

    fprintf(emprestimos_file, "%d;%d;%ld\n", emprestimo.id_leitor, emprestimo.id_livro,
            emprestimo.dataEmprestimo);
    exibirMensagem("Empréstimo realizado com sucesso!", GREEN_COLOR);

    fclose(file);
    fclose(emprestimos_file);
}

// Função para devolver um livro
void devolverLivro(int id_livro, int id_cliente)
{
    FILE *livros_file = fopen("livros.txt", "r+");
    FILE *emprestimos_file = fopen("emprestimos.txt", "r");
    FILE *temp_file = fopen("temp.txt", "w");

    if (livros_file == NULL || emprestimos_file == NULL || temp_file == NULL)
    {
        exibirMensagem("Erro ao abrir os arquivos.", RED_COLOR);
        if (livros_file != NULL)
            fclose(livros_file);
        if (emprestimos_file != NULL)
            fclose(emprestimos_file);
        if (temp_file != NULL)
            fclose(temp_file);
        return;
    }

    struct Livro livro;
    struct Emprestimo emprestimo;
    char linha[256];
    bool emprestimoEncontrado = false;
    long posicao = 0;

    // Verificar e atualizar a disponibilidade do livro
    while (fgets(linha, sizeof(linha), livros_file) != NULL)
    {
        sscanf(linha, "%d;%99[^;];%99[^;];%d;%d;%d\n", &livro.id, livro.titulo, livro.autor,
               &livro.anoPublicacao, &livro.classificacao, &livro.disponibilidade);
        if (livro.id == id_livro)
        {
            livro.disponibilidade = true;
            fseek(livros_file, posicao, SEEK_SET);
            fprintf(livros_file, "%d;%s;%s;%d;%d;%d\n", livro.id, livro.titulo, livro.autor,
                    livro.anoPublicacao, livro.classificacao, livro.disponibilidade);
        }
        posicao = ftell(livros_file);
    }

    // Verificar e remover o empréstimo
    while (fgets(linha, sizeof(linha), emprestimos_file) != NULL)
    {
        sscanf(linha, "%d;%d;%ld\n", &emprestimo.id_leitor, &emprestimo.id_livro, &emprestimo.dataEmprestimo);
        if (emprestimo.id_leitor == id_cliente && emprestimo.id_livro == id_livro)
        {
            emprestimoEncontrado = true;
            time_t dataAtual = time(NULL);
            double diffDias = difftime(dataAtual, emprestimo.dataEmprestimo) / (60 * 60 * 24);
            if (diffDias > 90)
            {
                double multa = (diffDias - 90) * 0.25;
                printf("Livro devolvido com atraso. Multa: R$ %.2f\n", multa);
            }
        }
        else
        {
            fprintf(temp_file, "%d;%d;%ld\n", emprestimo.id_leitor, emprestimo.id_livro, emprestimo.dataEmprestimo);
        }
    }

    fclose(livros_file);
    fclose(emprestimos_file);
    fclose(temp_file);

    remove("emprestimos.txt");
    rename("temp.txt", "emprestimos.txt");

    if (emprestimoEncontrado)
    {
        exibirMensagem("Livro devolvido com sucesso!", GREEN_COLOR);
    }
    else
    {
        exibirMensagem("Empréstimo não encontrado.", RED_COLOR);
    }
}

// Função para exibir avisos de prazos de devolução e multas
void exibirAvisos()
{
    FILE *emprestimos_file = fopen("emprestimos.txt", "r");
    if (emprestimos_file == NULL)
    {
        return;
    }

    struct Emprestimo emprestimo;
    char linha[256];
    time_t dataAtual = time(NULL);

    while (fgets(linha, sizeof(linha), emprestimos_file) != NULL)
    {
        sscanf(linha, "%d;%d;%ld\n", &emprestimo.id_leitor, &emprestimo.id_livro, &emprestimo.dataEmprestimo);
        double diffDias = difftime(dataAtual, emprestimo.dataEmprestimo) / (60 * 60 * 24);
        if (diffDias > 85 && diffDias <= 90)
        {
            printf(YELLOW_COLOR "Aviso: O leitor %d deve devolver o livro %d em breve.\n" RESET_COLOR,
                   emprestimo.id_leitor, emprestimo.id_livro);
        }
        else if (diffDias > 90)
        {
            double multa = (diffDias - 90) * 0.25;
            printf(RED_COLOR "Aviso: O leitor %d está em atraso com o livro %d. Multa acumulada: R$ %.2f\n" RESET_COLOR,
                   emprestimo.id_leitor, emprestimo.id_livro, multa);
        }
    }

    fclose(emprestimos_file);
}

// Função para ler a senha do usuário sem exibir os caracteres
void lerSenha(char *senha, int max_len)
{
    int i = 0;
    char c;
    while ((c = getch()) != '\r' && i < max_len - 1)
    {
        if (c == '\b')
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            senha[i++] = c;
            printf("*");
        }
    }
    senha[i] = '\0';
    printf("\n");
}

// Função para realizar login
bool realizarLogin()
{
    char usuario[50];
    char senha[50];
    bool autenticado = false;

    printf("Usuario: ");
    scanf("%49s", usuario);
    limparBuffer();
    printf("Senha: ");
    lerSenha(senha, 50);

    FILE *file = fopen("usuarios.txt", "r");
    if (file != NULL)
    {
        char linha[100];
        char usuarioFile[50], senhaFile[50];
        while (fgets(linha, sizeof(linha), file) != NULL)
        {
            sscanf(linha, "%49[^;];%49[^\n]", usuarioFile, senhaFile);
            if (strcmp(usuario, usuarioFile) == 0 && strcmp(senha, senhaFile) == 0)
            {
                autenticado = true;
                break;
            }
        }
        fclose(file);
    }

    if (!autenticado)
    {
        exibirMensagem("Usuário ou senha inválidos.", RED_COLOR);
    }

    return autenticado;
}

// Função principal
int main()
{
    if (!realizarLogin())
    {
        return 0;
    }

    int opcao;

    do
    {
        exibirAvisos();

        printf("\nBIBLIOTECA VIRTUAL\n");
        printf("==================\n");
        printf("1 - Cadastrar livro\n");
        printf("2 - Cadastrar leitor\n");
        printf("3 - Listar livros\n");
        printf("4 - Listar leitores\n");
        printf("5 - Emprestar Livro\n");
        printf("6 - Devolver Livro\n");
        printf("7 - Buscar Livro\n");
        printf("8 - Sair\n");
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
            titulo[strcspn(titulo, "\n")] = '\0'; // Remover a nova linha
            printf("Autor do livro: ");
            fgets(autor, sizeof(autor), stdin);
            autor[strcspn(autor, "\n")] = '\0'; // Remover a nova linha
            printf("Ano de publicacao: ");
            scanf("%d", &anoPublicacao);
            printf("Classificacao: ");
            scanf("%d", &classificacao);
            limparBuffer(); // Limpar o buffer de entrada

            // Cadastrar o livro
            cadastrarLivro(titulo, autor, anoPublicacao, classificacao);
            exibirMensagem("Livro cadastrado com sucesso!", GREEN_COLOR);
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
            nome[strcspn(nome, "\n")] = '\0'; // Remover a nova linha
            printf("Endereco do cliente: ");
            fgets(endereco, sizeof(endereco), stdin);
            endereco[strcspn(endereco, "\n")] = '\0'; // Remover a nova linha
            printf("Telefone do cliente: ");
            scanf("%d", &telefone);
            printf("Idade do cliente: ");
            scanf("%d", &idade);
            limparBuffer(); // Limpar o buffer de entrada

            // Cadastrar o leitor
            cadastrarLeitor(cpf, nome, endereco, telefone, idade);
            exibirMensagem("Leitor cadastrado com sucesso!", GREEN_COLOR);
            break;
        }
        case 3:
            listarLivros();
            break;
        case 4:
            listarLeitores();
            break;
        case 5:
        {
            int id_livro, id_cliente;
            printf("ID do livro: ");
            scanf("%d", &id_livro);
            printf("ID do cliente: ");
            scanf("%d", &id_cliente);
            limparBuffer();
            emprestarLivro(id_livro, id_cliente);
            break;
        }
        case 6:
        {
            int id_livro, id_cliente;
            printf("ID do livro: ");
            scanf("%d", &id_livro);
            printf("ID do cliente: ");
            scanf("%d", &id_cliente);
            limparBuffer();
            devolverLivro(id_livro, id_cliente);
            break;
        }
        case 7:
        {
            char titulo[100];
            printf("Titulo do livro: ");
            fgets(titulo, sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = '\0'; // Remover a nova linha
            buscarLivro(titulo);
            break;
        }
        case 8:
            exibirMensagem("Saindo do sistema...", CYAN_COLOR);
            break;
        default:
            exibirMensagem("Opcao invalida. Tente novamente.", RED_COLOR);
        }
    } while (opcao != 8);

    return 0;
}
