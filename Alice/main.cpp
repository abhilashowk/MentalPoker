#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <arpa/inet.h>
#include <math.h>
#include<unistd.h>
#include<pthread.h>

using namespace std;
int main()
{



char* buff;
   int sockfd;
   int n=17;
   int keys[4];
   int A;
   double sumBob=0;
   double sumAlice=0;

   double encBset[52];
   double decAliceByBob[5];
   double finalAlice[5];
   double alice[5];
   double bob[5];
   double setAlice[5];
   double setBob[5];
   int count=0;
   int count1=0;
   int i;
   struct sockaddr_in servaddr,cliaddr;
   char sendline[1000];
   char recvline[1000];
   socklen_t len;
   //int keys[20];
   int start[1];
   //string strtGame="Let's start the game";
   start[0]=1;
   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
   servaddr.sin_port=htons(32000);
   len=sizeof(servaddr);
   int aliceKey[1];
   int bobKey[1];
   int cheating=0;
   int round=0;

     sendto(sockfd,start,sizeof(start),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
      recvfrom(sockfd,keys,sizeof(keys),0,(struct sockaddr *)&servaddr,&len);
       cout<< "Received list of keys from Bob\n";
       for(i=0;i<4;i++){
           printf("%d  ",keys[i]);
       }

       printf("\nSelect a key\n");
       scanf("%d",&A);
        printf("Selected Key is %d\n",A);


       //Alice receives encrypted set of cards

        recvfrom(sockfd,encBset,sizeof(encBset),0,(struct sockaddr *)&servaddr,&len);


        //Alice chooses set for Bob setBob[] and for herself setAlice[]

        for(i=0;i<5;i++){
            count=rand()%52;
            count1=rand()%52;
            setBob[i]=encBset[count];
            setAlice[i]=fmod(pow(encBset[count1],A),n);

        }


        //Alice sends Bob's cards to him
       sendto(sockfd,setBob,sizeof(setBob),0,(struct sockaddr *)&servaddr,sizeof(servaddr));


     //Alice sends her cards to him
       sendto(sockfd,setAlice,sizeof(setAlice),0,(struct sockaddr *)&servaddr,sizeof(servaddr));

       //Alice receives her encrypted cards

       recvfrom(sockfd,decAliceByBob,sizeof(decAliceByBob),0,(struct sockaddr *)&servaddr,&len);

       //Alice decrypts her cards
       for(i=0;i<5;i++){
             finalAlice[i]=fmod(pow(decAliceByBob[i],A),n);


         }
       printf("\nAlice's chosen cards\n");
       for(i=0;i<5;i++){
        printf(" %f \n",finalAlice[i]);
}
       sendto(sockfd,finalAlice,sizeof(finalAlice),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
       recvfrom(sockfd,bob,sizeof(bob),0,(struct sockaddr *)&servaddr,&len);

       for(i=0;i<5;i++){
       alice[i]=finalAlice[i];
       }

       printf("\nFinal Cards\n");
       for(i=0;i<5;i++){
           printf("%f  %f\n",alice[i],bob[i]);

       }

       printf("\n --THE GAME BEGINS--\n");
        printf("=============================================================\n");

       for(i=0;i<5;i++){

           printf("\nRound %d",i+1);

           printf("\nAlice's Card %f",alice[i]);
           sumAlice=alice[i];
           sumBob=bob[i];
           if(sumAlice>sumBob){
               printf("\nRound %d is won by Alice",i+1);
                printf("\n-------------------------------------------------------\n");
           }
           else if(sumAlice<sumBob){
               printf("\nRound %d is won by Bob",i+1);
                printf("\n-------------------------------------------------------\n");
           }
           else
           {
               printf("\nRound %d is Drawn",i+1);
                printf("\n-------------------------------------------------------\n");
           }

           if(i==4){
               if(sumAlice>sumBob){
                   printf("\nThe game is won by Alice");
                    printf("\n-------------------------------------------------------\n");
               }
               else if(sumAlice<sumBob){
                   printf("\nThe game is won by Bob");
                    printf("\n-------------------------------------------------------\n");

               }
               else{
                   printf("\nThe game is Dawn");
                    printf("\n-------------------------------------------------------\n");
               }
           }

       }

       //Check for cheating

       //Alice receives Bob's key

       recvfrom(sockfd,bobKey,sizeof(bobKey),0,(struct sockaddr *)&servaddr,&len);



       //Alice decrypts Bob's cards with his key and compares

       for(i=0;i<5;i++){
            setBob[i]=fmod(pow(setBob[i],bobKey[0]),n);
            if(setBob[i]!=bob[i]){
                cheating=1;
                round=i;
                break;
            }
       }
       if(cheating==0){
           printf("\nFairplay by Bob\n");
       }
       else {
           printf("\nAlice found out that Bob Cheated in Round %d\n",round);
       }

       //Alice sends her key to Bob

       aliceKey[0]=A;
       sendto(sockfd,aliceKey,sizeof(aliceKey),0,(struct sockaddr *)&servaddr,sizeof(servaddr));


}


