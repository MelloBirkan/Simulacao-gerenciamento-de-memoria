#include <stdio.h>


typedef struct _tipo_MemLivre {
  int End_i;
  int tam;
  struct _tipo_MemLivre *prox;
}tipo_MemLivre;

typedef struct _tipo_MemAloc {
  int NProcesso;
  int End_i;
  int tam;
  struct _tipo_MemAloc *prox;
}tipo_MemAloc;


// Inicia o Bloco de Memoria Livre e Bloco de Memoria Alocada.
void inicia(void);

// Criar Bloco de Memoria Alocada, utilize a função calloc.
void InsereBlocoAloc (int Processo, int end_i, int tam);

// Criar Bloco de Memoria Livre, utilize a função calloc.
void InsereBlocoMemlivre (int end_i, int tam);

// Alocar Bloco de Memoria para Processo
// Antes de alocar, deve buscar o espaço livre
// que atenda o tamanho solicitado.
void AlocaMemoria(int Processo, int Tam);

// Buscar endereco inicial do Espaco disponivel a ser Alocado
int BuscaEspacoDisp (int Tam);

// Finaliza o processo 
void FinalizaProcesso (int n);

// Organiza a lista de Memoria Livre
void OrganizaBlocoMemLivre(void);

// Liberar a Lista encadeada
void LiberaLista (void);


int main(void) {
  printf("Hello World\n");
  return 0;
}