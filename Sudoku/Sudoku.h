#ifndef SODUKO_CH_H
#define SODUKO_CH_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define EMPTY_CELL -1
#define NOT_FINISH 0
#define FINISH_SUCCESS 1
#define FINISH_FAILURE -1
#define SUDOKO_SIZE 9
#define MAX_NAME_LEN 100


//Single square place in board
typedef struct _Cell
{
	short rowInd;
	short colInd;

} Cell;

//Each cell contain a short arr and the size of the short arr. 
//Will be set/intialized by possibleDigists fuc
typedef struct _Array
{
	short* arr;
	unsigned short size;
} Array;

//Contain playerName, sodukoBoard for each player, and matrix of pointers to Array (short arr, short arr size).
typedef struct _Player
{
	char playerName[MAX_NAME_LEN];
	short sodukuBoard[SUDOKO_SIZE][SUDOKO_SIZE];
	Array*** ptrToArrayMat;
}Player;

//Winners Linked List - including List type and ListNode type, the "data" is from Player type which defined above.
typedef struct _Winner_ListNode
{
	Player winnerPlayer;
	struct _Winner_ListNode* next;
	struct _Winner_ListNode* prev;
} Winner_ListNode;

typedef struct _WinnersList
{
	struct _Winner_ListNode* head;
	struct _Winner_ListNode* tail;
} WinnersList;

//Active players Linked List - Including List type and ListNode type. the "data" is from Player type
typedef struct _ActivePlayer_ListNode
{
	Player activePlayer;
	struct _ActivePlayer_ListNode* next;
	struct _ActivePlayer_ListNode* prev;
} ActivePlayers_ListNode;

typedef struct _ActivePlayer_List
{
	struct _ActivePlayer_ListNode* head;
	struct _ActivePlayer_ListNode* tail;
} ActivePlayersList;

//Active players Tree, each node contain a ptr to an Array.
typedef struct _ActivePlayers_TNODE
{
	ActivePlayers_ListNode* single_ptrToActPlayer_LNode; //Data!
	struct _ActivePlayers_TNODE* left;
	struct _ActivePlayers_TNODE* right;

} ActivePlayers_TNODE;

typedef struct _ActivePlayers_TR
{
	ActivePlayers_TNODE* root;

} ActivePlayers_TR;

//A linked list of Cell variable.
typedef struct _VacantCell_ListNode
{
	Cell cell;
	struct _VacantCell_ListNode* next;
	struct _VacantCell_ListNode* prev;

} VacantCell_ListNode;

typedef struct _vacantCellList
{
	VacantCell_ListNode* head;
	VacantCell_ListNode* tail;

} VacantCellList;


//FUNCTIONS
short getNumOfPlayers();
void sudokoPrintBoard(short board[][SUDOKO_SIZE]);

//implementing game session functions
int OneStage(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell);
int FillBoard(short board[][SUDOKO_SIZE], Array*** possibilities);

//OneStage and fillBoard functions
void scanForOptimalCell(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, int* numOfPossibilities);
void singlePlacementProcedure(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, short insertedValue, bool* dupFlag);
void freeSingleArrayCell(Array*** possibilities, Cell* currCell);
void postMove_UpdatePossibilities(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, short insertedValue, bool* dupFlag);
void updateRowPossibilities(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, short insertedValue, bool* dupFlag);
void updateColPossibilities(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, short insertedValue, bool* dupFlag);
void updateSquarePossibilities(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, short insertedValue, bool* dupFlag);
short findStartingRow_inCurrSquare(short rowInd);
short findStartingCol_inCurrSquare(short colInd);
bool checkDuplication(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, short currRowInd, short currColInd, short insertedValue, bool* cellIsNULL);
void updateArrayCell(Array* currArraypossibilities, short insertedValue);
bool isValidInput(short insertedValue, Array* ArrayCell);
short getValueToCurrCell(Array*** possibilities, Cell* currCell);
void printCurrCell_PossibleValues(Array*** possibilities, Cell* currCell);

//Random board functions
void setInitRandomBoard(short sudokoBoard[][SUDOKO_SIZE], Array**** possibilities, bool* filledCorrectly);
short generateRandNumOfCells();
short generateRandValueToFill(Array*** possibilities, Cell* currCell);
short generateRand_KCell(short currVacantCellLst_Size);
void setInitBoardPreparations(VacantCellList* vacantCellLst, short board[][SUDOKO_SIZE], Array*** possibilities);
void setVacantBoard(short board[][SUDOKO_SIZE]);
void setInitPossibilities(Array*** possibilities);
void setInitVacantCellList(VacantCellList* vacantCellLst);
void fillAllPossiblitiesForCell(short* singleCellShortArr);
Array*** allocatePossiblitiesMatrix();
Array* fullAllocSingleArrayCell();
VacantCell_ListNode* extractVacantCellNode(VacantCellList* vacantCellLst, short kIndInList);


