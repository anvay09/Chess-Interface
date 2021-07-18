import subprocess, time

engine = subprocess.Popen(
    'stockfish',
    universal_newlines=True,
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    bufsize=1,
)

def put(command):
    print('\nyou:\n\t'+command)
    engine.stdin.write(command+'\n')
    
def get():
    # using the 'isready' command (engine has to answer 'readyok')
    # to indicate current last line of stdout
    output = [] #list to store lines of output

    engine.stdin.write('isready\n')
    print('\nengine:')
    while True:
        text = engine.stdout.readline().strip()
        if text == 'readyok':
            break
        if text !='':
            output += [text]
            print('\t' + text)
    
    return output

def setFENpostion(FENpos): # set a position given a FEN
    put('position fen ' + FENpos)
    get()

def setNumVariations(n): # number of lines to evaluate
    put('setoption name multipv value ' + str(n))
    get()

def goDepth(numDepth, numsecondswait): # go to depth numDepth and give continuation best lines
    put('go depth ' + str(numDepth))
    time.sleep(numsecondswait)
    return get()

def stop():
    put('stop')
    get()

def quit():
    put('quit')

def parseOutputLine(engineOutputLine):
    words = engineOutputLine.split()
    # print(words)
    return words

def getNBestMoves(N, numDepth, numSecondsWait, FENpos): # outputs N best moves in a position
    setFENpostion(FENpos)
    setNumVariations(N)
    output = goDepth(numDepth,numSecondsWait)
    bestMoves = []

    for i in range(2,N+2):
        bestMoves = [parseOutputLine(output[-i])[19]] + bestMoves # the best move is the 19th word in the output line
        # example line : "info depth 10 seldepth 16 multipv 3 
        # score cp 0 nodes 138097 nps 1266944 tbhits 0 time 109 pv 
        # g1e2 c5d4 b5c6 b7c6 c3d4 d8a5 e3d2 a5a6 d2g5 a6a5"

    print('Best moves in this position are : ')
    print(bestMoves)
    return bestMoves

def displayBoard():
    put('d')
    get()

def getEngineEval(): # get engine evaluation for a position
    put('eval')
    output = get()
    return output[-1].split()[2]

def makeMove(move, FENpos):
    put('position fen ' + FENpos + ' moves ' + move)
    get()
    displayBoard()

def getCentipawnLossByMove(move, FENpos): # get centipawn loss for a move made in a position
    setFENpostion(FENpos)
    eval = float(getEngineEval())
    makeMove(move, FENpos)
    newEval = float(getEngineEval())
    return (eval - newEval)*100

get()
put('uci')
get()
FENposition = 'r1bqk2r/pp2ppbp/2n3p1/1Bp5/3PP3/2P1B3/P4PPP/R2QK1NR w KQkq - 2 9'
getNBestMoves(3, 10, 5, FENposition) # top 3 moves at depth 10 in 5 seconds
CPL = getCentipawnLossByMove("a2a4", FENposition)
print(CPL)

quit()
