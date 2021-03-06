/*
100% ANSI X3.159-1989 compliant (ANSI C89)

21C version 1.0.1 - September 30th 2020
Copyright (c) 2020, Alex Free 
All rights reserved. 

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met: 

 * Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer. 
 * Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the 
   documentation and/or other materials provided with the distribution. 
 * Neither the name of Alex Free nor the names of its contributors may be 
   used to endorse or promote products derived from this software without 
   specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define win 21
#define risk 16
#define deck 14
//clock differences, if not windows or dos guesses unix like
#if defined __WIN32__
#define delay 2000
#elif defined __DOS__ || __MSDOS__
#define delay 20
#else
#define delay 200
#endif

typedef enum { FALSE, TRUE } bool;

bool dealer_out;
bool dealer_busted;
bool your_out;
bool you_busted;
bool bad_input;
bool has_shown_2nd_card;
bool end_round;
bool valid_input;
bool bad_card_check;
bool even_money;
bool end_game;
bool skip_verbose_results;
bool bet_checked;
bool split;
bool double_down;
bool already_surrendered;

int your_split_card;
int your_ace_count;
int dealer_ace_count;
int player;
int input;
int mode;
int your_first_card;
int your_second_card;
int your_next_card;
int your_sum;
int your_new_sum;
int your_lower_sum;
int dealer_first_card;
int dealer_second_card;
int dealer_next_card;
int dealer_sum;
int dealer_new_sum;
int dealer_lower_sum;
int card;

float maximum_bet;
float your_total_money;
float your_new_money_total;
float your_insurance_bet_total;
float your_bet;
float minimum_bet;
float double_down_bet;

char yn_response[2];
char custom_bet[10];
char save_money[100];

FILE * save;

void gamble(int mode)
{

	switch(mode)
	{
//loose initial bet
	case 1:	your_new_money_total = your_total_money-your_bet;
			your_total_money = your_new_money_total;
			break;
//win initial bet
	case 2: your_new_money_total = your_total_money + your_bet;
			your_total_money = your_new_money_total;
			break;
//2 to 1 payout for winning insurance bet
	case 3: your_insurance_bet_total = your_bet * 0.5;
			your_new_money_total = your_total_money + your_insurance_bet_total * 2;
			your_total_money = your_new_money_total;
			break;			
//loose insurance bet, half of initial bet. Also used for surrendering as it is the same outcome
	case 4: your_insurance_bet_total = your_bet * 0.5;
			your_new_money_total = your_total_money-your_insurance_bet_total;
			your_total_money = your_new_money_total;
			break;
//did not take even money with 21 on insurance bet and and won 3:2
	case 5:	your_insurance_bet_total = your_bet * 0.5 * 3;
			your_new_money_total = your_total_money + your_insurance_bet_total;
			your_total_money = your_new_money_total;
			break;
//double_down bet, not supported on split
	case 6:	double_down_bet = your_bet * 2;
	your_bet = double_down_bet;
	printf("Your new bet is $%.2f\n", your_bet);
			break;
	}
	printf("You have $%.2f\n", your_total_money);
}

int get_input()
{
	while(!valid_input)
	{
	printf("(y/n): ");
	scanf(" %s", &yn_response);
		if(strcmp(yn_response,"y") == 0) 
		{
		input = 1;
		return input;
		valid_input = TRUE;
		}
		else if(strcmp(yn_response,"n") == 0) 	
		{
		input = 2;
		return input;
		valid_input = TRUE;				
		} 
		else
		{
		printf("Invalid input, try again\n");
		}
	}
}

int deal(int player) 
{
int card;
	switch(player) 
	{
	case 1: printf("Dealing to you...\n");
			break;
	case 2: printf("Dealing to Dealer...\n");
			break;
	}

	while(!bad_card_check)
	{
	clock_t current_time = clock();
		while(clock() < current_time + delay)
		;

	card = rand() % deck;

    switch(card) 
	{
    case 10: switch(player) 
			{
			 case 1: printf("-----\n| 10|\n|   |\n|   |\n|10 |\n-----\n");
			 		 break;
			 case 2: if(has_shown_2nd_card)
					printf("-----\n|  10|\n|   |\n|   |\n|10  |\n-----\n");
					 break;
			 }
			 break;
	case 11: card = 11;
             switch(player) 
			{
			 case 1: your_ace_count++;
			         printf("-----\n|  A|\n|   |\n|   |\n|A  |\n-----\n");
			 		 break;
			 case 2: dealer_ace_count++;
					 if(has_shown_2nd_card)
					 printf("-----\n|  A|\n|   |\n|   |\n|A  |\n-----\n");
					 break;
			 }
			 break;
	case 12: card = 10;
			 switch(player) 
			 {
			 case 1: printf("-----\n|  Q|\n|   |\n|   |\n|Q  |\n-----\n");
			 		 break;
			 case 2: if(has_shown_2nd_card)
					 printf("-----\n|  Q|\n|   |\n|   |\n|Q  |\n-----\n");
					 break;
			 }
			 break;
	case 13: card = 10;
			 switch(player) 
			 {
			 case 1: printf("-----\n|  K|\n|   |\n|   |\n|K  |\n-----\n");
			 		 break;
			 case 2: if(has_shown_2nd_card)
					 printf("-----\n|  K|\n|   |\n|   |\n|K  |\n-----\n");
					 break;
			}
			break;
	}
			
         if(card < 10 && card > 1)
         {
         	switch(player) 
			 {
			 case 1: printf("-----\n|  %d|\n|   |\n|   |\n|%d  |\n-----\n", card, card);
			 		 break;
			 case 2: if(has_shown_2nd_card)
					 printf("-----\n|  %d|\n|   |\n|   |\n|%d  |\n-----\n", card, card);
					 break;
			}
        }

         
//if not zero or 1 end loop
		if(card > 1) 
		{
		return card;
		bad_card_check = TRUE;
		}
	}
}

void ace_bust(){
	if (your_sum > win )  {
		if(your_ace_count > 0) {
		printf("You have %d soft ace(s) left!\n", your_ace_count);
		your_lower_sum = your_sum-10;
		your_sum = your_lower_sum;
		your_ace_count--;
		printf("1 soft ace used, hard ace valued at 1 because 11 busts. New sum: %d\n", your_sum);
		printf("You have %d soft ace(s) left!\n", your_ace_count);
		}
	}

	if (dealer_sum > win )  {
		if(dealer_ace_count > 0) {
			if(has_shown_2nd_card) 
			printf("Dealer has %d soft ace(s) left!\n", dealer_ace_count);
			dealer_lower_sum = dealer_sum-10;
			dealer_sum = dealer_lower_sum;
			dealer_ace_count--;
				if(has_shown_2nd_card)
				printf("1 soft ace used, hard ace valued at 1 because 11 busts. New sum: %d\n Dealer has %d soft ace(s) left!\n", dealer_sum, dealer_ace_count);
		}
	}

}

void lucky_stars(){
	printf("*******\n");
}


main(){

srand(time(NULL));
minimum_bet = 5.00;
maximum_bet = 10000.00;
split = FALSE;
your_split_card = FALSE;
lucky_stars();
printf("21C\n");
save = fopen("save.txt", "r");
	if(save)
    {
    printf("Load save?\n");
    get_input();
		switch (input) 
        {
		case 1:	if(fgets(save_money,sizeof(save_money), save)!=NULL)
				{
				your_total_money = strtod(save_money, NULL);
					if(save_money != 0) 
                    {
                    printf("Load ok\n");
                    }
                    else
                    {
                    printf("Load failed, starting at default\n");
                    your_total_money = 2500;
                    }
                }
                else
				{
                printf("Could not read save data!\n");
                your_total_money = 2500;
                }
				break;
        case 2:	printf("New game\n");
                your_total_money = 2500;
			   	break;
	    }  
		fclose(save);
	}
	else
	{
	your_total_money = 2500;
	}
	
while(!end_game){

//reset
	dealer_out = FALSE;
	dealer_busted = FALSE;
	your_out = FALSE;
	you_busted = FALSE;
	bad_input  = FALSE;
	has_shown_2nd_card = FALSE;
	end_round = FALSE;
	valid_input = FALSE;
	bad_card_check = FALSE;
	even_money = FALSE;
	end_game = FALSE;
	skip_verbose_results = FALSE;
	bet_checked = FALSE;
	double_down = FALSE;
	already_surrendered = FALSE;
	
	card = 0;
	your_ace_count = 0;
	dealer_ace_count = 0;
	player = 0;
	input = 0;
	mode = 0;
	your_first_card = 0;
	your_second_card = 0;
	your_next_card = 0;
	your_sum = 0;
	your_new_sum = 0;
	your_lower_sum = 0;
	dealer_first_card = 0;
	dealer_second_card = 0;
	dealer_next_card = 0;
	dealer_sum = 0;
	dealer_new_sum = 0;
	dealer_lower_sum = 0;
	player = 1;

		if(!split) 
        {
		printf("You have $%.2f\n", your_total_money);
		printf("Maximum bet is $%.2f\n", maximum_bet);
//first bet
		printf("Place the minimum bet ($%.2f)?\n", minimum_bet);
		get_input();
			switch (input) 
			{
				case 1: printf("You placed the minimum bet of $%.2f\n", minimum_bet);
						your_bet = minimum_bet;
						break;
				case 2: while(!bet_checked)
						{					
							custom_bet == 0;
							printf("Bet: $");
							scanf(" %10s", &custom_bet); 
							your_bet = strtod(custom_bet, NULL); 
								if (your_bet >= minimum_bet && your_bet <= maximum_bet && your_bet <= your_total_money) 
								{
								printf("You bet $%.2f\n", your_bet);
								bet_checked = TRUE;
								}
//failed conversion, such as letters = 0
								else if (your_bet < minimum_bet && your_bet != 0) 
								{
								printf("That's lower then the minimum bet of $%.2f, try again\n", minimum_bet);
								}
								else if (your_bet > maximum_bet && your_total_money >= your_bet) 
								{
								printf("That's higher then the maximum bet of $%.2f, try again\n", maximum_bet);
								}
								else if (your_bet > maximum_bet && your_total_money <= your_bet) 
								{
								printf("That's higher then the maximum bet of $%.2f, and you don't have enough money, try again\n", maximum_bet);						
								}
								else
								{
								printf("Invalid input, try again\n");
								}
						}
					 	break;
			} 
		your_first_card = deal(player);
		}
		else 
		{
		printf("This is your second hand of your previous split\n");
		your_first_card = your_split_card;
//infinite splitting supported
		split = FALSE;
		}
		
		player = 2;
		dealer_first_card = deal(player);
		player = 1;
		your_second_card = deal(player);
		player = 2;
		dealer_second_card = deal(player);
		your_sum = your_first_card + your_second_card;
		printf("Your 1st card is valued at %d\n", your_first_card);
		printf("Your 2nd card is valued at %d\n", your_second_card);
		printf("You have %d\n", your_sum);
		dealer_sum = dealer_first_card + dealer_second_card;
		printf("Dealer's 1st card is valued at %d\n",  dealer_first_card);
		
        if(your_sum != win) 
        {
		printf("Do you want to surrender early and loose half your bet?\n");
		get_input();
			switch (input) {
				case 1: printf("You surrender\n");
						mode = 4;
						gamble(mode);
						end_round = TRUE;
						your_out = TRUE;
						already_surrendered = TRUE;
						skip_verbose_results = TRUE;
						break;
				case 2: printf("You continue on\n");
						break;
			}		
      }
//unmatched tens supported on split pairs
		if(your_first_card == your_second_card && !already_surrendered){
		printf("Would you like to split these %d's?\n", your_first_card);
		get_input();
			switch (input) {
				case 1: printf("You split\n");
						split = TRUE;
						your_split_card = your_second_card;
						your_new_sum = your_sum-your_split_card;
						your_sum = your_new_sum;
						break;
				case 2: printf("You continue without splitting\n");
						break;
			}
		}

	ace_bust();

//insurance bet
		if(dealer_first_card == 11) {
		printf("The Dealer's first card is an Ace, would you like to place an insurance bet (half your bet)?\n");
		get_input();
			switch (input) {
				case 1: printf("Placed insurance bet\n");
//if you have 21, you can break even by being paid twice your insurance bet of half your original bet while loosing your entire original bet
//you have blackjack
						if(your_sum == win){
						printf("You have 21, do you want even money?\n");
						get_input();
							switch (input) {
								case 1: printf("Your taking the even money\n");
										even_money = TRUE;
										break;
								case 2: printf("Not placing insurance bet\n");											break;
							}			
//no money lost or gained due to tie
							if((dealer_second_card == 10) && (!even_money)){
							printf("This is a standoff, both of you have 21 no one wins anything\n");
							end_round = TRUE; 
							skip_verbose_results = TRUE;
							} else if((dealer_second_card == 10) && (even_money)){
							printf("Good choice choosing the even money, this would have been a standoff, both of you have 21\n");
							end_round = TRUE;
							skip_verbose_results = TRUE;
//no insurance bet, blackjack, no even money, won 3 * insurance bet 
							} else if((dealer_second_card != 10) && (!even_money)){
							end_round = TRUE;
							skip_verbose_results = TRUE;
							printf("You win the insurance bet 3:2, the Dealer's second card was %d, not blackjack\n", dealer_second_card);
							mode = 5;
							gamble(mode);
							}
						} 
//you don't have blackjack
						if(your_sum != win){
//dealer has blackjack, player paid half of the original bet (insurance) 2:1 but looses original bet so nothing lost or gained
							if(dealer_second_card == 10) {
							skip_verbose_results = TRUE;
							end_round = TRUE;
							printf("You win the insurance bet, the Dealer's second card was %d\n", dealer_second_card);
							mode = 3;
							gamble(mode);
//delaer does not have blackjack, you loose insurance (half of your original bet) but your original bet remains the same
							} else if(dealer_second_card != 10)  {
							printf("You loose the insurance bet, the Dealer's second card was %d\n", dealer_second_card);
							mode = 4;
							gamble(mode);
//keep playing if lost bet and you have not won
							}	 

						}
					break;
			case 2: printf("Not placing insurance bet\n");
					break;
		}
	}

//double down bet, not supported if split
		if (your_sum > 8 && your_sum < 12 && !end_round && !split) {
		printf("Double down? Your current sum is %d\n", your_sum);
		get_input();
			switch (input) {
				case 1: mode = 6;
						gamble(mode);
						double_down = TRUE;
						break;
				case 2: printf("Not doubling down\n");
						break;
			
			}
		}
		
		if(!has_shown_2nd_card) {
		printf("Dealer's 2nd card is valued at %d\n", dealer_second_card);
		has_shown_2nd_card = TRUE;
		}
		printf("Dealer has %d\n", dealer_sum);
		while(!end_round) {
			if(!your_out && your_sum < win && dealer_sum < win){
			printf("Hit?\n");
			get_input();
				switch (input) {
					case 1: player = 1;
							your_next_card = deal(player);
							printf("Your new card is valued at %d\n", your_next_card);
							your_new_sum = your_sum + your_next_card;
							your_sum = your_new_sum;
							printf("You have %d\n", your_sum);
//only can hit once on double down
								if(double_down)
								your_out = TRUE;
							break;
					case 2: your_out = TRUE;
							printf("You stand with %d\n", your_sum);
							break;		
				}	
		}

		ace_bust();
	
		if(dealer_sum < risk && your_sum < win) {
		ace_bust();
		printf("Dealer has to hit with %d\n", dealer_sum);
		player = 2;
		dealer_next_card = deal(player);
		printf("Dealer's new card value: %d\n", dealer_next_card);
		dealer_new_sum = dealer_sum + dealer_next_card;
		dealer_sum = dealer_new_sum;
		printf("Dealer's new sum: %d\n", dealer_sum);
		} else {
            if(your_sum < win)
	      	printf("Dealer stands with %d\n", dealer_sum);
		dealer_out = TRUE;
		}

		if(dealer_out && your_out) 
		end_round = TRUE;

		ace_bust();

		if (your_sum >= win) 
		end_round = TRUE;
		if (dealer_sum >= win) 
		end_round = TRUE;
		
		}
//results
lucky_stars();

		if(!skip_verbose_results){
		
			if((dealer_sum == win) && (your_sum == win)) {
			printf("No one, you and the Dealer tied with %d\n", your_sum);
			
			printf("you have $ %.2f\n", your_total_money);
			}

			if (your_sum > win) {
			printf("You busted\n");
			you_busted = TRUE;
			}

			if (dealer_sum > win) {
			printf("Dealer busted\n");
			dealer_busted = TRUE;
			}


			if(you_busted && dealer_busted) {
			printf("You and Dealer both busted so no one wins...\n");
			printf("You have $ %.2f\n", your_total_money);
			}

			if(!you_busted && dealer_busted) {
			printf("You beat the Dealer with %d, the Dealer had %d\n", your_sum, dealer_sum);
			mode = 2;
			gamble(mode);
			}

			if(you_busted && !dealer_busted) {
			printf("The Dealer beat you with %d, you had %d\n", dealer_sum, your_sum);
			mode = 1;
			gamble(mode);
			}

			if(!you_busted && !dealer_busted){

				if(your_sum == win && dealer_sum != win) {
				printf("You beat the Dealer with %d, the Dealer had %d\n", your_sum, dealer_sum);
				mode = 2;
				gamble(mode);
				}

				if(your_sum != win && dealer_sum == win) {
				printf("The Dealer beat you with %d, you had %d\n", dealer_sum, your_sum);
				mode = 1;
				gamble(mode);
				}


				if(your_sum != win && dealer_sum != win) {
				printf("No one got exactly 21, so the winner is...\n");
					if(your_sum > dealer_sum ) {
					printf("You with %d, the Dealer had %d\n", your_sum, dealer_sum); 
					mode = 2;
					gamble(mode);
					}

					if(dealer_sum > your_sum ) {
					printf("The Dealer with %d, you had %d\n", dealer_sum, your_sum);
					mode = 1;
					gamble(mode);
					}

					if(dealer_sum == your_sum ) {
					printf("No one, you and the Dealer tied with %d\n", your_sum);
					printf("you have $ %.2f\n", your_total_money);
					}
				}
			}
		}
	if(!split)
    {
	printf("Up for another game?\n");
	get_input();
		switch (input) 
        {
			case 1: printf("Next round...\n");
					break;
			case 2: printf("Save?\n");
				get_input();
		        switch (input) 
                {
		               	case 1: save = fopen("save.txt", "w+");
		               	             if(!save)
		               	             {
                                     printf("Could not save!\n");
                                     }
                                     else
                                     {
                                     fprintf(save, "%.2f\n", your_total_money);
                                     fclose(save);
                                     printf("Saved!\n");
                                     }
				             	break;
		            	case 2: printf("GAME OVER\n");
				              	break;		
               }		
               end_game = TRUE;
			   lucky_stars();
               break;
		}
	} 
//end_game loop
}

return(0);
}