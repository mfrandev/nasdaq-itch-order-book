# Trexquant Take Home Project (8/5/2024 - 8/12/2024): Michael Franceski

## Problem statement: Write an efficient program that parses trades from a NASDAQ ITCH 5.0 tick data file and outputs a running volume-weighted average price (VWAP) for each stock at every hour including market close.

## Critical Reasoning that Impacted Design and Implementation:
<ol>
    <li><b>This program will treat the ITCH 5.0 file contents as a <em>REAL TIME STREAM</em>.</b></li>
    <li>This means the VWAP internal state will only be accurate given the information the program has processed thus far.</li>
    <li>The program anticipates Broken Trade Or Order Execution messages may arrive anytime during system hours and <em>after</em> the execution with which its linked.</li> 
    <li>This means producing VWAP output at the end of each period may contain "eventually-inaccurate" data.</li>
    <li> Broken Trade Or Order Execution messages will only be processed if its respective trade or order was executed during <b>market hours</b>. 
    <li>Thus, I have chosen to <b>output a set of files</b> containing per-hour VWAP for each security <b>AFTER THE END OF SYSTEM HOURS.</b></li>
    <li>Per the problem statement, the last VWAP report should immediately following <em>Market Close</em>. For consistency, the first VWAP report will come on the first even hour after <em>Market Open</em> (10 hours after midnight) and <b>no pre-market or after-hours trading data will be considered.</b></li>
</ol>

## 3rd Party Libraries
My project uses one third party library that is unrelated to the problem domain.   
<ol>
    <b><a href="https://fmt.dev/11.0/" ><li>C++ String Formatting and IO management</li></a></b>
    This library is used to perform elegant string formatting and more efficient file output. 
</ol>

## Usage:

## Hardware I've Tested On:

## Implementation Notes:
<ol>
    <li>Processing is computationally expensive due to the required data structures and large file size, so I intentionally try to avoid as much overhead as possible and not get in the way of the stdlib. The need to retroactively process the BrokenTradeOrOrderExecution message significantly complicates things in this department.  
    <li>I decided to make all of the binary message parsing structs static and pass them everywhere by-reference. This means the header for every message and the body for each message type will have its own "reserved" chunk of memory ("reserved" because they are not access protected like a unique_ptr, but always occupy the same address).</li>
</ol>