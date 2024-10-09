#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


bool isDataReady = false;

std::mutex mutex;


std::condition_variable condition;

// ������� ��� ������������ ������
void produceData() {
    std::unique_lock<std::mutex> lock(mutex); 

    if (isDataReady) { 
        return; 
    }

    isDataReady = true; 
    std::cout << "Provided\n"; 

    condition.notify_one(); 
}

// ������� ��� ����������� ������
void consumeData() {
    std::unique_lock<std::mutex> lock(mutex); 

    while (!isDataReady) { 
        condition.wait(lock); 
        std::cout << "Awoke\n"; 
    }

    isDataReady = false; 
    std::cout << "Consumed\n"; 
}

// �������-����� ��� �������������
void producer() {
    while (true) {
        produceData();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

// �������-����� ��� �����������
void consumer() {
    while (true) { 
        consumeData();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); \
    }
}

int main() {
    
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    
    producerThread.join();
    consumerThread.join();

    return 0; 
}