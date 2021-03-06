# LOG_Analysis
Robocup SSL LOG Analysis



## File:

### newVisionModule

ZJUNLICT Vision Module for LOG processing

### ssl-logtools

Robocup SSL LOG Tools

### HeteroGraphMethod

- `main.py`: main function of LOG Analysis model training process

- `parameters.py`: parameters management of the training process

- `dataFormat.py`: basic data format of player, ball and game data

- `referee.py`: referee command defined in proto file

- `dataPreprocess.py`: preprocess the text file we get from our vision module, converting into formatted data we can use and doing Min-Max normalization

- `SSLDataset.py`: construct the graph structure for future gnn training

- `mys2v.py`: basic graph neural network we use 

- `pna/mypna.py`: Heterogeneous PNAConv

- `heterogeneous/myheter.py`: Heterogeneous graph neural network frame supporting different GNNs

- `Net.py`: neural network we construct

- `debug/debug.py`: draw gradient of graph neural network

- `visualize.py`: draw pictures of our training result

- `testOurModel.py`: load torch model and see results and also visualiztion



### HomoGraphMethod



## Research Record

### To Do:

#### 1.LOG Split

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

#### 2.GNN training

- Quality of data?

- GNN Conv
  - S2V
  - PNA

- Graph Structure Construction
  - Fully-connected layer???

- Heterogeneous or Homogeneous?