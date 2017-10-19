#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>

#define SERVER_PORT 8374

int parseExpr(char * expstr, int *op1,int *op2,char *oper);
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    char oper ;
    int op1,op2;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }


    memset(&serv_addr, '0', sizeof(serv_addr));


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);


    // Convert IPv4 and IPv6 addresses from text to binary form
    //127.0.0.1  local IP of PC , If remote PC use IP address of that PC
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }


    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    while(1){
        char buffer[100] = {0};
        printf("Enter valid expression in format <operator1><operand><operator2> , like  2+3  \n");
        //get input from user
        if (! (fgets(buffer, sizeof buffer, stdin)  )) {
                printf("Enter Valid Expression");
                exit(1);
         }
        //Remove newline '\n' at the end
        size_t ln = strlen(buffer)-1;
        if (buffer[ln] == '\n')
            buffer[ln] = '\0';
        //IF expression is not valid then return error message and exit
        if(parseExpr(buffer,&op1,&op2,&oper) == -1){
            printf("Enter valid expression \n");
            exit(1);
        }
        //IF expression is valid then send to Server
        send(sock , buffer , strlen(buffer) , 0 );
        //IF 0/0 then exit
        if(op1 == 0 && op2 == 0 && oper == '/'){
            printf("Client Exiting..\n");
            exit(1);
        }
        //Read result from Server and print
        char resBuf[50] = {0};
        valread = read( sock , resBuf, 50);
        printf("Resilt received for :: %s   is :: %s\n",buffer,resBuf );
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
