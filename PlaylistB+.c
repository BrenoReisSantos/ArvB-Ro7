#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int t = 2;

//Struct da chave da arvore B+.
typedef struct Chave
{
    char *artista;
    char *ano;
} CH;

//Struct do Album (informação da arvore B+);
typedef struct album
{
    char *artista;
    char *ano;
    char *nfaixas;
    char *duracao;
    char *album;
} Alb;

//Struct da Árvore B+.
typedef struct Arvb_mais
{
    int numero_de_chaves;
    CH **chaves;
    int folha;
    Alb **album;
    struct Arvb_mais **filho, *anterior, *proximo;
} ABM;

ABM *inicializa_arvBmais()
{
    return NULL;
}

//Função que dá "Split" em uma dada String e caractér.
char **split(const char *str, const char *chrs)
{
    char to_list[strlen(str)];
    char *ssplit;
    int x = 0, y = 0;
    char **res = 0;

    for (y = 0; y < strlen(str); y++)
    {
        if (str[y] == chrs[0])
            x++;
    }

    strcpy(to_list, str);
    ssplit = strtok(to_list, chrs);
    res = (char **)malloc(sizeof(char *) * (x + 1));
    y = 0;

    while (ssplit)
    {
        res[y] = (char *)malloc(sizeof(char) * strlen(ssplit));
        strcpy(res[y], ssplit);
        y++;
        ssplit = strtok(NULL, chrs);
    }

    return res;
}

//Função que cria uma arvore B+.
ABM *cria_arvBmais(int t)
{
    ABM *arvbmais;
    arvbmais = (ABM *)malloc(sizeof(ABM));
    arvbmais->numero_de_chaves = 0;
    arvbmais->chaves = (CH **)malloc(sizeof(CH *) * (2 * t) - 1);
    arvbmais->folha = 1;
    arvbmais->album = (Alb **)malloc(sizeof(Alb *) * (2 * t) - 1);
    arvbmais->filho = (ABM **)malloc(sizeof(ABM *) * 2 * t);
    arvbmais->anterior = (ABM *)malloc(sizeof(ABM));
    arvbmais->proximo = (ABM *)malloc(sizeof(ABM));
    int i = 0;
    for (i = 0; i < (t * 2); i++)
        arvbmais->filho[i] = NULL;
    arvbmais->album[i] = NULL;

    return arvbmais;
}

//Função que pega o tamanho do arquivo;
int tamanho_do_arquivo(char *nome)
{
    FILE *arq = fopen(nome, "r");
    if (!arq)
        exit(1);
    int contador = 0;
    while (!feof(arq))
    {
        char linha[255];
        fgets(linha, 255, arq);
        contador++;
    }
    fclose(arq);
    return contador;
}

//Função que lê e trata as informações do arquivo.
Alb **arquivo_para_album(char *nome)
{
    int tamanho = tamanho_do_arquivo(nome);
    FILE *arq = fopen(nome, "r");
    if (!arq)
        exit(1);

    char linha[255];
    Alb **album = (Alb **)malloc(sizeof(Alb *) * tamanho);
    int count = 0;

    while (!feof(arq))
    {
        Alb *novo = (Alb *)malloc(sizeof(Alb));
        fgets(linha, 100, arq);
        char **splitada = split(linha, "/");
        novo->artista = splitada[0];
        novo->ano = splitada[1];
        novo->nfaixas = splitada[2];
        novo->duracao = splitada[3];
        novo->album = splitada[4];

        album[count] = novo;
        //print_album(album[count]);

        count++;
    }
    fclose(arq);
    return album;
}

//Função que junta as strings Artista+ano.
char *concatena_chave(CH *ch)
{
    char *novo = (char *)malloc(sizeof(char) * (strlen(ch->artista + strlen(ch->ano) + 1)));
    strcpy(novo, ch->artista);
    strcat(novo, ch->ano);
    return novo;
}

//Função que junta as strings Artista+Ano.
char *concatena_album(Alb *alb)
{
    char *novo = (char *)malloc(sizeof(char) * (strlen(alb->artista + strlen(alb->ano) + 1)));
    strcpy(novo, alb->artista);
    strcat(novo, alb->ano);
    return novo;
}

