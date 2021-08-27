# Master-Control-Program

## Introduction
Implementation of a Master Control Program or MCP for short. The MCP handles 
processes by scheduling and executing various jobs, giving each job appropriate cpu time, and handling the 
respective termination/errors of these jobs. Developed in four parts.
* **MCP V1**: launch a workload
* **MCP V2**: control a workload using signal processing
* **MCP V3**: scheduling (simple round robin) that allocates static time slices to each process in order of its arrival
* **MCP V4**: additional information gathering functionality
 
## Background
Signals are essentially ways to pass information to different processes. When a process is running, we can send 
different signals to control the way it executes. For example, if we wish to terminate the program unnaturally, we 
can send the SIGKILL signal. If we would simply like to pause the execution of a process, we can send a 
SIGSTOP signal. There are a variety of signals that have various uses, some being directly called by the 
programmer, others being indirectly invoked. When working with signals, it is important to have a signal handler 
in order to – quite literally – handle these signals appropriately. A signal handler is a function that is automatically 
called when a signal is executed (as long as you make sure to bind that signal to the signal handler before hand) 
and performs some function, usually just printing a message or changing the value of a global variable. 
