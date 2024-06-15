# Documentação do projeto

## Integrantes do grupo

Bruno Freitas Brandão, Gabriel Henrique Vieira Timóteo, Gustavo Prado Bezerra de Araujo, Enzo Gabriel Artegiani

## Visão do Produto

O projeto "Biblioteca Para Todos" visa desenvolver um sistema básico para gerenciamento de bibliotecas públicas ou acadêmicas utilizando a linguagem de programação C. O sistema permitirá o cadastro, consulta e gerenciamento de livros e usuários, facilitando a administração de uma pequena biblioteca acadêmica.

## Frase de elevador (Biblioteca para Todos)

Para bibliotecas públicas ou acadêmicas que precisam de uma solução eficiente para gerenciar seus acervos, o Biblioteca Para Todos é um sistema desenvolvido em C que permite o cadastro, consulta e gerenciamento de livros e usuários, além de facilitar o controle de empréstimos e devoluções. Diferente de outros sistemas de gestão de bibliotecas, o Biblioteca Para Todos oferece uma interface simples e intuitiva, além de ser gratuito, proporcionando uma administração mais organizada e produtiva.

## Product Backlog

[Product Backlog - Biblioteca para Todos](arquivos/product_backlog.xlsx)

## Diagrama de casos de uso

![Diagrama de casos de uso](arquivos/diagrama_casos_de_uso.jpeg)

## Descrição de Fluxos de Eventos

### Nome: Empréstimo de Livro

### Fluxo de Eventos Normal

O cliente insere seu ID.

O programa analisa o ID e verifica se ele é aceitável.

O programa solicita os nomes dos livros.

O cliente informa o nome do livro.

O programa analisa o nome do livro e verifica se ele é aceitável.

O programa solicita a quantidades de livros que o cliente deseja.

O cliente informa a quantidade de livros que ele deseja fazer o empréstimo.

O programa analisa a quantidade de livros e verifica se ele é aceitável.

O cliente seleciona a opção empréstimo de livro.

O programa realiza uma requisição para que os livros escolhidos sejam enviados para o cliente.

Os livros são liberados para o cliente.

### Fluxo de Eventos de Exceção

2a – O ID não é aceitável: Seja porque os caracteres informados são incompatíveis ou aquele ID não foi cadastrado, será envia uma mensagem de erro, com a mensagem “ID incorreto” ou “ID não cadastrado”, e retorna ao passo 1.

5a – O nome não é aceitável: Se o nome informado esteja incorreto, será enviado a mensagem “Livro não encontrado”, e retorna ao passo 4.

8a – A quantidade não é aceitável: Se a quantidade de livros passar do limite, será enviado uma mensagem “Quantidade excedida”, o cliente retorna ao passo 7 e reduz a quantidade de livros.

10a – Empréstimo não autorizado: Uma mensagem de erro é enviada ao cliente, “erro ao realizar empréstimo”, e retorna à opção 9.

1 a 9 – Cancelamento: O cliente pode cancelar o empréstimo dos livros, enquanto o empréstimo não for autorizado pelo programa.

### Nome: Devolver Livro

### Fluxo de Eventos Normal

O cliente insere seu ID.

O programa analisa o ID e verifica se ele é aceitável.

O cliente informa a quantidade de livros que ele deseja devolver.

O programa analisa se quantidade de livros a serem devolvidos e verifica se ele é aceitável.

O programa solicita os IDs dos livros a serem devolvidos.

O cliente informa os IDs dos livros.

O programa analisa os IDs dos livros e verifica se eles são aceitáveis.

O cliente seleciona a opção devolver livros.

O programa realiza uma requisição para que os livros que o cliente deseja devolver sejam enviados de volta para a biblioteca.

Os livros são liberados para devolução.

### Fluxo de Eventos de Exceção

2a – O ID do cliente não é aceitável: Seja porque os caracteres informados são incompatíveis ou aquele ID não foi cadastrado, será envia uma mensagem de erro, com a mensagem “ID incorreto” ou “ID não cadastrado”, e retorna ao passo 1.

4a – A quantidade não é aceitável: Se a quantidade de livros passar do limite, será enviado uma mensagem “Quantidade excedida”, o cliente retorna ao passo 3 e reduz a quantidade de livros.

7a – O ID do livro não é aceitável: Seja porque os caracteres informados são incompatíveis ou aquele ID não foi cadastrado, será envia uma mensagem de erro, com a mensagem “ID do livro incorreto” ou “ID do livro não cadastrado”, e retorna ao passo 6.

9a – Devolução não autorizado: Uma mensagem de erro é enviada ao cliente, “erro ao realizar Devolução”, e retorna à opção 8.

1 a 8 – Cancelamento: O cliente pode cancelar a devolução dos livros, enquanto a devolução não for autorizado pelo programa.

## Diagrama de atividades

![Diagrama de Atividades](arquivos/diagrama_atividades.pdf)

## Guia do Usuário

1- O software é de código aberto e está disponível para importação gratuita via github, assim como toda sua documentação.
2- Após importado, o projeto estará pronto pra uso, cabendo ao usuário verificar a lista de usuários assim como suas devidas credenciais para acesso e login no sistema.
3- O sistema apresenta inúmeras funções para gerenciamento de uma biblioteca física, tais como:

![Diagrama de casos de uso](arquivos/diagrama_casos_de_uso.jpeg)

4- Para melhor uso do nosso sistema, certifique-se de estar inserindo valores válidos nos campos. Caso contrário, o sistema não permitirá a inserção desses valores.

5- Faça bom proveito!