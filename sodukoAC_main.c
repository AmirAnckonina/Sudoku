
//Welcome to soduko project.
//This program will run a classic soduko game for X players.
//Each game played "half by user" and "half by computer".
//try to insert numbers to cells in order to fill the board, with considering the following rules:
// *Every square has to contain a single number
// *Only the numbers from 1 through to 9 can be used
// *Each 3×3 box can only contain each number from 1 to 9 once
// *Each vertical column can only contain each number from 1 to 9 once
// *Each horizontal row can only contain each number from 1 to 9 once
//Enjoy!

#include <stdio.h>
#include "sodukoAC_final_h.h"

void main()
{
	short numOfPlayers, newPtrArrSize;
	ActivePlayersList activePlayersList;
	ActivePlayers_ListNode** ptrArr_ActivePlayers_LNode;
	ActivePlayers_TR activePlayersTree;
	WinnersList winnersLst;

    numOfPlayers = getNumOfPlayers();
	activePlayersList = createActivePlayersList(numOfPlayers);
	ptrArr_ActivePlayers_LNode = createAndSort_ptrArr_ActivePlayers_LNode(&activePlayersList, numOfPlayers);
	ptrArr_ActivePlayers_LNode = addNULLPlaces_ptrArrToActPlayersLNode(ptrArr_ActivePlayers_LNode, &numOfPlayers);
	activePlayersTree = BuildTreeFromArray(ptrArr_ActivePlayers_LNode, numOfPlayers);
	winnersLst = scanTreeInOrder_FullSession(&activePlayersTree);
	printWinnersList(&winnersLst);

	printf("\nThanks!\n");
}
