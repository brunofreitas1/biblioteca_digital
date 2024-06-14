#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>
#include <ctype.h>

#define RED_COLOR "\033[1;31m"
#define GREEN_COLOR "\033[1;32m"
#define YELLOW_COLOR "\033[1;33m"
#define CYAN_COLOR "\033[1;36m"
#define RESET_COLOR "\033[0m"

// Função para exibir mensagens coloridas
void exibirMensagem(const char *mensagem, const char *cor)
{
    printf("%s%s%s\n", cor, mensagem, RESET_COLOR);
}

// Função para limpar o buffer do teclado
void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

// Função para validar CPF (somente números e deve ter 11 dígitos)
bool validarCPF(const char *cpf)
{
    if (strlen(cpf) != 11)
        return false;
    for (int i = 0; i < 11; i++)
    {
        if (!isdigit(cpf[i]))
            return false;
    }
    return true;
}

// Função para validar telefone (somente números e deve ter pelo menos 11 dígitos)
bool validarTelefone(const char *telefone)
{
    if (strlen(telefone) != 11)
        return false;
    for (int i = 0; i < 11; i++)
    {
        if (!isdigit(telefone[i]))
            return false;
    }
    return true;
}

// Função para validar idade (deve ser um número entre 0 e 120)
bool validarIdade(int idade)
{
    return idade >= 0 && idade <= 120;
}

// Função para validar ano de publicação (deve ser um número positivo)
bool validarAnoPublicacao(int ano)
{
    return ano > 0;
}

// Função para ler a senha sem exibi-la
void lerSenha(char *senha, size_t tamanho)
{
    char c;
    size_t i = 0;
    while ((c = getch()) != '\r' && i < tamanho - 1)
    {
        if (c == '\b' && i > 0)
        {
            printf("\b \b");
            i--;
        }
        else if (c != '\b')
        {
            senha[i++] = c;
            printf("*");
        }
    }
    senha[i] = '\0';
    printf("\n");
}

// Função para verificar as credenciais de login
bool verificarCredenciais(const char *usuario, const char *senha)
{
    FILE *file = fopen("usuarios.txt", "r");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de usuários.", RED_COLOR);
        return false;
    }

    char linha[256];
    char usuarioArq[100], senhaArq[100];
    bool credenciaisValidas = false;

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        sscanf(linha, "%99[^;];%99[^\n]\n", usuarioArq, senhaArq);
        if (strcmp(usuario, usuarioArq) == 0 && strcmp(senha, senhaArq) == 0)
        {
            credenciaisValidas = true;
            break;
        }
    }

    fclose(file);
    return credenciaisValidas;
}

// Estruturas de dados para leitores e livros
struct Leitor
{
    int id;
    char cpf[12];
    char nome[100];
    char endereco[100];
    char telefone[12];
    int idade;
};

struct Livro
{
    int id;
    char titulo[100];
    char autor[100];
    int anoPublicacao;
    int classificacao;
    int disponibilidade;
};

struct Emprestimo
{
    int id_leitor;
    int id_livro;
    time_t dataEmprestimo;
};

// Função para gerar IDs únicos
int gerarID(const char *tipo)
{
    static int id_leitor = 0;
    static int id_livro = 0;

    if (strcmp(tipo, "leitor") == 0)
    {
        return ++id_leitor;
    }
    else if (strcmp(tipo, "livro") == 0)
    {
        return ++id_livro;
    }

    return -1;
}

// Função para cadastrar um leitor
void cadastrarLeitor(const char *cpf, const char *nome, const char *endereco, const char *telefone, int idade)
{
    FILE *file = fopen("leitores.txt", "a");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de leitores.", RED_COLOR);
        return;
    }

    struct Leitor leitor;
    leitor.id = gerarID("leitor");
    strcpy(leitor.cpf, cpf);
    strcpy(leitor.nome, nome);
    strcpy(leitor.endereco, endereco);
    strcpy(leitor.telefone, telefone);
    leitor.idade = idade;

    fprintf(file, "%d;%s;%s;%s;%s;%d\n", leitor.id, leitor.cpf, leitor.nome, leitor.endereco, leitor.telefone, leitor.idade);
    fclose(file);
}

