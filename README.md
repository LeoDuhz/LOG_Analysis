# LOG_Analysis
Robocup SSL LOG Analysis



## File:

`Hera`:ZJUNLICT LOG Tools

`ssl-logtools`:Robocup SSL LOG Tools



## To Do:

### LOG Split

- Find useful log part(skip the referee time)

  - Official LOG:https://tigers-mannheim.de/download/gamelogs/ or https://mirror.robojackets.org/robocup-small-size-league/gamelogs/

  - LOG file format(Reference: https://github.com/RoboCup-SSL/ssl-logtools)

    Each log file starts with the following header:

    > 1: String - File type ("SSL_LOG_FILE") <br>
    > 2: Int32  - Log file format version <br>

    Format version 1 encodes the protobuf messages in the following format:

    > 1: Int64  - Receiver timestamp in ns <br>
    > 2: Int32  - Message type <br>
    > 3: Int32  - Size of binary protobuf message <br>
    > 4: String - Binary protobuf message <br>

    The message types are:

    > MESSAGE_BLANK           = 0 (ignore message)<br>
    > MESSAGE_UNKNOWN         = 1 (try to guess message type by parsing the data)<br>
    > MESSAGE_SSL_VISION_2010 = 2<br>
    > MESSAGE_SSL_REFBOX_2013 = 3<br>
    > MESSAGE_SSL_VISION_2014 = 4<br>

    #### Read 

    Header->DataHeader->MessageType?->Message

    - Learn Protobuf

    - Protobuf format: Referee status judge:Command:Normal Start or Force Start

    - 2021.2.28 Problem:

      ![Screenshot from 2021-02-23 16-52-42](README.assets/Screenshot from 2021-02-23 16-52-42.png)

      - g++ examplereader.cpp -o examplereader -lprotobuf -lpthread -lm -lstdc++

      - 2021.3.1 fuck! Problem Solved!

        Solution:ssl-logtools-master/src/examplereader/CMakeLists.txt last line delete

    - LOG: may have error    **how to deal with error and some problems in LOG**
    
    - Raw data has to be processed by our **Vision Module**
    
    - 2021.3.6 fix many problems of **Hera**, see git commits
    
    - 2021.3.7 while running Hera, **segmentation fault**,  using gdb to debug:
    
      Program terminated with signal SIGSEGV, Segmentation fault.
      #0  google::protobuf::Message::GetDescriptor (this=0x7f0af7938348)
          at ./google/protobuf/message.h:353
      353	  const Descriptor* GetDescriptor() const { return GetMetadata().descriptor; }
    
    - fix by Keke, with a brand new vision module 
    
      using official logplayer, sending data to visionmodule by udp
    
    - newvisionmodule:
    
      port of referee and vision needs to match with logplayer player.cpp
    
      move the main.qml to debug directory
  
  