#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "Command.h"
#include "DynamicKomi.h"
#include "GoBoard.h"
#include "Gtp.h"
#include "Message.h"
#include "UctSearch.h"
#include "ZobristHash.h"

using namespace std;

const string command[COMMAND_MAX] = {
  "--playout",
  "--time",
  "--size",
  "--const-time",
  "--thread",
  "--komi",
  "--handicap",
  "--reuse-subtree",
  "--pondering",
  "--tree-size",
  "--no-debug",
  "--superko",
  "--sim-move",
  "--no-early-pass",
  "--no-nn",
  "--no-gpu",
  "--no-expand",
  "--device-id",
};

const string errmessage[COMMAND_MAX] = {
  "Set playouts",
  "Set all thinking time",
  "Set board size",
  "Set mode const time, and set thinking time per move",
  "Set threads",
  "Set komi",
  "Set the number of handicap stones (for testing)",
  "Reuse subtree",
  "Set pondering mode",
  "Set tree size (tree size must be 2 ^ n)",
  "Prohibit any debug message",
  "Prohibit superko move",
  "Play simulation move",
  "No early pass",
  "Don't use NN",
  "Don't use GPU",
  "No MCTS",
  "Set GPU to use",
};


//////////////////////
//  コマンドの処理  //
//////////////////////
void
AnalyzeCommand( int argc, char **argv )
{
  int n, size;
  
  for (int i = 1; i < argc; i++){
    n = COMMAND_MAX + 1;
    for (int j = 0; j < COMMAND_MAX; j++){
      if (!strcmp(argv[i], command[j].c_str())){
	n = j;
      }
    }

    switch (n) {
      case COMMAND_PLAYOUT:
	SetPlayout(atoi(argv[++i]));
	SetMode(CONST_PLAYOUT_MODE);
	break;
      case COMMAND_TIME:
	SetTime(atof(argv[++i]));
	SetMode(TIME_SETTING_MODE);
	break;
      case COMMAND_SIZE:
	i++;
	size = atoi(argv[i]);
	if (pure_board_size != size &&
	    size > 0 && size <= PURE_BOARD_SIZE) {
	  SetBoardSize(size);
	  SetParameter();
	}
	break;
      case COMMAND_CONST_TIME:
	SetMode(CONST_TIME_MODE);
	SetConstTime(atof(argv[++i]));
	break;
      case COMMAND_THREAD:
	SetThread(atoi(argv[++i]));
	break;
      case COMMAND_KOMI:
	SetKomi(atof(argv[++i]));
	break;
      case COMMAND_HANDICAP:
	SetConstHandicapNum(atoi(argv[++i]));
	SetHandicapNum(0);
	break;
      case COMMAND_REUSE_SUBTREE:
        SetReuseSubtree(true);
        break;
      case COMMAND_PONDERING :
	SetReuseSubtree(true);
	SetPonderingMode(true);
	break;
      case COMMAND_TREE_SIZE:
	SetHashSize((unsigned int)atoi(argv[++i]));
	break;
      case COMMAND_SUPERKO:
        SetSuperKo(true);
        break;
      case COMMAND_NO_DEBUG:
        SetDebugMessageMode(false);
        break;
      case COMMAND_SIM_MOVE:
	SetSimMove(true);
        break;
      case COMMAND_NO_EARLY_PASS:
	SetEarlyPass(false);
	break;
      case COMMAND_NO_NN:
	SetUseNN(false);
	break;
      case COMMAND_NO_GPU:
	SetDeviceId(-1);
	break;
      case COMMAND_NO_EXPAND:
        SetNoExpand(true);
        break;
      case COMMAND_DEVICE_ID:
        SetDeviceId(atoi(argv[++i]));
        break;
      default:
	for (int j = 0; j < COMMAND_MAX; j++){
	  fprintf(stderr, "%-22s : %s\n", command[j].c_str(), errmessage[j].c_str());
	}
	exit(1);
    }
  }

}
