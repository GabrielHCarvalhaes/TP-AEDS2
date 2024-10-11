#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_POKEMONS 801
#define MAX_SELECTED 801

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int id;
    int generation;
    char *name;
    char *description;
    char *type1;
    char *type2;
    char *abilities;
    double weight;
    double height;
    int captureRate;
    bool isLegendary;
    char *captureDate;
} Pokemon;

Pokemon pokemons[MAX_POKEMONS];
int selectedIds[MAX_SELECTED]; // Array para IDs dos Pokémons selecionados
int selectedCount = 0; // Contador de Pokémons selecionados

// Função para extrair a substring contendo a data do Pokémon
void substring(const char* original, char* data, int length) {
    strncpy(data, original + (strlen(original) - 11), length);
    data[length] = '\0';
}

// Função para preencher a Pokedex com os dados dos Pokémons do arquivo CSV
void preencherPokedex() {
    FILE *file = fopen("/tmp/pokemon.csv", "r");
    if (file == NULL) {
        return;
    }

    char linha[160];
    fgets(linha, 160, file); // Ignorar cabeçalho
    int i = 0;

    while (fgets(linha, 160, file) && i < MAX_POKEMONS) {
        char data[13];
        substring(linha, data, 12);
        char *token1 = strtok(linha, "\"");
        char *primeiraParte = token1;
        char *token2 = strtok(NULL, "\"");
        char *habilidades = token2 ? strdup(token2) : NULL;
        char *token3 = strtok(NULL, "\"");
        char *segundaParte = token3;

        char *tok = strtok(primeiraParte, ",");
        pokemons[i].id = atoi(tok);

        tok = strtok(NULL, ",");
        pokemons[i].generation = atoi(tok);

        tok = strtok(NULL, ",");
        pokemons[i].name = strdup(tok ? tok : "");

        tok = strtok(NULL, ",");
        pokemons[i].description = strdup(tok ? tok : "");

        tok = strtok(NULL, ",");
        pokemons[i].type1 = strdup(tok ? tok : "");

        tok = strtok(NULL, ",");
        pokemons[i].type2 = (tok && strlen(tok) > 0) ? strdup(tok) : strdup("0");

        pokemons[i].abilities = habilidades ? habilidades : strdup("");

        char *tok2 = strtok(segundaParte, ",");
        pokemons[i].weight = (tok2 && strlen(tok2) > 0) ? strtod(tok2, NULL) : 0;
        tok2 = strtok(NULL, ",");
        pokemons[i].height = (tok2 && strlen(tok2) > 0) ? strtod(tok2, NULL) : 0;

        tok2 = strtok(NULL, ",");
        pokemons[i].captureRate = atoi(tok2);

        tok2 = strtok(NULL, ",");
        pokemons[i].isLegendary = (tok2 && atoi(tok2) != 0);

        pokemons[i].captureDate = strdup(data);

        i++;
    }

    fclose(file);
}

// Função para ler IDs dos Pokémons selecionados
void leiaIds() {
    char str[10];
    while (scanf("%s", str) && strcmp(str, "FIM") != 0) {
        int id = atoi(str) - 1; // Ajusta o índice
        if (id >= 0 && id < MAX_POKEMONS) {
            selectedIds[selectedCount++] = id; // Adiciona o ID selecionado
        }
    }
}

// Função para obter o comprimento da string
int getMaxLength() {
    int maxLen = 0;
    for (int i = 0; i < selectedCount; i++) {
        int len = strlen(pokemons[selectedIds[i]].abilities);
        if (len > maxLen) {
            maxLen = len;
        }
    }
    return maxLen;
}

// Função de contagem para Radix Sort
void countingSort(int exp) {
    int output[MAX_SELECTED];
    int count[256] = {0}; // Para caracteres ASCII

    // Contagem
    for (int i = 0; i < selectedCount; i++) {
        int index = strlen(pokemons[selectedIds[i]].abilities) > exp ?
                    (unsigned char)(pokemons[selectedIds[i]].abilities[exp]) : 0;
        count[index]++;
    }

    // Cálculo do índice cumulativo
    for (int i = 1; i < 256; i++) {
        count[i] += count[i - 1];
    }

    // Construir o vetor de saída
    for (int i = selectedCount - 1; i >= 0; i--) {
        int index = strlen(pokemons[selectedIds[i]].abilities) > exp ?
                    (unsigned char)(pokemons[selectedIds[i]].abilities[exp]) : 0;
        output[count[index] - 1] = selectedIds[i];
        count[index]--;
    }

    // Copiar o vetor de saída para o vetor original
    for (int i = 0; i < selectedCount; i++) {
        selectedIds[i] = output[i];
    }
}

// Função para Radix Sort
void radixSort() {
    int maxLen = getMaxLength();

    // Aplicar counting sort para cada dígito
    for (int exp = 0; exp < maxLen; exp++) {
        countingSort(exp);
    }
}

// Função para imprimir os Pokémons selecionados
void imprimirPokemons() {
    for (int i = 0; i < selectedCount; i++) {
        Pokemon *p = &pokemons[selectedIds[i]];
        if (strcmp(p->type2, "0") != 0) {
            printf("[#%d -> %s: %s - ['%s', '%s'] - %s - %.1fkg - %.1fm - %d%% - %s - %d gen] - %s",
                p->id, p->name, p->description, p->type1, p->type2, p->abilities,
                p->weight, p->height, p->captureRate, p->isLegendary ? "true" : "false", p->generation,
                p->captureDate);
        } else {
            printf("[#%d -> %s: %s - ['%s'] - %s - %.1fkg - %.1fm - %d%% - %s - %d gen] - %s",
                p->id, p->name, p->description, p->type1, p->abilities,
                p->weight, p->height, p->captureRate, p->isLegendary ? "true" : "false", p->generation,
                p->captureDate); 
        }
    }
}

int main() {
    preencherPokedex();
    
    leiaIds(); // Lê os IDs dos Pokémons selecionados

    // Ordena os IDs usando Radix Sort
    radixSort();

    imprimirPokemons(); // Imprime os Pokémons selecionados

    // Liberar a memória alocada
    for (int i = 0; i < MAX_POKEMONS; i++) {
        free(pokemons[i].name);
        free(pokemons[i].description);
        free(pokemons[i].type1);
        free(pokemons[i].type2);
        free(pokemons[i].abilities);
        free(pokemons[i].captureDate);
    }

    return 0;
}
