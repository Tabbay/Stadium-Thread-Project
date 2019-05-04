#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
//TODO: Create objects

//contents of a ticket
typedef struct Ticket {
    int number_of_tickets; //number of tickets the customer will get
    int customer_id; 
    int ticket_id;
    struct Ticket *next;
} Ticket;

typedef struct Stadium {
    int maximum_seats; //Maxium seats in the stadium
    int seats_left; //# of seats left in the stadium
    int total_seats_handled; //# of tickets added 
    int seller_count; //count of how many people sell
    int MAX_SELLER_SEATS; //maximum amount of sellers
    Ticket* tickets; //Ticket List
    pthread_mutex_t mutex; 
    pthread_cond_t can_add_tickets, can_sell_tickets;
}Stadium;

//Create function protypes and descriptions

/**
 * Opens the Stadium
 *  - allocate space for the Stadium
 *  - initialize all its variables
 *  - initialize its synchronization objects
 */
Stadium* OpenStadium(int MAXIMUM_SEATS, int MAX_SELLER_SEATS);

/**
 * Closes the Stadium
 * -make sure there are no more seats to be fullfilled
 * -destroy all synchronization objects
 * -free the space of the Stadium
 */

void CloseStadium(Stadium *techxarena);

/**
 * Picks a random ticket number 1-6 and returns it.
 */
int randomTicketPicker();

/**
 * get a number of tickets 
 * allocate space for a new ticket object
 * Lock Mutex
 * check to see if there are seats avaliable
 * wait until there are avaliable seats 
 * Add tickets 
 * decrement seats left 
 * increments total_seats_handled
 * if there are less seats than wanted, keep decreasing tickets)
 */
bool addTicket(Stadium *techxarena);

//requestTicket
/** 
 * Adds a ticket object to the Stadium tickets
*/
void requestTicket(Ticket **listofTickets, Ticket *ticket);
 
/**
 * Sell tickets off the ticket list
 * Checks to see if seller count has reached the limit
 * increse seats left
 */
void sellTicket(Stadium *techxarena);

/**
 * -lock all threads
 * -get from the front list
 * -decrease seats_sold
 */
Ticket *getTicket(Ticket **listofticket);

/**
 * -get a ticket from the TicketList
 * -increment the seats_sold
 */
void print1(Ticket **listoftickets);