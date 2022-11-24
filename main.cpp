#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>

const int TICK_RATE = 1;

class Task {
    public:
    Task(int phi, int p, int e, int d) : phi(phi), p(p), e(e), d(d) {}

    // phase, period, execution time, relative deadline
    int phi, p, e, d;
};

class Job : public Task {
    public:
    Job(int phi, int p, int e, int d, int start) : Task(phi, p, e, d), start(start) {}
    
    // start time
    int start;
};

// comparator for EDF
bool cmp_edf(const Job &a, const Job &b) {
    return a.start + a.d < b.start + b.d;
}

int main() {
    // list of tasks
    std::vector<Task> periodic_tasks;
    for (int i = 0; i < 10; ++i) {
    }

    // initial jobs
    std::vector<Job> jobs;
    for (int i = 0; i < 10; ++i) {
        Job j(1, 2, 3, 4, 5);
        jobs.push_back(j);
    }

    // priority queue
    std::priority_queue<Job, std::vector<Job>, std::function<bool(Job, Job)>> pq(jobs.begin(), jobs.end());

    // run simulation
    for (int tiktok = 0; ; ++tiktok) {
        // does any job need to be added at this instant?

        // choose which job to execute based on the priority

        // execute the job

        // remove the jobs that have finished their execution
    }

    return 0;
}