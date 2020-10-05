#define SUCCESS 0

#define NUMBER_OF_STORE 5
#define NUMBER_OF_BUYER 3

#define MAX_START_RANGE_PRODUCTS_STORAGE 1200
#define MIN_START_RANGE_PRODUCTS_STORAGE 1000

#define MAX_FINISH_PRODUCTS_BUYER 3500
#define MIN_FINISH_PRODUCTS_BUYER 3000

#define MAX_LOAD_PRODUCTS_LOADER 500
#define MIN_LOAD_PRODUCTS_LOADER 300

#define SLEEP_LOADER 1
#define SLEEP_BUYER 3

struct store_s
{
	int storage;
	int busy;
	int buyer_finished;
	struct store_s *store_array;
};
typedef struct store_s store_t;

struct buyer_s
{
	int id;
	int max_products;
	int curr_products;
	int curr_products_finish;
	store_t *store_array;
	struct buyer_s *buyer_array;
};
typedef struct buyer_s buyer_t;


int threads_main(void);