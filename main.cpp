#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <cassert>

#define DEBUG

const int TICK_RATE = 1;

class Task {
    public:
    Task(int phi, int p, int e, int d) : phi(phi), p(p), e(e), d(d) {
        assert(phi >= 0);
        assert(p > 0);
        assert(e > 0);
        assert(d > 0);
    }

    // phase, period, execution time, relative deadline
    int phi, p, e, d;
};

class Job : public Task {
    public:
    Job(int phi, int p, int e, int d, int start) : Task(phi, p, e, d), start(start) {}
    
    // start time
    int start;
    int exec_time = 0;
};

// comparator for EDF
bool cmp_edf(const Job &a, const Job &b) {
    return a.start + a.d < b.start + b.d;
}

// comparator for SJF
bool cmp_sjf(const Job &a, const Job &b) {
    return a.e - a.exec_time - b.e - b.exec_time;
}

// comparator for RM
bool cmp_rm(const Job &a, const Job &b) {
    return a.p < b.p;
}

// comparator for DM
bool cmp_dm(const Job &a, const Job &b) {
    return a.d < b.d;
}

int main() {
    // list of tasks
    std::vector<Task> periodic_tasks;
    for (int i = 0; i < 10; ++i) {
        Task t(rand() % 10, rand() % 10, rand() % 10, rand() % 10);
        periodic_tasks.push_back(t);
    }

    // initial jobs
    std::vector<Job> jobs;
    for (int i = 0; i < 10; ++i) {
        Job j(1, 2, 3, 4, 5);
        jobs.push_back(j);
    }

    // priority queue
    std::priority_queue<Job, std::vector<Job>, std::function<bool(Job, Job)>> pq(jobs.begin(), jobs.end(), cmp_edf);

    // run simulation
    for (int tiktok = 0; ; ++tiktok) {
        #ifdef DEBUG
        std::cout << "TIKTOK = " << tiktok << std::endl;
        #endif

        // does any job need to be added at this instant?
        for (Task &task : periodic_tasks) {
            if (tiktok >= task.phi and (tiktok - task.phi) % task.p == 0) {
                pq.push(Job(task.phi, task.p, task.e, task.d, tiktok));
                #ifdef DEBUG
                std::cout << "Added task to queue." << std::endl;
                #endif
            }
        }

        // choose which job to execute based on the priority
        Job job = pq.top();
        pq.pop();

        // execute the job
        job.exec_time += 1;

        // if the chosen job is not done with it's execution, then put it back in the queue
        if (job.exec_time != job.e) {
            pq.push(job);
        }

        #ifdef DEBUG
        std::cout << "The current queue size is " << pq.size() << std::endl;
        #endif
    }

    return 0;
}