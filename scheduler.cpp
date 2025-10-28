#include <iostream>
#include <cstdint>
#include <queue>
using namespace std;

#define STACK_SIZE 1024

enum class ProcessState
{
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
};

struct PCB
{
    int processID;
    ProcessState state;

    int programCounter;
    int registers[8];

    char stack[STACK_SIZE];
    uintptr_t stack_top;
    uintptr_t stack_base;

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
    void execute()
    {
        programCounter++;
        registers[0] += processID; // Do some "work"
    }
};

class Scheduler
{
private:
    queue<PCB *> readyQueue;

public:
    Scheduler() {}
    // Adding process to ready queue
    void addProcess(PCB *process)
    {
        process->state = ProcessState::READY;
        readyQueue.push(process);
        cout << "Process " << process->processID << " added to ready queue.\n";
    }
    // Re-queueing a process
    void requeueProcess(PCB *process)
    {
        process->state = ProcessState::READY;
        readyQueue.push(process);
        cout << "Process " << process->processID << " re-queued to ready queue.\n";
    }
    // Selecting next process to run
    PCB *nextProcess()
    {
        if (readyQueue.empty())
        {
            // No processes in ready queue
            return nullptr;
        }
        PCB *process = readyQueue.front();
        readyQueue.pop();
        process->state = ProcessState::RUNNING;
        // Dispatching Process
        return process;
    }
    // Check if there are processes in the ready queue
    bool hasProcesses() const
    {
        return !readyQueue.empty();
    }
};

// Acts as a Dispatcher Simulation
int main()
{
    Scheduler scheduler;
    PCB *process1 = new PCB(1);
    PCB *process2 = new PCB(2);
    PCB *process3 = new PCB(3);

    // Adding processes to the scheduler
    scheduler.addProcess(process1);
    scheduler.addProcess(process2);
    scheduler.addProcess(process3);

    cout << "Starting Scheduler Simulation...\n";
    int cycles = 0;

    // Track current running process
    PCB *currentProcess = nullptr;

    while (scheduler.hasProcesses() && cycles++ < 12)
    {
        currentProcess = scheduler.nextProcess();
        if (currentProcess)
        {
            cout << "Dispatching Process " << currentProcess->processID << ":\n";
            currentProcess->execute();
            currentProcess->print();
            cout << "-------------------------\n";
            cout << "Cycle " << cycles + 1 << " complete.\n\n";

            scheduler.requeueProcess(currentProcess);
        }
    }

    cout << "Scheduler Simulation Complete.\n";

    // Clean-up Memory
    delete process1;
    delete process2;
    delete process3;
    return 0;
}
