#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <cassert>

#define DEBUG

/*
#define SCHEDULER_FPNS // non-preemptive scheduling
#define SCHEDULER_FPPS // preemptive scheduling
#define SCHEDULER_FPDS // deferred preemptive scheduling
*/

#define SCHEDULER_FPDS

const int TICK_RATE = 1;

class Task {
    public:
    Task(int phi, int p, int e, int d, int priority) : phi(phi), p(p), e(e), d(d), priority(priority) {
        assert(phi >= 0);
        assert(p > 0);
        assert(e > 0);
        assert(d > 0);
    }

    // phase, period, execution time, relative deadline
    int phi, p, e, d;
    int priority;
    // for FPDS, the non-preemptable region length
    int f = 0;
};

class Job : public Task {
    public:
    Job(int phi, int p, int e, int d, int start, int priority) : Task(phi, p, e, d, priority), start(start) {}
    
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

// comparator for priority
template <typename T>
bool cmp_priority(const T &a, const T &b) {
    return a.priority < b.priority;
}

bool is_schedulable_fpds(std::vector<Task> periodic_tasks) {
    // sort the tasks. higher priority to lower priority.
    std::sort(periodic_tasks.begin(), periodic_tasks.end(), cmp_priority<Task>);

    int B_i_fnr = 0;
    bool schedulable = true;
    for (int i = periodic_tasks.size() - 1; i >= 0; --i) {
        int B = B_i_fnr;

        // fixed point iteration for the calculation of active period A.
        int A_prev = periodic_tasks[i].e;
        int A;
        while (true) {
            A = B;
            // calculation is for hep(i)
            for (int j = 0; j <= i; ++j) {
                A += (A_prev + periodic_tasks[j].p - 1) / periodic_tasks[j].p * periodic_tasks[j].e;
            }

            if (A == A_prev) break;
            A_prev = A;
        }

        // number of jobs of i in the task t_i priority level
        int G_i = (A + periodic_tasks[i].p - 1) / periodic_tasks[i].p;

        int R = periodic_tasks[i].e;

        for (int g = 0; g < G_i; ++g) {
            int w_prev = B + (g + 1) * periodic_tasks[i].e - periodic_tasks[i].f;
            int w;
            while (true) {
                w = B + (g + 1) * periodic_tasks[i].e - periodic_tasks[i].f;
                // calculate for hp(i)
                for (int j = 0; j < i; ++j) {
                    w += (w_prev / periodic_tasks[j].p + 1) * periodic_tasks[j].e;
                }

                // check if fixed point iteration has to end
                if (w == w_prev) break;
                if (w + periodic_tasks[i].f - g * periodic_tasks[i].p > periodic_tasks[i].d) {
                    schedulable = false;
                    return schedulable;
                }
                w_prev = w;
            }

            R = std::max(R, w + periodic_tasks[i].f - g * periodic_tasks[i].p);
        }

        if (R > periodic_tasks[i].d) {
            schedulable = false;
            return schedulable;
        }

        // TODO: currently accounting only for non-preemptable region. check about resource allocation later.
        // calculation of B. maximum encountered till now.
        // calculation is for lp(i)
        B_i_fnr = std::max(B_i_fnr, periodic_tasks[i].f - 1);
    }

    return schedulable;
}

int main() {
    // list of tasks
    // TODO - fix this shit
    std::vector<Task> periodic_tasks;
    for (int i = 0; i < 10; ++i) {
        Task t(rand() % 10, rand() % 10 + 1, rand() % 10 + 1, rand() % 10 + 1, rand() % 10);
        periodic_tasks.push_back(t);
    }

    // initial jobs
    std::vector<Job> jobs;
    for (int i = 0; i < 10; ++i) {
        Job j(1, 2, 3, 4, 5, 6);
        jobs.push_back(j);
    }

    #ifdef SCHEDULER_FPDS
    // FNR algorithm - in the main loop, we iterate from lower priorities to higher priorities.
    bool s = is_schedulable_fpds(periodic_tasks);
    std::cout << s << std::endl;
    exit(0);
    #endif

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
                pq.push(Job(task.phi, task.p, task.e, task.d, tiktok, task.priority));
                #ifdef DEBUG
                std::cout << "Added task to queue." << std::endl;
                #endif
            }
        }

        // choose which job to execute based on the priority
        Job job = pq.top();
        pq.pop();

        // if the scheduler is non-preemptive, set the priority of this task to -1 so that this gets selected every time
        #ifdef SCHEDULER_FPNS
        job.priority = -1;
        #endif
        // if the scheduler is pre-emptive, don't do anything
        #ifdef SCHEDULER_FPPS
        #endif
        // if the scheduler is deferred pre-emptive
        #ifdef SCHEDULER_FPDS
        #endif

        // execute the job
        job.exec_time += 1;

        // if the chosen job is not done with it's execution, then put it back in the queue
        if (job.exec_time < job.e) {
            pq.push(job);
        }

        #ifdef DEBUG
        std::cout << "The current queue size is " << pq.size() << std::endl;
        #endif
    }

    return 0;
}