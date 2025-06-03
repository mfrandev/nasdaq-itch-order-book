## Critical Reasoning that Impacted Design and Implementation:
<ol>
    <li><b>This program will treat the ITCH 5.0 file contents as a <em>REAL TIME STREAM</em>.</b></li>
    <li>This means the VWAP internal state will only be accurate given the information the program has processed thus far.</li>
    <li>The program anticipates Broken Trade Or Order Execution messages may arrive anytime during system hours and <em>after</em> the execution with which its linked.</li> 
    <li>This means producing VWAP output at the end of each period may contain "eventually-inaccurate" data.</li>
    <li>Thus, I have chosen to <b>output a set of files</b> containing per-hour, broken trade adjusted VWAP for each security <b>AFTER THE END OF SYSTEM HOURS.</b></li>
    <li>Per the problem statement, the last VWAP report should be for <em>Market Close</em>. For consistency, the first VWAP report will come on the first even hour after <em>Market Open</em> (10 hours after midnight) and <b>no after-hours trading data will be considered.</b></li>
</ol>

## 3rd Party Libraries
My project uses one third party library that is unrelated to the problem domain.   
<ol>
    <b><a href="https://fmt.dev/11.0/" ><li>C++ String Formatting and IO management</li></a></b>
    This library is used to perform elegant string formatting and more efficient file output. 
</ol>

## Usage:
<ol>
    <li>Dependencies: An internet connection (fmt lib comes from a remote repository), CMake (within the version range 3.15...3.25), and a CXX 17 compatible compiler.</li>
    <li>Compiling: From the project's <em><b>root directory</b></em>, I recommend running <b>./buildandrunit.sh</b>. This calls both the buildit.sh and runit.sh scripts consecutively.</li>
    <li>Download the file "01302019.NASDAQ_ITCH50.gz" from this link: https://www.google.com/url?sa=D&q=https://emi.nasdaq.com/ITCH/Nasdaq%2520ITCH/01302019.NASDAQ_ITCH50.gz&ust=1748464380000000&usg=AOvVaw04xd8uwKynAZ2iQsUDXgha&hl=en&source=gmail. Unzip it in the "ITCHFiles" directory. 
    <li>Using other ITCH files: First, place an unzipped ITCH file in "ITCHFiles." Then, go into the <b>runit.sh</b> script and replace "./nasdaq-order-book" with "./nasdaq-order-book [Filename-In-ITCHFiles]." Do not specify more than 1 file per-execution.</li>
    <li>By default, the program will try to read "ITCHFiles/01302019.NASDAQ_ITCH50"</li>
</ol>

## Hardware I've Tested On:
<ol>
    <li>(Main Workstation) Dell XPS 8950<br>Fedora Linux 40<br>12th Gen Intel Core i7-12700 (X86_64) <br>Compiler: GNU 14.2.1<br>Total execution time: ~146 seconds</li><br>
    <li>(Other Laptop) 2017 13-Inch MacbookAir<br> MacOS Monterrey<br>1.8 GHz Intel Core i5 dual processor<br>Compiler: AppleClang 13.0.0.13000027<br>Total execution time: ~526 seconds</li>
</ol>

## Implementation Notes:
<ol>
    <li>The VWAPManager and OrderBook classes implement all of the calculation and bookkeeping required to produce the running VWAP. VWAPManager tracks trade executions for each security over time. OrderBook tracks each order's state leading up to execution.</li>
    <li>Processing is computationally expensive, so I intentionally try to take advantage of C++'s move semantics and copy elision features.
    <li>I decided to make all of the binary message parsing structs static and pass them everywhere by-reference. This means the header for every message and the body for each message type will have its own "reserved" chunk of memory ("reserved" not because they are access protected like a unique_ptr, but always occupy the same address). Note: I took a memory profile on my main workstation (referenced above) and saw ~0.4% memory usage :)</li>
    <li>I performed testing/debugging by placing asserts for certain conditions throughout the"VWAPManager" and "OrderBook" class implemetnations. These asserts were left in the code as comments, because I assume this will program will get tested on other ITCH files and I don't know if they might get triggered by file-specific irregularities. </li>
    <li>The biggest challenge this project posed by far was how to properly handle Broken Trades or Executions. I eventually settled on a strategy to screen for possibly erroneous trades using a heuristic Nasdaq alluded to in their Erroneous Trade Policy Page, separate them from clearly valid trades, then merge the remaining trades into the final "broken trade adjusted" VWAP, after the close of system hours. </li>
</ol>