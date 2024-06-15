#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.c"

/*Função main utiliza dass bibliotecas modularizadas para acesso as
diferentes funções da aplicação*/
int main()
{

    char usuario[100], senha[100];

    printf("Digite o usuario: ");
    fgets(usuario, sizeof(usuario), stdin);
    usuario[strcspn(usuario, "\n")] = '\0';

    printf("Digite a senha: ");
    lerSenha(senha, sizeof(senha));

    if (!verificarCredenciais(usuario, senha))
    {
        exibirMensagem("Usuario ou senha invalidos!", RED_COLOR);
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
        printf("5. Buscar Livro por Titulo\n");
        printf("6. Emprestar Livro\n");
        printf("7. Devolver Livro\n");
        printf("8. Listar Emprestimos\n");
        printf("9. Sair\n" RESET_COLOR);
        printf("Digite a sua opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao)
        {
        case 1:
        {
            char cpf[12], nome[100], endereco[100], telefone[15];
            int idade;

            printf("Digite o CPF: ");
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = '\0';
            limparBuffer();

            printf("Digite o nome: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = '\0';

            printf("Digite o endereco: ");
            fgets(endereco, sizeof(endereco), stdin);
            endereco[strcspn(endereco, "\n")] = '\0';

            printf("Digite o telefone: ");
            fgets(telefone, sizeof(telefone), stdin);
            telefone[strcspn(telefone, "\n")] = '\0';

            printf("Digite a idade: ");
            scanf("%d", &idade);
            limparBuffer();

            cadastrarLeitor(cpf, nome, endereco, telefone, idade);
            break;
        }
        case 2:
            listarLeitores();
            break;
        case 3:
        {
            char titulo[100], autor[100];
            int anoPublicacao, classificacao;

            printf("Digite o titulo: ");
            fgets(titulo, sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = '\0';

            printf("Digite o autor: ");
            fgets(autor, sizeof(autor), stdin);
            autor[strcspn(autor, "\n")] = '\0';

            printf("Digite o ano de publicacao: ");
            scanf("%d", &anoPublicacao);
            limparBuffer();

            printf("Digite a classificacao indicativa: ");
            scanf("%d", &classificacao);
            limparBuffer();

            cadastrarLivro(titulo, autor, anoPublicacao, classificacao);
            break;
        }
        case 4:
            listarLivros();
            break;
        case 5:
        {
            int idLeitor, idLivro;
            printf("Digite o ID do Leitor: ");
            scanf("%d", &idLeitor);
            limparBuffer();

            printf("Digite o ID do Livro: ");
            scanf("%d", &idLivro);
            limparBuffer();

            if (validarClassificacaoLeitor(idLeitor, idLivro))
            {
                exibirMensagem("Classificação válida. Empréstimo permitido.", "\x1b[32m"); // Green color
            }
            else
            {
                exibirMensagem("Classificação inválida. Empréstimo não permitido.", "\x1b[31m"); // Red color
            }
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
            exibirMensagem("Opcao invalida. Tente novamente.", RED_COLOR);
            break;
        }
    } while (opcao != 9);

    return 0;
}