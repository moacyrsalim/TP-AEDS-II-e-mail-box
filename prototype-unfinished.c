#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define C 100					//Tamanho Máximo do conteúdo de Mensagem
#define ID_MAX 100000			//Valor Máximo de ID

//INSERIR FIM

typedef struct mensagem{
	int prioridade;
	char *conteudo[100];
	struct mensagem *prox;
}msg;

typedef struct inbox{
	msg *mensagem;
	int quantidade;
}inbox;

typedef struct user{
	int id;
	struct user *prox;
	inbox *inbox;
}user;

typedef struct indice{
	user* primeiro;
	user* ultimo;
	int quantidade;
}indice;

//_______________________________________________________________________________________
void cadastrar(indice *lista, int id);
void remover(indice *lista, int id);
void entregar_mensagem(indice *lista, int id, int pr, char conteudo[C]);
void ver_mensagem(indice *lista, int id);

int verificar_conteudo(char conteudo[C]);
void get_command(indice *lista);
void help();

//_______________________________________________________________________________________

void cadastrar(indice *lista, int id){
	user *novo;
	user *p = lista->primeiro;
	
	if(id >= ID_MAX || id < 0){
		printf("ERRO: Valor de ID inválido\n");
		return;
	}


	if(lista->quantidade == 0){
		novo = (user*)malloc(sizeof(user));	
		novo -> id = id;
		lista->primeiro = novo;
		lista->ultimo = novo;
	}

	else{
		
		while (p != NULL){
			if(id == p->id){
				printf("ERRO: Conta %d já cadastrada.\n", id);
				return;
			}
			p = p->prox;
		}
		novo = (user*)malloc(sizeof(user));
		novo -> id = id;
		lista->ultimo->prox = novo;
		lista->ultimo = novo;
	}

	novo->inbox = (inbox*)malloc(sizeof(inbox));
	novo->prox = NULL;
	lista->quantidade++;

	novo->inbox->quantidade = 0;
	novo->inbox->mensagem = NULL;

	printf("OK: Conta %d Cadastrada\n", id);
}

void remover(indice *lista, int id){
	user *p;
	user *aux;

	if(lista->quantidade == 0){
		printf("Não há contas registradas.\n");
		return;
	}

	p = lista->primeiro;
	
	if(id > ID_MAX || id < 0){
		printf("ERRO: Valor de ID inválido.\n");
		return;
	}

	if (p->id != id){
		while (id != p->prox->id){
			if(p == NULL){
				printf("ERRO: Conta %d inexistente.\n", id);
				return;
			}
			p = p->prox;
		}
	}

	if (lista->quantidade == 1){
		free (p->inbox);
		free (p);
	}
	else{
		aux = p->prox->prox;
		free(p->prox);
		p->prox = aux;
	}

	lista->quantidade --;
	printf("OK: Conta %d Removida.\n", id);
}

int verificar_conteudo(char conteudo[C]){
	int t, i = 0;
	char fim[3];

	t = strlen (conteudo);
	while(i < 3){
		fim[i] = conteudo[t-4];
		i++;
		t++;
	}
	if(strcmp(fim, "FIM") == 0)
		return 0;
	
	else
		return 1;
}

void entregar_mensagem(indice* lista, int id, int pr, char *texto){
	user *p = lista->primeiro;
	msg *q, *aux, *x;
	int t;


	if(id > ID_MAX || id< 0){
		printf("ERRO: Valor de ID inválido\n");
		return;
	}
	if(pr > 5 || pr < 1){				//prioridade entre 1 e 5
		printf("ERRO: Valor de Prioridade Inválido\n");
		return;
	}

	while (p->id != id){
		if(p == NULL){
			printf("ERRO: conta %d inexistente\n", id);
			return;
		}
		p = p->prox;
	}

	q = p->inbox->mensagem;
	
	if(verificar_conteudo(texto)){
		printf("ERRO: Formato da mensasgem incorreto\n");
		return;
	}

	x = (msg*)malloc(sizeof(msg));
	x->prioridade = pr;
	strcpy (x->conteudo, texto);

	if(p->inbox->quantidade == 0){
		p->inbox->mensagem = x;
		x->prox = NULL;
		printf("OK: Mensagem para %d entregue.\n", id);
		return;
	}

	while(q != NULL){
		if(pr >= q->prioridade){
			aux = q;
			p->inbox->mensagem = x;
			q->prox = aux;
			return;
		}
		q = q->prox;
	}

	p->inbox->mensagem = x;
	p->inbox->mensagem->prox = NULL;
}

