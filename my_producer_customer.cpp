#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <semaphore.h>


const int BUFFER_SIZE = 5;
const int NUM_TASKS = 10;

// Buffer and semaphores
std::vector<int> buffer;
sem_t empty;
sem_t full;
sem_t mutex;

void producer() {
    for (int i = 1; i <= NUM_TASKS; ++i) {
        sem_wait(&empty); 
        sem_wait(&mutex); 
        buffer.push_back(i); 
        std::cout << "Produced: " << i << std::endl;
        sem_post(&mutex); 
        sem_post(&full); 
        
        
        if (buffer.size() == BUFFER_SIZE) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void consumer() {
    int item;
    for (int i = 0; i < NUM_TASKS; ++i) {
        sem_wait(&full); 
        sem_wait(&mutex); 
        item = buffer.back(); 
        buffer.pop_back(); 
        std::cout << "Consumed: " << item << std::endl;
        sem_post(&mutex); 
        sem_post(&empty); 
    }
}

int main() {
    
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    
    std::thread producer_thread(producer);
    std::this_thread::sleep_for(std::chrono::seconds(1)); 
    std::thread consumer_thread(consumer);

   
    producer_thread.join();
    consumer_thread.join();

    // Destroy semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
