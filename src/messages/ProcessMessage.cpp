#include <ProcessMessage.h>

#include <cassert>
#include <fmt/format.h>

#include <VWAPManager.h>
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

#include <time_utils.h>

void ProcessMessage::parseAndProcessMessageBody(const char* data,  std::size_t bytesToRead, BinaryMessageHeader* header) {

    // Check if the hour has changed and handle things appropriately
    uint8_t periodOfThisMessage = getCurrentPeriodFromTimestamp(header -> timestamp);
    if(periodOfThisMessage != currentPeriod) {
        // TODO: Tell the bookkeepers to transition to the next period
        
        // No need to continue keeping track of reporting period if market hours are done
        if(periodOfThisMessage < NUMBER_OF_PERIODS_PER_DAY)
            currentPeriod = periodOfThisMessage;
    }

    switch(header -> messageType) {

        // Messages we need to handle
        case MESSAGE_TYPE_STOCK_TRADING_ACTION: 
            {
            StockTradingAction* stockTradingAction = parseStockTradingActionBody(data);
            // VWAPManager::getInstance().processStockTradingAction(header -> stockLocate, stockTradingAction);
            // fmt::println("{},{},{},{}", stockTradingAction -> stock, stockTradingAction -> tradingState, stockTradingAction -> reserved, stockTradingAction -> reason);
            }
            break;
        case MESSAGE_TYPE_ORDER_EXECUTED:
            {
            OrderExecuted* orderExecuted = parseOrderExecutedBody(data);
            // VWAPManager::getInstance().processOrderExecuted(header -> stockLocate, orderExecuted);
            // fmt::println("{},{},{}", orderExecuted -> orderReferenceNumber, orderExecuted -> executedShares, orderExecuted -> matchNumber);
            }
            break;
        case MESSAGE_TYPE_ORDER_EXECUTED_WITH_PRICE:
            {
            OrderExecutedWithPrice* orderExecutedWithPrice = parseOrderExecutedWithPriceBody(data);
            // VWAPManager::getInstance().processOrderExecutedWithPrice(header -> stockLocate, orderExecutedWithPrice);
            // fmt::println("{},{},{},{},{}", orderExecutedWithPrice -> orderReferenceNumber, orderExecutedWithPrice -> executedShares, orderExecutedWithPrice -> matchNumber, orderExecutedWithPrice -> printable, orderExecutedWithPrice -> executionPrice);
            }
            break;
        case MESSAGE_TYPE_TRADE_NON_CROSS:
            {
            TradeNonCross* tradeNonCross = parseTradeNonCrossBody(data);
            // VWAPManager::getInstance().processTradeNonCross(header -> stockLocate, tradeNonCross);
            // fmt::println("{},{},{},{},{},{}", tradeNonCross -> orderReferenceNumber, tradeNonCross -> buySellIndicator, tradeNonCross -> shares, tradeNonCross -> stock, tradeNonCross -> price, tradeNonCross -> matchNumber);
            }
            break;
        case MESSAGE_TYPE_TRADE_CROSS:
            {
            CrossTrade* crossTrade = parseCrossTradeBody(data);
            // VWAPManager::getInstance().processCrossTrade(header -> stockLocate, crossTrade);
            // fmt::println("{},{},{},{},{}", crossTrade -> shares, crossTrade -> stock, crossTrade -> crossPrice, crossTrade -> matchNumber, crossTrade -> crossType);
            }
            break;
        case MESSAGE_TYPE_ADD_ORDER_NO_MPID:
            {
            AddOrder* addOrder = parseAddOrderBody(data);
            // VWAPManager::getInstance().processAddOrder(header -> stockLocate, addOrder);
            // fmt::println("{},{},{},{},{}", addOrder -> orderReferenceNumber, addOrder -> buySellIndicator, addOrder -> shares, addOrder -> stock, addOrder -> price);
            }
            break;
        case MESSAGE_TYPE_ADD_ORDER_WITH_MPID:
            {
            AddOrderMPID* addOrderMPID = parseAddOrderMPIDBody(data);
            // VWAPManager::getInstance().processAddOrderMPID(header -> stockLocate, addOrderMPID);
            // fmt::println("{},{},{},{},{},{}", addOrderMPID -> orderReferenceNumber, addOrderMPID -> buySellIndicator, addOrderMPID -> shares, addOrderMPID -> stock, addOrderMPID -> price, addOrderMPID -> attribution);
            }
            break;
        case MESSAGE_TYPE_SYSTEM_EVENT:
            {
            SystemEvent* systemEvent = parseSystemEventBody(data);
            // fmt::println("{}",systemEvent -> eventCode);
            switch(systemEvent -> eventCode) {
                case EVENT_CODE_START_OF_MARKET_HOURS: // 1st Period has already started, start VWAP manager
                    break;
                case EVENT_CODE_END_OF_MARKET_HOURS:
                    break;
                case EVENT_CODE_END_OF_SYSTEM_HOURS: // All trade breaks have been accounted for, so output VWAP !
                    break;
                default: 
                    break;
            }
            // VWAPManager::getInstance().processSystemEvent(header -> stockLocate, systemEvent);            }
            break;
            }
        case MESSAGE_TYPE_ORDER_CANCELLED:
            {
            OrderCancel* orderCancel = parseOrderCancelBody(data);
            // VWAPManager::getInstance().processOrderCancel(header -> stockLocate, orderCancel);
            // fmt::println("{},{}",orderCancel -> originalOrderReferenceNumber, orderCancel -> cancelledShares);
            }
            break;
        case MESSAGE_TYPE_ORDER_DELETE:
            {
            OrderDelete* orderDelete = parseOrderDeleteBody(data);
            // VWAPManager::getInstance().processOrderDelete(header -> stockLocate, orderDelete);
            // fmt::println("{}", orderDelete -> originalOrderReferenceNumber); 
            }
            break;
        case MESSAGE_TYPE_ORDER_REPLACE:
            {
            OrderReplace* orderReplace = parseOrderReplaceBody(data);
            // VWAPManager::getInstance().processOrderReplace(header -> stockLocate, orderReplace);
            // fmt::println("{},{},{},{}", orderReplace -> originalOrderReferenceNumber, orderReplace -> newOrderReferenceNumber, orderReplace -> shares, orderReplace -> price);
            }
            break;
        case MESSAGE_TYPE_TRADE_BROKEN:
            {
            BrokenTradeOrOrderExecution* brokenTradeOrOrderExecution = parseBrokenTradeOrOrderExecutionBody(data);
            // VWAPManager::getInstance().processBrokenTradeOrOrderExecution(header -> stockLocate, brokenTradeOrOrderExecution);
            }
            break;

        default:
            break;
    }
}