// Função para listar todos os leitores
void listarLeitores()
{
    FILE *file = fopen("leitores.txt", "r");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de leitores.", RED_COLOR);
        return;
    }

    printf("\nLISTA DE LEITORES\n");
    printf("=================\n");

    char linha[256];
    struct Leitor leitor;

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        sscanf(linha, "%d;%d;%99[^;];%99[^;];%d;%d\n", &leitor.id, &leitor.cpf, leitor.nome, leitor.endereco, &leitor.telefone, &leitor.idade);
        printf("ID: %d, CPF: %d, Nome: %s, Endereço: %s, Telefone: %d, Idade: %d\n",
               leitor.id, leitor.cpf, leitor.nome, leitor.endereco, leitor.telefone, leitor.idade);
    }

    fclose(file);
}

// Função para cadastrar um livro
void cadastrarLivro(const char *titulo, const char *autor, int anoPublicacao, int classificacao)
{
    FILE *file = fopen("livros.txt", "a");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de livros.", RED_COLOR);
        return;
    }

    struct Livro livro;
    livro.id = gerarID("livro");
    strcpy(livro.titulo, titulo);
    strcpy(livro.autor, autor);
    livro.anoPublicacao = anoPublicacao;
    livro.classificacao = classificacao;
    livro.disponibilidade = 1;

    fprintf(file, "%d;%s;%s;%d;%d;%d\n", livro.id, livro.titulo, livro.autor, livro.anoPublicacao, livro.classificacao, livro.disponibilidade);
    fclose(file);
}

// Função para listar todos os livros
void listarLivros()
{
    FILE *file = fopen("livros.txt", "r");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de livros.", RED_COLOR);
        return;
    }

    printf("\nLISTA DE LIVROS\n");
    printf("===============\n");

    char linha[256];
    struct Livro livro;

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        sscanf(linha, "%d;%99[^;];%99[^;];%d;%d;%d\n", &livro.id, livro.titulo, livro.autor,
               &livro.anoPublicacao, &livro.classificacao, &livro.disponibilidade);
        printf("ID: %d, Título: %s, Autor: %s, Ano: %d, Classificação: %d, Disponível: %s\n",
               livro.id, livro.titulo, livro.autor, livro.anoPublicacao, livro.classificacao,
               livro.disponibilidade ? "Sim" : "Não");
    }

    fclose(file);
}

// Função para buscar um livro por título
void buscarLivro(const char *titulo)
{
    FILE *file = fopen("livros.txt", "r");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de livros.", RED_COLOR);
        return;
    }

    char linha[256];
    struct Livro livro;
    bool encontrado = false;

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        sscanf(linha, "%d;%99[^;];%99[^;];%d;%d;%d\n", &livro.id, livro.titulo, livro.autor,
               &livro.anoPublicacao, &livro.classificacao, &livro.disponibilidade);
        if (strcmp(livro.titulo, titulo) == 0)
        {
            printf("ID: %d, Título: %s, Autor: %s, Ano: %d, Classificação: %d, Disponível: %s\n",
                   livro.id, livro.titulo, livro.autor, livro.anoPublicacao, livro.classificacao,
                   livro.disponibilidade ? "Sim" : "Não");
            encontrado = true;
            break;
        }
    }

    if (!encontrado)
    {
        exibirMensagem("Livro não encontrado!", RED_COLOR);
    }

    fclose(file);
}

// Função para verificar se um livro está disponível
bool verificarDisponibilidadeLivro(int id_livro)
{
    FILE *file = fopen("livros.txt", "r");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de livros.", RED_COLOR);
        return false;
    }

    char linha[256];
    struct Livro livro;

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        sscanf(linha, "%d;%99[^;];%99[^;];%d;%d;%d\n", &livro.id, livro.titulo, livro.autor,
               &livro.anoPublicacao, &livro.classificacao, &livro.disponibilidade);
        if (livro.id == id_livro && livro.disponibilidade == 1)
        {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}

// Função para validar a classificação indicativa do leitor
bool validarClassificacaoLeitor(int id_leitor, int classificacaoLivro)
{
    FILE *file = fopen("leitores.txt", "r");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de leitores.", RED_COLOR);
        return false;
    }

    char linha[256];
    struct Leitor leitor;

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        sscanf(linha, "%d;%d;%99[^;];%99[^;];%d;%d\n", &leitor.id, &leitor.cpf, leitor.nome,
               leitor.endereco, &leitor.telefone, &leitor.idade);
        if (leitor.id == id_leitor)
        {
            if (leitor.idade >= classificacaoLivro)
            {
                fclose(file);
                return true;
            }
            else
            {
                exibirMensagem("Leitor não possui idade suficiente para esta classificação indicativa.", RED_COLOR);
                fclose(file);
                return false;
            }
        }
    }

    fclose(file);
    return false;
}

