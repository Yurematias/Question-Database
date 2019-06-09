#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <windows.h>
#include <stdlib.h>
#include "colors.h"
#include <string.h>
#include "D:\Documentos\libpq\include\pgsql\libpq-fe.h"

struct questao
{
	char texto[300];
	char resposta[300];
	char tema[50];
	char dominio[50];	
	int dificulade;
	int id;
}questoes;

void gotoxy(int x, int y);
unsigned char menu(char menu[40]);
void menuBusca(unsigned char op);
void menuPrincipal(unsigned char op);
void cadastrarQuestao(PGconn *conn);
void cadastrarDominio(PGconn *conn);
void cadastrarTema(PGconn *conn);
void mostrarOpcoes(unsigned char op);
void telaCadastro(PGconn *conn);

static void exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    system("pause");
    exit(1);
}

int main()
{
	// parte de conexao com banco
	const char *conninfo = "host=localhost dbname=QuestionSystem user=postgres password=linuxyu";
	PGconn *conn;
	PGresult *res;
	int nFields;
    int i, j;
	
	/* Make a connection to the database */
    conn = PQconnectdb(conninfo);
    
    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        exit_nicely(conn);
    }
	    
	setlocale(LC_ALL, "Portuguese");
	system("color 0F");
	unsigned char op, opBusca;
	do
	{
		op = menu("principal");
		switch(op)
		{
			case 1:
				telaCadastro(conn);
				break;
			case 2:
				opBusca = menu("busca");
				break;
			case 3:
				PQexec(conn, "DELETE FROM QUESTION");	
				break;
			case 4:
				system("cls");
				printf("Programa finalizado");			
		}
	}while(op != 4);
	return 0;
}

// funcao que mostra as opcoes do menu principal
void menuPrincipal(unsigned char op)
{
	printf(CIANO"\t========= SISTEMA DE QUESTÕES ========="CINZA);
	printf("%s",(op == 1) ? PRETO FUNDOBRANCO "\n\n\t Cadastrar Questões "CINZA FUNDOPRETO" <==" : "\n\n\t Cadastrar Questões");
	printf("%s",(op == 2) ? PRETO FUNDOBRANCO "\n\n\t Buscar Questões "CINZA FUNDOPRETO" <==" : "\n\n\t Buscar Questões");
	printf("%s",(op == 3) ? PRETO FUNDOBRANCO "\n\n\t Apagar Registros "CINZA FUNDOPRETO" <==" : "\n\n\t Apagar Registros");
	printf("%s",(op == 4) ? PRETO FUNDOBRANCO "\n\n\t Sair "CINZA FUNDOPRETO " <==\n\n" : "\n\n\t Sair\n\n");
	printf(CIANO"\t=======================================\n"CINZA);
}
// funcao que mostra as opcoes do menu de busca
void menuBusca(unsigned char op)
{
	printf(CIANO"\t========= BUSCA DE QUESTÕES ========="CINZA);
	printf("%s",(op == 1) ? PRETO FUNDOBRANCO"\n\n\t Buscar por palavra "CINZA FUNDOPRETO" <==" : "\n\n\t Buscar por palavra");
	printf("%s",(op == 2) ? PRETO FUNDOBRANCO"\n\n\t Buscar por tema "CINZA FUNDOPRETO" <==" : "\n\n\t Buscar por tema");
	printf("%s",(op == 3) ? PRETO FUNDOBRANCO"\n\n\t Buscar por domínio "CINZA FUNDOPRETO " <==" : "\n\n\t Buscar por domínio");
	printf("%s",(op == 4) ? PRETO FUNDOBRANCO"\n\n\t Buscar por dificuldade "CINZA FUNDOPRETO" <==" : "\n\n\t Buscar por dificuldade");
	printf("%s",(op == 5) ? PRETO FUNDOBRANCO"\n\n\t Voltar "CINZA FUNDOPRETO" <==" : "\n\n\t Voltar");
 	printf(CIANO"\n\n\t=======================================\n"CINZA);
}

// funcao que faz o controle do menu com as setas 
unsigned char menu(char* menu)
{
	unsigned char limite;
	unsigned char op = 1;
	char tecla;
	do
	{
		system("cls");
		if(strcmp("principal",menu) == 0)
		{
			menuPrincipal(op);
			limite = 4;
		}
		else
		{
			menuBusca(op);
			limite = 5;
		}
		while(1)
			if(kbhit())
			{
				tecla = getch();
				switch(tecla)
				{
					case 'w':
						if(op > 1)
							op--;					
						break;
					case 72:			// valor ASCII da seta para cima
						if(op > 1)
							op--;					
						break;	
					case 'W':
						if(op > 1)
							op--;					
						break;	
					case 's':
						if(op < limite)
							op++;
						break;	
					case 80:			// valor ASCII da seta para baixo
						if(op < limite)
							op++;	
						break;	
					case 'S':
						if(op < limite)
							op++;			
				}
				break;	// sai do laço se o usuario teclar algo, isto para mostrar o menu com as opcoes ja alteradas
			}
	}while(tecla != 13); // 13 eh o enter da tabela ascii
	return op;
}

