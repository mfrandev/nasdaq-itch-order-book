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
 * Make sure that VWAPManager is aware of each issue's trading state
 * Responsible for initializing the VWAPManager's books
 */
void VWAPManager::processStockTradingAction(uint16_t stockLocate, std::shared_ptr<StockTradingAction> stockTradingAction) {
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
    _safeaddStockToPeriodicMap(stockLocate, stock);
    _safeaddStockToCumulativeMap(stockLocate, stock);
}

/**
 * No need to touch the cumulative mapping. 
 * Either stock has never been traded, 
 * or status will be updated at the end of the period
 */
void VWAPManager::unregisterStockWithVWAPManager(uint16_t stockLocate) {
    _safehaltStockInPeriodicMap(stockLocate);
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
    // If this assertion does not hold, something has gone horribly awry
    assert(_periodicStockLocateToData.size() == _cumulativeStockLocateToData.size());

    // Need to write this period's VWAP to the file. 
    // Use the fmt::os library to handle this, it can be much better than standard IO
    // TODO: Find a nice way to print the timestamp in the filename. look into fmt::chrono
    fmt::ostream vwapOutputFile = fmt::output_file(fmt::format("vwap_{}.txt", timestamp));

    // <uint16_t, std::unique_ptr<VWAPPeriodicData>>
    // We can do this because no dependency on iteration order
    for(auto& stockLocateAndData: _periodicStockLocateToData) {
        std::unique_ptr<VWAPCumulativeData>* vwapCumulativeData = _getCumulativeData(stockLocateAndData.first);
        _addPeriodicDataToCumulativeDataAndReset(&stockLocateAndData.second, vwapCumulativeData);
        uint32_t vwap = _calculateVWAPForStock(vwapCumulativeData);
        vwapOutputFile.print("{}: {}{}\n",
            stockLocateAndData.second -> stock,
            vwap,
            stockLocateAndData.second -> isTrading ? "" : ": H"
        );
    }
    vwapOutputFile.close();
}

// ==================================== Private Functions ====================================
/**
 * Adds a stock to the periodic trading mapping. 
 * Safeadd because if is already in the map, just set isTrading to true.
 */
void VWAPManager::_safeaddStockToPeriodicMap(uint16_t _stockLocate, const std::string& _stock) {
    if(_periodicStockLocateToData.find(_stockLocate) == _periodicStockLocateToData.end()) 
        _periodicStockLocateToData[_stockLocate] = std::make_unique<VWAPPeriodicData>(VWAPPeriodicData{_stock, 0u, 0u, 0u, 0u, true});
    else 
        _periodicStockLocateToData[_stockLocate] -> isTrading = true;
}

/**
 * Adds a stock to the cumulative trading mapping. 
 * Safeadd because if is already in the map, just set isTrading to true.
 */
void VWAPManager::_safeaddStockToCumulativeMap(uint16_t _stockLocate, const std::string& _stock) {
    if(_cumulativeStockLocateToData.find(_stockLocate) == _cumulativeStockLocateToData.end())
        _cumulativeStockLocateToData[_stockLocate] = std::make_unique<VWAPCumulativeData>(VWAPCumulativeData{_stock, 0u, 0u, true});
}

/**
 * If stock is present, note its trading status has changed
 */
void VWAPManager::_safehaltStockInPeriodicMap(uint16_t _stockLocate) {
    if(_periodicStockLocateToData.find(_stockLocate) != _periodicStockLocateToData.end()) 
        _periodicStockLocateToData[_stockLocate] -> isTrading = false;
}

/**
 * Given a stock locate, get a reference to the reference for its periodic data
 */
std::unique_ptr<VWAPPeriodicData>* VWAPManager::_getPeriodicData(uint16_t _stockLocate) {
    return &_periodicStockLocateToData[_stockLocate];
}

/**
 * Given a stock locate, get a reference to the reference for its cumulative data
 */
std::unique_ptr<VWAPCumulativeData>* VWAPManager::_getCumulativeData(uint16_t _stockLocate) {
    return &_cumulativeStockLocateToData[_stockLocate];
}

/**
 * TODO: IF BAD RESULTS, COME BACK HERE AND CHECK OUT THIS FUNCTION
 * VWAPPeriodicData rollover
 */
void VWAPManager::_addPeriodicDataToCumulativeDataAndReset(std::unique_ptr<VWAPPeriodicData>* _periodicData, std::unique_ptr<VWAPCumulativeData>* _cumulativeData) {

    bool wasTradingActiveThisPeriod = _periodicData -> get() -> isTrading;
    bool wasTradingActiveLastPeriod = _cumulativeData -> get() -> isTrading;

    // Nothing happened
    if(!wasTradingActiveThisPeriod && !wasTradingActiveLastPeriod) return;

    // Get the PV this period (Typical Price * Volume)
    uint32_t high = _periodicData -> get() -> highPrice;
    uint32_t low = _periodicData -> get() -> lowPrice;
    uint32_t close = _periodicData -> get() -> closingPrice;
    uint64_t volume = _periodicData -> get() -> volume;

    uint64_t typicalPriceThisPeriod = (high + low + close) / 3;
    uint64_t priceTimesVolumeThisPeriod = typicalPriceThisPeriod * volume; // Result

    // Accumulate this period's statistics for upcoming VWAP calc
    _cumulativeData -> get() -> cumulativePrice  += priceTimesVolumeThisPeriod;
    _cumulativeData -> get() -> cumulativeVolume += volume;

    // Reset for next period
    _periodicData -> get() -> highPrice = 0;
    _periodicData -> get() -> lowPrice = 0;
    _periodicData -> get() -> closingPrice = 0;
    _periodicData -> get() -> volume = 0;

    // No need to update based on the current period's activity
    if(wasTradingActiveLastPeriod && wasTradingActiveThisPeriod) return;

    // Note that this the issue's "isTrading" state has changed since the close of the previous
    _cumulativeData -> get() -> isTrading = wasTradingActiveThisPeriod;
}

/**
 * TODO: IF BAD RESULTS, COME BACK HERE AND CHECK OUT THIS CAST
 * Given the current state of this issue's cumulative VWAP data, return the VWAP
 * Benchmarked this shared_ptr access pattern and it's >25x faster than copying the pointer itself !
 */
uint32_t VWAPManager::_calculateVWAPForStock(std::unique_ptr<VWAPCumulativeData>* _cumulativeData) {
    uint64_t volume = _cumulativeData -> get() -> cumulativeVolume;
    uint64_t price = _cumulativeData -> get() -> cumulativePrice;
    return static_cast<uint32_t>(price/volume);
}