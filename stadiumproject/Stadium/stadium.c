#ifndef STADIUM_H_
#define STADIUM_H_
#include "stadium.h"
#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <string.h>

int ticketid = 0;
Stadium* OpenStadium(int MAXIMUM_SEATS, int MAX_SELLER_SEATS){
    Stadium *techxarena = malloc(sizeof(Stadium));
    techxarena->maximum_seats = MAXIMUM_SEATS;
    techxarena->seats_left = MAXIMUM_SEATS; 
    techxarena->total_seats_handled = 0;
    techxarena->seller_count = 0;
    techxarena->MAX_SELLER_SEATS = MAX_SELLER_SEATS;
    pthread_mutex_init(&techxarena->mutex, NULL);
    pthread_cond_init(&techxarena->can_add_tickets, NULL);
    pthread_cond_init(&techxarena->can_sell_tickets, NULL);
    
    return techxarena;
}
void CloseStadium(Stadium *techxarena){
    pthread_mutex_lock(&techxarena->mutex);
    
    //if the stadium has no more seats for the game, close the stadium
    printf("\nSeats Left: %d\n", techxarena->seats_left);
    if(techxarena->seats_left == 0){
        pthread_mutex_unlock(&techxarena->mutex);
        printf("Stadium is closed!\n");
        pthread_mutex_destroy(&techxarena->mutex);
        pthread_cond_destroy(&techxarena->can_sell_tickets);
        pthread_cond_destroy(&techxarena->can_add_tickets);
        free(techxarena);
    }
    
}

int randomTicketPicker(){
    //pick a random # 1-6
   return rand() % 6 + 1;
   
}
int sellLastSeats(int seatsleft, int numberofTickets){
    //if the there are no seats avaliable return 0 seats
    if (seatsleft <= 0)
        return 0;
    
    //check to see if there the seat number is valid
    while(seatsleft - numberofTickets != 0){
        numberofTickets--; //decrease seat count
    }
  return numberofTickets;
}
bool addTicket(Stadium *techxarena){
     //pick a number of tickets
     int numberofTickets = randomTicketPicker();
    
    //make space for new ticket object
     Ticket *ticket = malloc(sizeof(Ticket));
    
     pthread_mutex_lock(&techxarena->mutex); //lock the mutex
     ticket->ticket_id = techxarena->total_seats_handled; //create a ticket id
     ticket->number_of_tickets = numberofTickets; //assign the number of tickets
    
     //determine if there are any seats left
     if(techxarena->seats_left <= 0){
         pthread_mutex_unlock(&techxarena->mutex);
         return false;
     }
    else{
        
        //wait until the stadium has avaliable seats
         while(techxarena->seats_left <= 0 ){
             if (techxarena->seller_count >= techxarena->MAX_SELLER_SEATS){
                     pthread_cond_broadcast(&techxarena->can_add_tickets);
                     pthread_mutex_unlock(&techxarena->mutex);
                     return false;
             }
             pthread_cond_wait(&(techxarena->can_add_tickets), &techxarena->mutex);         
         }
         
         //make sure were not overselling
         if(techxarena->seats_left - numberofTickets < 0){
             ticket->number_of_tickets = sellLastSeats(techxarena->seats_left, numberofTickets);
         }
         
         //add a ticket to the ticket list 
         requestTicket(&techxarena->tickets, ticket);

         //decrease the seats left 
         techxarena->seats_left -= ticket->number_of_tickets;
         techxarena->total_seats_handled += numberofTickets; //increase total seats handled
 
         printf("\n%d seats bought, Seats Left: %d\n", numberofTickets, techxarena->seats_left);
         pthread_cond_broadcast(&techxarena->can_sell_tickets);
         pthread_mutex_unlock(&techxarena->mutex);
         return true; 
    }
    
}

void requestTicket(Ticket **listoftickets, Ticket *newTicket){
     //function that adds a ticket to the ticket list
    if( *listoftickets == NULL) {
         *listoftickets = newTicket;
    }
    else { 
        Ticket *current = *listoftickets;
        
        while(current->next != NULL) {
            current = current->next;
        }        
        current->next = newTicket;
    }
}

void sellTicket(Stadium *techxarena){
    pthread_mutex_lock(&techxarena->mutex);
    
    //wait until there are tickets to resale
    while(techxarena->total_seats_handled == 0 ){
         pthread_cond_wait(&(techxarena->can_sell_tickets), &techxarena->mutex);
          if (techxarena->seller_count == techxarena->MAX_SELLER_SEATS){
                     pthread_cond_broadcast(&techxarena->can_sell_tickets);
                     pthread_mutex_unlock(&techxarena->mutex);
             }
      }
    
    //take a ticket from the front of the list
    Ticket *sellticket = getTicket(&techxarena->tickets);
    
     techxarena->seats_left += sellticket->number_of_tickets;
    
     printf("Customer %d sold %d tickets, seats left: %d\n", sellticket->ticket_id, sellticket->number_of_tickets,techxarena->seats_left);
 
    //increase seller count 
    techxarena->seller_count ++;
    
    pthread_cond_broadcast(&(techxarena->can_add_tickets));
    pthread_mutex_unlock(&techxarena->mutex);
    free(sellticket);
    
}
Ticket *getTicket(Ticket **listofticket){
    //this will get the first ticket of the list
      Ticket *temp;
      temp = *listofticket;
      *listofticket = temp->next; 
      temp->next = NULL; 
      return temp;
}

void print1(Ticket **listoftickets){
    //function to print the list
    if( *listoftickets == NULL) {
       printf("customer list is empty");
    }
    
    Ticket *current = *listoftickets; 
        while(current->next != NULL) {
            ticketid++;
            printf("seat num: %d, #tickets: %d\n", current->ticket_id, current->number_of_tickets);
            current = current->next;
        } 
}
#endif