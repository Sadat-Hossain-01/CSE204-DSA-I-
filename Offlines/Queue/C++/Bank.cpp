#include <cstdlib>
#include <iostream>

#include "Arr.hpp"
#include "LL.hpp"
using namespace std;

#define endl "\n"

const bool isCommentOn = true;
int current_time, idx, done;

class Event;

class Event {
 public:
  int service_start_time;
  bool firstQueue;
  int index;
  Event(int index = -1, int service_start_time = -1, bool firstQueue = true) {
    this->index = index;
    this->service_start_time = service_start_time;
    this->firstQueue = firstQueue;
  }
  Event toggle() {
    firstQueue ^= 1;
    return *this;
  }
};

void startService(Queue<Event>& q, int& next_finish, bool& isOn) {
  int id = q.frontValue().index;
  next_finish = current_time + service_time[id];
  isOn = true;
  if (isCommentOn) {
    cout << "Time " << current_time << ": Person " << id + 1
         << " has started taking service in queue 1" << endl;
  }
}

int main() {
  int n;
  cin >> n;
  int* enter_time = new int[n];
  int* service_time = new int[n];

  for (int i = 0; i < n; i++) {
    cin >> enter_time[i] >> service_time[i];
  }

  AQueue<Event> q1;
  //   LLQueue<Event> q1;
  AQueue<Event> q2;
  //   LLQueue<Event> q1;
  int q1_next = -1, q2_next = -1;
  int q1_last_time = -1, q2_last_time = -1;
  bool q1_on = false, q2_on = false;

  while (done < n) {
    if (idx < n) {
      if (current_time == enter_time[idx]) {
        Event e(idx);
        if (q1.length() <= q2.length()) {
          q1.enqueue(e);
          if (isCommentOn)
            cout << "Time " << current_time << ": Person " << idx + 1
                 << " has entered queue 1" << endl;
        } else {
          q2.enqueue(e.toggle());
          if (isCommentOn)
            cout << "Time " << current_time << ": Person " << idx + 1
                 << " has entered queue 2" << endl;
        }
        idx++;
      }
    }

    if (!q1_on && q1.length() > 0) {
      int id = q1.frontValue().index;
      q1_next = current_time + service_time[id];
      q1_on = true;
      if (isCommentOn) {
        cout << "Time " << current_time << ": Person " << id + 1
             << " has started taking service in queue 1" << endl;
      }
    }

    if (!q2_on && q2.length() > 0) {
      int id = q2.frontValue().index;
      q2_next = current_time + service_time[id];
      q2_on = true;
      if (isCommentOn) {
        cout << "Time " << current_time << ": Person " << id + 1
             << " has started taking service in queue 2" << endl;
      }
    }

    if (q1_on && current_time == q1_next) {
      int id = q1.dequeue().index;
      if (isCommentOn) {
        cout << "Time " << current_time << ": Person " << id + 1
             << " has finished from queue 1" << endl;
      }
      done++;
      q1_last_time = current_time;
      q1_on = false;

      // checking queue switching
      while (q2.length() - q1.length() >= 2) {
        auto leave = q2.leaveQueue();
        if (isCommentOn)
          cout << "Time " << current_time << ": Person " << leave.index + 1
               << " has switched to queue " << 1 << endl;
        q1.enqueue(leave.toggle());
      }
    }

    if (q2_on && current_time == q2_next) {
      int id = q2.dequeue().index;
      if (isCommentOn) {
        cout << "Time " << current_time << ": Person " << id + 1
             << " has finished from queue 2" << endl;
      }
      done++;
      q2_last_time = current_time;
      q2_on = false;

      // checking queue switching
      while (q1.length() - q2.length() >= 2) {
        auto leave = q1.leaveQueue();
        if (isCommentOn)
          cout << "Time " << current_time << ": Person " << leave.index + 1
               << " has switched to queue " << 2 << endl;
        q2.enqueue(leave.toggle());
      }
    }
    int min_val =
        min(min(q1_next, q2_next), (idx < n ? enter_time[idx] : INT_MAX));
    if (min_val > current_time)
      current_time = min_val;
    else
      current_time++;
  }

  cout << "Booth 1 finishes service at t=" << q1_last_time << endl;
  cout << "Booth 2 finishes service at t=" << q2_last_time << endl;

  delete[] service_time;
  delete[] enter_time;
}