#ifndef TREXQUANTTAKEHOME_BOOKKEEPING_ORDER_BOOK_H
#define TREXQUANTTAKEHOME_BOOKKEEPING_ORDER_BOOK_H

#include <cstdint>
#include <string>

/**
 * Book is a collection of mapping of OrderNumberReference to OrderData structure.
 * Since Orders can be retroactively broken, must maintain a collection of executed orders.
 */

struct OrderData {
    uint16_t stockLocate;
    uint32_t shares;
    std::string stock;
    uint32_t price;
};



class OrderBook {
    public:

    private:

};

#endif // TREXQUANTTAKEHOME_BOOKKEEPING_ORDER_BOOK_H