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

    int totalWork; // Work to be done

    PCB(int pid, int totalWork) : processID(pid),
                                  state(ProcessState::NEW),
                                  programCounter(0),
                                  stack_top(reinterpret_cast<uintptr_t>(stack) + STACK_SIZE - 1),
                                  stack_base(reinterpret_cast<uintptr_t>(stack)),
                                  totalWork(totalWork)
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
        // cout << "Program Counter: " << programCounter << endl;
        // cout << "Stack Base: " << hex << stack_base << endl;
        // cout << "Stack Top: " << hex << stack_top << endl;
        cout << programCounter << "/" << totalWork << " units of work done.\n";
    }
    // One unit of work execution
    void execute()
    {
        if (state != ProcessState::RUNNING)
            return;
        if (programCounter < totalWork)
        {
            programCounter++;
            registers[0] += processID;
        }
        else
        {
            state = ProcessState::TERMINATED;
        }
    }
};

class Scheduler
{
private:
    queue<PCB *> readyQueue;

public:
    void addProcess(PCB *process)
    {
        process->state = ProcessState::READY;
        readyQueue.push(process);
        cout << "Process " << process->processID << " added to ready queue, Work: " << process->totalWork << ".\n";
    }
    void requeueProcess(PCB *process)
    {
        if (process && (process->state != ProcessState::TERMINATED || process->state != ProcessState::NEW))
        {
            process->state = ProcessState::READY;
            readyQueue.push(process);
        }
    }
    PCB *nextProcess()
    {
        if (readyQueue.empty())
        {
            return nullptr;
        }
        PCB *process = readyQueue.front();
        readyQueue.pop();
        process->state = ProcessState::RUNNING;
        return process;
    }
    bool hasProcesses() const
    {
        return !readyQueue.empty();
    }
};

// CPU + Dispatcher Simulation
int main()
{
    const int TIME_QUANTUM = 4; // Time quantum for Round Robin
    Scheduler scheduler;
    PCB *current = nullptr;
    int cycles = 0;

    PCB *process1 = new PCB(1, 11);
    PCB *process2 = new PCB(2, 16);
    PCB *process3 = new PCB(3, 13);

    scheduler.addProcess(process1);
    scheduler.addProcess(process2);
    scheduler.addProcess(process3);

    cout << "Starting Round Robin Scheduler Simulation...\n";

    while (current || scheduler.hasProcesses())
    {
        if (!current)
        {
            current = scheduler.nextProcess();
            if (current)
            {
                cout << "Dispatching Process " << current->processID << "\n";
                cycles = 0;
            }
            else
            {
                break; // No more processes
            }
        }
        current->execute();
        cycles++;

        if (current->state == ProcessState::TERMINATED)
        {
            cout << "Process " << current->processID << " has terminated.\n";
            current->print();
            delete current;
            current = nullptr;
            cout << "-------------------------\n";
        }
        else if (cycles >= TIME_QUANTUM)
        {
            cout << "Time quantum expired for Process " << current->processID << ". Re-queuing.\n";
            current->print();
            scheduler.requeueProcess(current);
            current = nullptr;
            cout << "-------------------------\n";
        }
    }

    cout << "Round Robin Scheduler Simulation Complete.\n";
    return 0;
}
