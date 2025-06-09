#ifndef NASDAQ_MESSAGE_H_
#define NASDAQ_MESSAGE_H_

/**
 * Parent class for all ITCH messages
 */

#include <utility> // for std::move

#include <MessageHeader.h>

class Message {

    protected:
        BinaryMessageHeader header;

    public:
        explicit Message(BinaryMessageHeader header) : 
        header(std::move(header))
        {}
        virtual ~Message() = default;
        virtual bool processMessage() const = 0; 

        const BinaryMessageHeader& getHeader() const { return header; }
};

#endif // NASDAQ_MESSAGE_H_