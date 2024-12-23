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
Pokemon selectedPokemons[MAX_SELECTED]; // Array para os Pokémons selecionados
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
        printf("Erro ao abrir o arquivo.\n");
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

// Função para comparar dois Pokémons pelo nome (usada para ordenação)
int compararPokemons(const void *a, const void *b) {
    Pokemon *pokemonA = (Pokemon *)a;
    Pokemon *pokemonB = (Pokemon *)b;
    return strcmp(pokemonA->name, pokemonB->name);
}

// Função de busca binária para verificar se o nome do Pokémon está entre os selecionados
bool binarySearch(char* target) {
    int left = 0;
    int right = selectedCount - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(selectedPokemons[mid].name, target);

        if (cmp == 0) {
            return true; // Nome encontrado
        } else if (cmp < 0) {
            left = mid + 1; // Pesquisa na metade superior
        } else {
            right = mid - 1; // Pesquisa na metade inferior
        }
    }
    return false; 
}

// Função para verificar se um Pokémon existe entre os selecionados
void verificarPokemon(char *nome) {
    if (binarySearch(nome)) {
        printf("SIM\n");
    } else {
        printf("NAO\n");
    }
}

int main() {
    preencherPokedex();

    // Receber IDs dos Pokémons
    char str[10];
    while (scanf("%s", str) && strcmp(str, "FIM") != 0) {
        int id = atoi(str);
        for (int i = 0; i < MAX_POKEMONS; i++) {
            if (pokemons[i].id == id) {
                selectedPokemons[selectedCount++] = pokemons[i]; // Adiciona o Pokémon selecionado
                break;
            }
        }
    }

    // Ordenar o array de Pokémon selecionados para que a busca binária funcione corretamente
    qsort(selectedPokemons, selectedCount, sizeof(Pokemon), compararPokemons);

    // Receber nomes de Pokémons e verificar sua existência
    while (scanf("%s", str) && strcmp(str, "FIM") != 0) {
        verificarPokemon(str);
    }

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
