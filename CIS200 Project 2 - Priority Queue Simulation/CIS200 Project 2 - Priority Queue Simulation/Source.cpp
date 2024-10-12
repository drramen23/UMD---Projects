/***************
*Author: Dominic Nguyen
*Creation Date: 4/9/24
*Modification Date: 4/16/24
*Purpose: make a simulation with priority queues to test the efficiency of different numbers of processors.
****************/

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;
const int PROCESSORS = 8 ;

struct Data {
    char job; // A, B, C, D
    int job_num;
    int job_type_num;
    int arr_time;
    int proc_time;
};

class Processor {
private:
    Data job;
    int idle_run_time = 0;
public:
    void assignJob(Data job) {
        this->job = job;
    }
    void removeJob() {
        job.job = NULL;
        job.job_num = NULL;
        job.job_type_num = NULL;
        job.arr_time = NULL;
        job.proc_time = NULL;
    }
    void process() {
        job.proc_time--;
    }
    bool isEmpty() {
        if (job.job == NULL) {
            return true;
        }
        else {
            return false;
        }
    }
    Data getJob() {
        return job;
    }
    void setArrTime(int time) {
        job.arr_time = time;
    }
    int getIRtime () {
        return idle_run_time;
    }
    void resetIRtime() {
        idle_run_time = 0;
    }
    void ppIRtime() {
        idle_run_time++;
    }
};

struct Queue {
    vector<Data>queue;

    void enqueue(Data newJob) {
        queue.push_back(newJob); //adds newJob to back of queue
    }
    void dequeue() {
        queue.erase(queue.begin()); //erases first element
    }
    bool isEmpty() {
        return queue.empty(); //returns bool true if empty
    }

}dataQ, regularQ, interruptedQ, priorityQ;


void processJobs(vector<Processor>&CPU, int &jobsDone, int &Ajobs, int &Bjobs, int &Cjobs, int &Djobs, int &run_time, ofstream& logFile, int &time) {
    for (int core = 0; core < PROCESSORS; core++) { //decrements processing time for every core
        if (!CPU.at(core).isEmpty()) { //if core not empty
            CPU.at(core).process(); //process
            run_time++;

            if (CPU.at(core).getJob().proc_time <= 0) { //if processing time <= 0 then job is done
                switch (CPU.at(core).getJob().job)
                {
                case 'A':
                    Ajobs++;
                    break;

                case 'B':
                    Bjobs++;
                    break;

                case 'C':
                    Cjobs++;
                    break;

                case 'D':
                    Djobs++;
                    break;
                default:
                    cerr << "ERROR: ";
                }
                jobsDone++;
                logFile << "Time " << time << ":  Complete Processing Job: " << CPU.at(core).getJob().job_num << ", Job " 
                    << CPU.at(core).getJob().job << ": " << CPU.at(core).getJob().job_type_num << endl;
                CPU.at(core).removeJob(); //handle finished job
            }
        }
    }
}

bool isCPU_full(vector<Processor>& CPU) {
    bool isCPU_full = true;

    for (int cores = 0; cores < PROCESSORS; cores++) {
        if (CPU.at(cores).isEmpty())
            isCPU_full = false;
    }
    return isCPU_full;
}
bool isCPU_pfull(vector<Processor>& CPU) {
    bool isCPU_pfull = true;

    for (int cores = 0; cores < PROCESSORS; cores++) {
        if (CPU.at(cores).isEmpty() || CPU.at(cores).getJob().job != 'D')
            isCPU_pfull = false;
    }
    return isCPU_pfull;
}


