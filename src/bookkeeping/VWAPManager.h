#ifndef TREXQUANTTAKEHOME_BOOKKEEPING_VWAP_MANAGER_H
#define TREXQUANTTAKEHOME_BOOKKEEPING_VWAP_MANAGER_H

#include <memory>
#include <cstdint>
#include <string>
#include <unordered_map>

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


/**
 * Use this to trace stock price and volume per-period
 * In mappings, this will be searchable by Stock Locate Code
 */
struct VWAPPeriodicData {
    std::string stock;
    uint32_t highPrice;
    uint32_t lowPrice;
    uint32_t closingPrice;
    uint64_t volume;
    bool isTrading; // Issues halted intraday will not be removed from VWAP report. Provide a mechanism to show whether or not the security is actively traded AT THE TIME OF THE REPORT.
};

struct VWAPCumulativeData {
    std::string stock;
    uint64_t cumulativePrice;
    uint64_t cumulativeVolume;
    bool isTrading; // Issues halted intraday will not be removed from VWAP report. Provide a mechanism to show whether or not the security is actively traded AT THE TIME OF THE REPORT.
};

/**
 * This class is reponsible for maintaining the minimal data required
 * to calculate VWAP
 */
class VWAPManager {
    public:
        static VWAPManager& getInstance();

        // Add processing for each mesage. Helper functions indented
        void processAddOrder(uint16_t stockLocate, std::shared_ptr<AddOrder> addOrder);
        void processAddOrderMPID(uint16_t stockLocate, std::shared_ptr<AddOrderMPID> addOrderMPID);
        void processBrokenTradeOrOrderExecution(uint16_t stockLocate, std::shared_ptr<BrokenTradeOrOrderExecution> brokenTradeOrOrderExecution);
        void processCrossTrade(uint16_t stockLocate, std::shared_ptr<CrossTrade> crossTrade); 
        void processOrderCancel(uint16_t stockLocate, std::shared_ptr<OrderCancel> orderCancel);
        void processOrderDelete(uint16_t stockLocate, std::shared_ptr<OrderDelete> orderDelete);
        void processOrderExecuted(uint16_t stockLocate, std::shared_ptr<OrderExecuted> orderExecuted);
        void processOrderExecutedWithPrice(uint16_t stockLocate, std::shared_ptr<OrderExecutedWithPrice> orderExecutedWithPrice);
        void processOrderReplace(uint16_t stockLocate, std::shared_ptr<OrderReplace> orderReplace);
        void processStockTradingAction(uint16_t stockLocate, std::shared_ptr<StockTradingAction> stockTradingAction);
            void registerStockWithVWAPManager(uint16_t stockLocate, const std::string& stock);
            void unregisterStockWithVWAPManager(uint16_t stockLocate);
        void processSystemEvent(uint16_t stockLocate, std::shared_ptr<SystemEvent> systemEvent);
        void processTradeNonCross(uint16_t stockLocate, std::shared_ptr<TradeNonCross> tradeNonCross);

        // Functions for controlling the VWAPManager's time related functionality
        bool isPeriodOver(uint64_t timestamp);
        void processEndOfPeriod(uint64_t timestamp);

    private:
        static VWAPManager* _instance;

        // Every order that is executed will be represented in these mappings. Should be maintained as same size.
        std::unordered_map<uint16_t, std::unique_ptr<VWAPPeriodicData>> _periodicStockLocateToData;
        std::unordered_map<uint16_t, std::unique_ptr<VWAPCumulativeData>> _cumulativeStockLocateToData;

        uint64_t _startOfPeriodTimestamp;

        void _safeaddStockToPeriodicMap(uint16_t _stockLocate, const std::string& _stock);
        void _safeaddStockToCumulativeMap(uint16_t _stockLocate, const std::string& _stock);
        void _safehaltStockInPeriodicMap(uint16_t _stockLocate);

        std::unique_ptr<VWAPPeriodicData>*   _getPeriodicData(uint16_t _stockLocate);
        std::unique_ptr<VWAPCumulativeData>* _getCumulativeData(uint16_t _stockLocate);     

        void _addPeriodicDataToCumulativeDataAndReset(std::unique_ptr<VWAPPeriodicData>* _periodicData, std::unique_ptr<VWAPCumulativeData>* _cumulativeData);
        
        // Actually calculate the VWAP
        uint32_t _calculateVWAPForStock(std::unique_ptr<VWAPCumulativeData>* _cumulativeData);

        int _writeVWAPDataToFile();
        
        static const uint64_t _NANOSECONDS_PER_HOUR;

        explicit VWAPManager():
            _periodicStockLocateToData(),
            _cumulativeStockLocateToData(),
            _startOfPeriodTimestamp(0u)
        {};
};  

#endif // TREXQUANTTAKEHOME_BOOKKEEPING_VWAP_MANAGER_H