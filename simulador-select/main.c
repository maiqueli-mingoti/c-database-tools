#include "main.h"
#include <stdio.h>
#include <string.h>

void print_header(Attr schema[], int attr_count) {               // printa o cabeçalho
    for (int i = 0; i < attr_count; i++) {
        printf("%-25s", schema[i].name);
    }
    printf("\n");
    for (int i = 0; i < attr_count; i++) {                      // imprime uma linha divisória entre o header e os dados
        for (int j = 0; j < 24; j++) {
            printf("-");
        }
        printf(" ");
    }
    printf("\n");
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
                    printf("%-25s", field_buffer);              // -25s alinha o texto na esquerda dentro do 25 espaços
                    break;
                }
                case 'I': {
                    int int_value;
                    memcpy(&int_value, field_buffer, sizeof(int));      // copia os bytes do buffer pra variavel acima
                    printf("%-25d", int_value);
                    break;
                }
                case 'F': {
                    float float_value;
                    memcpy(&float_value, field_buffer, sizeof(float));       // mesma coisa, copia os bytes pra varaivel acima
                    printf("%-25.2f", float_value);
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
    if (argc != 2) {                                            // verifica se passou o nome do arquivo de excucao
        printf("Uso: %s <table_name>\n", argv[0]);
        return 1;
    }

    char *table_name = argv[1];                                 // guarda o nome da table

    FILE *table_file = fopen("table.dic", "rb");                // abre o arquivo table.dic
    if (!table_file) {
        perror("Erro: table.dic não encontrado\n");
        return 1;
    }

    // procura a tabela no arquivo table.dic
    Table table;
    int table_found = 0;
    while (fread(&table, sizeof(Table), 1, table_file)) {
        if (strcmp(table.logical_name, table_name) == 0) {
            table_found = 1;
            break;
        }
    }
    fclose(table_file);

    if (!table_found) {
        printf("Erro: Tabela %s não encontrada\n", table_name);
        return 1;
    }
    printf("Tabela encontrada: %s (ID: %d, Arquivo: %s)\n", table.logical_name,
           table.id, table.physical_name);

    // abre o arquivo att.dic
    FILE *att_file = fopen("att.dic", "rb");
    if (!att_file) {
        perror("Erro: att.dic não encontrado");
        return 1;
    }

    // pega todos os atributos correspondentes
    Attr schema[MAX_ATTR];              // max de 50 fixo
    Attr current_attr;
    int attr_count = 0;
    while (fread(&current_attr, sizeof(Attr), 1, att_file)) {
        if (current_attr.table_id == table.id) {
            schema[attr_count] = current_attr;
            attr_count++;
        }
    }
    fclose(att_file);

    if (attr_count == 0) {
        printf("Erro: Nenhum atributo encontrado\n");
        return 1;
    }

    print_header(schema, attr_count);           // chama a funçao para printar o cabeçalho

    FILE *data_file = fopen(table.physical_name, "rb");
    if (!data_file) {
        perror("Erro: arquivo de dados não encontrado\n");
        return 1;
    }

    print_values(data_file, schema, attr_count);
    printf("\n");

    return 0;
}