// Função para registrar um empréstimo
void emprestarLivro(int id_livro, int id_leitor)
{
    if (!verificarDisponibilidadeLivro(id_livro))
    {
        exibirMensagem("Livro não disponível para empréstimo.", RED_COLOR);
        return;
    }

    FILE *fileLivros = fopen("livros.txt", "r+");
    FILE *fileEmprestimos = fopen("emprestimos.txt", "a");
    if (fileLivros == NULL || fileEmprestimos == NULL)
    {
        exibirMensagem("Erro ao abrir os arquivos necessários.", RED_COLOR);
        return;
    }

    char linha[256];
    struct Livro livro;
    long pos;

    while (fgets(linha, sizeof(linha), fileLivros) != NULL)
    {
        pos = ftell(fileLivros);
        sscanf(linha, "%d;%99[^;];%99[^;];%d;%d;%d\n", &livro.id, livro.titulo, livro.autor,
               &livro.anoPublicacao, &livro.classificacao, &livro.disponibilidade);
        if (livro.id == id_livro)
        {
            if (!validarClassificacaoLeitor(id_leitor, livro.classificacao))
            {
                fclose(fileLivros);
                fclose(fileEmprestimos);
                return;
            }

            livro.disponibilidade = 0;
            fseek(fileLivros, pos - strlen(linha), SEEK_SET);
            fprintf(fileLivros, "%d;%s;%s;%d;%d;%d\n", livro.id, livro.titulo, livro.autor,
                    livro.anoPublicacao, livro.classificacao, livro.disponibilidade);
            break;
        }
    }

    struct Emprestimo emprestimo;
    emprestimo.id_leitor = id_leitor;
    emprestimo.id_livro = id_livro;
    time(&emprestimo.dataEmprestimo);

    fprintf(fileEmprestimos, "%d;%d;%ld\n", emprestimo.id_leitor, emprestimo.id_livro, emprestimo.dataEmprestimo);

    fclose(fileLivros);
    fclose(fileEmprestimos);

    exibirMensagem("Livro emprestado com sucesso!", GREEN_COLOR);
}

// Função para registrar a devolução de um livro
void devolverLivro(int id_livro, int id_leitor)
{
    FILE *fileLivros = fopen("livros.txt", "r+");
    FILE *fileEmprestimos = fopen("emprestimos.txt", "r");
    if (fileLivros == NULL || fileEmprestimos == NULL)
    {
        exibirMensagem("Erro ao abrir os arquivos necessários.", RED_COLOR);
        return;
    }

    char linha[256];
    struct Livro livro;
    struct Emprestimo emprestimo;
    bool emprestimoEncontrado = false;
    long pos;

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        exibirMensagem("Erro ao criar arquivo temporário.", RED_COLOR);
        fclose(fileLivros);
        fclose(fileEmprestimos);
        return;
    }

    // Remover o empréstimo do arquivo de empréstimos
    while (fgets(linha, sizeof(linha), fileEmprestimos) != NULL)
    {
        sscanf(linha, "%d;%d;%ld\n", &emprestimo.id_leitor, &emprestimo.id_livro, &emprestimo.dataEmprestimo);
        if (emprestimo.id_leitor == id_leitor && emprestimo.id_livro == id_livro)
        {
            emprestimoEncontrado = true;
        }
        else
        {
            fputs(linha, tempFile);
        }
    }

    fclose(fileEmprestimos);
    fclose(tempFile);

    remove("emprestimos.txt");
    rename("temp.txt", "emprestimos.txt");

    if (!emprestimoEncontrado)
    {
        exibirMensagem("Empréstimo não encontrado.", RED_COLOR);
        fclose(fileLivros);
        return;
    }

    // Atualizar a disponibilidade do livro
    while (fgets(linha, sizeof(linha), fileLivros) != NULL)
    {
        pos = ftell(fileLivros);
        sscanf(linha, "%d;%99[^;];%99[^;];%d;%d;%d\n", &livro.id, livro.titulo, livro.autor,
               &livro.anoPublicacao, &livro.classificacao, &livro.disponibilidade);
        if (livro.id == id_livro)
        {
            livro.disponibilidade = 1;
            fseek(fileLivros, pos - strlen(linha), SEEK_SET);
            fprintf(fileLivros, "%d;%s;%s;%d;%d;%d\n", livro.id, livro.titulo, livro.autor,
                    livro.anoPublicacao, livro.classificacao, livro.disponibilidade);
            break;
        }
    }

    fclose(fileLivros);

    // Calcular multa
    time_t dataAtual;
    time(&dataAtual);
    double diferencaDias = difftime(dataAtual, emprestimo.dataEmprestimo) / (60 * 60 * 24);
    if (diferencaDias > 90)
    {
        double multa = (diferencaDias - 90) * 0.25;
        printf("Multa por atraso: R$%.2f\n", multa);
    }

    exibirMensagem("Livro devolvido com sucesso!", GREEN_COLOR);
}

