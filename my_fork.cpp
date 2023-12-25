#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int number = 330;  
    std::cout << "The initial value of number is " << number << std::endl;

    pid_t pid = fork();  // First fork

    if (pid == -1) {
        // Fork failed
        std::cerr << "Fork failed!" << std::endl;
        return 1;
    } else if (pid == 0) {
        // Child process
        number *= 2;  // Double the number
        std::cout << "In the child, the number is " << number << " -- PID is " << getpid() << std::endl;
        
        // Second fork inside the child process
        pid_t pid_child = fork();
        
        if (pid_child == 0) {
            // Grandchild process 
            
            std::cout << "In the child (of child), the number is " << number << " -- PID is " << getpid() << std::endl;
        } else if (pid_child > 0) {
            // Child process after second fork
            wait(NULL); // Wait for the grandchild to finish
        } else {
            // Second fork failed
            std::cerr << "Second fork failed!" << std::endl;
            return 1;
        }
    } else {
        // Parent process
        wait(NULL);  // Wait for the child to finish
        std::cout << "In the parent, the number is " << number << " -- PID is " << getpid() << std::endl;
    }

    return 0;
}

