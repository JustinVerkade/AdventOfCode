//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Set_t Set_t;
struct Set_t {
    uint8_t generator:4;
    uint8_t microchip:4;
};

typedef struct State_t State_t;
struct State_t {
    uint8_t elevator;
    uint8_t set_count;
    Set_t sets[8];
    uint32_t steps;
};

typedef struct Queue_t Queue_t;
struct Queue_t {
    State_t* buffer;
    uint32_t count;
    uint32_t size;
    uint32_t write_ptr;
    uint32_t read_ptr;
};

typedef struct Cache_t Cache_t;
struct Cache_t {
    State_t* buffer;
    uint32_t count;
    uint32_t size;
    uint32_t write_ptr;
};

//######################################################################################################################
// functions

int8_t state_check(State_t* state)
{
    for (int8_t i=0; i<state->set_count; i++) {
        Set_t check_set = state->sets[i];

        // if generator is on same floor as microchip the chip is safe.
        if (check_set.microchip == check_set.generator) {
            continue;
        }

        // if not the case check if microchip is on the same floor as a different generator.
        for (int8_t j=0; j<state->set_count; j++) {
            Set_t test_set = state->sets[j];

            // ignore itself
            if (j == i) {
                continue;
            }

            // check if microchip is o n the same floor as another generator
            // if so the state is invalid.
            if (check_set.microchip == test_set.generator) {
                return 0;
            }
        }
    }
    return 1;
}

int8_t state_done(State_t* state)
{
    if (state->elevator != 3) {
        return 0;
    }
    for (int8_t i=0; i<state->set_count; i++) {
        Set_t check_set = state->sets[i];
        if (check_set.generator != 3 || check_set.microchip != 3) {
            return 0;
        }
    }
    return 1;
}

int8_t state_sort(State_t* state)
{
    for (;;) {
        int8_t sorted = 1;
        for (int8_t i=0; i<state->set_count-1; i++) {
            Set_t* set_0 = &state->sets[i];
            Set_t* set_1 = &state->sets[i + 1];

            if (set_0->generator < set_1->generator) {
                Set_t temp;
                temp.generator = set_1->generator;
                temp.microchip = set_1->microchip;
                set_1->generator = set_0->generator;
                set_1->microchip = set_0->microchip;
                set_0->generator = temp.generator;
                set_0->microchip = temp.microchip;
                sorted = 0;
                break;
            } else if (set_0->generator == set_1->generator) {
                if (set_0->microchip < set_1->microchip) {
                    Set_t temp;
                    temp.generator = set_1->generator;
                    temp.microchip = set_1->microchip;
                    set_1->generator = set_0->generator;
                    set_1->microchip = set_0->microchip;
                    set_0->generator = temp.generator;
                    set_0->microchip = temp.microchip;
                    sorted = 0;
                    break;
                }
            }
        }
        if (sorted) {
            break;
        }
    }
    return 1;
}

void state_print(State_t* state)
{
    printf("\n");
    for (int8_t i=3; i>=0; i--) {
        printf("F%d ", i + 1);
        if (state->elevator == i) {
            printf("E  ");
        } else {
            printf(".  ");
        }

        for (int8_t j=0; j<state->set_count; j++) {
            if (state->sets[j].generator == i) {
                printf("%cG ", j + 'A');
            } else {
                printf(".  ");
            }
            if (state->sets[j].microchip == i) {
                printf("%cM ", j + 'A');
            } else {
                printf(".  ");
            }
        }
        printf("\n");
    }
}

Queue_t* queue_create(uint32_t size)
{
    Queue_t* queue = (Queue_t*)malloc(sizeof(Queue_t));
    if (queue == NULL) {
        printf("Failed to allocate Queue!\n");
        return NULL;
    }

    queue->buffer = (State_t*)malloc(sizeof(State_t) * size);
    if (queue->buffer == NULL) {
        free(queue);
        printf("Failed to allocate Queue buffer!\n");
        return NULL;
    }
    
    queue->count = 0;
    queue->size = size;
    queue->write_ptr = 0;
    queue->read_ptr = 0;

    return queue;
}

void queue_destroy(Queue_t* queue)
{
    free(queue->buffer);
    free(queue);
}

int8_t queue_push(Queue_t* queue, State_t* state)
{
    // copy data into queue
    State_t* queue_state = &queue->buffer[queue->write_ptr];
    memcpy(queue_state, state, sizeof(State_t));
    queue->write_ptr = (queue->write_ptr + 1) % queue->size;
    queue->count++;

    if (queue->count > queue->size) {
        printf("Queue overflowed!\n");
        for (;;);
        return 1;
    }
    return 0;
}

State_t* queue_pop(Queue_t* queue)
{
    // return NULL if empty
    if (queue->count == 0) {
        return NULL;
    }

    // get state
    State_t* queue_state = &queue->buffer[queue->read_ptr];
    queue->read_ptr = (queue->read_ptr + 1) % queue->size;
    queue->count--;
    return queue_state;
}

Cache_t* cache_create(uint32_t size)
{
    Cache_t* cache = (Cache_t*)malloc(sizeof(Cache_t));
    if (cache == NULL) {
        printf("Failed to allocate Cache!\n");
        return NULL;
    }

    cache->buffer = (State_t*)malloc(sizeof(State_t) * size);
    if (cache->buffer == NULL) {
        free(cache);
        printf("Failed to allocate Cache buffer!\n");
        return NULL;
    }
    
    cache->count = 0;
    cache->size = size;
    cache->write_ptr = 0;

    return cache;
}

void cache_destroy(Cache_t* cache)
{
    free(cache->buffer);
    free(cache);
}

