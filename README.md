## Implementation Notes:
<h4>General Comments, Applicable for both single and multi-threaded approaches.</h4>
<ol>
    <li>The VWAPManager and OrderBook classes implement all of the calculation and bookkeeping required to produce the running VWAP. VWAPManager tracks trade executions for each security over time. OrderBook tracks each order's state leading up to execution.</li>
    <li>Processing is computationally expensive, so I intentionally try to take advantage of C++'s move semantics and copy elision features. </li>
    <li>I performed testing/debugging by placing asserts for certain invariants throughout the"VWAPManager" and "OrderBook" class implemetnations.</li>
    <li>The biggest challenge this project posed by far was how to properly handle Broken Trades or Executions. I eventually settled on a strategy to screen for possibly erroneous trades using a heuristic Nasdaq alluded to in their Erroneous Trade Policy Page, separate them from clearly valid trades, then merge the remaining trades into the final "broken trade adjusted" VWAP, after the close of system hours.</li>
    <li>I have also verified that each concurrency model produces the same output, so each implementation is as correct as my approach.</li>
</ol>
<h4>Single Prodcer/Single Consumer (SPSC)</h4>
<ol>
    <li>In the SPSC implementation, I use the "boost::lockfree:spsc_queue" to allow for basic, lockfree multithreading.</li>
    <li>Allocating and deallocating heap memory for every message parsed is EXTREMELY expensive, as there are >300,000,000 messages per-ITCH file. Thus, I avoid this excessive memory overhead by creating a per-message memory pool on the stack.</li>
    <li>The per-message memory pool is allocated "SPSC_QUEUE_SIZE + 2" spaces, since at most "SPSC_QUEUE_SIZE + 1" of any given message can exist at a time. The design of the lockfree ring buffer requires an empty space for "fullness" detection. 
    <li>The basic correctness precondition is, as the name suggests, that only one thread can push to the spsc_queue and only one thread can pop from it.</li>
    <li>In practice, one thread reads the binary file and places all elements in an instance of this lockfree queue, then a second thread goes through and processes each of those messages in parallel.</li>
    <li>Unlike the single threaded model, the SPSC model requires each of the message classes to inherit from a base superclass, "Message," so a pointer of each item can be placed on the queue.</li>
    <li>Regrettably, the boost lockfree queue requires its elements to be copyable, so using std::unique_ptr<Message> in the queue is not a viable strategy.</li>
    <li>Thus, I had to opt for raw pointers instead. I chose to forego casting to std::unique_ptr<Message> in the consumer thread and manually invoke "delete" to avoid the performance hit when transferring ownership of each message.</li>  
    <li>Said performance hit was very significant. Using RAII in this case increased runtimes by ~20%, nullifying any benefit gained from multithreading.</li>
</ol>
<h4>Performance</h4>
<ol>
    <li>For hardware details, see the "Hardware I've Tested On" section below.</li>
    <li>Single Threading generally executes in the range 60-65 seconds. Outlier performance is +/- 3 seconds.</li>
    <li>SPSC generally executes in the range 40-45 seconds. Outlier performance is typically within 2 seconds below the low end of the range.</li>
</ol>

## Critical Reasoning that Impacted Design and Implementation:
<ol>
    <li><b>This program will treat the ITCH 5.0 file contents as a <em>REAL TIME STREAM</em>.</b></li>
    <li>This means the VWAP internal state will only be accurate given the information the program has processed thus far.</li>
    <li>The program anticipates Broken Trade Or Order Execution messages may arrive anytime during system hours and <em>after</em> the execution with which it's linked.</li> 
    <li>This means producing VWAP output at the end of each period may contain "eventually-accurate" data.</li>
    <li>Thus, I have chosen to <b>output a set of files</b> containing per-hour, broken trade adjusted VWAP for each security <b>AFTER THE END OF SYSTEM HOURS.</b></li>
    <li>Per the problem statement, the last VWAP report should be for <em>Market Close</em>. For consistency, the first VWAP report will come on the first even hour after <em>Market Open</em> (10 hours after midnight) and <b>no after-hours trading data will be considered.</b></li>
    <li>As long as messages are processed sequentially, it doesn't matter what kind of producer/consumer scheme is used. As of now, I have tried the single threaded approach, as well as a single producer/single consumer multi-threaded strategy. Details on performance below.</li>
</ol>

## 3rd Party Libraries
My project uses one third party library that is unrelated to the problem domain.   
<ol>
    <b><a href="https://fmt.dev/11.0/" ><li>C++ String Formatting and IO management</li></a></b>
    This library is used to perform elegant string formatting and more efficient file output. 
    <b><a href="https://www.boost.org/doc/libs/latest/doc/html/doxygen/classboost_1_1lockfree_1_1spsc__queue.html#doxygen.classboost_1_1lockfree_1_1spsc__queue_1ad631dce12b26271ec5658f3f3f8104d6"><li>Boost Lockfree Queue for the Single Produce/Single Consumer multithreading pattern.</li></a></b>
    This is the queue I used for the lockfree, SPSC implementation model.
</ol>

## Usage:
<ol>
    <li>Dependencies: An internet connection (fmt lib comes from a remote repository), CMake (within the version range 3.15...3.25), and a CXX 17 compatible compiler.</li>
    <li>Compiling: From the project's <em><b>root directory</b></em>, I recommend running <b>./buildandrunit.sh</b>. This calls both the buildit.sh and runit.sh scripts consecutively.</li>
    <li>"Boost" must also be installed on the machine, as I use their lockfree single producer/single consumer queue in one of the basic multithread implementations.</li>
    <li>Download the file "01302019.NASDAQ_ITCH50.gz" from this link: https://www.google.com/url?sa=D&q=https://emi.nasdaq.com/ITCH/Nasdaq%2520ITCH/01302019.NASDAQ_ITCH50.gz&ust=1748464380000000&usg=AOvVaw04xd8uwKynAZ2iQsUDXgha&hl=en&source=gmail. Unzip it in the "ITCHFiles" directory. 
    <li>Using other ITCH files: First, place an unzipped ITCH file in "ITCHFiles." Then, go into the <b>runit.sh</b> script and replace "./nasdaq-order-book" with "./nasdaq-order-book [Filename-In-ITCHFiles]." Do not specify more than 1 file per-execution.</li>
    <li>By default, the program will try to read "ITCHFiles/01302019.NASDAQ_ITCH50"</li>
</ol>

## Hardware I've Tested On:
<ol>
    <li>(Main Workstation) Dell XPS 8950<br>Fedora Linux 40<br>12th Gen Intel Core i7-12700 (X86_64) <br>Compiler: GNU 15.1.1
</ol>