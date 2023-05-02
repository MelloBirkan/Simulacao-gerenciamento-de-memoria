#include <stdio.h>
#include <stdlib.h>

// Estrutura para armazenar informações sobre blocos de memória livre
typedef struct _tipo_MemLivre {
  int End_i;
  int tam;
  struct _tipo_MemLivre *prox;
} tipo_MemLivre;

// Estrutura para armazenar informações sobre blocos de memória alocados
typedef struct _tipo_MemAloc {
  int NProcesso;
  int End_i;
  int tam;
  struct _tipo_MemAloc *prox;
} tipo_MemAloc;

// Função que inicializa a lista de memória livre com o tamanho total da memória
void inicia(int memoriaTotal);

// Função que insere um bloco de memória alocado na lista de memória alocada
void InsereBlocoAloc(int Processo, int end_i, int tam);

// Função que insere um bloco de memória livre na lista de memória livre
void InsereBlocoMemlivre(int end_i, int tam);

// Função que busca um espaço disponível na lista de memória livre com tamanho suficiente para alocação
int BuscaEspacoDisp(int Tam);

// Função que aloca um bloco de memória para um processo
void AlocaMemoria(int Processo, int Tam);

// Função que finaliza um processo e libera o espaço de memória ocupado por ele
void FinalizaProcesso(int n);

// Função que organiza os blocos de memória livre, juntando blocos contíguos
void OrganizaBlocoMemLivre(void);

// Função que libera a memória das listas de memória alocada e livre
void LiberaLista(void);

// Função que imprime a situação atual das listas de memória alocada e livre
void imprimir_situacao_atual();

// Ponteiros para as listas de memória alocada e livre
tipo_MemLivre *memLivre = NULL;
tipo_MemAloc *memAloc = NULL;


void inicia(int memoriaTotal) {
  memLivre = (tipo_MemLivre *)calloc(1, sizeof(tipo_MemLivre));
  memLivre->End_i = 0;
  memLivre->tam = memoriaTotal;
  memLivre->prox = NULL;

  memAloc = NULL;
}

void InsereBlocoAloc(int Processo, int end_i, int tam) {
  tipo_MemAloc *novo_bloco = (tipo_MemAloc *)calloc(1, sizeof(tipo_MemAloc));
  novo_bloco->NProcesso = Processo;
  novo_bloco->End_i = end_i;
  novo_bloco->tam = tam;
  novo_bloco->prox = memAloc;
  memAloc = novo_bloco;
}

void InsereBlocoMemlivre(int end_i, int tam) {
  tipo_MemLivre *novo_bloco = (tipo_MemLivre *)calloc(1, sizeof(tipo_MemLivre));
  novo_bloco->End_i = end_i;
  novo_bloco->tam = tam;
  novo_bloco->prox = memLivre;
  memLivre = novo_bloco;
}

int BuscaEspacoDisp(int Tam) {
  tipo_MemLivre *atual = memLivre;
  while (atual != NULL) {
    if (atual->tam >= Tam) {
      return atual->End_i;
    }
    atual = atual->prox;
  }
  return -1;
}

void AlocaMemoria(int Processo, int Tam) {
  int endereco = BuscaEspacoDisp(Tam);
  if (endereco != -1) {
    InsereBlocoAloc(Processo, endereco, Tam);
    // Atualizar a lista de memória livre
    tipo_MemLivre *atual = memLivre, *anterior = NULL;
    while (atual != NULL) {
      if (atual->End_i == endereco) {
        if (anterior == NULL) {
          memLivre = atual->prox;
        } else {
          anterior->prox = atual->prox;
        }
        free(atual);
        break;
      }
      anterior = atual;
      atual = atual->prox;
    }
    InsereBlocoMemlivre(endereco + Tam, Tam);
  } else {
    printf("Não há espaço disponível para alocar a memória solicitada.\n");
  }
}

void FinalizaProcesso(int n) {
  tipo_MemAloc *atual = memAloc, *anterior = NULL;
  while (atual != NULL) {
    if (atual->NProcesso == n) {
      if (anterior == NULL) {
        memAloc = atual->prox;
      } else {
        anterior->prox = atual->prox;
      }
      InsereBlocoMemlivre(atual->End_i, atual->tam);
      OrganizaBlocoMemLivre();
      free(atual);
      break;
    }
    anterior = atual;
    atual = atual->prox;
  }
}

void OrganizaBlocoMemLivre(void) {
  tipo_MemLivre *atual = memLivre, *anterior = NULL;
  while (atual != NULL && atual->prox != NULL) {
    if (atual->End_i + atual->tam == atual->prox->End_i) {
      tipo_MemLivre *temp = atual->prox;
      atual->tam += temp->tam;
      atual->prox = temp->prox;
      free(temp);
    } else {
      anterior = atual;
      atual = atual->prox;
    }
  }
}

void LiberaLista(void) {
  tipo_MemLivre *tempLivre;
  tipo_MemAloc *tempAloc;
  while (memLivre != NULL) {
    tempLivre = memLivre;
    memLivre = memLivre->prox;
    free(tempLivre);
  }

  while (memAloc != NULL) {
    tempAloc = memAloc;
    memAloc = memAloc->prox;
    free(tempAloc);
  }
}

void imprimir_situacao_atual() {
  printf("Blocos de memória livre:\n");
  tipo_MemLivre *atualLivre = memLivre;
  while (atualLivre != NULL) {
    printf("Endereço: %d, Tamanho: %d\n", atualLivre->End_i, atualLivre->tam);
    atualLivre = atualLivre->prox;
  }

  printf("Blocos de memória alocados:\n");
  tipo_MemAloc *atualAloc = memAloc;
  while (atualAloc != NULL) {
    printf("Processo: %d, Endereço: %d, Tamanho: %d\n", atualAloc->NProcesso,
           atualAloc->End_i, atualAloc->tam);
    atualAloc = atualAloc->prox;
  }
}

int main() {
  int memoriaTotal;
  printf("Informe a quantidade total de memória disponível: ");
  scanf("%d", &memoriaTotal);
  inicia(memoriaTotal);

  int opcao, processo, tamanho;
  while (1) {
    printf("\nSelecione uma opção:\n");
    printf("1 - Alocar memória\n");
    printf("2 - Finalizar processo\n");
    printf("3 - Imprimir situação atual\n");
    printf("4 - Sair\n");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      printf("Informe o número do processo e a quantidade de memória "
             "necessária: ");
      scanf("%d %d", &processo, &tamanho);
      AlocaMemoria(processo, tamanho);
      break;
    case 2:
      printf("Informe o número do processo a ser finalizado: ");
      scanf("%d", &processo);
      FinalizaProcesso(processo);
      break;
    case 3:
      imprimir_situacao_atual();
      break;
    case 4:
      LiberaLista();
      exit(0);
    default:
      printf("Opção inválida.\n");
    }
  }

  return 0;
}
