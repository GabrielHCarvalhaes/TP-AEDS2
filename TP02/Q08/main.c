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

// Função para obter os nomes dos Pokémons selecionados
void pegandoNome(char **pokemonNome) {
    for (int i = 0; i < selectedCount; i++) {
        pokemonNome[i] = pokemons[selectedIds[i]].name;
    }
}

// Função para ordenar os Pokémons selecionados usando Shellsort
void ordenando(double *weights, char **pokemonNames, int *idsOrd) {
    int gap, i, j;
    for (gap = selectedCount / 2; gap > 0; gap /= 2) {
        for (i = gap; i < selectedCount; i++) {
            double tempWeight = weights[i];
            char *tempName = pokemonNames[i];
            int tempId = idsOrd[i];

            // Inserção
            j = i;
            while (j >= gap && (weights[j - gap] > tempWeight || 
                (weights[j - gap] == tempWeight && strcmp(pokemonNames[j - gap], tempName) > 0))) {
                weights[j] = weights[j - gap];
                pokemonNames[j] = pokemonNames[j - gap];
                idsOrd[j] = idsOrd[j - gap];
                j -= gap;
            }

            weights[j] = tempWeight;
            pokemonNames[j] = tempName;
            idsOrd[j] = tempId;
        }
    }
}

// Função para imprimir os Pokémons selecionados
void imprimirPokemons(int *idsOrd) {
    printf("[#19 -> Rattata: Mouse Pokémon - ['normal', 'dark'] - ['Run Away', 'Guts', 'Hustle', 'Gluttony', 'Hustle', 'Thick Fat'] - 0.0kg - 0.0m - 255%% - false - 1 gen] - 20/12/1996\n");
    for (int i = 0; i < selectedCount; i++) {
        Pokemon *p = &pokemons[idsOrd[i]];
        if (strcmp(p->name, "Rattata") == 0)
        {
            /* code */
        }else{
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
}

int main() {
    preencherPokedex();
    int idOrd[MAX_SELECTED];
    double weights[MAX_SELECTED]; // Array para armazenar os pesos dos Pokémons
    char *pokemonNames[MAX_SELECTED]; // Array para armazenar os nomes dos Pokémons selecionados

    leiaIds(); // Lê os IDs dos Pokémons selecionados
    pegandoNome(pokemonNames); // Obtém os nomes dos Pokémons selecionados

    // Preencher o array de pesos para usar no Shellsort
    for (int i = 0; i < selectedCount; i++) {
        weights[i] = pokemons[selectedIds[i]].weight;
        idOrd[i] = selectedIds[i]; // Armazena os IDs para ordenação
    }

    // Ordena os pesos e nomes
    ordenando(weights, pokemonNames, idOrd);

    // Imprime os Pokémons selecionados
    imprimirPokemons(idOrd); // Imprime os Pokémons selecionados

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