// Função para listar todos os empréstimos
void listarEmprestimos()
{
    FILE *file = fopen("emprestimos.txt", "r");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de empréstimos.", RED_COLOR);
        return;
    }

    printf("\nLISTA DE EMPRÉSTIMOS\n");
    printf("====================\n");

    char linha[256];
    struct Emprestimo emprestimo;
    struct tm *data;
    char dataStr[11];

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        sscanf(linha, "%d;%d;%ld\n", &emprestimo.id_leitor, &emprestimo.id_livro, &emprestimo.dataEmprestimo);
        data = localtime(&emprestimo.dataEmprestimo);
        strftime(dataStr, sizeof(dataStr), "%d/%m/%Y", data);
        printf("ID do Leitor: %d, ID do Livro: %d, Data do Empréstimo: %s\n",
               emprestimo.id_leitor, emprestimo.id_livro, dataStr);
    }

    fclose(file);
}

// Central de avisos para prazos de devolução e multas
void centralDeAvisos()
{
    FILE *file = fopen("emprestimos.txt", "r");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de empréstimos.", RED_COLOR);
        return;
    }

    char linha[256];
    struct Emprestimo emprestimo;
    time_t dataAtual;
    time(&dataAtual);

    printf("\nCENTRAL DE AVISOS\n");
    printf("=================\n");

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        sscanf(linha, "%d;%d;%ld\n", &emprestimo.id_leitor, &emprestimo.id_livro, &emprestimo.dataEmprestimo);
        double diferencaDias = difftime(dataAtual, emprestimo.dataEmprestimo) / (60 * 60 * 24);
        if (diferencaDias > 90)
        {
            double multa = (diferencaDias - 90) * 0.25;
            exibirMensagem("Empréstimo vencido! Multa aplicada.", YELLOW_COLOR);
            printf("ID do Leitor: %d, ID do Livro: %d, Multa: R$%.2f\n", emprestimo.id_leitor, emprestimo.id_livro, multa);
        }
        else if (diferencaDias > 75)
        {
            exibirMensagem("Aviso: O prazo de devolução está próximo.", YELLOW_COLOR);
            printf("ID do Leitor: %d, ID do Livro: %d, Dias restantes: %.0f\n", emprestimo.id_leitor, emprestimo.id_livro, 90 - diferencaDias);
        }
    }

    fclose(file);
}