//ActivePlayers tree help functions
void makeEmpty_ActivePlayersTree(ActivePlayers_TR* ActivePlayersTree);
ActivePlayers_TR BuildTreeFromArray(ActivePlayers_ListNode** ptrArr_ActPlayers_LNode, short ptrArrSize);
ActivePlayers_TNODE* BuildTreeFromArrayHelper(ActivePlayers_ListNode** ptrArr_ActPlayers_LNode, short ptrArrSize);
ActivePlayers_TNODE* createNew_ActivePlayer_TNode(ActivePlayers_ListNode* ptr_ActPlayer_LNode, ActivePlayers_TNODE* left, ActivePlayers_TNODE* right);
WinnersList scanTreeInOrder_FullSession(ActivePlayers_TR* activePlayersTree);
void scanTreeInOrder_FullSession_Helper(ActivePlayers_TNODE* currRoot, WinnersList* newWinnersLst);


//ptrArr to active players listNode help functions
ActivePlayers_ListNode** createAndSort_ptrArr_ActivePlayers_LNode(ActivePlayersList* ptrArr_ActPlayers_LNode, short numOfPlayers);
void Active_Players_ptrArr_mergeSort(ActivePlayers_ListNode** ptrArrActivePlayers, short ptrArrsize);
ActivePlayers_ListNode** merge_Active_Players_ptrArr(ActivePlayers_ListNode** ptrArrActivePlayers1, short size1, ActivePlayers_ListNode** ptrArrActivePlayers2, short size2);
void copyArr(ActivePlayers_ListNode** dest, ActivePlayers_ListNode** src, short size);
ActivePlayers_ListNode** addNULLPlaces_ptrArrToActPlayersLNode(ActivePlayers_ListNode** ptrArr_ActivePlayers_LNode, short* ptrArrsize);
short calcNewPtrArrSize(short ptrArrsize);
short numOfFilledCells(short board[][SUDOKO_SIZE]);


//activePlayers list help functions
ActivePlayersList createActivePlayersList(short sizeOfList);
Player createNewPlayer();
void makeEmpty_ActivePlayersList(ActivePlayersList* activePlayersLst);
bool isEmpty_ActivePlayersList(ActivePlayersList* activePlayersLst);
ActivePlayers_ListNode* createNew_ActivePlayer_ListNode(Player player, ActivePlayers_ListNode* next, ActivePlayers_ListNode* prev);
void insertDataToEnd_ActivePlayersList(ActivePlayersList* newActivePlayersList, Player newPlayer);
void insertNodeToEnd_ActivePlayersList(ActivePlayersList* newActivePlayersList, ActivePlayers_ListNode* tail);
void removeActivePlayerListNode(ActivePlayers_ListNode* currNode);
void free_ptrToArrayMat(Array*** ptrToArrayMat);

//VacantList help functions
void insertDataToEnd_VacantCellList(VacantCellList* vacantCellLst, Cell cell);
VacantCell_ListNode* createNew_VacantCellNode(Cell cell, VacantCell_ListNode* next, VacantCell_ListNode* prev);
void insertNodeToEnd_VacantCellList(VacantCellList* vacantCellLst, VacantCell_ListNode* tail);
bool isEmpty_VacantCellList(VacantCellList* vacantCellLst);
void makeEmpty_VacantCellList(VacantCellList* vacantCellLst);
void freeVacantCellList(VacantCellList* vacantCellLst);
void removeVacantCellNode(VacantCellList* vacantCellLst, VacantCell_ListNode* currCellNode);

//Winners list help functions
void makeEmpty_WinnersList(WinnersList* winnersLst);
bool isEmpty_WinnersList(WinnersList* winnersLst);
Winner_ListNode* createNew_WinnerListNode(Player newWinnerPlayer, Winner_ListNode* next, Winner_ListNode* prev);
void insertDataToEnd_WinnersList(WinnersList* newWinnersLst, Player newWinnerPlayer);
void insertNodeToEnd_WinnersList(WinnersList* newWinnersLst, Winner_ListNode* tail);
void freeWinnersList(WinnersList* winnersLst);
void printWinnersList(WinnersList* winnersLst);

//Check Allocation functions
void checkAllocation(void* ptr);

//#include "Sudoku.c" //For VS code envrinment

#endif 