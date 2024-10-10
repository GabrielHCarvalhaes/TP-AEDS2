#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct Data {
    int dia;
    int mes;
    int ano;
} Data;

char *dataParaString(Data data) {
    char *str = (char *)malloc(11 * sizeof(char));
    sprintf(str, "%02d/%02d/%04d", data.dia, data.mes, data.ano);
    return str;
}

Data stringParaData(char *str) {
    Data data;
    if (str != NULL && strlen(str) > 0) {
        sscanf(str, "%d/%d/%d", &data.dia, &data.mes, &data.ano);
    } else {
        data.dia = 0;
        data.mes = 0;
        data.ano = 0;
    }
    return data;
}

typedef struct Pokemon {
    int id;
    int generation;
    char *name;
    char *description;
    char type[2][50];
    char abilities[6][50];
    double weight;
    double height;
    int captureRate;
    bool isLegendary;
    Data captureDate;
} Pokemon;

char* duplicarString(const char* s) {
    char* copia = (char*)malloc(strlen(s) + 1);
    if (copia != NULL) {
        strcpy(copia, s);
    }
    return copia;
}

int obterCodigo(Pokemon *p) {
    return p->id;
}

void definirCodigo(Pokemon *p, int id) {
    p->id = id;
}

int obterGeracao(Pokemon *p) {
    return p->generation;
}

void definirGeracao(Pokemon *p, int generation) {
    p->generation = generation;
}

char* obterNome(Pokemon *p) {
    return p->name;
}

void definirNome(Pokemon *p, char *name) {
    p->name = name;
}

char* obterDescricao(Pokemon *p) {
    return p->description;
}

void definirDescricao(Pokemon *p, char *description) {
    p->description = description;
}

char* obterTipos(Pokemon *p, int indice) {
    return p->type[indice];
}

int obterNumeroDeTipos(Pokemon *p) {
    int contador = 0;
    for (int i = 0; i < 2; i++) {
        if (strlen(p->type[i]) > 0) {
            contador++;
        }
    }
    return contador;
}

void definirTipos(Pokemon *p, int indice, char *tipo) {
    strncpy(p->type[indice], tipo, sizeof(p->type[indice]) - 1);
    p->type[indice][sizeof(p->type[indice]) - 1] = '\0';
}

char* obterHabilidades(Pokemon *p, int indice) {
    return p->abilities[indice];
}

int obterNumeroDeHabilidades(Pokemon *p) {
    int contador = 0;
    for (int i = 0; i < 6; i++) {
        if (strlen(p->abilities[i]) > 0) {
            contador++;
        }
    }
    return contador;
}

void definirHabilidades(Pokemon *p, int indice, const char *habilidade) {
    strncpy(p->abilities[indice], habilidade, sizeof(p->abilities[indice]) - 1);
    p->abilities[indice][sizeof(p->abilities[indice]) - 1] = '\0';
}

double obterPeso(Pokemon *p) {
    return p->weight;
}

void definirPeso(Pokemon *p, double weight) {
    p->weight = weight;
}

double obterAltura(Pokemon *p) {
    return p->height;
}

void definirAltura(Pokemon *p, double height) {
    p->height = height;
}

int obterTaxaCaptura(Pokemon *p) {
    return p->captureRate;
}

void definirTaxaCaptura(Pokemon *p, int captureRate) {
    p->captureRate = captureRate;
}

bool obterEhLendario(Pokemon *p) {
    return p->isLegendary;
}

void definirEhLendario(Pokemon *p, bool isLegendary) {
    p->isLegendary = isLegendary;
}

Data obterDataCaptura(Pokemon *p) {
    return p->captureDate;
}

void definirDataCapturaData(Pokemon *p, Data captureDate) {
    p->captureDate = captureDate;
}

void definirDataCapturaString(Pokemon *p, char *dataCaptura) {
    p->captureDate = stringParaData(dataCaptura);
}

Pokemon criarPokemon(int codigo, int geracao, char *nome,
    char *descricao, char *tipo1, char *tipo2, char *habilidades[6], double peso,
    double altura, int taxaCaptura, bool ehLendario, Data dataCaptura) {

    Pokemon p;

    definirCodigo(&p, codigo);
    definirGeracao(&p, geracao);

    char *nomeCopia = duplicarString(nome);
    char *descricaoCopia = duplicarString(descricao);

    definirNome(&p, nomeCopia);
    definirDescricao(&p, descricaoCopia);
    
    definirTipos(&p, 0, tipo1);
    if (tipo2 != NULL) {
        definirTipos(&p, 1, tipo2);
    }

    for (int i = 0; i < 6; i++) {
        if (habilidades[i] != NULL) {
            definirHabilidades(&p, i, habilidades[i]);
        } else {
            strcpy(p.abilities[i], "");
        }
    }

    definirPeso(&p, peso);
    definirAltura(&p, altura);
    definirTaxaCaptura(&p, taxaCaptura);
    definirEhLendario(&p, ehLendario);
    definirDataCapturaData(&p, dataCaptura);

    return p;
}