//Função que imprime a arvore.
void imprime(ABM *a, int andar)
{
    if (a)
    {
        int i, j;
        for (i = 0; i <= a->numero_de_chaves - 1; i++)
        {
            imprime(a->filho[i], andar + 1);
            for (j = 0; j <= andar; j++)
                printf("   ");
            char *chave = concatena_chave(a->chaves[i]);
            printf("%s\n", chave);
        }
        imprime(a->filho[i], andar + 1);
    }
}

//Função que divide um nó para uma inserção completa.
ABM *divisao(ABM *abm, int i, ABM *abm2, int t)
{
    ABM *abm3 = cria_arvBmais(t);
    abm3->folha = abm2->folha;
    int j;
    if (abm3->folha)
    {
        abm3->numero_de_chaves = t;
        abm3->chaves[0] = abm2->chaves[t - 1];
        abm3->album[0] = abm2->album[t - 1];
        for (j = 0; j < t - 1; j++)
        {
            abm3->chaves[j + 1] = abm2->chaves[j + t];
            abm3->album[j + 1] = abm2->album[j + t];
        }
    }
    else
    {
        abm3->numero_de_chaves = t - 1;
        for (j = 0; j < t - 1; j++)
        {
            abm3->chaves[j] = abm2->chaves[j + t];
            abm3->album[j] = abm2->album[j + t];
        }
    }
    if (!abm2->folha)
    {
        for (j = 0; j < t; j++)
        {
            abm3->filho[j] = abm2->filho[j + t];
            abm2->filho[j + t] = NULL;
        }
    }
    abm2->numero_de_chaves = t - 1;
    for (j = abm->numero_de_chaves; j >= i; j--)
    {
        abm->filho[j + 1] = abm->filho[j];
    }
    abm->filho[i] = abm3;
    for (j = abm->numero_de_chaves; j >= i; j--)
    {
        abm->chaves[j] = abm->chaves[j - 1];
        abm->album[j] = abm->album[j - 1];
    }
    abm->chaves[i - 1] = abm2->chaves[t - 1];
    abm->numero_de_chaves++;
    if ((abm2->folha) && (abm3->folha))
    {
        abm2->proximo = abm3;
        abm3->anterior = abm2;
    }
    return abm;
}

//Função que busca um dado elemento e retorna a incidência dele.
ABM *busca(ABM *a, int t, Alb *album)
{
    if (!a)
        return a;
    int i = 0;

    while ((i < a->numero_de_chaves) && (strcmp(concatena_album(album), concatena_chave(a->chaves[i])) > 0))
    {
        i++;
    }
    if (a->folha)
    {
        if ((i < a->numero_de_chaves) && (strcmp(concatena_album(album), concatena_chave(a->chaves[i])) == 0))
            return a;
        else
            return busca(a->filho[i], t, album);
    }
    if ((i < a->numero_de_chaves) && (strcmp(concatena_album(album), concatena_chave(a->chaves[i])) == 0))
        return a;
    return busca(a->filho[i], t, album);
}

//Função que pega um album e transforma em uma chave.
CH *constroi_chave_com_album(Alb *alb)
{
    CH *novo = (CH *)malloc(sizeof(CH));
    novo->artista = alb->artista;
    novo->ano = alb->ano;
    return novo;
}

//Função que insere em um nó não completo;
ABM *insere_nao_completo(ABM *abm, Alb *album, int t)
{
    int i = abm->numero_de_chaves - 1;
    if (abm->folha)
    {

        while ((i >= 0) && ((strcmp(concatena_album(album), concatena_chave(abm->chaves[i]))) < 0))
        {
            abm->chaves[i + 1] = abm->chaves[i];
            abm->album[i + 1] = abm->album[i];
            i--;
        }

        abm->chaves[i + 1] = constroi_chave_com_album(album);
        abm->album[i + 1] = album;
        abm->numero_de_chaves++;
        return abm;
    }
    while ((i >= 0) && ((strcmp(concatena_album(album), concatena_chave(abm->chaves[i])) < 0)))
    {
        i--;
    }
    i++;
    if (abm->filho[i]->numero_de_chaves == ((2 * t) - 1))
    {
        abm = divisao(abm, (i + 1), abm->filho[i], t);
        if (strcmp(concatena_album(album), concatena_chave(abm->chaves[i])) > 0)
            i++;
    }
    abm->filho[i] = insere_nao_completo(abm->filho[i], album, t);
    return abm;
}

