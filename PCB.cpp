#include <iostream>
#include <cstdint>
using namespace std;

#define STACK_SIZE 1024 // Define stack size for each process = 1 kb

// States of Process
enum class ProcessState
{
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
};

// PCB Definition
struct PCB
{
    int processID;      // Process Identifier
    ProcessState state; // Current state of the process

    // CPU Context
    int programCounter; // Program Counter
    int registers[8];   // Simulated CPU registers

    // Memory management
    char stack[STACK_SIZE];
    uintptr_t stack_top;  // Top of the stack
    uintptr_t stack_base; // Base of the stack

    // Constructor
    PCB(int pid) : processID(pid),
                   state(ProcessState::NEW),
                   programCounter(0),
                   stack_top(reinterpret_cast<uintptr_t>(stack) + STACK_SIZE - 1),
                   stack_base(reinterpret_cast<uintptr_t>(stack))
    {
        for (int i = 0; i < 8; i++)
        {
            registers[i] = 0;
        }
    }

    // Method to print PCB details
    void print() const
    {
        cout << "Process ID: " << processID << endl;

        cout << "Process State: ";
        switch (state)
        {
        case ProcessState::NEW:
            cout << "NEW";
            break;
        case ProcessState::READY:
            cout << "READY";
            break;
        case ProcessState::RUNNING:
            cout << "RUNNING";
            break;
        case ProcessState::WAITING:
            cout << "WAITING";
            break;
        case ProcessState::TERMINATED:
            cout << "TERMINATED";
            break;
        }
        cout << "\n";
        cout << "Program Counter: " << programCounter << endl;
        cout << "Stack Base: " << hex << stack_base << endl;
        cout << "Stack Top: " << hex << stack_top << endl;
    }
};

int main()
{
    // Create a PCB for a process with ID 101
    PCB process1(101);
    // Display initial state of the process
    process1.print();
    process1.state = ProcessState::READY;

    // Simulate state change
    PCB process2(102);
    process2.state = ProcessState::READY;
    cout << "\nDispatching Process " << process2.processID << ":\n";
    process2.state = ProcessState::RUNNING;
    cout << "\nRunning Process " << process2.processID << ":\n";
    process2.programCounter += 5;
    process2.registers[0] = 42; // Simulate some register operation
    process2.print();

    // Context switch back to process1
    cout << "\nSwitching back to Process " << process1.processID << ":\n";
    process1.state = ProcessState::RUNNING;
    process1.programCounter += 3;
    process1.print();

    return 0;
}