int separar_linha_csv(char *linha, char **campos, int max_campos) {
    int contadorCampos = 0;
    char *ptr = linha;
    int dentro_aspas = 0;
    char *inicioCampo = ptr;

    while (*ptr && contadorCampos < max_campos) {
        if (*ptr == '"') {
            dentro_aspas = !dentro_aspas;
        } else if (*ptr == ',' && !dentro_aspas) {
            *ptr = '\0';
            campos[contadorCampos++] = inicioCampo;
            inicioCampo = ptr + 1;
        }
        ptr++;
    }
    if (contadorCampos < max_campos) {
        campos[contadorCampos++] = inicioCampo;
    }

    return contadorCampos;
}

void lerPokemons(FILE *file, Pokemon *pokedex, int *n) {
    char linha[1024];

    fgets(linha, sizeof(linha), file);

    while (fgets(linha, sizeof(linha), file) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';

        Pokemon p;
        memset(&p, 0, sizeof(Pokemon));

        char *campos[12];
        int contadorCampos = separar_linha_csv(linha, campos, 12);

        p.id = atoi(campos[0]);

        p.generation = atoi(campos[1]);

        p.name = duplicarString(campos[2]);

        p.description = duplicarString(campos[3]);
        
        definirTipos(&p, 0, campos[4]);
        if (strlen(campos[5]) > 0) {
            definirTipos(&p, 1, campos[5]);
        } else {
            strcpy(p.type[1], "");
        }

        char *campoHabilidades = campos[6];
        if (campoHabilidades[0] == '"' && campoHabilidades[strlen(campoHabilidades) - 1] == '"') {
            campoHabilidades[strlen(campoHabilidades) - 1] = '\0';
            campoHabilidades++;
        }
        if (campoHabilidades[0] == '[' && campoHabilidades[strlen(campoHabilidades) - 1] == ']') {
            campoHabilidades[strlen(campoHabilidades) - 1] = '\0';
            campoHabilidades++;
        }

        char *tokenHabilidade;
        char *restoHabilidades = campoHabilidades;
        int indiceHabilidade = 0;
        while ((tokenHabilidade = strtok_r(restoHabilidades, ",", &restoHabilidades)) && indiceHabilidade < 6) {
            while (*tokenHabilidade == ' ' || *tokenHabilidade == '\'') tokenHabilidade++;
            char *fimTemp = tokenHabilidade + strlen(tokenHabilidade) - 1;
            while (fimTemp > tokenHabilidade && (*fimTemp == ' ' || *fimTemp == '\'')) {
                *fimTemp = '\0';
                fimTemp--;
            }
            if (strlen(tokenHabilidade) > 0) {
                definirHabilidades(&p, indiceHabilidade, tokenHabilidade);
                indiceHabilidade++;
            }
        }
        for (; indiceHabilidade < 6; indiceHabilidade++) {
            strcpy(p.abilities[indiceHabilidade], "");
        }

        p.weight = atof(campos[7]);

        p.height = atof(campos[8]);

        p.captureRate = atoi(campos[9]);

        p.isLegendary = atoi(campos[10]);

        p.captureDate = stringParaData(campos[11]);
        
        pokedex[*n] = p;
        (*n)++;
    }
}

void exibirPokemon(Pokemon *p) {
    printf("[#%d -> %s: %s - ['", obterCodigo(p), obterNome(p), obterDescricao(p));

    int numTipos = obterNumeroDeTipos(p);
    if(numTipos > 0) {
        printf("%s", obterTipos(p, 0));
    }
    if (numTipos > 1) {
        printf("', '%s", obterTipos(p, 1));
    }
    printf("'] - ");

    int numHabilidades = obterNumeroDeHabilidades(p);
    printf("[");
    for (int i = 0 ; i < numHabilidades ; i++) {
        printf("'%s'", obterHabilidades(p, i));
        if (i < numHabilidades - 1) {
            printf(", ");
        }
    }
    printf("] - ");

    printf("%.1fkg - ", obterPeso(p));
    printf("%.1fm - ", obterAltura(p));
    printf("%d%% - ", obterTaxaCaptura(p));
    printf("%s - ", obterEhLendario(p) ? "true" : "false");
    printf("%d gen] - ", obterGeracao(p));
    char *data = dataParaString(obterDataCaptura(p));
    printf("%s", data);
    free(data);

    printf("\n");
}

int main () {
    char *caminhoCSV = "/tmp/pokemon.csv";
    FILE *arquivo = fopen(caminhoCSV, "r");   

    if (arquivo == NULL) {
        return 1;
    } 

    Pokemon pokedex[801];
    int n = 0;

    lerPokemons(arquivo, pokedex, &n);

    fclose(arquivo);

    char idEntrada[10];
    scanf("%s", idEntrada);
    while (strcmp(idEntrada, "FIM") != 0) {
        int codigo = atoi(idEntrada);
        bool encontrado = false;
        for (int i = 0 ; i < n ; i++) {
            if (pokedex[i].id == codigo) {
                exibirPokemon(&pokedex[i]);
                encontrado = true;
                break;
            }
        }
        
        scanf("%s", idEntrada); 
    }

    for (int i = 0; i < n; i++) {
        free(pokedex[i].name);
        free(pokedex[i].description);
    }

    return 0;
}