//Função que trata os casos de inserção.
ABM *insere(ABM *abm, Alb *album, int t)
{
    if (busca(abm, t, album))
    {
        return abm;
    }
    if (!abm)
    {
        CH *chv = (CH *)malloc(sizeof(CH));
        chv->artista = album->artista;
        chv->ano = album->ano;
        abm = cria_arvBmais(t);
        abm->numero_de_chaves = 1;
        abm->chaves[0] = chv;
        abm->album[0] = album;
        return abm;
    }
    if (abm->numero_de_chaves == (2 * t) - 1)
    {
        ABM *nova = cria_arvBmais(t);
        nova->numero_de_chaves = 0;
        nova->folha = 0;
        nova->filho[0] = abm;
        nova = divisao(nova, 1, abm, t);
        nova = insere_nao_completo(nova, album, t);
        return nova;
    }
    abm = insere_nao_completo(abm, album, t);
    return abm;
}

//Função que retorna 1 ou 0 para pertence e não pertence respectivamente.
int pertence(ABM *a, Alb *album)
{
    if (!a)
        return 0;
    int i = 0;

    while ((i < a->numero_de_chaves) && (strcmp(concatena_album(album), concatena_chave(a->chaves[i])) > 0))
    {
        i++;
    }
    if (a->folha)
    {
        if ((i < a->numero_de_chaves) && (strcmp(concatena_album(album), concatena_chave(a->chaves[i])) == 0))
            return 1;
        else
            return 0;
    }
    if ((i < a->numero_de_chaves) && (strcmp(concatena_album(album), concatena_chave(a->chaves[i])) == 0))
        return pertence(a->filho[i + 1], album);
    return pertence(a->filho[i], album);
}

//Função para liberar os ponteiros.
void liberar(ABM *a)
{
    if (a)
    {
        int i;
        for (i = 0; i < a->numero_de_chaves; i++)
        {
            if (a->folha)
                free(a->album[i]);
            liberar(a->filho[i]);
        }
        liberar(a->filho[a->numero_de_chaves]);
        free(a->album);
        free(a->chaves);
        free(a->filho);
        free(a);
    }
}

