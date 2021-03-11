# dataset for robocup ssl game
import numpy as np
import torch
from torch_geometric.data import Data
from torch_geometric.data import InMemoryDataset
from torch_geometric.data import DataLoader

# class BluePlayer:
#     def __init__(self, number, x, y, orientation, velx, vely, w):
#         super().__init__()
#         self.number = number
#         self.x = x
#         self.y = y
#         self.orientation = orientation
#         self.velx = velx
#         self.vely = vely
#         self.w = w

# class YellowPlayer:
#     def __init__(self, number, x, y, orientation, velx, vely, w):
#         super().__init__()
#         self.number = number
#         self.x = x
#         self.y = y
#         self.orientation = orientation
#         self.velx = velx
#         self.vely = vely
#         self.w = w


class Player:
    def __init__(self, team, number, x, y, orientation, velx, vely, w):
        super().__init__()
        self.team = team
        self.number = number
        self.x = x
        self.y = y
        self.orientation = orientation
        self.velx = velx
        self.vely = vely
        self.w = w

class BallData:
    def __init__(self):
        super().__init__()
        self.x = 0
        self.y = 0
        self.velx = 0
        self.vely = 0
    
    def reset(self, x, y, velx, vely):
        self.x = x
        self.y = y
        self.velx = velx
        self.vely = vely

class GameData:
    def __init__(self):
        super().__init__()
        self.time = 0
        self.blueData = []
        self.yellowData = []
        self.ballData = BallData()

def convertPlayData2List(playerData):
    data_list = []
    for data in playerData:
        temp = [data.x, data.y, data.orientation, data.velx, data.vely, data.w]
        data_list.append(temp)
    
    return data_list

def convertBallData2List(ballData):
    data_list = []
    temp = [ballData.x, ballData.y, ballData.velx, ballData.vely]
    data_list.append(temp)

    return data_list

def generateFullyConnetedEdge(num):
    edge_index = []
    for i in range(num):
        for j in range(num):
            if(i != j):
                temp_list = [i,j]
                edge_index.append(temp_list)
    
    return edge_index

def gengerateY(playerData):
    data_list = []
    for data in playerData:
        temp_list = [data.orientation]
        data_list.append(temp_list)

    return data_list

#File preprocessing
NORMAL_START = 0
FORCE_START = 0
f = open('./dhzDATA', 'r')
allData = []


line = f.readline()
dat = line.split(' ')

cnt = 0
while True:  
    # dat = line.split(' ')
    cnt += 1
    # if(cnt % 20 == 0):
    #     print('reading effective data', cnt)
    # print('length of all data',len(allData))
    if not line:
        break

    if(dat[0] == 'time:'):
        time = dat[1]
        line = f.readline()
        dat = line.split(' ')
        if(dat[0] == 'referee:'):
            referee = int(dat[1])
            if(referee == 0):
                gameData = GameData()
                gameData.time = float(time)
                while True:
                    line = f.readline()
                    dat = line.split(' ')
                    if(dat[0] != 'team:' and dat[0] != 'ballpos:'):
                        #update all data
                        allData.append(gameData)
                        break
                    elif(dat[0] == 'team:'):
                        team = int(dat[1])
                        #process blue player data
                        if(team == 0): 
                            if(dat[2] == 'num:'): 
                                number = int(dat[3])
                            if(dat[4] == 'position:'):
                                x = float(dat[5])
                                y = float(dat[6])
                                orientation = float(dat[7])
                            if(dat[8] == 'velocity:'):
                                velx = float(dat[9])
                                vely = float(dat[10])
                                w = float(dat[11])
                            bluePlayer = Player(team, number, x, y, orientation, velx, vely, w)
                            gameData.blueData.append(bluePlayer)
                        #process yellow player data
                        elif(team == 1):
                            if(dat[2] == 'num:'): 
                                number = int(dat[3])
                            if(dat[4] == 'position:'):
                                x = float(dat[5])
                                y = float(dat[6])
                                orientation = float(dat[7])
                            if(dat[8] == 'velocity:'):
                                velx = float(dat[9])
                                vely = float(dat[10])
                                w = float(dat[11])
                            yellowPlayer = Player(team, number, x, y, orientation, velx, vely, w)
                            gameData.yellowData.append(yellowPlayer)
                    #process ball data
                    elif(dat[0] == 'ballpos:'):
                        x = float(dat[1])
                        y = float(dat[2])
                        velx = float(dat[4])
                        vely = float(dat[5])
                        ball = gameData.ballData.reset(x, y, velx, vely)
            else:
                while True:
                    line = f.readline()
                    dat = line.split(' ')
                    if(dat[0] == 'time:' or not line):
                        break
                
                
print('reading end')
f.close()

class MyOwnDataset(InMemoryDataset):
    def __init__(self, root, transform=None, pre_transform=None):
        super(MyOwnDataset, self).__init__(root, transform, pre_transform)
        # self.data, self.slices = torch.load(self.processed_paths[0])
        self.data, self.slices = None, None
    
    @property
    def raw_file_names(self):
        return []
    
    @property
    def processed_file_names(self):
        return ['./data.pt']
    
    def download(self):
        pass

    def process(self):
        #process allData into geometric data format
        data_list = []
        for raw_data in allData:
            blue_num = len(raw_data.blueData)
            yellow_num = len(raw_data.yellowData)
            
            x = torch.tensor(convertPlayData2List(raw_data.blueData), dtype=torch.float)

            y = torch.tensor(gengerateY(raw_data.blueData), dtype=torch.float)

            edge_index = torch.tensor(generateFullyConnetedEdge(blue_num), dtype=torch.long)
            edge_index = np.transpose(edge_index)
            data = Data(x=x, y=y, edge_index=edge_index.contiguous())

            data_list.append(data)

        if self.pre_filter is not None:
            data_list = [data for data in data_list if self.pre_filter(data)]
        
        if self.pre_transform is not None:
            data_list = [self.pre_transform(data) for data in data_list]

        
        data, slices = self.collate(data_list)
        torch.save((data, slices), self.processed_paths[0])

dataset = MyOwnDataset(root='~/dhz/dataset')

loader = DataLoader(dataset, batch_size=32, shuffle=False)
