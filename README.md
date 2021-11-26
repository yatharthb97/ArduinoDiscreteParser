# ArduinoDiscreteParsar
---	

A discrete parser for Arduino devices.


## Mini Tutorial

1. Create object with parser group of size 5.
```
Parser<5> parser;
```

2. In `setup()`:
```
parser.add("parser_key", corresponding_parser_fn);
```

3. In `loop()`:
```
parser.check_input();
parser.loop()
```

Check the `Template` folder for a template file.


## Signals emitted by `Parser` object

The device emits " # SIGNAL KEY".

*SIGNAL* types:
* ACK - When a parsed string(KEY) is acknowledged and queued
* NACK - When a parsed string(KEY) is *not* acknowledged and ignored
* COMPLETE - When a queued task is completed

