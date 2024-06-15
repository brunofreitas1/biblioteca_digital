#ifndef UNTITLED_BIBLIOTECA_H
#define UNTITLED_BIBLIOTECA_H

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
    int disponibilidade; // 0 = indisponível | 1 = disponível
};

struct Emprestimo
{
    int id_leitor;
    int id_livro;
    time_t dataEmprestimo;
};

void exibirMensagem(const char *mensagem, const char *cor);
int gerarID(const char *tipo);
void carregarID();
void salvarID();
void limparBuffer();
bool validarCPF(const char *cpf);
bool validarTelefone(const char *telefone);
bool validarIdade(int idade);
bool validarAnoPublicacao(int ano);
void lerSenha(char *senha, size_t tamanho);
bool verificarCredenciais(const char *usuario, const char *senha);
int gerarID(const char *tipo);
void cadastrarLeitor(const char *cpf, const char *nome, const char *endereco, const char *telefone, int idade);
void listarLeitores();
void cadastrarLivro(const char *titulo, const char *autor, int anoPublicacao, int classificacao);
void listarLivros();
void buscarLivro(const char *titulo);
bool verificarDisponibilidadeLivro(int id_livro);
int validarClassificacaoLeitor(int id_leitor, int idLivro);
void emprestarLivro(int id_livro, int id_leitor);
void devolverLivro(int id_livro, int id_leitor);
void listarEmprestimos();
void centralDeAvisos();

#endif