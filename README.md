# Tarefas de Banco de Dados II em C

Este repositório contém duas aplicações desenvolvidas em C para a disciplina de Banco de Dados II, focadas na manipulação de arquivos de dados binários.

---

## Leitor de Dados com Cabeçalho Embutido (`leitor-dat`)

Este programa lê e exibe dados de arquivos `.dat` cujo esquema de metadados está contido no próprio cabeçalho do arquivo.

### Funcionalidades

-   Interpreta um cabeçalho binário para determinar os atributos, tipos (`S`, `C`, `I`, `F`) e tamanhos.
-   Lê os registos de dados sequencialmente após o cabeçalho.
-   Exibe os dados em formato tabular na consola.

### Compilação e Execução

É necessário um compilador C, como o GCC.

```bash
# Navegue para o diretório do projeto
cd leitor-dat/

# Compile o código-fonte
gcc main.c -o leitor

# Execute, fornecendo o nome do arquivo de dados como argumento
./leitor customer.dat
```

---

## Simulador de SELECT * (`simulador-select`)

Esta aplicação simula a operação `SELECT *` de um SGBD, utilizando arquivos de dicionário de dados (`.dic`) para obter os metadados e exibir o conteúdo de uma tabela.

### Funcionalidades

- Consulta `table.dic` para mapear um nome lógico de tabela a um arquivo físico (`.dat`).
- Consulta `att.dic` para obter o esquema de atributos (colunas) da tabela especificada.
- Lê o arquivo de dados correspondente e exibe os registos de acordo com o esquema obtido.

### Compilação e Execução

Os arquivos `table.dic`, `att.dic` e os respetivos arquivos `.dat` devem estar no mesmo diretório do executável.

```bash
# Navegue para o diretório do projeto
cd simulador-select/

# Compile o código-fonte
gcc main.c -o select_sim

# Execute, fornecendo o nome lógico da tabela como argumento
./select_sim product
```
