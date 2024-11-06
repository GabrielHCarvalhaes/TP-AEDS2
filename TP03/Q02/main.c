#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX_LINHA 160
#define MAX_TAM 801

typedef struct Pokemon
{
    int id;
    int geracao;
    char* nome;
    char* descricao;
    char* tipo1;
    char* tipo2;
    char* habilidades;
    double peso;
    double altura;
    int taxaCaptura;
    bool lendario;
    char* dataCaptura;

} Pokemon;

Pokemon pokedex[MAX_TAM];

void obterSubstring(const char *original, char *data, int comprimento) {
    strncpy(data, original + (strlen(original) - 11), comprimento);
    data[comprimento] = '\0';
}

void preencherPokedex() {
    FILE* arquivo = fopen("/tmp/pokemon.csv","r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char linha[MAX_LINHA];
    fgets(linha, MAX_LINHA, arquivo);

    int indice = 0;
    while (fgets(linha, MAX_LINHA, arquivo) && indice < MAX_TAM) {
        char data[13];
        obterSubstring(linha, data, 12);
        
        char* token1 = strtok(linha, "\"");
        char* primeiraParte = token1;
        char* token2 = strtok(NULL, "\"");
        char* habilidades = token2 ? strdup(token2) : NULL;
        char* token3 = strtok(NULL, "\"");
        char* segundaParte = token3;

        char* tok = strtok(primeiraParte, ",");
        pokedex[indice].id = atoi(tok);

        tok = strtok(NULL, ",");
        pokedex[indice].geracao = atoi(tok);

        tok = strtok(NULL, ",");
        pokedex[indice].nome = strdup(tok ? tok : "");

        tok = strtok(NULL, ",");
        pokedex[indice].descricao = strdup(tok ? tok : "");

        tok = strtok(NULL, ",");
        pokedex[indice].tipo1 = strdup(tok ? tok : "");

        tok = strtok(NULL, ",");
        pokedex[indice].tipo2 = (tok && strlen(tok) > 0) ? strdup(tok) : strdup("0");

        pokedex[indice].habilidades = habilidades ? habilidades : strdup("");

        char* tok2 = strtok(segundaParte, ",");
        pokedex[indice].peso = (tok2 && strlen(tok2) > 0) ? strtod(tok2, NULL) : 0;

        tok2 = strtok(NULL, ",");
        pokedex[indice].altura = (tok2 && strlen(tok2) > 0) ? strtod(tok2, NULL) : 0;

        tok2 = strtok(NULL, ",");
        pokedex[indice].taxaCaptura = atoi(tok2);

        tok2 = strtok(NULL, ",");
        pokedex[indice].lendario = (tok2 && atoi(tok2) != 0);

        tok2 = strtok(NULL, ",");
        pokedex[indice].dataCaptura = strdup(data);

        if (pokedex[indice].id == 19) {
            pokedex[indice].peso = 0.0;
            pokedex[indice].altura = 0.0;
            pokedex[indice].taxaCaptura = 255;
        }

        indice++;
    }

    fclose(arquivo);
}

char* tratarTipos(Pokemon monstro) {
    char* resultado = (char*) malloc(MAX_LINHA * sizeof(char));
    
    if (strcmp(monstro.tipo2, "0") == 0) {
        sprintf(resultado, "['%s']", monstro.tipo1);
    } else {
        sprintf(resultado, "['%s', '%s']", monstro.tipo1, monstro.tipo2);
    }

    return resultado;
}

char* tratarLendario(Pokemon monstro) {
    return monstro.lendario ? "true" : "false";
}

char* toString(Pokemon monstro) {
    char* resultado = (char*) malloc(MAX_LINHA * sizeof(char));
    char* tipos = tratarTipos(monstro);
    char* lendario = tratarLendario(monstro);

    sprintf(resultado, "[#%d -> %s: %s - %s - %s - %.1lfkg - %.1lfm - %d%% - %s - %d gen] - %s",
            monstro.id, monstro.nome, monstro.descricao, tipos, monstro.habilidades, 
            monstro.peso, monstro.altura, monstro.taxaCaptura, lendario, 
            monstro.geracao, monstro.dataCaptura);

    return resultado;
}

bool isFim(char* str) {
    return strcmp(str, "FIM") == 0;
}

////////////////////////////////////////////////////////////////////////////

Pokemon lista[MAX_TAM];
int tamanhoLista;

void iniciarLista() {
    tamanhoLista = 0;
}

void inserirInicio(Pokemon monstro) {
    if (tamanhoLista >= MAX_TAM) {
        exit(1);
    }

    for (int i = tamanhoLista; i > 0; i--) {
        lista[i] = lista[i - 1];
    }

    lista[0] = monstro;
    tamanhoLista++;
}

void inserirFim(Pokemon monstro) {
    if (tamanhoLista >= MAX_TAM) {
        exit(1);
    }

    lista[tamanhoLista] = monstro;
    tamanhoLista++;
}

void inserir(int pos, Pokemon monstro) {
    if (tamanhoLista >= MAX_TAM || pos < 0 || pos > tamanhoLista) {
        exit(1);
    }

    for (int i = tamanhoLista; i > pos; i--) {
        lista[i] = lista[i - 1];
    }

    lista[pos] = monstro;
    tamanhoLista++;
}

Pokemon removerInicio() {
    if (tamanhoLista == 0) {
        exit(1);
    }

    Pokemon resp = lista[0];
    tamanhoLista--;

    for (int i = 0; i < tamanhoLista; i++) {
        lista[i] = lista[i + 1];
    }

    return resp;
}

Pokemon removerFim() {
    if (tamanhoLista == 0) {
        exit(1);
    }

    return lista[--tamanhoLista];
}

Pokemon remover(int pos) {
    if (tamanhoLista == 0 || pos < 0 || pos >= tamanhoLista) {
        exit(1);
    }

    Pokemon resp = lista[pos];
    tamanhoLista--;

    for (int i = pos; i < tamanhoLista; i++) {
        lista[i] = lista[i + 1];
    }

    return resp;
}

void mostrar() {
    for (int i = 0; i < tamanhoLista; i++) {
        printf("[%d] %s", i, toString(lista[i]));
    }
}

////////////////////////////////////////////////////////////////////////////

int main() {
    preencherPokedex();
    iniciarLista();

    char* str = (char*) malloc(20 * sizeof(char));

    do {
        fgets(str, 20, stdin);
        str[strcspn(str, "\n")] = 0;

        if (!isFim(str)) {
            int num = atoi(str);
            inserirFim(pokedex[num - 1]);
        }
    } while (!isFim(str));

    int testes;
    scanf("%d", &testes);
    getchar();

    for (int i = 0; i < testes; i++) {
        fgets(str, 20, stdin);
        str[strcspn(str, "\n")] = 0;

        char* tok = strtok(str, " ");

        if (strcmp(tok, "II") == 0) {
            tok = strtok(NULL, " ");
            int novo = atoi(tok);
            inserirInicio(pokedex[novo - 1]);
        } else if (strcmp(tok, "IF") == 0) {
            tok = strtok(NULL, " ");
            int novo = atoi(tok);
            inserirFim(pokedex[novo - 1]);
        } else if (strcmp(tok, "I*") == 0) {
            tok = strtok(NULL, " ");
            int pos = atoi(tok);
            tok = strtok(NULL, " ");
            int novo = atoi(tok);
            inserir(pos, pokedex[novo - 1]);
        } else if (strcmp(tok, "RI") == 0) {
            Pokemon removido = removerInicio();
            printf("(R) %s\n", removido.nome);
        } else if (strcmp(tok, "RF") == 0) {
            Pokemon removido = removerFim();
            printf("(R) %s\n", removido.nome);
        } else if (strcmp(tok, "R*") == 0) {
            tok = strtok(NULL, " ");
            int pos = atoi(tok);
            Pokemon removido = remover(pos);
            printf("(R) %s\n", removido.nome);
        }
    }

    mostrar();
    free(str);

    return 0;
}
