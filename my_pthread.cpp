#include <iostream>
#include <pthread.h>
#include <sstream>

int number = 330;
pthread_mutex_t lock;

void *mythread(void *arg) {
    long thread_id = reinterpret_cast<long>(arg);

    pthread_mutex_lock(&lock);  

    std::cout << "thread " << thread_id << ": begin (number = " << number << ")" << std::endl;
    number *= 2;
    std::cout << "thread " << thread_id << ": done (number = " << number << ")" << std::endl;

    pthread_mutex_unlock(&lock);  
    return NULL;
}

int main() {
    // Initialize the mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        std::cerr << "Mutex init has failed" << std::endl;
        return 1;
    }

    std::cout << "main: begin (number = " << number << ")" << std::endl;

    pthread_t threads[3];

    for (long i = 0; i < 3; ++i) {
        if (pthread_create(&threads[i], NULL, &mythread, reinterpret_cast<void *>(i + 1)) != 0) {
            std::cerr << "Error creating thread " << i << std::endl;
            return 1;
        }
    }

    
    mythread(reinterpret_cast<void *>(0));

    
    for (int i = 0; i < 3; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            std::cerr << "Error joining thread " << i << std::endl;
            return 1;
        }
    }

    std::cout << "main: end (number = " << number << ")" << std::endl;

   
    pthread_mutex_destroy(&lock);

    return 0;
}
