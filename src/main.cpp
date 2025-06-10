#include <fstream>
#include <vector>
#include <fmt/format.h>
#include <chrono>
#include <string>
#include <thread>
#include <memory>

#include <ProcessMessage.h>
#include <MessageHeader.h>
#include <VWAPManager.h>

#include <queue_utils.h>
#include <time_utils.h>

/**
 * Parse and calculate a VWAP for a binary NASDAQ ITCH 5.0 file
 */
int main(int argc, char* argv[]) {

    if(argc > 2) {
        fmt::println("Do not specify more than 1 file. Quitting...");
        return 1;
    }
    std::string filepath("../ITCHFiles/01302019.NASDAQ_ITCH50");
    if(argc == 2) filepath = fmt::format("../ITCHFiles/{}",std::string(argv[1]));

    fmt::println("Using file: {}", filepath);

    auto start = std::chrono::system_clock::now();

    // Open File    
    std::ifstream file(filepath, std::ios::binary);

    if (!file) {
        fmt::println("Failed to open the file. Quitting...");
        return 1;
    }

    // Initialize the buffer
    constexpr std::size_t bufferSize = 64;  // 64 is a pretty ok buffer size. The largest message is 50 bytes, so why not leave a few more for good measure.
    std::vector<char> buffer(bufferSize);

    LockfreeSPSC& mq = LockfreeSPSC::getInstance();

    extern bool workFinished;
    // uint64_t consumerCounter = 0;
    // uint64_t throwawayCounter = 0;
    // Launch the "ITCH Messages" Queue processing thread
    std::thread itchConsumer([&mq/*, &consumerCounter, &throwawayCounter*/] {
        uint64_t previousTimestamp = 0;
        // uint64_t counter = 0;
        while(true) { // Sets to true once EVENT_CODE_END_OF_SYSTEM_HOURS event comes in
            if (mq.read_available() == 0) {
                if (isWorkFinished()) break; // Exit cleanly
                std::this_thread::yield();
                continue;
            }

            Message* message = nullptr;
            bool success = mq.popMesageFromLockfreeSPSCQueue(message);
            assert(success); // If this fails, we lost sequentiality invariant
            // consumerCounter++;
           
            if(message == nullptr) continue;

            uint64_t currentTimestamp = message -> getHeader().getTimestamp();
            // Bookkeep time
            ProcessMessage::processHeaderTimestamp(currentTimestamp);
            // fmt::println("current: {}, prev: {}", currentTimestamp, previousTimestamp);
            assert(previousTimestamp <= currentTimestamp || previousTimestamp == 0); // Similarly, we are processing messages out of order if this is ever false
            previousTimestamp = currentTimestamp;
            
            // Do bookkeeping, etc.
            message -> processMessage();
            // if(!processed) throwawayCounter++;
            delete message;
        }
        fmt::println("Exiting the consumer thread");
    });

    // And here is the magic
    // uint64_t counter = 0;
    // uint64_t counterToQueue = 0;
    while(file) {

        // For some reason, there happens to be two leading bytes at the start of each line
        file.read(buffer.data(), NUMBER_OF_BYTES_FOR_HEADER_CHUNK);
        BinaryMessageHeader header = parseHeader(&buffer[NUMBER_OF_BYTES_OFFSET_FOR_HEADER_CHUNK]);
        // fmt::println("{}. {} {} /*{} */{}", ++counter, header.getMessageType(), header.getStockLocate(), /*header.getTrackingNumber(),*/ header.getTimestamp());

        // Get the message body 
        std::size_t numberOfBytesForBody = ProcessMessage::messageTypeToNumberOfBytes(header.getMessageType());
        file.read(&buffer[NUMBER_OF_BYTES_FOR_HEADER_CHUNK], numberOfBytesForBody);

        // Parse the binary message, then add to the processing queue (if not nullptr)
        Message* messagePtr = ProcessMessage::getMessage(&buffer[NUMBER_OF_BYTES_FOR_HEADER_CHUNK], numberOfBytesForBody, std::move(header));
        if(messagePtr == nullptr) {
            delete messagePtr;
            continue;
        }
        // std::unique_ptr<Message> message(messagePtr);
        // counter++;

        while(mq.write_available() == 0) {
            std::this_thread::yield();
        }
        bool success = mq.pushMesageToLockfreeSPSCQueue(messagePtr);
        assert(success); // If this fails, we lost sequentiality invariant
        // counterToQueue++;
    }
    // Close file
    file.close();
    // fmt::println("Processed {} messages, {} were passed to the queue and {} were consumed. {} were nullptr or afterhours for a total of {} fully processed", counter, counterToQueue, consumerCounter, throwawayCounter, consumerCounter - throwawayCounter);
    workFinished = true;

    // Synchronize the producer (main) and consumer threads.
    fmt::println("Waiting on consumer thread to join... Is work done? {}", isWorkFinished());
    if(itchConsumer.joinable()) itchConsumer.join();
    fmt::println("Consumer thread joined.");

    // Produce the output
    VWAPManager::getInstance().outputBrokenTradeAdjustedVWAP();

    auto end = std::chrono::system_clock::now();
    // Convert nanos to seconds
    fmt::println("======== Total program execution time: {} seconds ========", static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(end - start).count()));

    return 0;
}