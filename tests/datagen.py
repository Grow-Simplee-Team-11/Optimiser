import numpy as np
import random

def generate_latlon(func):
    NUM_BOXES = 30
    BIN_LEN = 100
    BIN_BRE = 80
    BIN_HEI = 4
    MEAN_LEN = 20
    MEAN_BRE = 20
    MEAN_HEI = 10
    longi = random.randint(77469960, 77703250)/(1e6)
    lat = random.randint(12873190, 13065670)/(1e6)  
    s=""
    s+=str(longi)
    s+=" "
    s+=str(lat)
    s+="\n"
    longit = func(longi,0.1 , size = NUM_BOXES)
    lati = func(lat, 0.1 , size = NUM_BOXES)
    lat=""
    longi=""
    s+=str(BIN_LEN)
    s+=" "
    s+=str(BIN_BRE)
    s+=" "
    s+=str(BIN_HEI)
    s+="\n"
    s+=str(NUM_BOXES)
    s+="\n"
    leng = func(MEAN_LEN, 7.5 , size = NUM_BOXES)
    bre = func(MEAN_BRE, 7.5 , size = NUM_BOXES)
    hei = func(MEAN_HEI, 3 , size = NUM_BOXES)
    for i in range(1,NUM_BOXES+1):
        longi = str(longit[i-1])
        lat = str(lati[i-1])
        s+=longi
        s+=" "
        s+=lat
        s+=" "
        s+=str(leng[i-1])
        s+=" "
        s+=str(bre[i-1])
        s+=" "
        s+=str(hei[i-1])
        s+="\n"

    return s

func = np.random.default_rng().normal
s = generate_latlon(func)
f = open("output.txt", "w")
f.write(s)


