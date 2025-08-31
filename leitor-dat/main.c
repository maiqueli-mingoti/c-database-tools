#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// max de atributos e tamanho do nome
#define MAX_ATTR 10
#define MAX_NAME_LEN 16

// struct pra guardar as infos de cada coluna
typedef struct {
    char name[MAX_NAME_LEN];
    char type;
    int size;
} Attr;

// le o cabeçalho do arquivo
int read_header(FILE *file, Attr schema[]) {
    int attr_count = 0;                                 // contador de qts atributos achou
    for (int i = 0; i < 10; i++) {                      // le ate 10 atrr
        long pos = ftell(file);                         // pegamos a posicao atual do ponteiro no arquivo
        if (fread(schema[i].name, 15, 1, file) != 1) {          // tenta ler o name, se n para
            break;
        }
        if (fread(&schema[i].type, 1, 1, file) != 1) {          // tenta ler o type, se n para
            break;
        }
        if (fread(&schema[i].size, sizeof(int), 1, file) != 1) {        // tenta ler o size, se n para
            break;
        }
        schema[i].name[15] = '\0';                  // bota o finalizador de str pra garantir que eh um texto valido

        if (schema[i].name[0] == '#') {             // se o name comeca com # eh o fim do cabecalho
            fseek(file, pos + 1, SEEK_SET);         // volta o ponteiro pra pular o # e comecar nos dados
            break;
        }
        attr_count++;                               // se chegou aqui eh um atributo valido, aí incrementa
    }
    return attr_count;                              // retorna a qtd de atributos encontrados
}

// le e imprime os dados do corpo(?) do arquivo
void print_values(FILE *file, Attr schema[], int attr_count) {
    while (1) {                                     // loop infinito, so sai quando o fread la embaixo falha
        for (int i = 0; i < attr_count; i++) {      // percorre cada coluna do registro
            unsigned char field_buffer[256];        // espaco pra guardar o value que a gente ler do arquivo
            size_t bytes_read = fread(field_buffer, 1, schema[i].size, file);       // le o dado da coluna pro buffer

            if (bytes_read != (size_t)schema[i].size) {         // testa se leu o tamanho certo
                return;
            }

            // verifica o tipo do dado e o que fazer com ele
            switch (schema[i].type) {
            case 'C':
            case 'S': {
                field_buffer[schema[i].size] = '\0';        // bota o finalizador de str para imprimir certo
                printf("%-20s", field_buffer);              // -20s alinha o texto na esquerda dentro do 20 espaços
                break;
            }
            case 'I': {
                int int_value;
                memcpy(&int_value, field_buffer, sizeof(int));      // copia os bytes do buffer pra variavel acima
                printf("%-20d", int_value);
                break;
            }
            case 'F': {
                float float_value;
                memcpy(&float_value, field_buffer, sizeof(float));       // mesma coisa, copia os bytes pra varaivel acima
                printf("%-20.2f", float_value);
                break;
            }
            default:
                break;
            }
        }
        printf("\n");                                           // pulo a linha apos printar todas as colunas
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {                                     // verifica se passou o nome do arquivo de excucao
        fprintf(stderr, "uso: %s <arquivo.dat>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "rb");                  // abre o arquvi no read binary, se n abrir encerra
    if (!file) {
        perror("erro ao abrir o arquivo");
        return EXIT_FAILURE;
    }
    // array para guardar as info das colunas
    Attr schema[MAX_ATTR];
    int attr_count = read_header(file, schema);         // chama a funçao de ler cabeçalho e descbrir as coluna

    if (attr_count == 0) {                              // se n achar coluna, encerra
        printf("nenhum atributo válido encontrado\n");
        fclose(file);
        return EXIT_SUCCESS;
    }

    for (int i = 0; i < attr_count; i++) {              // se achamos colunas, printamos os nomes delas no loop
        printf("%-20s", schema[i].name);
    }
    printf("\n");

    print_values(file, schema, attr_count);             // chama a funcao pra ler e printar os valores

    printf("\n");
    fclose(file);                                       // fecha o arquivo
    return EXIT_SUCCESS;
}
