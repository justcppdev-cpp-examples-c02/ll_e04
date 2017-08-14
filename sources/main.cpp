#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

std::vector<int> data;
std::atomic<int> count;

void populate_queue()
{
  unsigned int const number_of_items = 20;
  data.clear();
  for( unsigned int i = 0; i < number_of_items; ++i ) {
    data.push_back( i );
  }

  count.store( number_of_items, std::memory_order_release );
}

void consume_queue_items()
{
  for( ;; ) {
    int item_index;
    if( 0 >= ( item_index = count.fetch_sub( 1, std::memory_order_acquire ) ) ) { // There can be a reference to same index in different threads
      std::this_thread::yield();
      continue;
    }

    std::cout << "processed " << item_index << '\n';
  }
}

int main()
{
  std::thread a{ populate_queue };
  std::thread b{ consume_queue_items };
  std::thread c{ consume_queue_items };

  a.join();
  b.join();
  c.join();
}
