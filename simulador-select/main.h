#ifndef MAIN_H
#define MAIN_H

#define MAX_ATTR 50
#define MAX_NAME_LENGTH 20

typedef struct {
    int id;
    char logical_name[MAX_NAME_LENGTH];         // nome da tabela
    char physical_name[MAX_NAME_LENGTH];        // nome do arquivo
} Table;

typedef struct {
    int table_id;
    char name[MAX_NAME_LENGTH];
    char type;
    char opitonal;
    int size;
} Attr;

#endif