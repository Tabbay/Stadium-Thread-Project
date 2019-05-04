#include <stdio.h>
#include <errno.h> 
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>

#include "stadium.h"
#define MAX_SELLER_SEATS 5//total amount of seats a seller can sell
#define MAXIMUM_SEATS 30 //Max amount of seats in the stadium
#define MAXIMUM_CUSTOMERS 15 //Max number of customers
#define MAXIMUM_SELLERS 3 //Max number of sellers

Stadium *techxarena;

void *Customer(void* n){
    int customer_id = (int)(long)n;
    while(addTicket(techxarena)){
        printf("Customer #%d got a ticket!\n", customer_id);
    }
    printf("Customer #%d Could not get a ticket.\n", customer_id);
  
}

void *Seller(void* n){
    sellTicket(techxarena);
}


int main(){
    
    srand(time(NULL)); 
     
    techxarena = OpenStadium(MAXIMUM_SEATS, MAX_SELLER_SEATS);
    
    pthread_t customers[MAXIMUM_CUSTOMERS];
    pthread_t sellers[MAXIMUM_SELLERS];
    
    //create customer and seller threads
    for (int i = 0; i < MAXIMUM_CUSTOMERS; i++) {
        pthread_create(&(customers[i]), NULL, Customer,(void*)(long)i);
    }
    
    for (int i = 0; i < MAXIMUM_SELLERS; i++) {
        pthread_create(&(sellers[i]), NULL, Seller,(void*)(long)i);
    }
   

    //Wait for customer & seller threads to be done
    for (int i = 0; i < MAXIMUM_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }
   
    for (int i = 0; i < MAXIMUM_SELLERS; i++) {
        pthread_join(sellers[i], NULL);
    }
    
    //total amount of tickets handled
    printf("\nTotal Tickets Handled: %d\n",techxarena->total_seats_handled);
    CloseStadium(techxarena); 
    return 0;
}