int main() {

    ifstream jobIn("jobs.txt");
    if (!jobIn) {
        cerr << "Error: \"jobs.txt\" could not be opened.";
    }
    int job_num = 1;
    int A = 1, B = 1, C = 1, D = 1;
    while (!jobIn.eof()) { //parsing file into structs in vector
        Data inJob;

        jobIn >> inJob.job;
        inJob.job_num = job_num;
        switch (inJob.job) {
        case 'A':
            inJob.job_type_num = A++;
            break;

        case 'B':
            inJob.job_type_num = B++;
            break;

        case 'C':
            inJob.job_type_num = C++;
            break;

        case 'D':
            inJob.job_type_num = D++;
            break;
        }
        jobIn >> inJob.arr_time;
        jobIn >> inJob.proc_time;

        job_num++;
        dataQ.enqueue(inJob);
    }
    dataQ.queue.pop_back(); //removes last endl from file
    jobIn.close();


    ofstream logFile("log.txt");

    vector<Processor>CPU(PROCESSORS);


    int Ajobs = 0, Bjobs = 0, Cjobs = 0, Djobs = 0;
    int Adone = 0, Bdone = 0, Cdone = 0, Ddone = 0;
    int idle_time = 0, run_time = 0;
    int arrived = 0, jobsDone = 0;
    int avgQSize = 0, maxQsize = 0, qSize = 0;
    int timeInQ = 0;

    double averageQtime = 0;
    double averageQsize = 0;

    int time = 0; // milliseconds unit

    for (time = 0; time < 10000; time++) {


        processJobs(CPU, jobsDone, Adone, Bdone, Cdone, Ddone, run_time, logFile, time);
        \

        avgQSize += (regularQ.queue.size() + interruptedQ.queue.size() + priorityQ.queue.size());
        timeInQ = avgQSize;

        qSize = regularQ.queue.size() + interruptedQ.queue.size() + priorityQ.queue.size();
        if (qSize > maxQsize)
            maxQsize = qSize;

        do {
            for (int core = 0; core < PROCESSORS; core++) {
                if (CPU.at(core).isEmpty())
                {
                    if (!priorityQ.isEmpty()) { //priority Q
                        CPU.at(core).resetIRtime();
                        logFile << "Time " << time << ":  Begin Processing Job:" << dataQ.queue.at(0).job_num << ", Job " << dataQ.queue.at(0).job << ": "
                            << dataQ.queue.at(0).job_type_num << " in CPU " << core +1 << endl;
                        CPU.at(core).assignJob(priorityQ.queue.at(0));
                        priorityQ.dequeue();
                    }
                    else if (!interruptedQ.isEmpty()) { //interuppted Q
                        CPU.at(core).resetIRtime();
                        logFile << "Time " << time << ":  Begin Processing Job:" << dataQ.queue.at(0).job_num << ", Job " << dataQ.queue.at(0).job << ": "
                            << dataQ.queue.at(0).job_type_num << " in CPU " << core + 1 << endl;
                        CPU.at(core).assignJob(interruptedQ.queue.at(0));
                        interruptedQ.dequeue();
                    }
                    else if (!regularQ.isEmpty()) { //regular Q
                        CPU.at(core).resetIRtime();
                        logFile << "Time " << time << ":  Begin Processing Job:" << dataQ.queue.at(0).job_num << ", Job " << dataQ.queue.at(0).job << ": "
                            << dataQ.queue.at(0).job_type_num << " in CPU " << core + 1 << endl;
                        CPU.at(core).assignJob(regularQ.queue.at(0));
                        regularQ.dequeue();
                    }
                    else if (dataQ.queue.at(0).arr_time == time) { //straight from data Q
                        logFile << "Time " << time << ":  Arrival: Overall Job:" << dataQ.queue.at(0).job_num << " Job " << dataQ.queue.at(0).job 
                            << ": " << dataQ.queue.at(0).job_type_num << ", Processing Time " << dataQ.queue.at(0).proc_time << endl;
                        switch (dataQ.queue.at(0).job)
                        {
                        case 'A':
                            Ajobs++;
                            break;

                        case 'B':
                            Bjobs++;
                            break;

                        case 'C':
                            Cjobs++;
                            break;

                        case 'D':
                            Djobs++;
                            break;
                        default:
                            cerr << "ERROR: ";
                        }
                        arrived++;

                        CPU.at(core).resetIRtime();
                        logFile << "Time " << time << ":  Begin Processing Job:" << dataQ.queue.at(0).job_num << ", Job " << dataQ.queue.at(0).job << ": "
                            << dataQ.queue.at(0).job_type_num << " in CPU " << core + 1 << endl;
                        CPU.at(core).assignJob(dataQ.queue.at(0));
                        dataQ.dequeue();
                    }
                    break;
                }
            }

            if (isCPU_full(CPU) && !isCPU_pfull(CPU)) {
                if (!priorityQ.isEmpty()) { //if priority Q not empty
                    for (int core = 0; core < PROCESSORS; core++) { //find processor with regular job and interrupt it
                        if (CPU.at(core).getJob().job != 'D') {

                            CPU.at(core).setArrTime(time);
                            interruptedQ.enqueue(CPU.at(core).getJob());
                            CPU.at(core).removeJob();

                            CPU.at(core).resetIRtime();
                            logFile << "Time " << time << ":  Begin Processing Job:" << dataQ.queue.at(0).job_num << ", Job " << dataQ.queue.at(0).job << ": "
                                << dataQ.queue.at(0).job_type_num << " in CPU " << core + 1 << endl;
                            CPU.at(core).assignJob(priorityQ.queue.at(0));
                            priorityQ.dequeue();
                            break;
                        }
                    }
                }
                else if (dataQ.queue.at(0).arr_time == time && dataQ.queue.at(0).job == 'D') { //if priority Q empty and data Q next job is D job and cpu not full of D jobs
                    logFile << "Time " << time << ":  Arrival: Overall Job:" << dataQ.queue.at(0).job_num << " Job " << dataQ.queue.at(0).job 
                        << ": " << dataQ.queue.at(0).job_type_num << ", Processing Time " << dataQ.queue.at(0).proc_time << endl;
                    for (int core = 0; core < PROCESSORS; core++) { //find processor with regular job and interrupt it
                        if (CPU.at(core).getJob().job != 'D') {

                            CPU.at(core).setArrTime(time);
                            interruptedQ.enqueue(CPU.at(core).getJob());
                            CPU.at(core).removeJob();

                            switch (dataQ.queue.at(0).job)
                            {
                            case 'A':
                                Ajobs++;
                                break;

                            case 'B':
                                Bjobs++;
                                break;

                            case 'C':
                                Cjobs++;
                                break;

                            case 'D':
                                Djobs++;
                                break;
                            default:
                                cerr << "ERROR: ";
                            }
                            arrived++;

                            CPU.at(core).resetIRtime();
                            logFile << "Time " << time << ":  Begin Processing Job:" << dataQ.queue.at(0).job_num << ", Job " << dataQ.queue.at(0).job << ": "
                                << dataQ.queue.at(0).job_type_num << " in CPU " << core + 1 << endl;
                            CPU.at(core).assignJob(dataQ.queue.at(0));
                            dataQ.dequeue();
                            break;
                        }
                    }
                }
            }

            while ((dataQ.queue.at(0).arr_time == time) && (dataQ.queue.at(0).job == 'D')) {
                logFile << "Time " << time << ":  Arrival: Overall Job:" << dataQ.queue.at(0).job_num << " Job " << dataQ.queue.at(0).job 
                    << ": " << dataQ.queue.at(0).job_type_num << ", Processing Time " << dataQ.queue.at(0).proc_time << endl;
                switch (dataQ.queue.at(0).job)
                {
                case 'A':
                    Ajobs++;
                    break;

                case 'B':
                    Bjobs++;
                    break;

                case 'C':
                    Cjobs++;
                    break;

                case 'D':
                    Djobs++;
                    break;
                default:
                    cerr << "ERROR: ";
                }
                arrived++;
                priorityQ.enqueue(dataQ.queue.at(0));
                dataQ.dequeue();
            }
            while ((dataQ.queue.at(0).arr_time == time)) {
                logFile << "Time " << time << ":  Arrival: Overall Job:" << dataQ.queue.at(0).job_num << " Job " << dataQ.queue.at(0).job 
                    << ": " << dataQ.queue.at(0).job_type_num << ", Processing Time " << dataQ.queue.at(0).proc_time << endl;
                switch (dataQ.queue.at(0).job)
                {
                case 'A':
                    Ajobs++;
                    break;

                case 'B':
                    Bjobs++;
                    break;

                case 'C':
                    Cjobs++;
                    break;

                case 'D':
                    Djobs++;
                    break;
                default:
                    cerr << "ERROR: ";
                }
                arrived++;
                regularQ.enqueue(dataQ.queue.at(0));
                dataQ.dequeue();
            }

            for (int core = 0; core < PROCESSORS; core++) {
                if (CPU.at(core).isEmpty()) {
                    idle_time++;
                }
            }

            logFile << "Time " << time << ":  Queue: " << regularQ.queue.size() + interruptedQ.queue.size() + priorityQ.queue.size() << "; ";
            for (int core = 0; core < PROCESSORS; core++) {
                logFile << " CPU " << core + 1 << (CPU.at(core).isEmpty() ? " Idle Time " : " Run Time ") << CPU.at(core).getIRtime() << "; ";
                CPU.at(core).ppIRtime();
            }
            logFile << endl;

        } while (dataQ.queue.at(0).arr_time == time);

        averageQsize = static_cast<double>(avgQSize) / arrived;

        qSize = regularQ.queue.size() + interruptedQ.queue.size() + priorityQ.queue.size();

        if (time == 550) {
            cout << left << setw(36) << setfill('.') << "INITIAL METRICS" << endl;
            cout << left << setw(36) << setfill('.') << "Number of processors being used" << CPU.size() << endl;
            cout << left << setw(36) << setfill('.') << "Current queue size" << qSize << endl;
            cout << left << setw(36) << setfill('.') << "Average queue size" << averageQsize << endl;
            cout << left << setw(36) << setfill('.') << "Maximum jobs in queue" << maxQsize << endl;
            cout << left << setw(36) << setfill('.') << "Total time jobs are in queue" << timeInQ << "ms" << endl;
            cout << left << setw(36) << setfill('.') << "Average time jobs are in queue" << static_cast<double>(timeInQ) / 10000 << "ms" << endl;

            cout << left << setw(36) << setfill('.') << "Total number of A jobs arrived" << Ajobs << endl;
            cout << left << setw(36) << setfill('.') << "Total number of A jobs completed" << Adone << endl;
            cout << left << setw(36) << setfill('.') << "Total number of B jobs arrived" << Bjobs << endl;
            cout << left << setw(36) << setfill('.') << "Total number of B jobs completed" << Bdone << endl;
            cout << left << setw(36) << setfill('.') << "Total number of C jobs arrived" << Cjobs << endl;
            cout << left << setw(36) << setfill('.') << "Total number of C jobs completed" << Cdone << endl;
            cout << left << setw(36) << setfill('.') << "Total number of D jobs arrived" << Djobs << endl;
            cout << left << setw(36) << setfill('.') << "Total number of D jobs completed" << Ddone << endl;

            cout << left << setw(36) << setfill('.') << "Total jobs completed" << jobsDone << endl;
            cout << left << setw(36) << setfill('.') << "Total time CPU(s) were processing" << run_time << "ms" << endl;
            cout << left << setw(36) << setfill('.') << "Total time CPU(s) were idle" << idle_time << "ms" << endl;
            cout << endl << endl;
        }
    }

    

    cout << left << setw(36) << setfill('.') << "FINAL METRICS" << endl;
    cout << left << setw(36) << setfill('.') << "Number of processors being used" << CPU.size() << endl;
    cout << left << setw(36) << setfill('.') << "Current queue size" << qSize << endl;
    cout << left << setw(36) << setfill('.') << "Average queue size" << averageQsize << endl;
    cout << left << setw(36) << setfill('.') << "Maximum jobs in queue" << maxQsize << endl;
    cout << left << setw(36) << setfill('.') << "Total time jobs are in queue" << timeInQ << "ms" << endl;
    cout << left << setw(36) << setfill('.') << "Average time jobs are in queue" << static_cast<double>(timeInQ)/10000 << "ms" << endl;

    cout << left << setw(36) << setfill('.') << "Total number of A jobs arrived" << Ajobs << endl;
    cout << left << setw(36) << setfill('.') << "Total number of A jobs completed" << Adone << endl;
    cout << left << setw(36) << setfill('.') << "Total number of B jobs arrived" << Bjobs << endl;
    cout << left << setw(36) << setfill('.') << "Total number of B jobs completed" << Bdone << endl;
    cout << left << setw(36) << setfill('.') << "Total number of C jobs arrived" << Cjobs << endl;
    cout << left << setw(36) << setfill('.') << "Total number of C jobs completed" << Cdone << endl;
    cout << left << setw(36) << setfill('.') << "Total number of D jobs arrived" << Djobs << endl;
    cout << left << setw(36) << setfill('.') << "Total number of D jobs completed" << Ddone << endl;

    cout << left << setw(36) << setfill('.') << "Total jobs completed" << jobsDone << endl;
    cout << left << setw(36) << setfill('.') << "Total time CPU(s) were processing" << run_time << "ms" << endl;
    cout << left << setw(36) << setfill('.') << "Total time CPU(s) were idle" << idle_time << "ms" << endl;

    return 0;
}