void ver_mensagem(indice *lista, int id){
	user *p = lista-> primeiro;
	msg *aux;

	if (id > ID_MAX || id <0){
		printf("ERRO: Valor de ID inválido\n");
		return;
	}

	while (p->id != id){
		if(p == NULL){
			printf("ERRO: conta %d inexistente\n", id);
			return;
		}
		p = p->prox;
	}

	printf("CONSULTA %d: %s\n", id, p->inbox->mensagem->conteudo);
	aux = p->inbox->mensagem;
	p->inbox->mensagem = p->inbox->mensagem->prox;
	free(aux);

	return;
}

void help(){
	printf("Comandos:\n CADASTRA <ID>\n REMOVE <ID>\n ENTREGA <ID> <prioridade> <mensagem>\n");
	printf(" CONSULTA <ID>\n SAIR");
}

void get_command(indice *lista){
	char comando[15];
	int id;
	
	printf("__________________________________________________\n|\n|\n|         Digite Help ver os Comandos . . .          \n|\n|\n|\n|\n");
	printf("__________________________________________________\n");
	setbuf(stdin, NULL);
	
	scanf ("%s" ,comando);
	system("clear");
	if(strcmp(comando, "CADASTRA")== 0){
		scanf("%d", &id);
		cadastrar(lista, id);
		//sleep(3);
		get_command(lista);
	}

	if (strcmp(comando, "REMOVE") == 0){
		scanf("%d", &id);
		remover(lista, id);
		//sleep(3);
		get_command(lista);
	}
	
	if (strcmp(comando, "ENTREGA") == 0){
		int prioridade;
		char conteudo[C];
		//sleep(3);
		scanf("%d %d", &id, &prioridade);
		fgets(conteudo, C, 	stdin);

		entregar_mensagem(lista, id, prioridade, conteudo);

		get_command(lista);
	}
	
	if (strcmp(comando, "CONSULTA") == 0){
		scanf("%d", &id);
		ver_mensagem(lista, id);

		get_command(lista);
	
	}
	
	if (strcmp(comando, "SAIR") == 0){
		exit(1);
	}

	if (strcmp(comando, "MOSTRA") == 0){
		user *p = lista->primeiro;
		while (p != NULL){
			printf("%d \n", p->id);
			p = p-> prox;
		}
	}

	else{
		printf("%s %d", comando, id);
		printf("Comando Não Encontrado\n");
		return;
	}
}

void get_command_arquivo(indice *lista, FILE* arquivo){
	char comando[15];
	int id;
	
	printf("__________________________________________________\n|\n|\n|         Digite Help ver os Comandos . . .          \n|\n|\n|\n|\n");
	printf("__________________________________________________\n");
	
	fscanf (arquivo, "%s" ,comando);
	system("clear");
	if(strcmp(comando, "CADASTRA")== 0){
		fscanf(arquivo, "%d", &id);
		cadastrar(lista, id);
		get_command_arquivo(lista, arquivo);
	}

	if (strcmp(comando, "REMOVE") == 0){
		scanf("%d", &id);
		remover(lista, id);
		get_command_arquivo(lista, arquivo);
	}
	
	if (strcmp(comando, "ENTREGA") == 0){
		int prioridade;
		char conteudo[C];
		fscanf(arquivo, "%d %d", &id, &prioridade);
		fgets(conteudo, C, 	arquivo);

		entregar_mensagem(lista, id, prioridade, conteudo);

		get_command_arquivo(lista, arquivo);
	}
	
	if (strcmp(comando, "CONSULTA") == 0){
		fscanf(arquivo, "%d", &id);
		ver_mensagem(lista, id);

		get_command_arquivo(lista, arquivo);
	}
	
	if (strcmp(comando, "SAIR") == 0){
		exit(1);
	}

	if (strcmp(comando, "MOSTRA") == 0){
		user *p = lista->primeiro;
		while (p != NULL){
			printf("%d \n", p->id);
			p = p-> prox;
		}
	}

	else{
		printf("%s %d", comando, id);
		printf("Comando Não Encontrado\n");
		return;
	}
}

//_______________________________________________________________________________________

int main(){
	FILE* arq;
	char comando[100];
	
	indice *lista;
	lista = (indice*)malloc(sizeof(indice));
	lista->primeiro = NULL;
	lista->quantidade = 0;

	arq = fopen("comandos.txt", "r");
	if (arq != NULL){
		while (!feof){
			get_command_arquivo(lista, arq);
		}
	}

	else get_command(lista);

	return 0;
}