// funcao da tela de cadastro, onde o usuario digita todas as entradas 
void telaCadastro(PGconn *conn)
{	
	system("cls");
	gotoxy(5,22);
	printf(CIANO"\t==============================="CINZA);
	gotoxy(5,1);
	printf(CIANO"\t===== CADASTRO DE QUESTÕES ====="CINZA);
	printf(AMARELO"\n\n\tEscreva a questão:\n\n\t"CINZA);
	setbuf(stdin,NULL);
	fgets(questoes.texto,300,stdin);
	printf(AMARELO"\n\tEscreva a resposta da questão:\n\n\t"CINZA);
	fgets(questoes.resposta,300,stdin);
	printf(AMARELO"\n\tInsira o dominio da questão:\n\n\t"CINZA);
	fgets(questoes.dominio,50,stdin);
	printf(AMARELO"\n\tInsira o tema da questão:\n\n\t"CINZA);
	fgets(questoes.tema,50,stdin);
	printf(AMARELO"\n\tInsira a dificuldade da questão:\n\n\t"CINZA);
	scanf("%d",&questoes.dificulade);
	cadastrarDominio(conn);
	cadastrarTema(conn);
	cadastrarQuestao(conn);
    printf("\n\t");
	Sleep(1700);	
}

// prepara a query e insire o tema anteriormente digitado no banco de dados, 
// esta funcao eh chamada dentro da funcao telaCadastro.
void cadastrarTema(PGconn *conn)
{
	char query[500];
	strcpy(query,"INSERT INTO Theme VALUES('");
	strcat(query,questoes.tema);
	strcat(query,"')");
	PGresult *res = PQexec(conn, query);
		/* handle the response */
	printf("\n");	
	switch (PQresultStatus(res)) 
	{
		case PGRES_EMPTY_QUERY: 
		case PGRES_COMMAND_OK: break;
		case PGRES_TUPLES_OK:  break;
		case PGRES_BAD_RESPONSE: printf(VERMELHO"\t[ERROR] Bad Response\n"CINZA); system("pause"); break;
		case PGRES_NONFATAL_ERROR: 
		case PGRES_FATAL_ERROR: break;
		default: printf(VERMELHO"\tUnexpected Response\n"CINZA); system("pause");
	}	
    PQclear(res);
}

// prepara a query e insire o dominio anteriormente digitado no banco de dados, 
// esta funcao eh chamada dentro da funcao telaCadastro.
void cadastrarDominio(PGconn *conn)
{
	char query[500];
	strcpy(query,"INSERT INTO Domain VALUES('");
	strcat(query,questoes.dominio);
	strcat(query,"')");
	PGresult *res = PQexec(conn, query);
		/* handle the response */
	printf("\n");	
	switch (PQresultStatus(res)) 
	{
		case PGRES_EMPTY_QUERY: 
		case PGRES_COMMAND_OK: break;
		case PGRES_TUPLES_OK:  break;
		case PGRES_BAD_RESPONSE: printf(VERMELHO"\t[ERROR] Bad Response\n"CINZA); system("pause"); break;
		case PGRES_NONFATAL_ERROR: 
		case PGRES_FATAL_ERROR: break;
		default: printf(VERMELHO"\tUnexpected Response\n"CINZA); system("pause");
	}	
    PQclear(res);
}

// prepara a query e insire a questao no banco de dados, 
// esta funcao eh chamada dentro da funcao telaCadastro.
void cadastrarQuestao(PGconn *conn)
{ 
	char query[500];
	strcpy(query,"INSERT INTO Question VALUES(");
	strcat(query, "DEFAULT,'");		    	// colocar DEFAULT para ocorrer o auto-incremento no banco 
	strcat(query,questoes.texto);
	strcat(query,"','");
	strcat(query,questoes.resposta);
	strcat(query,"','");
	strcat(query,questoes.dominio);
	strcat(query,"','");
	strcat(query,questoes.tema);
	strcat(query,"',");
	
	// eh necessario converter a dificuldade para char 
	// para que o valor seja colocado na string
	char dificulty[15];
	sprintf(dificulty, "%d", questoes.dificulade);
	// fim da conversao
	
	strcat(query,dificulty);
	strcat(query,")");
	PGresult *res = PQexec(conn, query);
		/* handle the response */
	switch (PQresultStatus(res)) 
	{
		case PGRES_EMPTY_QUERY: 
		case PGRES_COMMAND_OK: printf(CIANO"\tCadastro realizado com sucesso...\n"CINZA); break;
		case PGRES_TUPLES_OK:  break;
		case PGRES_BAD_RESPONSE: printf(VERMELHO"\t[ERROR] Bad Response\n"CINZA); system("pause"); break;
		case PGRES_NONFATAL_ERROR: 
		case PGRES_FATAL_ERROR: printf(PQresultErrorMessage(res)); break;
		default: printf(VERMELHO"\tUnexpected Response\n"CINZA); system("pause");
	}	
    PQclear(res);
}

//	funcao que mexe o cursor do terminal para as cordenadas x,y  
void gotoxy(int x, int y)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x-1,y-1});
}