// Função principal
int main()
{
    char usuario[100], senha[100];

    printf("Digite o usuário: ");
    fgets(usuario, sizeof(usuario), stdin);
    usuario[strcspn(usuario, "\n")] = '\0';

    printf("Digite a senha: ");
    lerSenha(senha, sizeof(senha));

    if (!verificarCredenciais(usuario, senha))
    {
        exibirMensagem("Usuário ou senha inválidos!", RED_COLOR);
        return 1;
    }

    int opcao;
    do
    {
        centralDeAvisos();
        printf(CYAN_COLOR "\nMENU\n");
        printf("1. Cadastrar Leitor\n");
        printf("2. Listar Leitores\n");
        printf("3. Cadastrar Livro\n");
        printf("4. Listar Livros\n");
        printf("5. Buscar Livro por Título\n");
        printf("6. Emprestar Livro\n");
        printf("7. Devolver Livro\n");
        printf("8. Listar Empréstimos\n");
        printf("9. Sair\n" RESET_COLOR);
        printf("Digite a sua opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao)
        {
        case 1:
        {
            char cpf[12];
            char telefone[12];
            int idade;
            char nome[100], endereco[100];

            printf("Digite o CPF (apenas números): ");
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = '\0'; // Remover o caractere de nova linha
            if (!validarCPF(cpf))
            {
                exibirMensagem("CPF inválido. Certifique-se de digitar 11 números.", RED_COLOR);
                break;
            }
            limparBuffer();
            printf("Digite o nome: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = '\0';
            if (strlen(nome) == 0)
            {
                exibirMensagem("Nome não pode ser vazio.", RED_COLOR);
                break;
            }

            printf("Digite o endereço: ");
            fgets(endereco, sizeof(endereco), stdin);
            endereco[strcspn(endereco, "\n")] = '\0';
            if (strlen(endereco) == 0)
            {
                exibirMensagem("Endereço não pode ser vazio.", RED_COLOR);
                break;
            }

            printf("Digite o telefone (apenas números): ");
            fgets(telefone, sizeof(telefone), stdin);
            telefone[strcspn(telefone, "\n")] = '\0'; // Remover o caractere de nova linha
            if (!validarTelefone((const char *)&telefone))
            {
                exibirMensagem("Telefone inválido. Certifique-se de digitar pelo menos 11 números.", RED_COLOR);
                break;
            }

            printf("Digite a idade: ");
            scanf("%d", &idade);
            limparBuffer();
            if (!validarIdade(idade))
            {
                exibirMensagem("Idade inválida. Deve ser entre 0 e 120.", RED_COLOR);
                break;
            }

            cadastrarLeitor(cpf, nome, endereco, telefone, idade);
            exibirMensagem("Leitor cadastrado com sucesso!", GREEN_COLOR);
            break;
        }
        case 2:
            listarLeitores();
            break;
        case 3:
        {
            char titulo[100], autor[100];
            int anoPublicacao, classificacao;

            printf("Digite o título: ");
            fgets(titulo, sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = '\0';
            if (strlen(titulo) == 0)
            {
                exibirMensagem("Título não pode ser vazio.", RED_COLOR);
                break;
            }

            printf("Digite o autor: ");
            fgets(autor, sizeof(autor), stdin);
            autor[strcspn(autor, "\n")] = '\0';
            if (strlen(autor) == 0)
            {
                exibirMensagem("Autor não pode ser vazio.", RED_COLOR);
                break;
            }

            printf("Digite o ano de publicação: ");
            scanf("%d", &anoPublicacao);
            limparBuffer();
            if (!validarAnoPublicacao(anoPublicacao))
            {
                exibirMensagem("Ano de publicação inválido.", RED_COLOR);
                break;
            }

            printf("Digite a classificação indicativa: ");
            scanf("%d", &classificacao);
            limparBuffer();

            cadastrarLivro(titulo, autor, anoPublicacao, classificacao);
            exibirMensagem("Livro cadastrado com sucesso!", GREEN_COLOR);
            break;
        }
        case 4:
            listarLivros();
            break;
        case 5:
        {
            char titulo[100];
            printf("Digite o título: ");
            fgets(titulo, sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = '\0';
            buscarLivro(titulo);
            break;
        }
        case 6:
        {
            int id_livro, id_leitor;
            printf("Digite o ID do livro: ");
            scanf("%d", &id_livro);
            limparBuffer();
            printf("Digite o ID do leitor: ");
            scanf("%d", &id_leitor);
            limparBuffer();
            emprestarLivro(id_livro, id_leitor);
            break;
        }
        case 7:
        {
            int id_livro, id_leitor;
            printf("Digite o ID do livro: ");
            scanf("%d", &id_livro);
            limparBuffer();
            printf("Digite o ID do leitor: ");
            scanf("%d", &id_leitor);
            limparBuffer();
            devolverLivro(id_livro, id_leitor);
            break;
        }
        case 8:
            listarEmprestimos();
            break;
        case 9:
            exibirMensagem("Saindo do sistema...", YELLOW_COLOR);
            break;
        default:
            exibirMensagem("Opção inválida. Tente novamente.", RED_COLOR);
            break;
        }
    } while (opcao != 9);

    return 0;
}
