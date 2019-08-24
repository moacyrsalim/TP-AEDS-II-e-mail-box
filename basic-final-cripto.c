//basic version

#include <stdio.h>
#include <stdlib.h>

#define E 30					//Tamanho Máximo do Email
#define C 500					//Tamanho Máximo do conteúdo de Mensagem
#define ID_MAX 100000			//Valor Máximo de ID
#define M 100					//Limite de Mensagens na caixa de entrada

typedef struct user{
	char email[E];
	int id;
	struct caixa_de_mensagem* caixa_entrada;
	struct caixa_de_mensagem* caixa_saida;
	struct user* prox;
} User;

typedef struct mensagem{
	int id, prioridade;
	char conteudo[C], assunto[C];
	struct user* usuario_origem;
	struct user* usuario_destino;
} Mensagem;

typedef struct caixa_de_mensagem{
    Mensagem emails[M];
	int auto_increment;
	int qtd_msgs;
} CaixaMensagem;

typedef struct indice{
	User* head;
	int auto_increment;
	int qtd_users;
} Lista;



void cadastrar_user(Lista*);
void remover_user(Lista*);
void enviar_msg(Lista*);
void listar_msgs(CaixaMensagem*);
void ver_msg(Lista*);
void ordenar_msg(CaixaMensagem*);
void mostra_lista(Lista*);

void limpa_tela();
void pressione_para_continuar();

void menu(Lista* lista);

void cadastrar_user(Lista* lista){
	User* novo_usuario = (User*)malloc(sizeof(User));

	printf("\nDigite o Email de usuario: ");
	scanf("%s", &novo_usuario->email);

	novo_usuario->id = lista->auto_increment++;
	novo_usuario->caixa_entrada = (CaixaMensagem*)malloc(sizeof(CaixaMensagem));
	novo_usuario->caixa_entrada->qtd_msgs = 0;
	novo_usuario->caixa_entrada->auto_increment = 0;
	novo_usuario->caixa_saida = (CaixaMensagem*)malloc(sizeof(CaixaMensagem));
	novo_usuario->caixa_saida->qtd_msgs = 0;
	novo_usuario->caixa_saida->auto_increment = 0;
	novo_usuario->prox = lista->head;
	lista->head = novo_usuario;

	lista->qtd_users++;

	printf("\nUsuario cadastrado com sucesso!\n\n");
}

void remover_user(Lista *lista){
	int id;
	User* p, *aux;

	p = lista->head;

	printf("Digite o ID da conta que será removida: ");
	scanf("%d", &id);

	if(p->id == id) {
        p = p; // Primeiro nó ja tem o id
	} else {
        while (id != p->prox->id){
            if (p == NULL){
                printf("ID não encontrado\n");
                return;
            }
            p = p->prox;
        }
	}

	if(lista->qtd_users > 0) {
        if(lista->qtd_users == 1) {
            free(p->prox);
        } else {
            aux = p->prox->prox;
            free(p->prox);
            p->prox = aux;
        }
        lista->qtd_users--;
        printf("\nConta Removida.\n\n");
	} else {
        printf("Nenhuma conta encontrada.\n\n");
	}
}

void enviar_msg(Lista *lista){
	int id_origem, id_destino;
	User* usuario_origem = lista->head;
	User* usuario_destino = lista->head;
	Mensagem mensagem;

  printf("\nDigite o ID do usuario de origem: ");
	scanf("%d", &id_origem);

	while(usuario_origem->id != id_origem){
		if (usuario_origem == NULL){
			printf("ID de origem inexistente. \n");
			return;
		}
        usuario_origem = usuario_origem->prox;
	}

	printf("Digite o ID do usuario de destino: ");
	scanf("%d", &id_destino);

	while(usuario_destino->id != id_destino){
		if (usuario_destino == NULL){
			printf("ID de destino inexistente. \n");
			return;
		}
        usuario_destino = usuario_destino->prox;
	}

	printf("Digite o assunto do email: ");
	fflush(stdin);
	fgets(mensagem.assunto, sizeof(mensagem.assunto), stdin);

	printf("Digite o conteúdo da mensagem(Limite de 500 caracteres): ");
	fflush(stdin);
	fgets(mensagem.conteudo, sizeof(mensagem.conteudo), stdin);

	printf("Digite a prioridade(De 1 a 5 sendo, 5 o mais urgente): ");
	scanf("%d", &mensagem.prioridade);

    mensagem.usuario_origem = usuario_origem;
    mensagem.usuario_destino = usuario_destino;

    // Insere email na caixa de saida do usuario de origem
    mensagem.id = usuario_origem->caixa_saida->auto_increment;
    usuario_origem->caixa_saida->emails[usuario_origem->caixa_saida->auto_increment] = mensagem;
    usuario_origem->caixa_saida->qtd_msgs++;

    // Insere email na caixa de entrada do usuario de destino
    mensagem.id = usuario_destino->caixa_entrada->auto_increment;
    usuario_destino->caixa_entrada->emails[usuario_destino->caixa_entrada->auto_increment] = mensagem;
    usuario_destino->caixa_entrada->qtd_msgs++;

    usuario_origem->caixa_saida->auto_increment++;
    usuario_destino->caixa_entrada->auto_increment++;

    ordenar_msg(usuario_origem->caixa_saida);
	ordenar_msg(usuario_destino->caixa_entrada);
}

