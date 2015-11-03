
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

int computeGCD(int num1,int num2){
    int i,hcf;
    for(i=1; i<=num1 || i<=num2; ++i)
        {
            if(num1%i==0 && num2%i==0)   /* Checking whether i is a factor of both number */
                hcf=i;
        }
    return hcf;

}

int main()
{
    double sumBob=0;
    double sumAlice=0;
    int size=0;
    int n=17;
    int set[52];
    double encBset[52];
    //double encABset[52];
    double setAlice1[5]={0};
    double setBob1[5]={0};
    double finalBob[20];
    double decAliceByBob[5];
    double finalAlice[5];
    double alice[5];
    double bob[5];
    int B;
    int pArr[16];
    int resArr[16];
    int nonResArr[16];
    int i=0;
    int j=0;
    int flag=0;
    int count=0,count1=0;
    int keys[16];
    int aliceKey[1];
    int bobKey[1];
    int cheating=0;
    int round=0;



   int sockfd;
   struct sockaddr_in servaddr,cliaddr;
   //servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
   socklen_t len;
   int start[1];
   start[0]=0;
   cout<<"Ok,let's start the game";
   int mesg[1];

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
   servaddr.sin_port=htons(32000);
   bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   //Key compute

   for(i=0;i<16;i++){
       pArr[i]=i+1;
   }

   for(i=0;i<16;i++){
       resArr[i]=(pArr[i]*pArr[i])%n;
   }


   for(i=0;i<16;i++){
       for(j=0;j<16;j++){
           if(pArr[i]==resArr[j]){
               flag=1;

           }

       }

       if(flag==0){

           nonResArr[count]=pArr[i];

           count++;



       }
       flag=0;
   }
    j=0;

   for(i=0;i<count;i++){
       int temp=computeGCD(nonResArr[i],n-1);

       if(temp==1){
           keys[j]=nonResArr[i];
          // printf("%d  ",keys[j]);
           j++;
       }
   }
   size=j;




int whileflag=1;
   for (;;)
   {
      len = sizeof(cliaddr);

      recvfrom(sockfd,mesg,1,0,(struct sockaddr *)&cliaddr,&len);
      if (mesg[0]==1){
          printf("Ok Let's start");
      }

    cout<< "\nSending list of keys to Alice : ";
    for(i=0;i<size;i++){
        printf("%d  ",keys[i]);
    }


      sendto(sockfd,keys,sizeof(keys),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));


      printf("\n-------------------------------------------------------\n");

      printf("Select your key\n");
      scanf("%d",&B);
      printf("Your Key is %d\n",B);


      //Next bob puts the 52 cards in an array
       count=0;
       for(i=0;i<4;i++){
           for(j=2;j<=14;j++){
               set[count]=j;

               count++;
           }
       }

       //bob encrypts the array and saves to encBset[]

        for(i=0;i<count;i++){
         //   printf("%d ",set[i]);
            encBset[i]=fmod(pow(set[i],B),n);
           // printf("%f ",encBset[i]);
        }
        //printf("\n\n");

        //Send the encrypted cards to Alice in encBset[]

        sendto(sockfd,encBset,sizeof(encBset),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));

        //Received Bobs and Alices encrypted set of 5 cards

      recvfrom(sockfd,setBob1,sizeof(setBob1),0,(struct sockaddr *)&cliaddr,&len);

       recvfrom(sockfd,setAlice1,sizeof(setAlice1),0,(struct sockaddr *)&cliaddr,&len);

       printf("\n");


        //Bob decrypts both the sets

        for(i=0;i<5;i++){
            finalBob[i]=fmod(pow(setBob1[i],B),n);
            decAliceByBob[i]=fmod(pow(setAlice1[i],B),n);

         }

        printf("\nDecrypted Cards of Alice by Bob's key are\n");
        for(i=0;i<5;i++){
            printf("%f \n",decAliceByBob[i]);
        }
         printf("\nBob's' Chosen Cards are\n");
        for(i=0;i<5;i++){
            printf("%f \n",finalBob[i]);

        }


     //Bob sends Alice's cards to her

     sendto(sockfd,decAliceByBob,sizeof(decAliceByBob),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));

     recvfrom(sockfd,alice,sizeof(alice),0,(struct sockaddr *)&servaddr,&len);
     sendto(sockfd,finalBob,sizeof(finalBob),0,(struct sockaddr *)&servaddr,sizeof(servaddr));

     for(i=0;i<5;i++){
         printf("%f\n",alice[i]);

     }
     for(i=0;i<5;i++){
     bob[i]=finalBob[i];
     }
     printf("\nFinal Cards\n");
     for(i=0;i<5;i++){
         printf("%f  %f\n",alice[i],bob[i]);

     }

     printf("\n --THE GAME BEGINS--\n");
      printf("=============================================================\n");
     for(i=0;i<5;i++){

         printf("\nRound %d",i+1);

         printf("\nBob's Card %f",bob[i]);
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

     //Check for cheating case

    //Bob sends Alice his key

     bobKey[0]=B;
     sendto(sockfd,bobKey,sizeof(bobKey),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));

     //BOb receives Alice's Key

     recvfrom(sockfd,aliceKey,sizeof(aliceKey),0,(struct sockaddr *)&servaddr,&len);



   //Bob decrypts Alice's cards with her key and compares
printf("\n");
   for(i=0;i<5;i++){
        decAliceByBob[i]=fmod(pow(decAliceByBob[i],aliceKey[0]),n);
        if(decAliceByBob[i]!=alice[i]){
            cheating=1;
            round=i;
            break;
        }
   }
   if(cheating==0){
       printf("\nFairplay by Alice\n");
   }
   else {
       printf("\nBob found out that Alice Cheated in Round %d\n",round);
   }

}
}


