
* deal with transmit status messages
--- these are dealt with in the transmit function, that is what they return.

* (consider) move back to callback interface, rather than busy waiting for messages
* (consider) queue for sending out messages (no busy waiting for transmit confirmations)

Next steps:

* add osc interface
* add sending data to minibee


* read xml configuration file

* add interpreting RSSI data

* send osc info message
* send osc status message
* parse extra message
* parse trigger message