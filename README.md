# Description
Memory editing software.
Software is only aviable on WINDOWS!

# details
Version: 1.0.0
Release date: 2021-12-25

# contact
If you notice any bugs or want to give feedback, write to: 
https://github.com/R-Michi/SimpleRayTracer/issues

# ------------------- COMMANDS: -------------------
Command: exit
Syntax: exit
Description: exits the program


Command: help
Syntax: help [<command>]
Description: shows help information for commands
Arguments:
    - NO ARGUMENT                               # shows general information about all commands
    - <command>                                 # shows more detailed information about one specific command


Command: config
Syntax: config [<setting>] [<new value>]
Description: manages application settings
Arguments: 
    - NO ARGUMENT                               # shows all settings
    - <setting>                                 # shows the curret setting
    - <new value>                               # new value for setting
Options:
    - --reset                                   # resets all settings to default values or only one setting to its default value
Aviable settings:
    - type              STRING                  # type setting
    - aligment          2B unsigned DECIMAL     # memory aligment
    - start_address     8B HEXADECIMAL          # start address of searching
    - end_address       8B HEXADECIMAL          # end address of searching
    - search_split_size 8B unsigned DECIMAL     # how large a memory block can be before it gets split
    - search_limit_size 8B unsigned DECIMAL     # how much memory (in bytes) is allowed to be stored when searching
Aviable types:
    - int8                                      # reads signed 1 byte values from memory
    - uint8                                     # reads unisgned 1 byte values from memory
    - int16                                     # reads signed 2 byte values from memory
    - uint16                                    # reads unsigned 2 byte values from memory
    - int32                                     # reads signed 4 byte values from memory
    - uint32                                    # reads unsigned 4 byte values from memory
    - int64                                     # reads signed 8 byte values from memory
    - uint64                                    # reads unsigned 8 byte values from memory
    - float                                     # reads 4 byte floating point values (IEEE 754 standard)
    - double                                    # reads 8 byte floating point values (IEEE 754 standard)
    - string                                    # reads strings from memory


Command: list
Syntax: list
Description: lits all running processes
Options:
    - NO OPTION                                 # lists all running processes
    - -a or --access                            # lists only accessable processes


Command: info
Syntax: info
Description: prints information about the application


Command: open
Syntax: open <pid>|<window name>
Description: opens a process ID or finds a window by name
Arguments:
    - <pid>             4B unsigned DECIMAL     # ID of the process that should be opened
    - <window name>     STRING                  # name of the window, which process should be opened (STRING)
Options:
    - NO OPTION                                 # if no option is set, a process is opened by its process ID
    - -w or --window                            # findes a window by its name and opens its process


Command: close
Syntax: close
Descriptions: closes current running secondary threads and processes, or closes the current open process
Options:
    - NO OPTION:                                # closes everything that has been opened
    - -p or --process:                          # only closes the current open process
    - -l or --live                              # only closes live memory showcase
    - -d or --dump                              # only closes hex dump


Command: cls or clear
Syntax: cls | clear
Description: cleares the screen


Command: read_single or rs
Syntax: read_single | rs <address> [<str_size>]
Description: reads data from a single memory address
Arguments:
    - <address>         8B HEXADECIMAL          # address where data should be read
    - <str_size>        4B unsigned DECIMAL     # optional parameter, if the current data-type is a string, it defines the size of the string to read


Command: read_block or rb
Syntax: read_block | rb <start> <range> [<str_size>]
Description: reads data from a range of memory addresses
Arguments:
    - <start>           8B HEXADECIMAL          # start address where data should be read (*1)(*2)
    - <range>           8B HEXADECIMAL          # number of bytes that should be read (*1)(*3)
    - <str_size>        4B unsigned DECIMAL     # optional parameter, if the current data-type is a string, it defines the size of the string to read


Command: search_exact or se
Syntax: search_exact | se <value>
Description: searches for an exact value in memory
Arguments:
    - <value>           set data-type           # exact value to search for
Options:
    - -a or --all                               # searches in all accessable processes


