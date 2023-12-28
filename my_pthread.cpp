#include <iostream>
#include <pthread.h>
#include <sstream>

int number = 330;
pthread_mutex_t lock;

void *mythread(void *arg) {
    long thread_id = reinterpret_cast<long>(arg);

    pthread_mutex_lock(&lock);  // Lock the mutex

    std::cout << "thread " << thread_id << ": begin (number = " << number << ")" << std::endl;
    number *= 2;
    // Print after doubling
    std::cout << "thread " << thread_id << ": done (number = " << number << ")" << std::endl;

    pthread_mutex_unlock(&lock);  // Unlock the mutex
    return NULL;
}

int main() {
    if (pthread_mutex_init(&lock, NULL) != 0) {
        std::cerr << "Mutex init has failed" << std::endl;
        return 1;
    }

    std::cout << "main: begin (number = " << number << ")" << std::endl;

    // Array of threads
    pthread_t threads[3];

    for (long i = 0; i < 3; ++i) {
        if (pthread_create(&threads[i], NULL, &mythread, reinterpret_cast<void *>(i + 1)) != 0) {
            std::cerr << "Error creating thread " << i + 1 << std::endl;
            return 1;
        }
    }

    for (int i = 0; i < 3; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            std::cerr << "Error joining thread " << i + 1 << std::endl;
            return 1;
        }
    }

    std::cout << "main: end (number = " << number << ")" << std::endl;

    pthread_mutex_destroy(&lock);

    return 0;
}
