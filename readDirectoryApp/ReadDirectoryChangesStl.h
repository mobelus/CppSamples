
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

mutex mutex_;
condition_variable con_, prd_;
atomic_bool done_ = {false};
atomic_bool process_ = {false}; // possibly overkill, but hey-ho
int counter = 0; // protected resource

void consumer()
{
  while (true)
  {
    unique_lock<mutex> lock(mutex_);       // acquire the mutex before waiting
    // this is the correct way to wait on the condition (as highlighted by @Dale)
    con_.wait(lock, []
	{
		return process_.load() || done_.load(); 
	});

    if (!done_.load())
    {
      cout << counter << endl;
      process_.store(false);               // we're done with this item
      lock.unlock();                       // release before notifying
      prd_.notify_one();
    }
    else
	{
      break;                               // we're done completely
	}
  }
}


void producer()
{
  unique_lock<mutex> lock(mutex_);
  for (int i = 0; i < 10; ++i)
  {
    this_thread::sleep_for(chrono::milliseconds(2000));
    ++counter;
    process_.store(true);                 // indicate that there is data
    con_.notify_one();
    // wait for the consumer to complete
    prd_.wait(lock, []
	{
		return !process_.load();
	});
  }

  done_.store(true);                      // we're done
  lock.unlock();                          // release before notifying
  con_.notify_one();
}

//int main(void)
void stlConsumerProducerTest()
{
  thread c(consumer);
  producer();
  c.join();
}

