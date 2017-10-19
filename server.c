// Server side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_PORT 8374

int parseExpr(char * expstr, int *op1,int *op2,char *oper);
int calcResult(int op1,int op2,char oper);
char oper;
int op1, op2;

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( SERVER_PORT );


    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    //Accept Cli8ent Connections
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    //Read inputs from client in loop until get 0/0
    while(1){
        char buffer[100] = {0};
        //Read input expression from Clients
        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        //parse it
         parseExpr(buffer,&op1,&op2,&oper);
         //IF 0/0 then exit
         if(op1 == 0 && op2 == 0 && oper == '/'){
             printf("Server Exiting...\n");
             exit(1);
         }
         //Calculate result
         int result = calcResult(op1,op2,oper);
         //Covert result to string and send to client
         char resultBuf[50] = {0};
         // convert 123 to string [buf]
         sprintf(resultBuf, "%d", result);
        send(new_socket , resultBuf , strlen(resultBuf) , 0 );
        printf("Result message sent :: %s\n",resultBuf);
    }


    return 0;
}
int parseExpr(char * expstr, int *op1,int *op2,char *oper){
    //Expresion string is empty
    if(strlen(expstr) == 0){
        return -1;
    }
    *op1 = -1;
    *op2 = -1;
    *oper = 0;
    int operidx = -1;
    for(int i = 0 ; i < strlen(expstr) ; i++){
        char c = expstr[i];
        if(c == '+' || c == '-' || c == '*' || c == '/'){
            if(i == 0 || operidx != -1){
                //Found operator before first operand so retrun fail
                return -1;
            }
            //Store index of operator
            operidx = i;
            //Copy first operand
            char op1sub[i+1] ;
            memcpy(&op1sub[0],expstr,i);
            op1sub[i] = '\0';
            //Convert to int
            *op1   = atoi(op1sub);
            //memcpy(&op1,&val,sizeof(int));


            *oper = c;
        }
        else if( c >= '0' && c <= '9' ){
            //If reached end of string then read 2nd operand from index of operator
            if(i == strlen(expstr)-1 && operidx > 0){
                int op2len = i- operidx;
                char op2sub[op2len + 1] ;


                memcpy(&op2sub[0],&expstr[operidx+1],op2len);
                op2sub[op2len] = '\0';
                *op2  = atoi(op2sub);
                //memcpy(&op2,&val,sizeof(int));
            }
            continue;
        }
        else{
            //Found other than Operator and Digits so return failue
            return -1;
        }
    }
    if(operidx == -1 || *op1 == -1 || *op2 == -1){
        return -1;
    }
}


int calcResult(int op1,int op2,char oper){


    switch (oper) {
    case '+':
        return op1+op2;
        break;
    case '-':
        return op1-op2;
        break;
    case '*':
        return op1*op2;
        break;
    case '/':
        return op1/op2;
        break;
    default:
        break;
    }
}
