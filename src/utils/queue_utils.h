#ifndef NASDAQ_UTILS_QUEUE_UTILS_H_
#define NASDAQ_UTILS_QUEUE_UTILS_H_

#include <memory>

#include <Message.h>

#include <boost/lockfree/spsc_queue.hpp>

/**
 * A little wrapper for the boost lockfree SPSC queue
 */
class LockfreeSPSC {

    public:
        static LockfreeSPSC& getInstance();

        bool pushMesageToLockfreeSPSCQueue(Message* message);
        bool popMesageFromLockfreeSPSCQueue(Message*& message);
        size_t write_available() { return mq.write_available(); }
        size_t read_available() { return mq.read_available(); }

    private:
        static LockfreeSPSC* _instance;
        boost::lockfree::spsc_queue<Message*, boost::lockfree::capacity<32>> mq;
};

#endif // NASDAQ_UTILS_QUEUE_UTILS_H_