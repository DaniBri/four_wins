#ifndef GLOBALDEFINER_H
#define GLOBALDEFINER_H

//Board Settings
#define NBR_HOLE_HOR 5
#define NBR_HOLE_VER 5

//AI Settings
#define TRAINING 1
#define NN_LAYER_NBR 7              // At least 1 to compress input down before going to output
#define NN_LAYER_NODES 50


#define NN_BIASES_RANGE 2
#define NN_WEIGHTS_RANGE 2
#define NN_SIZE_OF_SAVE NN_LAYER_NBR*(NN_LAYER_NODES+1)+(NBR_HOLE_HOR+1) // How many lines the save file should have

#define MOVES_TIMEOUT (NBR_HOLE_HOR*NBR_HOLE_VER)+1
#define AI_WINNER_NAME "winnerAI"
#define AI_OLD_WINNER_NAME "oldWinnerAI"

//Pool Settings
#define NBR_TOURN 5000
#define POOL_RAND_AI 2      // Randomized children
#define POOL_CHILD_AI 2     // Children from two winners
#define POOL_MUTANT_AI 2    // Tweaked children
#define POOL_SIZE (1+POOL_MUTANT_AI+POOL_CHILD_AI+POOL_RAND_AI) // 1 model and others
#define POOL_PER_TOURNAMENT POOL_SIZE

// HERITAGE Settings
#define HERITAGE_RATE 3     // rand()%rate == 1 => rate

// Because AI don't act in any way random this number doesn't impact but keep it small to do less matches
#define MATCHES_PER_AI_IN_POOL 1 // Will be actually multiplied by 2 cause once AI 1 plays player 1 and once ai2 plays player 2

#endif // GLOBALDEFINER_H
