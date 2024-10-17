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
    int codigo;
    int geracao;
    char *nome;
    char *descricao;
    char tipos[2][50];
    char habilidades[6][50];
    double peso;
    double altura;
    int taxaCaptura;
    bool ehLendario;
    Data dataCaptura;
} Pokemon;

char* duplicarString(const char* s) {
    char* copia = (char*)malloc(strlen(s) + 1);
    if (copia != NULL) {
        strcpy(copia, s);
    }
    return copia;
}

int obterCodigo(Pokemon *p) {
    return p->codigo;
}

void definirCodigo(Pokemon *p, int codigo) {
    p->codigo = codigo;
}

int obterGeracao(Pokemon *p) {
    return p->geracao;
}

void definirGeracao(Pokemon *p, int geracao) {
    p->geracao = geracao;
}

char* obterNome(Pokemon *p) {
    return p->nome;
}

void definirNome(Pokemon *p, char *nome) {
    p->nome = nome;
}

char* obterDescricao(Pokemon *p) {
    return p->descricao;
}

void definirDescricao(Pokemon *p, char *descricao) {
    p->descricao = descricao;
}

char* obterTipos(Pokemon *p, int indice) {
    return p->tipos[indice];
}

int obterNumeroDeTipos(Pokemon *p) {
    int contador = 0;
    for (int i = 0; i < 2; i++) {
        if (strlen(p->tipos[i]) > 0) {
            contador++;
        }
    }
    return contador;
}

void definirTipos(Pokemon *p, int indice, char *tipo) {
    strncpy(p->tipos[indice], tipo, sizeof(p->tipos[indice]) - 1);
    p->tipos[indice][sizeof(p->tipos[indice]) - 1] = '\0';
}

char* obterHabilidades(Pokemon *p, int indice) {
    return p->habilidades[indice];
}

int obterNumeroDeHabilidades(Pokemon *p) {
    int contador = 0;
    for (int i = 0; i < 6; i++) {
        if (strlen(p->habilidades[i]) > 0) {
            contador++;
        }
    }
    return contador;
}

void definirHabilidades(Pokemon *p, int indice, const char *habilidade) {
    strncpy(p->habilidades[indice], habilidade, sizeof(p->habilidades[indice]) - 1);
    p->habilidades[indice][sizeof(p->habilidades[indice]) - 1] = '\0';
}

double obterPeso(Pokemon *p) {
    return p->peso;
}

void definirPeso(Pokemon *p, double peso) {
    p->peso = peso;
}

double obterAltura(Pokemon *p) {
    return p->altura;
}

void definirAltura(Pokemon *p, double altura) {
    p->altura = altura;
}

int obterTaxaCaptura(Pokemon *p) {
    return p->taxaCaptura;
}

void definirTaxaCaptura(Pokemon *p, int taxaCaptura) {
    p->taxaCaptura = taxaCaptura;
}

bool obterEhLendario(Pokemon *p) {
    return p->ehLendario;
}

void definirEhLendario(Pokemon *p, bool ehLendario) {
    p->ehLendario = ehLendario;
}

Data obterDataCaptura(Pokemon *p) {
    return p->dataCaptura;
}

void definirDataCapturaData(Pokemon *p, Data dataCaptura) {
    p->dataCaptura = dataCaptura;
}

void definirDataCapturaString(Pokemon *p, char *dataCaptura) {
    p->dataCaptura = stringParaData(dataCaptura);
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
            strcpy(p.habilidades[i], "");
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

        p.codigo = atoi(campos[0]);

        p.geracao = atoi(campos[1]);

        p.nome = duplicarString(campos[2]);

        p.descricao = duplicarString(campos[3]);
        
        definirTipos(&p, 0, campos[4]);
        if (strlen(campos[5]) > 0) {
            definirTipos(&p, 1, campos[5]);
        } else {
            strcpy(p.tipos[1], "");
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
            strcpy(p.habilidades[indiceHabilidade], "");
        }

        p.peso = atof(campos[7]);

        p.altura = atof(campos[8]);

        p.taxaCaptura = atoi(campos[9]);

        p.ehLendario = atoi(campos[10]);

        p.dataCaptura = stringParaData(campos[11]);
        
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

int compararPokemon(Pokemon *p1, Pokemon *p2) {
    int habilidadeComparacao = strcmp(p1->habilidades[0], p2->habilidades[0]);
    if (habilidadeComparacao == 0) {
        return strcmp(obterNome(p1), obterNome(p2));
    }
    return habilidadeComparacao;
}

int getMaxLength(Pokemon *array[], int n) {
    int maxLen = strlen(array[0]->habilidades[0]);
    for (int i = 1; i < n; i++) {
        int len = strlen(array[i]->habilidades[0]);
        if (len > maxLen) {
            maxLen = len;
        }
    }
    return maxLen;
}

void radixSort(Pokemon *array[], int n) {
    int maxLen = getMaxLength(array, n);

    for (int pos = maxLen - 1; pos >= 0; pos--) {
        Pokemon *output[n];
        int count[256] = {0};

        for (int i = 0; i < n; i++) {
            int idx = (strlen(array[i]->habilidades[0]) > pos) ? (int)(unsigned char)array[i]->habilidades[0][pos] : 0;
            count[idx]++;
        }

        for (int i = 1; i < 256; i++) {
            count[i] += count[i - 1];
        }

        for (int i = n - 1; i >= 0; i--) {
            int idx = (strlen(array[i]->habilidades[0]) > pos) ? (int)(unsigned char)array[i]->habilidades[0][pos] : 0;
            output[count[idx] - 1] = array[i];
            count[idx]--;
        }

        for (int i = 0; i < n; i++) {
            array[i] = output[i];
        }
    }
}


int main() {
    char *caminhoCSV = "/tmp/pokemon.csv";
    FILE *arquivo = fopen(caminhoCSV, "r");

    if (arquivo == NULL) {
        return 1;
    }

    Pokemon pokedex[801];
    int n = 0;

    lerPokemons(arquivo, pokedex, &n);
    fclose(arquivo);

    Pokemon *pokemonsParaOrdenar[801];
    int contadorIds = 0;

    char idEntrada[10];
    scanf("%s", idEntrada);
    while (strcmp(idEntrada, "FIM") != 0) {
        int codigo = atoi(idEntrada);
        bool encontrado = false;

        for (int i = 0; i < n; i++) {
            if (pokedex[i].codigo == codigo) {
                pokemonsParaOrdenar[contadorIds] = &pokedex[i];
                contadorIds++;
                encontrado = true;
                break;
            }
        }

        if (!encontrado) {
            printf("Pokemon com ID %d não encontrado.\n", codigo);
        }

        scanf("%s", idEntrada);
    }

    radixSort(pokemonsParaOrdenar, contadorIds);

    for (int i = 0; i < contadorIds; i++) {
        exibirPokemon(pokemonsParaOrdenar[i]);
    }

    for (int i = 0; i < n; i++) {
        free(pokedex[i].nome);
        free(pokedex[i].descricao);
    }

    return 0;
}
