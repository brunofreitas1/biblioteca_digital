#include "biblioteca.h"

/*define de maneira estática o id inicial tanto de
livros como de leitores*/
static int id_leitor = 0;
static int id_livro = 0;

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

int gerarID(const char *tipo)
{
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

void carregarID()
{
    FILE *file = fopen("ids.txt", "r");
    if (file != NULL)
    {
        fscanf(file, "%d %d", &id_leitor, &id_livro);
        fclose(file);
    }
}

void salvarID()
{
    FILE *file = fopen("ids.txt", "w");
    if (file != NULL)
    {
        fprintf(file, "%d %d", id_leitor, id_livro);
        fclose(file);
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
        exibirMensagem("Erro ao abrir o arquivo de usuarios.", RED_COLOR);
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

// Função para cadastrar leitores no sistema
void cadastrarLeitor(const char *cpf, const char *nome, const char *endereco, const char *telefone, int idade)
{
    FILE *file = fopen("leitores.txt", "a");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de leitores.", "\x1b[31m"); // Red color
        return;
    }

    struct Leitor leitor;
    leitor.id = gerarID("leitor");
    strncpy(leitor.cpf, cpf, sizeof(leitor.cpf) - 1);
    strncpy(leitor.nome, nome, sizeof(leitor.nome) - 1);
    strncpy(leitor.endereco, endereco, sizeof(leitor.endereco) - 1);
    strncpy(leitor.telefone, telefone, sizeof(leitor.telefone) - 1);
    leitor.idade = idade;

    fprintf(file, "%d;%s;%s;%s;%s;%d\n", leitor.id, leitor.cpf, leitor.nome, leitor.endereco, leitor.telefone, leitor.idade);
    fclose(file);

    salvarID(); // Salva o novo valor do ID após o cadastro
}

// Lista os leitores cadastrados
void listarLeitores()
{
    FILE *file = fopen("leitores.txt", "r");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de leitores.", "\x1b[31m"); // Red color
        return;
    }

    printf("\nLISTA DE LEITORES\n");
    printf("=================\n");

    char linha[256];
    struct Leitor leitor;

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        sscanf(linha, "%d;%11[^;];%99[^;];%99[^;];%14[^;];%d\n",
               &leitor.id, leitor.cpf, leitor.nome, leitor.endereco, leitor.telefone, &leitor.idade);
        printf("ID: %d, CPF: %s, Nome: %s, Endereco: %s, Telefone: %s, Idade: %d\n",
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
        exibirMensagem("Erro ao abrir o arquivo de livros.", "\x1b[31m"); // Red color
        return;
    }

    struct Livro livro;
    livro.id = gerarID("livro");
    strncpy(livro.titulo, titulo, sizeof(livro.titulo) - 1);
    strncpy(livro.autor, autor, sizeof(livro.autor) - 1);
    livro.anoPublicacao = anoPublicacao;
    livro.classificacao = classificacao;
    livro.disponibilidade = 1;

    fprintf(file, "%d;%s;%s;%d;%d;%d\n", livro.id, livro.titulo, livro.autor, livro.anoPublicacao, livro.classificacao, livro.disponibilidade);
    fclose(file);

    salvarID(); // Salva o novo valor do ID após o cadastro
}

// Função para listar todos os livros
void listarLivros()
{
    FILE *file = fopen("livros.txt", "r");
    if (file == NULL)
    {
        exibirMensagem("Erro ao abrir o arquivo de livros.", "\x1b[31m"); // Red color
        return;
    }

    printf("\nLISTA DE LIVROS\n");
    printf("===============\n");

    char linha[256];
    struct Livro livro;

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        sscanf(linha, "%d;%99[^;];%99[^;];%d;%d;%d\n",
               &livro.id, livro.titulo, livro.autor, &livro.anoPublicacao, &livro.classificacao, &livro.disponibilidade);
        printf("ID: %d, Titulo: %s, Autor: %s, Ano de Publicacao: %d, Classificacao: %d, Disponibilidade: %d\n",
               livro.id, livro.titulo, livro.autor, livro.anoPublicacao, livro.classificacao, livro.disponibilidade);
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
int validarClassificacaoLeitor(int idLeitor, int idLivro)
{
    FILE *fileLeitores = fopen("leitores.txt", "r");
    FILE *fileLivros = fopen("livros.txt", "r");

    if (fileLeitores == NULL || fileLivros == NULL)
    {
        exibirMensagem("Erro ao abrir arquivos para validação.", "\x1b[31m"); // Red color
        if (fileLeitores != NULL)
            fclose(fileLeitores);
        if (fileLivros != NULL)
            fclose(fileLivros);
        return 0;
    }

    struct Leitor leitor;
    struct Livro livro;
    char linha[256];
    int idade = -1, classificacao = -1;

    while (fgets(linha, sizeof(linha), fileLeitores) != NULL)
    {
        sscanf(linha, "%d;%11[^;];%99[^;];%99[^;];%14[^;];%d\n",
               &leitor.id, leitor.cpf, leitor.nome, leitor.endereco, leitor.telefone, &leitor.idade);
        if (leitor.id == idLeitor)
        {
            idade = leitor.idade;
            break;
        }
    }

    while (fgets(linha, sizeof(linha), fileLivros) != NULL)
    {
        sscanf(linha, "%d;%99[^;];%99[^;];%d;%d;%d\n",
               &livro.id, livro.titulo, livro.autor, &livro.anoPublicacao, &livro.classificacao, &livro.disponibilidade);
        if (livro.id == idLivro)
        {
            classificacao = livro.classificacao;
            break;
        }
    }

    fclose(fileLeitores);
    fclose(fileLivros);

    if (idade >= classificacao)
    {
        return 1;
    }
    else
    {
        exibirMensagem("Classificação indicativa não atendida.", "\x1b[31m"); // Red color
        return 0;
    }
}

// Função para registrar um empréstimo
void emprestarLivro(int id_livro, int id_leitor)
{
    if (!verificarDisponibilidadeLivro(id_livro))
    {
        exibirMensagem("Livro não disponivel para emprestimo.", RED_COLOR);
        return;
    }

    FILE *fileLivros = fopen("livros.txt", "r+");
    FILE *fileEmprestimos = fopen("emprestimos.txt", "a");
    if (fileLivros == NULL || fileEmprestimos == NULL)
    {
        exibirMensagem("Erro ao abrir os arquivos necessarios.", RED_COLOR);
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
        exibirMensagem("Erro ao abrir os arquivos necessarios.", RED_COLOR);
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
        exibirMensagem("Erro ao criar arquivo temporario.", RED_COLOR);
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
        exibirMensagem("Emprestimo nao encontrado.", RED_COLOR);
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
        exibirMensagem("Erro ao abrir o arquivo de emprestimos.", RED_COLOR);
        return;
    }

    printf("\nLISTA DE EMPRESTIMOS\n");
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
        printf("ID do Leitor: %d, ID do Livro: %d, Data do Emprestimo: %s\n",
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
        exibirMensagem("Erro ao abrir o arquivo de emprestimos.", RED_COLOR);
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
        // Calculo para exibir a diferença em dias
        double diferencaDias = difftime(dataAtual, emprestimo.dataEmprestimo) / (60 * 60 * 24);
        if (diferencaDias > 0) // valor original = 90
        {
            double multa = (diferencaDias - 10) * 0.25;
            exibirMensagem("Emprestimo vencido! Multa aplicada.", YELLOW_COLOR);
            printf("ID do Leitor: %d, ID do Livro: %d, Multa: R$%.2f\n", emprestimo.id_leitor, emprestimo.id_livro, multa);
        }
        else if (diferencaDias > 0) // valor original = 75
        {
            exibirMensagem("Aviso: O prazo de devolucao esta proximo.", YELLOW_COLOR);
            printf("ID do Leitor: %d, ID do Livro: %d, Dias restantes: %.0f\n", emprestimo.id_leitor, emprestimo.id_livro, 90 - diferencaDias);
        }
    }

    fclose(file);
}