CaixaMensagem* get_caixa(char tipo[7], User* user) {
    if(tipo == "saida") {
        return user->caixa_saida;
    } else if(tipo == "entrada") {
        return user->caixa_entrada;
    } else {
        CaixaMensagem* tmp = (CaixaMensagem*)malloc(sizeof(CaixaMensagem));
        tmp->auto_increment = 0;
        tmp->qtd_msgs = 0;
        return tmp;
    }
}

void listar_msgs(CaixaMensagem* caixa) {
    int i;
    for(i=0; i<caixa->qtd_msgs; i++) {
        Mensagem msg = caixa->emails[i];
        printf("ID: %d\n", msg.id);
        printf("Assunto: %s\n\n", msg.assunto);
    }
    printf("\n");
}

void ver_msg(Lista* lista) {
	int i, id_usuario, id_caixa, id_email;

	User *p = lista->head;

	printf("Digite o ID do usario: ");
	scanf("%d", &id_usuario);
	while (p->id != id_usuario) {
		if (p == NULL){
			printf("ID de usuario não encontrado\n");
			return;
		}
		p = p->prox;
	}

	printf("Digite a caixa da qual deseja ver a mensagem (1 - Entrada | 2 - Saida): ");
	scanf("%d", &id_caixa);
	while(id_caixa < 1 || id_caixa > 2) {
        printf("Digite um valor valido para a caixa de mensagem (1 - Entrada | 2 - Saida): ");
        scanf("%d", id_caixa);
	}

	CaixaMensagem* caixa;
	if(id_caixa == 1) {
        caixa = p->caixa_entrada;
	} else {
        caixa = p->caixa_saida;
	}

	listar_msgs(caixa);

	printf("Digite o id do email que você deseja visualizar: ");
	scanf("%d", &id_email);

    int encontrou = 0;
	for (i = 0; i < caixa->qtd_msgs; i++){
		Mensagem msg = caixa->emails[i];
		if(msg.id == id_email) {
            printf("\nEmail:\n\n-------------------------------------------------------------------------------------------\n");
            printf("De: %s\n", msg.usuario_origem->email);
            printf("Para: %s\n", msg.usuario_destino->email);
            printf("Assunto: %s\n\n", msg.assunto);
            printf("%s\n\n-------------------------------------------------------------------------------------------", msg.conteudo);
            encontrou = 1;
            break;
		}
	}

	if(!encontrou) {
        printf("Mensagem nao encontrada na caixa de %s\n", (id_caixa == 1? "entrada": "saida"));
	}
}

int compare(const void *a1, const void *a2) {
    struct mensagem *m1 = (struct mensagem *)a1;
    struct mensagem *m2 = (struct mensagem *)a2;
    if (m1->prioridade == m2->prioridade) return m1->id < m2->id;
    else return m1->prioridade < m2->prioridade;
}

void ordenar_msg(CaixaMensagem* caixa){
    qsort(caixa->emails, caixa->qtd_msgs, sizeof(struct mensagem), compare);
}

void mostra_lista(Lista* lista) {
    User *pointer = lista->head;

    if(pointer == NULL) {
        printf("Lista de usuarios vazia\n");
        return;
    }

    printf("\nUsuarios:\n");

    while(pointer != NULL) {
        printf("ID: %d\n", pointer->id);
        printf("Email: %s\n", pointer->email);
        printf("Quantidade de emails - Caixa de entrada: %d\n", pointer->caixa_entrada->qtd_msgs);
        printf("Quantidade de emails - Caixa de saida: %d\n\n", pointer->caixa_saida->qtd_msgs);
        pointer = pointer->prox;
    }
}

void limpa_tela() {
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

void pressione_para_continuar() {
    printf("Pressione qualquer tecla para continuar...\n");
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        getchar();
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        getch();
    #endif
}


void menu(Lista *lista){
	int opt = 1;
	while(opt >= 1 && opt <= 6) {
        //limpa_tela();

        printf("\n ***********************************");
        printf("\n *  Servidor de emails:            *");
        printf("\n *  1. Cadastrar usuario           *");
        printf("\n *  2. Remover usuario             *");
        printf("\n *  3. Enviar mensagem             *");
        printf("\n *  4. Ver mensagens               *");
        printf("\n *  5. Mostrar informacoes         *");
        printf("\n *  6. Sair                        *");
        printf("\n ***********************************\n");

        printf("\nOpcao: ");
        scanf("%d", &opt);

        switch(opt){
            case 1:
                cadastrar_user(lista);
                //pressione_para_continuar();
                break;

            case 2:
                remover_user(lista);
                //pressione_para_continuar();
                break;

            case 3:
                enviar_msg(lista);
                //pressione_para_continuar();
                break;

            case 4:
                ver_msg(lista);
                //pressione_para_continuar();
                break;

            case 5:
                mostra_lista(lista);
                //pressione_para_continuar();
                break;

            case 6:
                printf("Finalizando programa. \n");
                exit(1);
                break;
        }
	}
}

int main(){
	Lista *lista = (Lista *)malloc(sizeof(Lista));
	lista->head = NULL;
	lista->qtd_users = 0;
	lista->auto_increment = 0;

	menu (lista);
}
