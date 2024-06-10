#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PRAZO_EMPRESTIMO 90 // prazo de 90 dias para devolução
#define MULTA_POR_DIA 0.25  // multa de 0,25 centavos por dia de atraso

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

// Estrutura para representar um empréstimo
struct Emprestimo
{
    int id_cliente;
    int id_livro;
    struct tm data_emprestimo;
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
    leitor.nome[sizeof(leitor.nome) - 1] = '\0'; // Garantir terminação
    strncpy(leitor.endereco, endereco, sizeof(leitor.endereco) - 1);
    leitor.endereco[sizeof(leitor.endereco) - 1] = '\0'; // Garantir terminação
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
    livro.titulo[sizeof(livro.titulo) - 1] = '\0'; // Garantir terminação
    strncpy(livro.autor, autor, sizeof(livro.autor) - 1);
    livro.autor[sizeof(livro.autor) - 1] = '\0'; // Garantir terminação
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

// Função para buscar a idade do leitor pelo ID
int buscarIdadeLeitor(int id_cliente)
{
    FILE *file = fopen("leitores.txt", "r");
    if (file != NULL)
    {
        struct Leitor leitor;
        while (fscanf(file, "%d;%d;%99[^;];%99[^;];%d;%d\n", &leitor.id_cliente, &leitor.cpf, leitor.nome,
                      leitor.endereco, &leitor.telefone, &leitor.idade) == 6)
        {
            if (leitor.id_cliente == id_cliente)
            {
                fclose(file);
                return leitor.idade;
            }
        }
        fclose(file);
    }
    return -1; // Retorna -1 se não encontrar o leitor
}

// Função para registrar um empréstimo no arquivo
void registrarEmprestimo(int id_livro, int id_cliente)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    FILE *emprestimos_file = fopen("emprestimos.txt", "a");
    if (emprestimos_file != NULL)
    {
        fprintf(emprestimos_file, "%d;%d;%02d/%02d/%04d\n",
                id_cliente, id_livro, t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
        fclose(emprestimos_file);
    }
    else
    {
        printf("Erro ao abrir o arquivo de empréstimos.\n");
    }
}

// Função para emprestar um livro para um leitor
void emprestarLivro(int id_livro, int id_cliente)
{
    // Verificar a idade do leitor
    int idadeLeitor = buscarIdadeLeitor(id_cliente);
    if (idadeLeitor == -1)
    {
        printf("Leitor não encontrado.\n");
        return;
    }

    // Abrir arquivo de livros para leitura e escrita
    FILE *file = fopen("livros.txt", "r+");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo de livros.\n");
        return;
    }

    struct Livro livro;
    long pos;
    bool encontrado = false;

    while (fscanf(file, "%d;%99[^;];%99[^;];%d;%d;%d\n", &livro.id_livro, livro.titulo, livro.autor,
                  &livro.anoPublicacao, &livro.classificacao,
                  &livro.disponibilidade) == 6)
    {
        if (livro.id_livro == id_livro)
        {
            encontrado = true;
            if (livro.disponibilidade)
            {
                if (idadeLeitor < livro.classificacao)
                {
                    printf("Empréstimo não permitido. A classificação indicativa do livro é %d anos.\n", livro.classificacao);
                }
                else
                {
                    // Marcar o livro como indisponível
                    livro.disponibilidade = false;

                    // Mover o ponteiro para a posição do registro no arquivo
                    pos = ftell(file) - strlen(livro.titulo) - strlen(livro.autor) - 16;
                    fseek(file, pos, SEEK_SET);
                    fprintf(file, "%d;%s;%s;%d;%d;%d\n", livro.id_livro, livro.titulo, livro.autor,
                            livro.anoPublicacao, livro.classificacao, livro.disponibilidade);

                    // Registrar o empréstimo
                    registrarEmprestimo(id_livro, id_cliente);

                    printf("Livro emprestado com sucesso!\n");
                }
            }
            else
            {
                printf("Livro indisponível para empréstimo.\n");
            }
            break;
        }
    }
    if (!encontrado)
    {
        printf("Livro não encontrado.\n");
    }

    fclose(file);
}

// Função para cadastrar um novo usuário (admin)
void cadastrarUsuario(const char *username, const char *password)
{
    FILE *file = fopen("usuarios.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%s;%s\n", username, password);
        fclose(file);
    }
    else
    {
        printf("Erro ao abrir o arquivo de usuários.\n");
    }
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

// Função para calcular a diferença de dias entre duas datas
int diferencaDias(struct tm data_inicial, struct tm data_final)
{
    time_t t_inicial = mktime(&data_inicial);
    time_t t_final = mktime(&data_final);
    return (int)difftime(t_final, t_inicial) / (60 * 60 * 24);
}

