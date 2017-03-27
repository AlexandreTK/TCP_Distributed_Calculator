/* Lab Sistemas Distribuidos - Prof. Fernando W. Cruz */
/*           Calculadora distribuida  TCP             */
/* Arquivo: tcpServer.c			     */
/* ***************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define QLEN      5     /* tamanho da fila de clientes  */
#define MAX_SIZE	80		/* tamanho do buffer */


int atende_cliente(int descritor, struct sockaddr_in endCli);
int calcula(char *s);


int main(int argc, char *argv[]) {
   struct sockaddr_in endServ;  /* endere�o do servidor   */
   struct sockaddr_in endCli;   /* endere�o do cliente    */
   int    sd, novo_sd;          /* socket descriptors */
   int    pid, alen,n;

   if (argc<3) {
	  printf("Digite IP e Porta para este servidor\n");
	  exit(1); }
   memset((char *)&endServ,0,sizeof(endServ)); /* limpa variavel endServ    */
   endServ.sin_family 		= AF_INET;           	/* familia TCP/IP   */
   endServ.sin_addr.s_addr 	= inet_addr(argv[1]); 	/* endere�o IP      */
   endServ.sin_port 		= htons(atoi(argv[2])); /* PORTA	    */

   /* Cria socket */
   sd = socket(AF_INET, SOCK_STREAM, 0);
   if (sd < 0) {
     fprintf(stderr, "Falha ao criar socket!\n");
     exit(1); }

   /* liga socket a porta e ip */
   if (bind(sd, (struct sockaddr *)&endServ, sizeof(endServ)) < 0) {
     fprintf(stderr,"Ligacao Falhou!\n");
     exit(1); }

   /* Ouve porta */
   if (listen(sd, QLEN) < 0) {
     fprintf(stderr,"Falhou ouvindo porta!\n");
     exit(1); }

   printf("Servidor ouvindo no IP %s, na porta %s ...\n\n", argv[1], argv[2]);
   /* Aceita conexoes */
   alen = sizeof(endCli);
   for ( ; ; ) {
	 /* espera nova conexao de um processo cliente ... */
	if ( (novo_sd=accept(sd, (struct sockaddr *)&endCli, &alen)) < 0) {
		fprintf(stdout, "Falha na conexao\n");
		exit(1); }
	fprintf(stdout, "Cliente %s: %u conectado.\n", inet_ntoa(endCli.sin_addr), ntohs(endCli.sin_port));
	atende_cliente(novo_sd, endCli);
   }
}

// Essa aplicacao permite com que apenas um cliente se conecte por vez
// e esse cliente e atendido por essa funcao.
int atende_cliente(int descritor, struct sockaddr_in endCli)  {
   char bufin[MAX_SIZE];
   char bufout[MAX_SIZE];
   int  n;
   int result;

   memset(&bufin, 0x0, sizeof(bufin));
   n = recv(descritor, &bufin, sizeof(bufin),0);
   //printf("Received: %s", bufin );
   result = calcula(bufin);
   //printf("Resultado: %d", result);
   memset(&bufout, 0x0, sizeof(bufout));
   sprintf(bufout, "%d",result);
   send(descritor,bufout,strlen(bufout),0);
   //fprintf(stdout, "Encerrando conexao com %s:%u ...\n\n", inet_ntoa(endCli.sin_addr), ntohs(endCli.sin_port));
   close (descritor);
   return 0;
 }


// Funcao que recebe uma string como parametro e a separa em 3 tokens:
// operando 1, operador, operando 2. Dependendo do operando, realiza a
// operacao associada.
int calcula(char *s) {
  int i = 0;
  char operation;
  while(s[i] != '\0') {

    if(s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') {
      operation = s[i];
      s[i] = '\0';
      break;    
    }
    i++;
  }

  int a, b;
  a = atoi(s);
  b = atoi(&s[i+1]);

  switch (operation) {
    case '+':
      return a+b;
    case '-':
      return a-b;
    case '*':
      return a*b;
    case '/':
      return a/b;
  }
  return 0;
}


