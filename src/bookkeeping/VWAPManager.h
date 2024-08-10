#ifndef TREXQUANTTAKEHOME_BOOKKEEPING_VWAP_MANAGER_H
#define TREXQUANTTAKEHOME_BOOKKEEPING_VWAP_MANAGER_H

#include <memory>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>

#include <OrderBook.h>
#include <AddOrder.h>
#include <AddOrderMPID.h>
#include <BrokenTradeOrOrderExecution.h>
#include <CrossTrade.h>
#include <OrderCancel.h>
#include <OrderDelete.h>
#include <OrderExecuted.h>
#include <OrderExecutedWithPrice.h>
#include <OrderReplace.h>
#include <StockTradingAction.h>
#include <SystemEvent.h>
#include <TradeNonCross.h>

// Store these in the priority queue to manage high/low/close data
typedef std::pair<uint32_t, uint64_t> IndexablePrice; 

/**
 * Use this to trace stock price and volume per-period
 * In mappings, this will be searchable by Stock Locate Code
 * All of this memory is used for high/low/close to avoid recalculation if any of these values are broken and need to be retroactively updated
 */
struct VWAPPeriodicData {
    IndexablePrice highPrice;
    IndexablePrice lowPrice; 
    IndexablePrice closingPrice; 
    uint64_t volume;
    bool isTrading; // Issues halted intraday will not be removed from VWAP report. Provide a mechanism to show whether or not the security is actively traded AT THE TIME OF THE REPORT.
};

// This can stay simple. 
struct VWAPCumulativeData {
    uint64_t cumulativeTypicalPrice;
    uint64_t cumulativeVolume;
    bool isTrading; // Issues halted intraday will not be removed from VWAP report. Provide a mechanism to show whether or not the security is actively traded AT THE TIME OF THE REPORT.
};

/**
 * This class is reponsible for calculating VWAP
 */
class VWAPManager {
    public:
        static VWAPManager& getInstance();

        // Add processing for each mesage. Helper functions indented
        void processAddOrder(uint16_t stockLocate, AddOrder* addOrder);
        void processAddOrderMPID(uint16_t stockLocate, AddOrderMPID* addOrderMPID);
        void processBrokenTradeOrOrderExecution(uint16_t stockLocate, BrokenTradeOrOrderExecution* brokenTradeOrOrderExecution);
        void processCrossTrade(uint16_t stockLocate, CrossTrade* crossTrade); 
        void processOrderCancel(uint16_t stockLocate, OrderCancel* orderCancel);
        void processOrderDelete(uint16_t stockLocate, OrderDelete* orderDelete);
        void processOrderExecuted(uint16_t stockLocate, OrderExecuted* orderExecuted);
        void processOrderExecutedWithPrice(uint16_t stockLocate, OrderExecutedWithPrice* orderExecutedWithPrice);
        void processOrderReplace(uint16_t stockLocate, OrderReplace* orderReplace);
        void processStockTradingAction(uint16_t stockLocate, StockTradingAction* stockTradingAction);
            void registerStockWithVWAPManager(uint16_t stockLocate, const std::string& stock);
            void unregisterStockWithVWAPManager(uint16_t stockLocate);
        void processSystemEvent(uint16_t stockLocate, SystemEvent* systemEvent);
        void processTradeNonCross(uint16_t stockLocate, TradeNonCross* tradeNonCross);

        // Functions for controlling the VWAPManager's time related functionality
        void processEndOfPeriod(uint64_t timestamp);

    private:
        static VWAPManager* _instance;

        // Make a simple mapping for Stock Locate to Stock Symbol so I can store uint16_t instead of std::string
        std::unordered_map<uint16_t, std::string> _stockLocateToStockSymbol;
        void _saveStockLocateToStockSymbolMapping(uint16_t stockLocate, std::string stockSymbol);

        // Every order that is executed will be represented in these mappings. Should be maintained as same size.
        std::unordered_map<uint16_t, std::unique_ptr<VWAPPeriodicData>> _periodicStockLocateToData;
        std::unordered_map<uint16_t, std::unique_ptr<VWAPCumulativeData>> _cumulativeStockLocateToData;

        void _safeaddStockToPeriodicMap(uint16_t _stockLocate, const std::string& _stock);
        void _safeaddStockToCumulativeMap(uint16_t _stockLocate, const std::string& _stock);
        void _safehaltStockInPeriodicMap(uint16_t _stockLocate);

        std::unique_ptr<VWAPPeriodicData>*   _getPeriodicData(uint16_t _stockLocate);
        std::unique_ptr<VWAPCumulativeData>* _getCumulativeData(uint16_t _stockLocate);     

        void _addPeriodicDataToCumulativeDataAndReset(std::unique_ptr<VWAPPeriodicData>* _periodicData, std::unique_ptr<VWAPCumulativeData>* _cumulativeData);
        
        // Actually calculate the VWAP
        uint32_t _calculateVWAPForStock(std::unique_ptr<VWAPCumulativeData>* _cumulativeData);

        explicit VWAPManager():
            _periodicStockLocateToData(),
            _cumulativeStockLocateToData()
        {};
};  

#endif // TREXQUANTTAKEHOME_BOOKKEEPING_VWAP_MANAGER_H