//Função que retira um dado elemento(album) da árvore.
ABM *retira(ABM *a, Alb *album, int t)
{
    if (!pertence(a, album))
        return a;

    int i = 0, j, igual = 0, BEsq = 0;
    ABM *irmao, *aux, *esq, *dir;

    while ((i < a->numero_de_chaves) && ((strcmp(concatena_album(album), concatena_chave(a->chaves[i]))) > 0))
    {
        i++;
    }
    if (a->folha)
    { //C1
        a->numero_de_chaves--;
        if (a->numero_de_chaves == 0)
        {
            liberar(a);
            return NULL;
        }
        free(a->album[i]);
        a->album[i] = NULL;
        while (i < a->numero_de_chaves)
        {
            a->chaves[i] = a->chaves[i + 1];
            a->album[i] = a->album[i + 1];
            a->album[i + 1] = NULL;
            i++;
        }
        return a;
    }
    if ((i < a->numero_de_chaves) && (strcmp(concatena_album(album), concatena_chave(a->chaves[i])) == 0))
    {
        i++;
        igual = 1;
    }
    aux = a->filho[i];
    if (a->filho[i]->numero_de_chaves == t - 1)
    { // C3
        if (i - 1 >= 0)
        { // Esquerda
            irmao = a->filho[i - 1];
            if (irmao->numero_de_chaves > t - 1)
            { // C3 - A
                if (!irmao->folha)
                { // Nós Internos
                    aux->filho[t] = aux->filho[t - 1];
                    aux->filho[t - 1] = NULL;
                    for (j = t - 2; j >= 0; j--)
                    {
                        aux->chaves[j + 1] = aux->chaves[j];
                        aux->filho[j + 1] = aux->filho[j];
                        aux->filho[j] = NULL;
                    }
                    aux->filho[0] = irmao->filho[irmao->numero_de_chaves];
                    irmao->filho[irmao->numero_de_chaves] = NULL;
                    aux->chaves[0] = a->chaves[i - 1];
                    irmao->numero_de_chaves--;
                    aux->numero_de_chaves++;
                    a->chaves[i - 1] = irmao->chaves[irmao->numero_de_chaves];
                } // Nós Internos
                if (irmao->folha)
                { // Nó Interno e Folha
                    for (j = t - 2; j >= 0; j--)
                    {
                        aux->chaves[j + 1] = aux->chaves[j];
                        aux->album[j + 1] = aux->album[j];
                        aux->album[j] = NULL;
                    }
                    irmao->numero_de_chaves--;
                    aux->numero_de_chaves++;
                    aux->album[0] = irmao->album[irmao->numero_de_chaves];
                    irmao->album[irmao->numero_de_chaves] = NULL;
                    aux->chaves[0] = irmao->chaves[irmao->numero_de_chaves];
                    a->chaves[i - 1] = aux->chaves[0];
                } // Nó Interno e Folha
                a->filho[i] = retira(a->filho[i], album, t);
                return a;
            } // C3 - A
        }     // Esquerda
        if (i + 1 <= a->numero_de_chaves)
        { // Direita
            irmao = a->filho[i + 1];
            if (irmao->numero_de_chaves > t - 1)
            { // C3 - A
                if (!irmao->folha)
                { // Nós Internos
                    aux->filho[t] = irmao->filho[0];
                    aux->chaves[t - 1] = a->chaves[i];
                    aux->numero_de_chaves++;
                    a->chaves[i] = irmao->chaves[0];
                    for (j = 0; j <= irmao->numero_de_chaves - 2; j++)
                    {
                        irmao->chaves[j] = irmao->chaves[j + 1];
                        irmao->filho[j] = irmao->filho[j + 1];
                        irmao->filho[j + 1] = NULL;
                    }
                    irmao->filho[irmao->numero_de_chaves - 1] = irmao->filho[irmao->numero_de_chaves];
                    irmao->filho[irmao->numero_de_chaves] = NULL;
                    irmao->numero_de_chaves--;
                } // Nós internos
                if (irmao->folha)
                { // Nó interno e folha
                    aux->chaves[t - 1] = irmao->chaves[0];
                    aux->album[t - 1] = irmao->album[0];
                    aux->numero_de_chaves++;
                    a->chaves[i] = irmao->chaves[1];
                    for (j = 0; j <= irmao->numero_de_chaves - 2; j++)
                    {
                        irmao->chaves[j] = irmao->chaves[j + 1];
                        irmao->album[j] = irmao->album[j + 1];
                        irmao->album[j + 1] = NULL;
                    }
                    irmao->numero_de_chaves--;
                } // Nó Interno e Folha
                a->filho[i] = retira(a->filho[i], album, t);
                return a;
            } // C3 - A
        }     // Direita
        if (i - 1 >= 0)
        { //C3 - B
            // Esquerda
            esq = a->filho[i - 1];
            dir = aux;
            BEsq = 1;
        } // Esquerda
        else if (i + 1 <= a->numero_de_chaves)
        { // Direita
            esq = aux;
            dir = a->filho[i + 1];
        } // Direita
        if (!esq->folha)
        { // Nós Internos
            esq->chaves[t - 1] = a->chaves[i - BEsq];
            for (j = 0; j < t - 1; j++)
            {
                esq->chaves[t + j] = dir->chaves[j];
                esq->filho[t + j] = dir->filho[j];
                dir->filho[j] = NULL;
            }
            esq->filho[2 * t - 1] = dir->filho[t - 1];
            dir->filho[t - 1] = NULL;
            esq->numero_de_chaves = 2 * t - 1;
            free(dir);
            a->numero_de_chaves--;
            if (a->numero_de_chaves == 0)
            {
                free(a);
                return retira(esq, album, t);
            }
            else
            {
                for (j = i - BEsq; j < a->numero_de_chaves; j++)
                    a->chaves[j] = a->chaves[j + 1];
                for (j = i + 1 - BEsq; j <= a->numero_de_chaves; j++)
                {
                    a->filho[j] = a->filho[j + 1];
                    a->filho[j + 1] = NULL;
                }
            }
        } // Nós Internos
        else
        { // Nó Interno e Folha
            for (j = 0; j < t - 1; j++)
            {
                esq->chaves[t - 1 + j] = dir->chaves[j];
                esq->album[t - 1 + j] = dir->album[j];
                dir->album[j] = NULL;
            }
            esq->proximo = dir->proximo;
            dir->proximo = NULL;
            esq->anterior = dir->anterior;
            dir->anterior = NULL;
            esq->numero_de_chaves = 2 * t - 2;
            free(dir);
            a->numero_de_chaves--;
            if (a->numero_de_chaves == 0)
            {
                free(a);
                return retira(esq, album, t);
            }
            else
            {
                for (j = i - BEsq; j < a->numero_de_chaves; j++)
                    a->chaves[j] = a->chaves[j + 1];
                for (j = i + 1 - BEsq; j <= a->numero_de_chaves; j++)
                {
                    a->filho[j] = a->filho[j + 1];
                    a->filho[j + 1] = NULL;
                }
            }
        } // Nó Interno e Folha
    }     // C3
    a->filho[i - BEsq] = retira(a->filho[i - BEsq], album, t);
    return a;
}

