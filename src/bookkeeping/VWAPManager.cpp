#include <VWAPManager.h>

#include <fmt/format.h>
#include <fmt/os.h>

#include <assert.h>

// Singleton declaration and getter
VWAPManager* VWAPManager::_instance = nullptr;
VWAPManager& VWAPManager::getInstance() {
    if(_instance == nullptr) 
        _instance = new VWAPManager();
    return *_instance;
}

/**
 * 
 */
void VWAPManager::processAddOrder(uint16_t stockLocate, std::shared_ptr<AddOrder> addOrder) {

}

/**
 * 
 */
void VWAPManager::processAddOrderMPID(uint16_t stockLocate, std::shared_ptr<AddOrderMPID> addOrderMPID) {

}

/**
 * 
 */
void VWAPManager::processBrokenTradeOrOrderExecution(uint16_t stockLocate, std::shared_ptr<BrokenTradeOrOrderExecution> brokenTradeOrOrderExecution) {

}

/**
 * 
 */
void VWAPManager::processCrossTrade(uint16_t stockLocate, std::shared_ptr<CrossTrade> crossTrade) {

} 
/**
 * 
 */
void VWAPManager::processOrderCancel(uint16_t stockLocate, std::shared_ptr<OrderCancel> orderCancel) {

}

/**
 * 
 */
void VWAPManager::processOrderDelete(uint16_t stockLocate, std::shared_ptr<OrderDelete> orderDelete) {

}

/**
 * 
 */
void VWAPManager::processOrderExecuted(uint16_t stockLocate, std::shared_ptr<OrderExecuted> orderExecuted) {

}

/**
 * 
 */
void VWAPManager::processOrderExecutedWithPrice(uint16_t stockLocate, std::shared_ptr<OrderExecutedWithPrice> orderExecutedWithPrice) {

}

/**
 * 
 */
void VWAPManager::processOrderReplace(uint16_t stockLocate, std::shared_ptr<OrderReplace> orderReplace) {

}

/**
 * Make sure that VWAPManager is aware of each issue's trading state and symbol
 * Responsible for initializing the VWAPManager's books
 */
void VWAPManager::processStockTradingAction(uint16_t stockLocate, std::shared_ptr<StockTradingAction> stockTradingAction) {
    _saveStockLocateToStockSymbolMapping(stockLocate, stockTradingAction -> stock);
    switch(stockTradingAction -> tradingState) {
        case TRADING_STATE_REASON_TRADING_ON_NASDAQ:
            registerStockWithVWAPManager(stockLocate, stockTradingAction -> stock);
            break;
        default:
            unregisterStockWithVWAPManager(stockLocate);
            break;
    }
}

/**
 * Register this stock with the VWAP manager
 */
void VWAPManager::registerStockWithVWAPManager(uint16_t stockLocate, const std::string& stock) {

}

/**
 * No need to touch the cumulative mapping. 
 * Either stock has never been traded, 
 * or status will be updated at the end of the period
 */
void VWAPManager::unregisterStockWithVWAPManager(uint16_t stockLocate) {

}

/**
 * 
 */
void VWAPManager::processSystemEvent(uint16_t stockLocate, std::shared_ptr<SystemEvent> systemEvent) {

}

/**
 * 
 */
void VWAPManager::processTradeNonCross(uint16_t stockLocate, std::shared_ptr<TradeNonCross> tradeNonCross) {

}

/**
 * Execute when the current period expires for each issue.
 * 1. Calculate the VWAP and prepare for the next period
 * 2. File I/O
 */
void VWAPManager::processEndOfPeriod(uint64_t timestamp) {

}

// ==================================== Private Functions ====================================
/**
 * Save a mapping of stock locate to symbol so each symbol for easy translation
 */
void VWAPManager::_saveStockLocateToStockSymbolMapping(uint16_t stockLocate, std::string stockSymbol) {
    if(!_stockLocateToStockSymbol.count(stockLocate))
        _stockLocateToStockSymbol[stockLocate] = stockSymbol;
}

/**
 * Adds a stock to the periodic trading mapping. 
 * Safeadd because if is already in the map, just set isTrading to true.
 */
void VWAPManager::_safeaddStockToPeriodicMap(uint16_t _stockLocate, const std::string& _stock) {

}

/**
 * Adds a stock to the cumulative trading mapping. 
 * Safeadd because if is already in the map, just set isTrading to true.
 */
void VWAPManager::_safeaddStockToCumulativeMap(uint16_t _stockLocate, const std::string& _stock) {

}

/**
 * If stock is present, note its trading status has changed
 */
void VWAPManager::_safehaltStockInPeriodicMap(uint16_t _stockLocate) {

}

/**
 * Given a stock locate, get a reference to the reference for its periodic data
 */
std::unique_ptr<VWAPPeriodicData>* VWAPManager::_getPeriodicData(uint16_t _stockLocate) {

}

/**
 * Given a stock locate, get a reference to the reference for its cumulative data
 */
std::unique_ptr<VWAPCumulativeData>* VWAPManager::_getCumulativeData(uint16_t _stockLocate) {

}

/**
 * TODO: IF BAD RESULTS, COME BACK HERE AND CHECK OUT THIS FUNCTION
 * VWAPPeriodicData rollover
 */
void VWAPManager::_addPeriodicDataToCumulativeDataAndReset(std::unique_ptr<VWAPPeriodicData>* _periodicData, std::unique_ptr<VWAPCumulativeData>* _cumulativeData) {

}

/**
 * TODO: IF BAD RESULTS, COME BACK HERE AND CHECK OUT THIS CAST
 * Given the current state of this issue's cumulative VWAP data, return the VWAP
 * Benchmarked this shared_ptr access pattern and it's >25x faster than copying the pointer itself !
 */
uint32_t VWAPManager::_calculateVWAPForStock(std::unique_ptr<VWAPCumulativeData>* _cumulativeData) {

}