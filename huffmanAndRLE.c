#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node{
    int simb;
    unsigned int freq;
    struct node *left, *right;
}no_MinHeap;

typedef struct heap{
    unsigned size;
    unsigned capacity;
    no_MinHeap **array;
}MinHeap;

typedef struct reg{
    int simb;
    int freq;
    int *bits;
    int n;       //quantidade de bits do simbolo
}histograma;

typedef struct reg1{
    int simb;
    int *bits;
}simbolos;

typedef struct reg2{
    char simb[9];
    char freq[9];
}histograma_rle;

//Funcao que cria um heap minimo (fila de prioridade minima) e retorna o heap minimo criado
MinHeap* criar_MinHeap(unsigned capacity){
    MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (no_MinHeap**)malloc(minHeap->capacity * sizeof(no_MinHeap*));
    return minHeap;
}

//Funcao que troca o conteudo de dois nos
void swapNodes(no_MinHeap** a, no_MinHeap** b){
    no_MinHeap* t = *a;
    *a = *b;
    *b = t;
}

//Funcao heapify para garantir as propriedades de um heap minimo
void heapify(MinHeap* minHeap, int i){
    int parent = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[parent]->freq)
      parent = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[parent]->freq)
      parent = right;

    if (parent != i){
        swapNodes(&minHeap->array[parent], &minHeap->array[i]);
        heapify(minHeap, parent);
    }
}

//Funcao que virifica se o tamanho do heap é um
int isSizeOne(MinHeap* minHeap){
    return (minHeap->size == 1);
}

//Funcao que extrai o noh raiz que é o menor, aplica heapify e retorna o noh extraido
no_MinHeap* extrair_min(MinHeap* minHeap){
    no_MinHeap* menor = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    heapify(minHeap, 0);
    return menor;
}

void construir_heap(MinHeap* minHeap){
    int n = minHeap->size;
    int i;
    for (i = n / 2; i >= 0; i--)
        heapify(minHeap, i);
}

//Funcao que copia a sequencia de bits de cada simbolo para seus respectivos histogramas
void getBits(int bits[], int n, histograma hist[], int pos){
    hist[pos].bits = malloc(n*sizeof(char*));
    int i;
    hist[pos].n = n;
    for (i = 0; i < n; i++){
        hist[pos].bits[i] = bits[i];
    }
}

//Funcao que verifica se um noh é folha
int isLeaf(no_MinHeap* root){
    return !(root->left) && !(root->right) ;
}

void inserir_no(MinHeap* minHeap, unsigned int freq, int simb, no_MinHeap *left, no_MinHeap *right){
    no_MinHeap* newNode = (no_MinHeap*) malloc(sizeof(no_MinHeap));  //cria um novo noh
    newNode->left =left;
    newNode->right = right;
    newNode->simb = simb;
    newNode->freq = freq;

    minHeap->array[minHeap->size] = newNode;                        //coloca o novo noh no heap
    minHeap->size++;                                                //atualiza o tamanho do heap
    construir_heap(minHeap);
}

no_MinHeap* construir_HuffmanTree(int simb[], int freq[], int size){
    unsigned int i;
    MinHeap* minHeap = criar_MinHeap(size);
    for(i = 0; i < size; i++)
        inserir_no(minHeap, freq[i], simb[i], NULL, NULL);
    while (!isSizeOne(minHeap)){
        no_MinHeap *x = extrair_min(minHeap);
        no_MinHeap *y = extrair_min(minHeap);
        inserir_no(minHeap, x->freq + y->freq, '\0', x, y);
    }

    return extrair_min(minHeap);
}

//Funcao recursiva que define os bits de cada noh folha
void getCodes(no_MinHeap* root, int codigo[], int top, histograma hist[]){
    if (root->left){
        codigo[top] = 0;
        getCodes(root->left, codigo, top + 1, hist);
    }

    if (root->right){
        codigo[top] = 1;
        getCodes(root->right, codigo, top + 1, hist);
    }

    if (isLeaf(root)){
        getBits(codigo, top, hist, root->simb);
    }
}

//Funcao final para construir a arvore de Huffman e pegar todos os codigos de todos os simbolos
void huffman(int simb[], int freq[], int size, histograma hist[]){
   no_MinHeap* root = construir_HuffmanTree(simb, freq, size);
   int codigo[10000], top = 0;
   getCodes(root, codigo, top, hist);
}