int8_t cache_push(Cache_t* cache, State_t* state)
{
    // copy data into queue
    State_t* cache_state = &cache->buffer[cache->write_ptr];
    memcpy(cache_state, state, sizeof(State_t));
    cache->write_ptr = (cache->write_ptr + 1) % cache->size;
    cache->count++;

    if (cache->count > cache->size) {
        printf("Cache overflowed!\n");
        for (;;);
        return 1;
    }
    return 0;
}

int8_t cache_exists(Cache_t* cache, State_t* state)
{
    for (uint32_t i=0; i<cache->count; i++) {
        State_t* cache_state = &cache->buffer[i];
        if (cache_state->elevator != state->elevator) {
            continue;
        }
        int8_t valid = 1;
        for (int8_t j=0; j<state->set_count; j++) {
            if (cache_state->sets[j].generator != state->sets[j].generator) {
                valid = 0;
                break;
            } else if (cache_state->sets[j].microchip != state->sets[j].microchip) {
                valid = 0;
                break;
            }
        }
        if (valid) {
            return 1;
        }
    }
    return 0;
}

//######################################################################################################################

int solve_puzzle(char* file_name)
{
    // create base state
    // State_t base_state;
    // base_state.elevator = 0;
    // base_state.set_count = 2;
    // base_state.sets[0].generator = 1;
    // base_state.sets[0].microchip = 0;
    // base_state.sets[1].generator = 2;
    // base_state.sets[1].microchip = 0;
    // base_state.steps = 0;

    State_t base_state;
    base_state.elevator = 0;
    base_state.set_count = 5;
    base_state.sets[0].generator = 0; // polonium
    base_state.sets[0].microchip = 1;
    base_state.sets[1].generator = 0; // thulium
    base_state.sets[1].microchip = 0;
    base_state.sets[2].generator = 0; // promethium
    base_state.sets[2].microchip = 1;
    base_state.sets[3].generator = 0; // ruthenium
    base_state.sets[3].microchip = 0;
    base_state.sets[4].generator = 0; // cobalt
    base_state.sets[4].microchip = 0;
    base_state.steps = 0;

    // check base state
    state_sort(&base_state);

    // create queue variables
    Queue_t* queue = queue_create(10000);
    Cache_t* cache = cache_create(100000);

    // insert the base case
    queue_push(queue, &base_state);
    cache_push(cache, &base_state);

    // execute BFS
    while (queue->count) {
        // pop the next queue state
        State_t* state = queue_pop(queue);

        // exit if we are done
        int8_t done = state_done(state);
        if (done) {
            printf("Steps: %d\n", state->steps);
            break;
        }

        // up or down (0 = UP, 1 = DOWN)
        for (int8_t direction=0; direction<2; direction++) {

            // we do not continue if the elevator is at the top 
            // or bottom and tries to move out of bounds.
            if (direction == 0 && state->elevator == 3) {
                continue;
            } else if (direction == 1 && state->elevator == 0) {
                continue;
            }

            // find first item to cary.
            // check all valid moves
            for (uint8_t first_item=0; first_item<base_state.set_count * 2; first_item++) {
                int8_t set_index = first_item / 2;
                int8_t type_index = first_item % 2 != 0;

                // check if item is on the same floor
                Set_t* item0 = &state->sets[set_index];
                int8_t platform = type_index ? item0->microchip : item0->generator;
                
                if (platform != state->elevator) {
                    continue;
                }

                // create copy of item and move first object
                State_t new_state;
                memcpy(&new_state, state, sizeof(State_t));
                int8_t change = direction == 0 ? 1 : -1;

                new_state.steps++;
                new_state.elevator += change;
                if (!type_index) {
                    new_state.sets[set_index].generator += change;
                } else {
                    new_state.sets[set_index].microchip += change;
                }

                // sort state
                state_sort(&new_state);
                
                // skip if the state is invalid.
                if (state_check(&new_state) == 0) {
                    continue;
                }

                // skip if state is already in cache
                if (cache_exists(cache, &new_state) == 0) {
                    // add state to queue and cache
                    queue_push(queue, &new_state);
                    cache_push(cache, &new_state);
                }

                // find second item to cary.
                // check all valid moves
                for (uint8_t second_item=0; second_item<base_state.set_count * 2; second_item++) {
                    int8_t set_index = second_item / 2;
                    int8_t type_index = second_item % 2 != 0;

                    // check if item is on the same floor
                    Set_t* item0 = &new_state.sets[set_index];
                    int8_t platform = type_index ? item0->microchip : item0->generator;
                    
                    if (platform != state->elevator) {
                        continue;
                    }

                    // create copy of item and move second object
                    State_t new_state2;
                    memcpy(&new_state2, &new_state, sizeof(State_t));
                    int8_t change = direction == 0 ? 1 : -1;
                    
                    if (!type_index) {
                        new_state2.sets[set_index].generator += change;
                    } else {
                        new_state2.sets[set_index].microchip += change;
                    }

                    // sort state
                    state_sort(&new_state2);
                    
                    // skip if the state is invalid.
                    if (state_check(&new_state2) == 0) {
                        continue;
                    }

                    // skip if state is already in cache
                    if (cache_exists(cache, &new_state2)) {
                        continue;
                    }

                    // add state to queue and cache
                    queue_push(queue, &new_state2);
                    cache_push(cache, &new_state2);
                }
            }
        }


    }

    if (queue->count == 0) {
        printf("Fuck!\n");
    }

    // destroy objects
    queue_destroy(queue);
    cache_destroy(cache);
}

//######################################################################################################################

int main(int argc, char* argv[])
{
    if (argc == 1) {
        printf("Please add the file name with the exeutable!\n");
        return 1;
    }
    char* file_name = argv[1];
    solve_puzzle(file_name);
    return 0;
}