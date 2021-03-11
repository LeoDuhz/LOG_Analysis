# dataset for robocup ssl game 

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
    def __init__(self, number, x, y, orientation, velx, vely, w):
        super().__init__()
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
        self.x = x
        self.y = y
        self.velx = velx
        self.vely = vely
    
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


# test file I/O
NORMAL_START = 3
FORCE_START = 4
f = open('./dhzDATA', 'r')
allData = []


line = f.readline()
dat = line.split(' ')

cnt = 0
while True:  
    # dat = line.split(' ')
    cnt += 1
    if(cnt % 20 == 0):
        print('reading effective data', cnt)

    if not line:
        break

    if(dat[0] == 'time:'):
        time = dat[1]
        line = f.readline()
        dat = line.split(' ')
        if(dat[0] == 'referee:'):
            referee = dat[1]
            if(dat[1] == NORMAL_START or dat[1] == FORCE_START):
                gameData = GameData()
                gameData.time = time
                while True:
                    line = f.readline()
                    dat = line.split(' ')
                    if(dat[0] != 'team:' and dat[0] != 'ballpos:'):
                        allData.append(gameData)
                        break
                    elif(dat[0] == 'team:'):
                        if(dat[1] == 0): #process blue player data
                            if(dat[2] == 'num:'): 
                                number = dat[3]
                            if(dat[4] == 'position:'):
                                x = dat[5]
                                y = dat[6]
                                orientation = dat[7]
                            if(dat[8] == 'velocity:'):
                                velx = dat[9]
                                vely = dat[10]
                                w = dat[11]
                            bluePlayer = Player(x, y, orientation, velx, vely, w)
                            gameData.blueData.append(bluePlayer)
                        elif(dat[1] == 1):
                            if(dat[2] == 'num:'): 
                                number = dat[3]
                            if(dat[4] == 'position:'):
                                x = dat[5]
                                y = dat[6]
                                orientation = dat[7]
                            if(dat[8] == 'velocity:'):
                                velx = dat[9]
                                vely = dat[10]
                                w = dat[11]
                            yellowPlayer = Player(x, y, orientation, velx, vely, w)
                            gameData.yellowData.append(yellowPlayer)
                    elif(dat[0] == 'ballpos:'):
                        x = dat[1]
                        y = dat[2]
                        velx = dat[4]
                        vely = dat[5]
                        ball = gameData.ballData.reset(x, y, velx, vely)
            else:
                while True:
                    line = f.readline()
                    dat = line.split(' ')
                    if(dat[0] == 'time:' or not line):
                        break
                
                
print('reading end')
f.close()

                            
                            
                            

