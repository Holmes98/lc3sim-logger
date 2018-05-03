# lc3sim-logger
A simple logging extension for lc3sim on Windows.

Usage
-----
Load the .dll as an extension, then run the program.
The output is saved to `log.txt` in the same directory as the extension.
The state of the simulator is printed after every instruction until a HALT instruction is reached – in order to log a new program, the machine should be reinitialized (and the extension reloaded).