//Funcao que recebe uma string em haxadecimal e retorna o valor dela em decimal
int hex_to_dec(char hexadecimal[]){
    long long decimalNumber = 0;
    char hexDigits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
      '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    int i, j, power = 0;
    for(i = strlen(hexadecimal) - 1; i >= 0; i--){
        for(j = 0; j < 16; j++){
            if(hexadecimal[i] == hexDigits[j]){
                decimalNumber += j*pow(16, power);
            }
        }
        power++;
    }

    return decimalNumber;
}

//Funcao que recebe um inteiro (decimal) e a qtd de bits (m) necessaria para codificar
//e retorna o valor dele em binario como uma string
char *dec_to_bin(int n, int m){
   int c, d, count;
   char *pointer;

   count = 0;
   pointer = (char*)malloc(m+1);

   if (pointer == NULL)
      exit(EXIT_FAILURE);

   for (c = m; c >= 0; c--){
      d = n >> c;
      if ( d & 1 )
         *(pointer+count) = 1 + '0';
      else
         *(pointer+count) = 0 + '0';

      count++;
   }

   *(pointer+count) = '\0';
   return pointer;
}

//Funcao que recebe um inteiro que representa um binario e grava o seu valor em hexadecimal no arquivo de saída
void bin_to_hex(long int binaryval, FILE* output){
    long int hexadecimalval = 0, i = 1, remainder;
    while (binaryval != 0){
        remainder = binaryval % 10;
        hexadecimalval = hexadecimalval + remainder * i;
        i = i * 2;
        binaryval = binaryval / 10;
    }
    if(hexadecimalval == 0) fprintf(output, "0x00");
    else fprintf(output, "0x%.2lX", hexadecimalval);
}

//Funcao que inicializa um array do tipo histograma
void initialize(histograma hist[]){
    int i;
    for(i = 0; i < 256; i++){
        hist[i].simb = -1;
        hist[i].freq = 0;
        hist[i].n = 0;
        hist[i].bits = NULL;
    }
}

//Funcao que inicializa um array do tipo simbolos
void initialize_simb(simbolos simbs[], int tam){
    int i;
    for(i = 0; i < tam; i++){
        simbs[i].simb = -1;
        simbs[i].bits = NULL;
    }
}

//Funcao que copia um array para outro
void copiaArray(int origem[], int dest[], int n){
    int i;
    for(i = 0; i < n; i++)
        dest[i] = origem[i];
}

//Funcao que recebe uma sequencia de bits gerados pela Huffman Tree
//e vai convertendo de 8 em 8 para hexadecimal para gravar no arquivo de saida
void converte(char str[], int x, FILE* output){
    char aux[9];
    int num;
    int i, j = 0;

    for(i = 0; i < 9; i++)      //inicializa a string auxiliar
            aux[i] = -1;

    for(i = 0; i < x; i++){
        aux[j] = str[i];
        j++;
        if(j == 8){
            num = atoi(aux);
            bin_to_hex(num, output);
            j = 0;
            if(i != x-1)
                fprintf(output, " ");
        }

    }
}

//Funcao que recebe um simbolo e sua frequencia de acordo com a codificacao RLE
//e convete ambos para hexadecimal para gravar no arquivo de saida
void converte_rle(char freq[], char simb[], FILE* output){
        int num;
        num = atoi(freq);
        bin_to_hex(num, output);
        fprintf(output, " ");
        num = atoi(simb);
        bin_to_hex(num, output);
}

double taxa_compressao(int tam, int bits){
    int bits_total = tam * 8;
    int bits_compressao = bits;
    double res = (double) bits_compressao/bits_total;
    return res*100;
}

//Funcao que calcula a taxa de compressao da seq
float RLE_calculo(histograma hist[], histograma_rle hist_rle[], int tam, simbolos simbs[]){
    int i, count = 0, aux = 1;
    for(i = 0; i < tam; i++){
        if(simbs[i].simb == simbs[i+1].simb)
            aux++;
        else{
            char simb[9], freq[9];
            strcpy(simb, dec_to_bin(simbs[i].simb, 7));
            strcpy(freq, dec_to_bin(aux, 7));
            count += strlen(simb) + strlen(freq);
            aux = 1;
        }
    }

    float tc = taxa_compressao(tam, count);
    return tc;
}

