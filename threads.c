#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h> 

#include "threads.h"

int get_rand_range_int(const int min, const int max);
void* add_products(void *store);
void* buy_products(void *buyer);

int threads_main(void)
{
	int status;
	int status_addr;

	store_t store[NUMBER_OF_STORE];
	buyer_t buyer[NUMBER_OF_BUYER];

	pthread_t thread_buyer[NUMBER_OF_BUYER];
	pthread_t thread_loader;

	for (int i = 0; i < NUMBER_OF_STORE; i++)
	{
		store[i].storage = get_rand_range_int(MIN_START_RANGE_PRODUCTS_STORAGE, MAX_START_RANGE_PRODUCTS_STORAGE);
		printf("Store №%d storage = %d\n", i, store[i].storage);
		store[i].buyer_finished = 0;
		pthread_mutex_init(&store[i].mutex, NULL);
	}
	store[0].store_array = store;

	for (int i = 0; i < NUMBER_OF_BUYER; i++)
	{
		buyer[i].max_products = get_rand_range_int(MIN_FINISH_PRODUCTS_BUYER, MAX_FINISH_PRODUCTS_BUYER);
		printf("Buyer №%d max_products = %d\n", i, buyer[i].max_products);
		buyer[i].curr_products = 0;
		buyer[i].curr_products_finish = 0;
		buyer[i].store_array = store;
		buyer[i].id = i;
	}
	buyer[0].buyer_array = buyer;

	printf("Create thread_buyer\n");
	for(int i = 0; i < NUMBER_OF_BUYER; i++){
		status = pthread_create(&thread_buyer[i], NULL, buy_products, &buyer[i]);
		if (status != 0){
			printf("threads_main error: can't create thread_buyer, status = %d\n", status);
		}
	}

	printf("create thread_loader\n");
	status = pthread_create(&thread_loader, NULL, add_products, &store[0]);
	if (status != 0)
		printf("threads_main error: can't create thread_buyer, status = %d\n", status);

    printf("join thread_loader\n");
    status = pthread_join(thread_loader, (void**)&status_addr);
    if (status != SUCCESS)
        printf("threads_main error: can't join thread_loader, status = %d\n", status);
    printf("loader joined with address %d\n", status_addr);
	
    for (int i = 0; i < NUMBER_OF_BUYER; i++) {
    	printf("join thread_buyer №%d\n", i);
        status = pthread_join(thread_buyer[i], (void**)&status_addr);
        if (status != SUCCESS)
            printf("threads_main error: can't join thread_buyer, status = %d\n", status);
        printf("buyer joined with address %d\n", status_addr);
    }


    for (int i = 0; i < NUMBER_OF_STORE; i++)
    	pthread_mutex_destroy(&store[i].mutex);

    sleep(1);
    for (int i = 0; i < NUMBER_OF_BUYER; i++){
    	printf("Buyer №%d curr_products = %d max_products = %d \n", buyer[i].id, buyer[i].curr_products, buyer[i].max_products);
    }

    return SUCCESS;

}

void* add_products(void *store)
{
	store_t *stor = (store_t*) store;
	while (stor->buyer_finished < NUMBER_OF_BUYER){
		int i = get_rand_range_int(0, NUMBER_OF_STORE - 1);
		pthread_mutex_lock(&stor->store_array[i].mutex);
		int count = get_rand_range_int(MIN_LOAD_PRODUCTS_LOADER, MAX_LOAD_PRODUCTS_LOADER);
		stor->store_array[i].storage += count;
		printf("\nAdd products in store №%d quantity of goods = %d\n", i+1, count);
		printf("Storage in all store:\n1)%d 2)%d 3)%d 4)%d 5)%d\n\n", 
			stor->store_array[0].storage,
			stor->store_array[1].storage,
			stor->store_array[2].storage,
			stor->store_array[3].storage,
			stor->store_array[4].storage);
		pthread_mutex_unlock(&stor->store_array[i].mutex);
		sleep(SLEEP_LOADER);
	}
	return SUCCESS;
}

void* buy_products(void *buyer)
{
	int count = 0;
	buyer_t *temp_buyer = (buyer_t*) buyer;
	
	while(temp_buyer->curr_products_finish == 0){
		int i = get_rand_range_int(0, NUMBER_OF_STORE - 1);
		pthread_mutex_lock(&temp_buyer->store_array[i].mutex);
		if (temp_buyer->store_array[i].storage < (temp_buyer->max_products - temp_buyer->curr_products)){
			count = temp_buyer->store_array[i].storage;
			temp_buyer->store_array[i].storage -= count;
			temp_buyer->curr_products += count;
		}
		else{
			count = temp_buyer->max_products - temp_buyer->curr_products;
			temp_buyer->store_array[i].storage -= count;
			temp_buyer->curr_products += count;
		}
		pthread_mutex_unlock(&temp_buyer->store_array[i].mutex);
		if (temp_buyer->curr_products >= temp_buyer->max_products)
			temp_buyer->curr_products_finish = 1;
		printf("Buyer №%d take from Store №%d quantity of goods = %d curr_products = %d \n", temp_buyer->id, i, count, temp_buyer->curr_products);
		sleep(SLEEP_BUYER);
	}
	temp_buyer->store_array[0].buyer_finished++;

	return SUCCESS;
}

int get_rand_range_int(const int min, const int max)
{
    return rand() % (max - min + 1) + min;
}