//Função que pega a primeira incidência de folha de um dado artista;
ABM *busca_artista(ABM *a, char *artista)
{
    if (!a)
        return a;

    if (a->folha)
    {
        int i = 0;
        while ((i < a->numero_de_chaves) && (strcmp(artista, a->chaves[i]->artista) > 0))
        {
            i++;
        }
        if (a->folha)
        {
            if ((i < a->numero_de_chaves) && (strcmp(artista, a->chaves[i]->artista) == 0))
                return a;
            else
                return busca_artista(a->filho[i], artista);
        }
        if ((i < a->numero_de_chaves) && (strcmp(artista, a->chaves[i]->artista) == 0))
            return a;
        return busca_artista(a->filho[i], artista);
    }
}

//Função que identifica a posição no vetor de albuns de um dado artista.
int acha_indice_artisca(ABM *a, char *artista)
{
    if (!a)
        return -1;
    int i = 0;

    while ((i < a->numero_de_chaves) && (strcmp(artista, concatena_chave(a->chaves[i])) > 0))
    {
        i++;
    }
    if (a->folha)
    {
        if ((i < a->numero_de_chaves) && (strcmp(artista, concatena_chave(a->chaves[i])) == 0))
            return i;
        else
            return -1;
    }
    if ((i < a->numero_de_chaves) && (strcmp(artista, concatena_chave(a->chaves[i])) == 0))
        return acha_indice_artisca(a->filho[i + 1], artista);
    return acha_indice_artisca(a->filho[i], artista);
}

//Função que remove um dado Artista da arvore até acabar com todas as suas obras;
void remove_artista(ABM *abm, char *artista)
{

    while (busca_artista(abm, artista))
    {
        printf("Achei!");
        abm = retira(abm, busca_artista(abm, artista)->album[acha_indice_artisca(abm, artista)], t);
    }
}

int main(void)
{
    ABM *abm;
    char nome[255];
    int opc = 0;
    char artista[50];

    printf("Digite o nome do Arquivo: ");
    scanf("%s", nome);

    Alb **tudo = arquivo_para_album(nome);
    abm = inicializa_arvBmais(abm);

    /*
    O insere só funciona até 62.
    a partir do 63 o programa alerta 
    sysmalloc assertion error.
    Eu não faço a mínima idéia do oque
    possa ser isso.  
    */

    int i = 0;
    while (i < 62) // (i < tamanho_do_arquivo(nome));
    {
        abm = insere(abm, tudo[i], t);
        i++;
    }

    while (opc != -1)
    {
        printf("Digite 0 para imprimir, 1 para remover um dado artista e -1 para sair : ");
        scanf("%i", &opc);
        if (opc == 0)
        {
            imprime(abm, 0);
        }
        else if (opc == 1)
        {
            //Remove Artista não está funcionando corretamente.
            printf("\nDigite o nome do Artista: ");
            scanf("%s", artista);
            remove_artista(abm, artista);
        }
        else if (opc == -1)
        {
            printf("Saindo!\n");
        }
        else
        {
            printf("Digite uma opção válida\n");
        }
    }
    return 0;
}