//Funcao final para codificar a sequencia com o RLE e gravar num arquivo de saida
void RLE(histograma hist[], histograma_rle hist_rle[], int tam, simbolos simbs[], FILE* output){
    int i, count = 0, aux = 1;
    for(i = 0; i < tam; i++){
        if(simbs[i].simb == simbs[i+1].simb)
            aux++;
        else{
            char simb[9], freq[9];
            strcpy(simb, dec_to_bin(simbs[i].simb, 7));
            strcpy(freq, dec_to_bin(aux, 7));
            count += strlen(simb) + strlen(freq);
            aux = 1;
            converte_rle(freq, simb, output);
            if(i != tam-1)
                fprintf(output, " ");
        }
    }
}

//Funcao que le a entrada dada
void ler_simbolos(long long int tam, simbolos simbs[], histograma hist[], FILE* input){
    long long int i;
    for(i = 0; i < tam; i++){
        char hex[5];
        fscanf(input, "%s", hex);
        int num = hex_to_dec(hex);
        simbs[i].simb = num;
        hist[num].simb = num;
        hist[num].freq = hist[num].freq + 1;
    }
}

int main(int argc, char* argv[]){
    printf("#ARGS = %i\n", argc);
    printf("PROGRAMA = %s\n", argv[0]);
    printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    int qtd_seq;
    fscanf(input, "%d", &qtd_seq);
    int tam, i, m;

    for(m = 0; m < qtd_seq; m++){
        fscanf(input, "%d", &tam);
        histograma hist[256];
        initialize(hist);
        simbolos simbs[tam];
        initialize_simb(simbs, tam);
        ler_simbolos(tam, simbs, hist, input);
        int arr[256], j = 0;
        int freq[256];

        for(i = 0; i < 256; i++){     //copia os simbolos e freqs do histograma para dois arrays de inteiros
            if(hist[i].freq != 0){
                arr[j] = hist[i].simb;
                freq[j] = hist[i].freq;
                j++;
            }
        }

        int size = j;
        double rle, huff;
        histograma_rle hist_rle[256];

        huffman(arr, freq, size, hist);                    //chama o huffman para pegar a sequencia de bits e colocar no histograma

        for(i = 0; i < tam; i++){                          //copia cada seq de bits do histograma para seus respectivos
            int n = hist[simbs[i].simb].n;                 //simbolos de acordo com a ordem de entrada
            simbs[i].bits = malloc(n*sizeof(char*));
            for(j = 0; j < n; j++)
                simbs[i].bits[j] = hist[simbs[i].simb].bits[j];
        }


        int count = 0;
        for(i = 0; i < tam; i++){                            //conta a quantidade total de bits
            int n = hist[simbs[i].simb].n;
            for(j = 0; j < n; j++)
                count++;
        }

        int *array, k = 0;
        array = malloc (count * sizeof(char*));
        for(i = 0; i < tam; i++){                          //copia cada seq de bits de cada simbolo para um unico array
            int n = hist[simbs[i].simb].n;
            for(j = 0; j < n; j++){
                array[k] = simbs[i].bits[j];
                k++;
            }
        }

        int aux = count;
        if(count % 8 != 0) {                               //completa os bits da sequencia total com 0
            int c = 0;                                    //para que a sequencia possa ser multiplo de 8
            while(aux % 8 != 0){
                c++; aux++;
            }
        }
        int array2[aux];
        copiaArray(array, array2, count);
        for(i = count; i < aux; i++)
            array2[i] = 0;

        rle = RLE_calculo(hist, hist_rle, tam, simbs);      //calcula a taxa de compressao do RLE
        huff = taxa_compressao(tam, aux);                   //calcula a taxa de compressao do Huffman

        if(rle < huff){                                    //verifica qual a menor taxa de compressao
            fprintf(output, "%d: [RLE %.2f%%] ", m, rle);
            RLE(hist, hist_rle, tam, simbs, output);
            fprintf(output, "\n");
        }
        else{
            char out[aux];
            for(i = 0; i < aux; i++)
                out[i] = array2[i] + '0';
            fprintf(output, "%d: [HUF %.2f%%] ", m, huff);
            converte(out, aux, output);
            fprintf(output, "\n");
        }

        //printf("m: %d\n", m);
        for(i = 0; i < tam; i++)
            free(simbs[i].bits);
    }

    //fprintf(output, "\n");
    fclose(input);
    fclose(output);
    return 0;
}
