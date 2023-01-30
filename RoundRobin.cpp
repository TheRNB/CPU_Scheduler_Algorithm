#include <iostream>
#include <deque>
#include <utility>

using namespace std;

struct Process {

	int pid;			//managed by input
	int arrival_time;	//managed by input
	int burst_time;		//managed by input
	int start_time;
	int finishing_time; 
	int turnaround_time;
	int waiting_time;

};


class CPUScheduler {

public:

	pair<int, int> RoundRobinNextQuanta() {
		if (ProcessList.size() <= 0)
			return make_pair(cpu_time, -1);

		process_turn = (process_turn + 1) % ProcessList.size();
		
		if (ProcessList[process_turn].start_time == -1)
			ProcessList[process_turn].start_time = cpu_time;

		if (ProcessList[process_turn].burst_time >= time_quantum) {
			ProcessList[process_turn].burst_time -= time_quantum;
			cpu_time += time_quantum;

			int temp_id = ProcessList[process_turn].pid;
			if (ProcessList[process_turn].burst_time == 0) {
				ProcessList[process_turn].finishing_time = cpu_time;

				ProcessList[process_turn].turnaround_time =
					ProcessList[process_turn].finishing_time -
					ProcessList[process_turn].arrival_time;

				ProcessList[process_turn].waiting_time =
					ProcessList[process_turn].turnaround_time -
					ProcessList[process_turn].burst_time;


				remove(process_turn);
			}
			return make_pair(cpu_time, temp_id);
		} 
		else if (ProcessList[process_turn].burst_time < time_quantum) {
			cpu_time += ProcessList[process_turn].burst_time;
			int temp_id = ProcessList[process_turn].pid;
			ProcessList[process_turn].finishing_time = cpu_time;

			ProcessList[process_turn].turnaround_time =
					ProcessList[process_turn].finishing_time -
					ProcessList[process_turn].arrival_time;

			ProcessList[process_turn].waiting_time =
					ProcessList[process_turn].turnaround_time -
					ProcessList[process_turn].burst_time;

			remove(process_turn);
			return make_pair(cpu_time, temp_id);
		}
		return make_pair(cpu_time, -1);
	}


	void add_process (Process newProcess) {
		newProcess.start_time = -1;
		ProcessList.push_back(newProcess);
		return;
	}


	int get_current_time() {
		return cpu_time;
	}


	void pass_time() {
		cpu_time = cpu_time + 1;
		return;
	}


private:

	deque<Process> ProcessList;
	void remove(int id) {
		deque<Process> tmp;
		for (int i = ProcessList.size(); i > id; --i) {
			tmp.push_back(ProcessList[ProcessList.size()-1]);
			ProcessList.pop_back();
		}
		tmp.pop_back();
		while(tmp.size() > 0) {
			ProcessList.push_back(tmp[tmp.size()-1]);
			tmp.pop_back();
		}
		return;
	}
	int time_quantum = 2;
	int process_turn = -1;
	int cpu_time = 0;

};


int main() {
	int n;
	cout << "number of processes: ";
	cout.flush();
	cin >> n;

	CPUScheduler testScheduler;
	while (true) {
		pair<int, int> scheduler = testScheduler.RoundRobinNextQuanta();
		cout << "current time is " << scheduler.first << endl;
		if (scheduler.second != -1) {
			cout << "current process in progress is PID: " << scheduler.second << endl;
		} else {
			cout << "no process scheduled" << endl;
		}

		int opt;
		cout << "enter 0 to let the time pass, or 1 to add a process";
		cin >> opt;

		if (opt == 0) {
			testScheduler.pass_time();
			continue;
		} 
		else if (opt == 1) {
			cout << "\n for the process enter PID and Burst Time (and arrival time offset, 0 for now, 1 for 1 seconds before)";
			int id, bt, ofs;
			cin >> id >> bt >> ofs;
			Process newProcess;
			newProcess.pid = id;
			newProcess.burst_time = bt;
			newProcess.arrival_time = testScheduler.get_current_time() - ofs;
			testScheduler.add_process(newProcess);
		}

	}
	return 0;
}