Command: search_range or sr
Syntax: search_range | sr <begin value> <end value>
Description: searches for a range of values in memory
Arguments:
    - <begin value>     set data-type           # lower end of the value range to search for
    - <end value>       set data-type           # upper end of the value range to search for
Options:
    - -a or --all                               # searches in all accessable processes


Command: write_all or wa
Syntax: write_all | wa <value>
Description: writes to all addresses that are currently stored
Arguments:
    - <value>           set data-type           # value that should be written


Command: write_single or ws
Syntax: write_single | ws <address> <value>
Description: writes to a single memory address
Arguments: 
    - <address>         8B HEXADECIMAL          # address where data should be written to
    - <value>           set data-type           # value that should be written


Command: write_range or wr
Syntax: write_range | wr <start> <range> <value>
Description: writes to a range of memory addresses
Arguments:
    - <start>           8B HEXADECIMAL          # start address where data should be written to(*1)(*2)
    - <range>           8B HEXADECIMAL          # number of bytes that should be written (*1)(*3)
    - <value>           set data-type           # value that should be written


Command: update
Syntax: update
Description: updates the currently stored values

Command: update_exact or ue
Syntax: update_exact | ue <value>
Description: searches for an exact new value in the currently stored values
Arguments:
    - <value>           set data-type           # new exact value to search for

Command: update_range or ur
Syntax: update_range | ur <begin value> <end value>
Description: searches for an range of values in the currently stored values
Arguments:
    - <begin value>     set data-type           # lower end of the value range to search for
    - <end value>       set data-type           # upper end of the value range to search for


Command: undo
Syntax: undo
Description: undoes the current read or search operation


Command: redo
Syntax: redo
Description: redoes the current read or search operation


Command: show
Syntax: show <start_entry> <amount>
Description: showes the currently read addresses and values
Arguments:
    - <start_entry>     4B unsigned DECIMAL     # number of the start entry 
    - <amount>          4B unsigned DECIMAL     # amount of entries that should be shown


Command: show_live or sl
Syntax: show_live | sl <start_entry> <amount> <update speed>
Description: showes the currently read addresses and values with live update
Arguments:
    - <start_entry>     4B unsigned DECIMAL     # number of the start entry 
    - <amount>          4B unsigned DECIMAL     # amount of entries that should be shown
    - <update speed>    4B unsigned DECIMAL     # update speed in milliseconds


Command: dump
Syntax: dump <begin address> <range> <width> <update speed>
Decription: makes a memory dump
Arguments:
    - <begin address>   8B HEXADECIMAL          # start address of the memory dump
    - <range>           8B HEXADECIMAL          # number of bytes that should be dumped
    - <width>           4B HEXADECIMAL          # number of bytes that are contained by one line of the memory dump
    - <update speed>    4B unsigned DECIMAL     # update speed in milliseconds


# *1 Definition
Definitions:
    - set start address (s')
    - start address (s)
    - set range (r')
    - range (r)
    - aligment (a)
    - size of the curremt data-type (ts)

# *2 calculation of start address
The start address of the read operation must be a modulo of the set aligment and must not be less than the begin address.
It is calculated by the following formula:
    s = s' + (a - s' % a)   if s' % a is greather than 0
    s = s'                  if s' % a is equal to 0
Example:
    s' = 97
    a = 4
    s = 97 + (4 - 97 % 4) = 97 + (4 - 1) = 97 + 3 = 100

# *3 calculation of range
The last address where data is read must be a modulo of the set aligment.
It is calculated by follwoing formula:
    r = (r' - r' % a) - ts  if r' % a is greather than 0
    r = r' - ts             if r' % a is equal to 0
Example:
    s = 100
    r' = 303
    a = 4
    ts = 8
    r = (303 - 303 % 4) - 8 = 303 - 3 - 8 = 292 -> the last address is 100 + 292 = 392
    The last 8-byte word would go from address 392 - 399 which is in our range, the next wold go from 400 - 407 which is outside of the range.