// Função para verificar e exibir avisos de prazos de devolução próximos de expirar
void verificarPrazos()
{
    FILE *file = fopen("emprestimos.txt", "r");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo de empréstimos.\n");
        return;
    }

    printf("\nAVISOS DE EMPRÉSTIMOS\n");
    printf("=====================\n");

    struct Emprestimo emprestimo;
    char linha[256];
    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        sscanf(linha, "%d;%d;%d/%d/%d\n", &emprestimo.id_cliente, &emprestimo.id_livro,
               &emprestimo.data_emprestimo.tm_mday, &emprestimo.data_emprestimo.tm_mon,
               &emprestimo.data_emprestimo.tm_year);

        emprestimo.data_emprestimo.tm_mon -= 1;     // Ajustar mês
        emprestimo.data_emprestimo.tm_year -= 1900; // Ajustar ano

        time_t now = time(NULL);
        struct tm *data_atual = localtime(&now);

        int diasEmprestimo = diferencaDias(emprestimo.data_emprestimo, *data_atual);
        int diasRestantes = PRAZO_EMPRESTIMO - diasEmprestimo;

        if (diasRestantes <= 10 && diasRestantes >= 0)
        {
            printf("Aviso: O prazo para devolução do Livro ID: %d pelo Leitor ID: %d expira em %d dias.\n",
                   emprestimo.id_livro, emprestimo.id_cliente, diasRestantes);
        }
        else if (diasRestantes < 0)
        {
            double multa = -diasRestantes * MULTA_POR_DIA;
            printf("Aviso: O Livro ID: %d emprestado pelo Leitor ID: %d está atrasado em %d dias. Multa acumulada: R$%.2f.\n",
                   emprestimo.id_livro, emprestimo.id_cliente, -diasRestantes, multa);
        }
    }

    fclose(file);
}

int main()
{
    int opcao;
    char username[100], password[100];

    printf("Bem-vindo ao Sistema de Biblioteca\n");
    printf("=================================\n");

    // Autenticação do usuário
    printf("Digite seu username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Digite sua senha: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    if (!autenticarUsuario(username, password))
    {
        printf("Username ou senha incorretos.\n");
        return 1;
    }

    printf("Login realizado com sucesso!\n");

    // Verificar prazos de empréstimos
    verificarPrazos();

    do
    {
        printf("\nSistema de Biblioteca\n");
        printf("=====================\n");
        printf("1. Cadastrar leitor\n");
        printf("2. Cadastrar livro\n");
        printf("3. Listar livros\n");
        printf("4. Listar leitores\n");
        printf("5. Buscar livro por título\n");
        printf("6. Emprestar livro\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao)
        {
        case 1:
        {
            int cpf, telefone, idade;
            char nome[100], endereco[100];

            printf("Digite o CPF do leitor: ");
            scanf("%d", &cpf);
            limparBuffer();

            printf("Digite o nome do leitor: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = '\0';

            printf("Digite o endereco do leitor: ");
            fgets(endereco, sizeof(endereco), stdin);
            endereco[strcspn(endereco, "\n")] = '\0';

            printf("Digite o telefone do leitor: ");
            scanf("%d", &telefone);
            limparBuffer();

            printf("Digite a idade do leitor: ");
            scanf("%d", &idade);
            limparBuffer();

            cadastrarLeitor(cpf, nome, endereco, telefone, idade);
            printf("Leitor cadastrado com sucesso!\n");
            break;
        }
        case 2:
        {
            char titulo[100], autor[100];
            int anoPublicacao, classificacao;

            printf("Digite o titulo do livro: ");
            fgets(titulo, sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = '\0';

            printf("Digite o autor do livro: ");
            fgets(autor, sizeof(autor), stdin);
            autor[strcspn(autor, "\n")] = '\0';

            printf("Digite o ano de publicacao do livro: ");
            scanf("%d", &anoPublicacao);
            limparBuffer();

            printf("Digite a classificacao indicativa do livro: ");
            scanf("%d", &classificacao);
            limparBuffer();

            cadastrarLivro(titulo, autor, anoPublicacao, classificacao);
            printf("Livro cadastrado com sucesso!\n");
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
            char titulo[100];
            printf("Digite o titulo do livro: ");
            fgets(titulo, sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = '\0';

            buscarLivro(titulo);
            break;
        }
        case 6:
        {
            int id_livro, id_cliente;
            printf("Digite o ID do livro: ");
            scanf("%d", &id_livro);
            limparBuffer();

            printf("Digite o ID do cliente: ");
            scanf("%d", &id_cliente);
            limparBuffer();

            emprestarLivro(id_livro, id_cliente);
            break;
        }
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida!\n");
            break;
        }
    } while (opcao != 0);

    return 0;
}
