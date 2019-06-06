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
	char resposta[200];
	char tema[50];
	char dominio[50];	
	int dificulade;
	int id;
}questoes[100];

void fazerQuery(unsigned char cont, char* query);
void gotoxy(int x, int y);
unsigned char menu(char menu[40]);
void menuBusca(unsigned char op);
void menuPrincipal(unsigned char op);
int cadastroQuestoes(unsigned char cont, PGconn *conn);
void mostrarOpcoes(unsigned char op);

static void
exit_nicely(PGconn *conn)
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
	    
	int cont = 0;
	setlocale(LC_ALL, "Portuguese");
	system("color 0F");
	unsigned char op, opBusca;
	do
	{
		op = menu("principal");
		switch(op)
		{
			case 1:
				cont = cadastroQuestoes(cont, conn);
				break;
			case 2:
				opBusca = menu("busca");
				break;
			case 3:
				system("cls");
				printf("Programa finalizado");			
		}
	}while(op != 3);
	return 0;
}

void menuPrincipal(unsigned char op)
{
	printf(CIANO"\t========= SISTEMA DE QUESTÕES ========="CINZA);
	(op == 1) ? printf(PRETO FUNDOBRANCO"\n\n\t Cadastrar Questões "CINZA FUNDOPRETO" <==") : printf("\n\n\t Cadastrar Questões");
	(op == 2) ? printf(PRETO FUNDOBRANCO"\n\n\t Buscar Questões "CINZA FUNDOPRETO" <==") : printf("\n\n\t Buscar Questões");
	(op == 3) ? printf(""PRETO FUNDOBRANCO"\n\n\t Sair "CINZA FUNDOPRETO " <==\n\n") : printf("\n\n\t Sair\n\n");
	printf(CIANO"\t=======================================\n"CINZA);
}

void menuBusca(unsigned char op)
{
	printf(CIANO"\t========= BUSCA DE QUESTÕES ========="CINZA);
	(op == 1) ? printf(PRETO FUNDOBRANCO"\n\n\t Buscar por palavra "CINZA FUNDOPRETO" <==") : printf("\n\n\t Buscar por palavra");
	(op == 2) ? printf(PRETO FUNDOBRANCO"\n\n\t Buscar por tema "CINZA FUNDOPRETO" <==") : printf("\n\n\t Buscar por tema");
	(op == 3) ? printf(""PRETO FUNDOBRANCO"\n\n\t Buscar por domínio "CINZA FUNDOPRETO " <==") : printf("\n\n\t Buscar por domínio");
	(op == 4) ? printf(PRETO FUNDOBRANCO"\n\n\t Buscar por dificuldade "CINZA FUNDOPRETO" <==") : printf("\n\n\t Buscar por dificuldade");
	(op == 5) ? printf(PRETO FUNDOBRANCO"\n\n\t Voltar "CINZA FUNDOPRETO" <==") : printf("\n\n\t Voltar");
 	printf(CIANO"\n\n\t=======================================\n"CINZA);
}

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
			limite = 3;
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
				break;			// sai do laço se o usuario teclar algo 
			}
	}while(tecla != 13); // 13 eh o enter da tabela ascii
	return op;
}

int cadastroQuestoes(unsigned char cont, PGconn *conn)
{	
	system("cls");
	gotoxy(5,26);
	printf(CIANO"\t==============================="CINZA);
	gotoxy(5,1);
	printf(CIANO"\t===== CADASTRO DE QUESTÕES ====="CINZA);
	printf(AMARELO"\n\n\tEscreva a questão:\n\n\t"CINZA);
	setbuf(stdin,NULL);
	fgets(questoes[cont].texto,300,stdin);
	printf(AMARELO"\n\tEscreva a resposta da questão:\n\n\t"CINZA);
	fgets(questoes[cont].resposta,50,stdin);
	printf(AMARELO"\n\tInsira o tema da questão:\n\n\t"CINZA);
	setbuf(stdin,NULL);
	fgets(questoes[cont].tema,50,stdin);
	printf(AMARELO"\n\tInsira o dominio da questão\n\n\t"CINZA);
	fgets(questoes[cont].dominio,50,stdin);
	printf(AMARELO"\n\tInsira a dificuldade da questão: de 1 a 10\n\n\t"CINZA);
	scanf("%d",&questoes[cont].dificulade);
	printf(AMARELO"\n\tInsira o ID da questão:\n\n\t"CINZA);
	scanf("%d",&questoes[cont].id);
	
	char query[500];
	strcpy(query,"insert into question values('");
	fazerQuery(cont, query);
	PGresult *res = PQexec(conn, query);
		/* handle the response */
	printf("\n");	
	switch (PQresultStatus(res)) 
	{
		case PGRES_EMPTY_QUERY: 
		case PGRES_COMMAND_OK: printf("Cadastro realizado com sucesso...\n"); break;
		case PGRES_TUPLES_OK:  break;
		case PGRES_BAD_RESPONSE: printf("[ERROR] Bad Response"); break;
		case PGRES_NONFATAL_ERROR: 
		case PGRES_FATAL_ERROR: printf(PQresultErrorMessage(res)); break;
		default: printf("Unexpected Response");
	}	
    PQclear(res);
    printf("\n");
	system("pause");
	return ++cont;	
}

void fazerQuery(unsigned char cont, char* query)
{ 
	strcat(query,questoes[cont].texto);
	strcat(query,"','");
	strcat(query,questoes[cont].resposta);
	strcat(query,"','");
	char a[10]; 
	sprintf(a,"%d",questoes[cont].id);
	strcat(query ,a);
	strcat(query,"')");
}

void gotoxy(int x, int y)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x-1,y-1});
}