/**
 * Given a message type, return how many bytes its body is
 */
size_t ProcessMessage::messageTypeToNumberOfBytes(char messageType) {
    size_t messageSize;
    switch(messageType) {
        case MESSAGE_TYPE_SYSTEM_EVENT:
            messageSize = MESSAGE_SIZE_SYSTEM_EVENT;
            break;

        case MESSAGE_TYPE_STOCK_DIRECTORY:
            messageSize = MESSAGE_SIZE_STOCK_DIRECTORY;
            break;
        case MESSAGE_TYPE_STOCK_TRADING_ACTION:
            messageSize = MESSAGE_SIZE_STOCK_TRADING_ACTION;
            break;
        case MESSAGE_TYPE_REG_SHO:
            messageSize = MESSAGE_SIZE_REG_SHO;
            break;
        case MESSAGE_TYPE_MARKET_PARTICIPANT_POSITION:
            messageSize = MESSAGE_SIZE_MARKET_PARTICIPANT_POSITION;
            break;
        case MESSAGE_TYPE_MWCB_DECLINE_LEVEL:
            messageSize = MESSAGE_SIZE_MWCB_DECLINE_LEVEL;
            break;
        case MESSAGE_TYPE_MWCB_STATUS:
            messageSize = MESSAGE_SIZE_MWCB_STATUS;
            break;
        case MESSAGE_TYPE_QUOTING_PERIOD_UPDATE:
            messageSize = MESSAGE_SIZE_QUOTING_PERIOD_UPDATE;
            break;
        case MESSAGE_TYPE_LULD_AUCTION_COLLAR:
            messageSize = MESSAGE_SIZE_LULD_AUCTION_COLLAR;
            break;
        case MESSAGE_TYPE_OPERATIONAL_HALT:
            messageSize = MESSAGE_SIZE_OPERATIONAL_HALT ;
            break;

        case MESSAGE_TYPE_ADD_ORDER_NO_MPID:
            messageSize = MESSAGE_SIZE_ADD_ORDER_NO_MPID;
            break;
        case MESSAGE_TYPE_ADD_ORDER_WITH_MPID:
            messageSize = MESSAGE_SIZE_ADD_ORDER_WITH_MPID;
            break;

        case MESSAGE_TYPE_ORDER_EXECUTED:
            messageSize = MESSAGE_SIZE_ORDER_EXECUTED;
            break;
        case MESSAGE_TYPE_ORDER_EXECUTED_WITH_PRICE:
            messageSize = MESSAGE_SIZE_ORDER_EXECUTED_WITH_PRICE;
            break;
        case MESSAGE_TYPE_ORDER_CANCELLED:
            messageSize = MESSAGE_SIZE_ORDER_CANCELLED;
            break;
        case MESSAGE_TYPE_ORDER_DELETE:
            messageSize = MESSAGE_SIZE_ORDER_DELETE;
            break;
        case MESSAGE_TYPE_ORDER_REPLACE:
            messageSize = MESSAGE_SIZE_ORDER_REPLACE;
            break;

        case MESSAGE_TYPE_TRADE_NON_CROSS:
            messageSize = MESSAGE_SIZE_TRADE_NON_CROSS;
            break;
        case MESSAGE_TYPE_TRADE_CROSS:
            messageSize = MESSAGE_SIZE_TRADE_CROSS;
            break;
        case MESSAGE_TYPE_TRADE_BROKEN:
            messageSize = MESSAGE_SIZE_TRADE_BROKEN;
            break;

        case MESSAGE_TYPE_NOII:
            messageSize = MESSAGE_SIZE_NOII;
            break;

        case MESSAGE_TYPE_RPII:
            messageSize = MESSAGE_SIZE_RPII;
            break;

        case MESSAGE_TYPE_DLCR_PRICE_DISCOVERY:
            messageSize = MESSAGE_SIZE_DLCR_PRICE_DISCOVERY;
            break;

        default:
            messageSize = 0;
            break;
    }
    return messageSize;
}