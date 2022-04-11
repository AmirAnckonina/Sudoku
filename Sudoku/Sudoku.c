
#include "sudoku.h"

short getNumOfPlayers()
{
	short numOfPlayers;
	char containerChar;

	printf("Please enter the number of players:\n");
	scanf("%hi", &numOfPlayers);
	containerChar = getchar();

	return numOfPlayers;
}

//The following function responsible to fill the board automatically for cells which the number of possible values is one.
//Fill board will call the following function.
int OneStage(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell)
{
	short insertedValue;
	bool dupFlag = false;
	int finishedFlag, minPossibilitiesSize;

	scanForOptimalCell(board, possibilities, currCell, &minPossibilitiesSize);

	//As long there is a cell with only one possility, and there's no duplication, continue to fill the board with vales.
	while (minPossibilitiesSize == 1 && dupFlag == false)
	{
		board[currCell->rowInd][currCell->colInd] = insertedValue = possibilities[currCell->rowInd][currCell->colInd]->arr[0];
		//Will free the cell and update possibilitis matrix. dup flag will be updated if duplication has found.
		singlePlacementProcedure(board, possibilities, currCell, insertedValue, &dupFlag);
		scanForOptimalCell(board, possibilities, currCell, &minPossibilitiesSize);
	}

	//In case duplication found.
	if (dupFlag == true)
		finishedFlag = FINISH_FAILURE;

	else //otherwise, the player has to choose a value for currCell or if there no options left, the game will ended successfully.
	{
		if (minPossibilitiesSize > 1)
			finishedFlag = NOT_FINISH;
		else //minPossibilitiesSize == 0
			finishedFlag = FINISH_SUCCESS;
	}

	return finishedFlag;
}

//This function will run a game session.
//The user will be asked to insert number in specific currCell in the board.
//after player insertion, the board will be filled automatically in cells that has only one possible value,
// by calling oneStage function, Until the player will have to choose a value again.  
int FillBoard(short board[][9], Array*** possibilities)
{
	int finishedFlag = NOT_FINISH;
	Cell currCell;
	short insertedValue;
	bool dupFlag = false;

	//One iteration outside, to update cells with one possibily only.
	finishedFlag = OneStage(board, possibilities, &currCell);

	//As long as the game hasn't finished yet, ask the player to insert values
	while (finishedFlag == NOT_FINISH)
	{
		sudokoPrintBoard(board);

		//ask for a value in current currCell.
		insertedValue = getValueToCurrCell(possibilities, &currCell);
		board[currCell.rowInd][currCell.colInd] = insertedValue;

		//Update possibilities matrix in the relevant row, col & square.
		singlePlacementProcedure(board, possibilities, &currCell, insertedValue, &dupFlag);

		//Fill all cells with one possibilty.
		finishedFlag = OneStage(board, possibilities, &currCell);
	}

	if (finishedFlag == FINISH_SUCCESS)
	{
		sudokoPrintBoard(board);
		return FINISH_SUCCESS;
	}
	else //finished = FINISH_FAILURE
		return FINISH_FAILURE;
}

//oneStage && FillBoard
//serach for a cell with only one possibleValue, and return the currCell as an inputParam
void scanForOptimalCell(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, int* numOfPossibilities)
{
	short rowInd, colInd, minPossArrSize = 10;
	for (rowInd = 0; rowInd < SUDOKO_SIZE; rowInd++)
	{
		for (colInd = 0; colInd < SUDOKO_SIZE; colInd++)
		{
			if (possibilities[rowInd][colInd] == NULL)
				continue;
			//In case there's a cell with one possibility, i want to return him.
			else if (((possibilities[rowInd][colInd])->size) == 1)
			{
				currCell->rowInd = rowInd;
				currCell->colInd = colInd;
				*numOfPossibilities = 1;
				return;
			}
			//Save the currCell of the cell with the minimum options, except 1 possibilty, which handled above.
			else if ((possibilities[rowInd][colInd])->size < minPossArrSize)
			{
				minPossArrSize = ((possibilities[rowInd][colInd])->size);
				currCell->rowInd = rowInd;
				currCell->colInd = colInd;
			}
		}
	}

	//minPossArrSize is also a "flag" for a case there's no option to fill.
	//So in this case there's no possibilities, so the minimal should be 0.
	if (minPossArrSize == 10)
		*numOfPossibilities = 0;
	else
		*numOfPossibilities = minPossArrSize;

	return;
}

//The following function will be called after inserting value, and responsible to clear the "Array"
//and also, the function will run postMove procedure.
void singlePlacementProcedure(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, short insertedValue, bool* dupFlag)
{
	freeSingleArrayCell(possibilities, currCell); //Also update the cell to point NULL.
	postMove_UpdatePossibilities(board, possibilities, currCell, insertedValue, dupFlag);
}

/*updates the possible values of each cell in the relevant row, column, and square, which are those of the cell we just inserted a value.*/
void postMove_UpdatePossibilities(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, short insertedValue, bool* dupFlag)
{
	updateRowPossibilities(board, possibilities, currCell, insertedValue, dupFlag);
	updateColPossibilities(board, possibilities, currCell, insertedValue, dupFlag);
	updateSquarePossibilities(board, possibilities, currCell, insertedValue, dupFlag);
}

/*runs over the row and updates the possibilities array in addition to the new num on board*/
void updateRowPossibilities(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, short insertedValue, bool* dupFlag)
{
	short currColInd = 0;
	bool resDup = false, cellIsNULL;

	/*runs and update the possibilities short arr in a row.*/
	for (currColInd = 0; currColInd < SUDOKO_SIZE; currColInd++)
	{
		resDup = checkDuplication(board, possibilities, currCell, currCell->rowInd, currColInd, insertedValue, &cellIsNULL);

		if (resDup == true)
		{
			*dupFlag = resDup;
			return;
		}

		if (cellIsNULL == false) //updates the currCell in addition to the optional values
			updateArrayCell(possibilities[currCell->rowInd][currColInd], insertedValue);

	}
}

/*runs over the col and updates the possibilities array in addition to the new num on board*/
void updateColPossibilities(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, short insertedValue, bool* dupFlag)
{
	short currRowInd = 0;
	bool resDup = false, cellIsNULL;

	/*runs and update the possibilities short arr in a row.*/
	for (currRowInd = 0; currRowInd < SUDOKO_SIZE; currRowInd++)
	{
		resDup = checkDuplication(board, possibilities, currCell, currRowInd, currCell->colInd, insertedValue, &cellIsNULL);
		if (resDup == true)
		{
			*dupFlag = resDup;
			return;
		}

		if (cellIsNULL == false) //updates the currCell in addition to the optional values
			updateArrayCell(possibilities[currRowInd][currCell->colInd], insertedValue);

	}
}

/*checks in which square of 3X3 the curr cell is and in addition to the spot, checks all of the cells in this square*/
void updateSquarePossibilities(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, short insertedValue, bool* dupFlag)
{
	short currRowInd, currColInd, startSqrRowInd, startSqrColInd;
	bool resDup = false, cellIsNULL;

	//updates the starting row and col indexes to scan the sub-square efficiently.
	startSqrRowInd = findStartingRow_inCurrSquare(currCell->rowInd);
	startSqrColInd = findStartingCol_inCurrSquare(currCell->colInd);

	/*runs and update the possibilities short arr in the square.*/
	for (currRowInd = startSqrRowInd; currRowInd < startSqrRowInd + 3; currRowInd++)
	{
		for (currColInd = startSqrColInd; currColInd < startSqrColInd + 3; currColInd++)
		{
			resDup = checkDuplication(board, possibilities, currCell, currRowInd, currColInd, insertedValue, &cellIsNULL);

			if (resDup == true)
			{
				*dupFlag = resDup;
				return;
			}

			if (cellIsNULL == false) //updates the currCell in addition to the optional values
				updateArrayCell(possibilities[currRowInd][currColInd], insertedValue);
		}
	}
}

/*finds the starting rowInd ind of the squre which the param rowInd is in.*/
short findStartingRow_inCurrSquare(short rowInd)
{
	short firstrowInd;
	if (rowInd >= 0 && rowInd <= 2)
	{
		firstrowInd = 0;
	}
	else if (rowInd >= 3 && rowInd <= 5)
	{
		firstrowInd = 3;
	}
	else if (rowInd >= 6 && rowInd <= 8)
	{
		firstrowInd = 6;
	}
	return firstrowInd;

}

/*finds the starting colInd ind of the squre which the param rowInd is in.*/
short findStartingCol_inCurrSquare(short colInd)
{
	short firstcolInd;
	if (colInd >= 0 && colInd <= 2)
	{
		firstcolInd = 0;
	}
	else if (colInd >= 3 && colInd <= 5)
	{
		firstcolInd = 3;
	}
	else if (colInd >= 6 && colInd <= 8)
	{
		firstcolInd = 6;
	}
	return firstcolInd;
}

//The following function will be called by sub-functions of postMove, and will check for each cell if a duplication might occur.
//also, the function will give an indication if possibilities in a each ind is null or not (input param),
// so we could know if any update is necessary in each cell
bool checkDuplication(short board[][SUDOKO_SIZE], Array*** possibilities, Cell* currCell, short currRowInd, short currColInd, short insertedValue, bool* cellIsNULL)
{
	if (possibilities[currRowInd][currColInd] == NULL)
	{
		*cellIsNULL = true;
		if ((board[currRowInd][currColInd] == insertedValue) && (currCell->rowInd != currRowInd || currCell->colInd != currColInd))
			return true;
	}
	else //possibilities[currCell->rowInd][currColInd] != NULL
	{
		*cellIsNULL = false;
		if (possibilities[currRowInd][currColInd]->size == 1 && possibilities[currRowInd][currColInd]->arr[0] == insertedValue)
			return true;
	}

	//Non of the case recoginze a dupliction.
	return false;
}

/*updates the curr Array node in addition to the possible values after the insert of the new value in the board*/
void updateArrayCell(Array* currArraypossibilities, short insertedValue)
{
	short readInd = 0;
	short writeInd = 0;

	while (readInd < currArraypossibilities->size)
	{
		//if one of the curr possibilities is equal to the inserted number
		if (currArraypossibilities->arr[readInd] == insertedValue)
			readInd++;
		else
		{
			currArraypossibilities->arr[writeInd] = currArraypossibilities->arr[readInd];//updates the short type arr.
			readInd++;
			writeInd++;
		}
	}
	if (writeInd < readInd)//update the size of the short type arr.
	{
		currArraypossibilities->arr = (short*)realloc(currArraypossibilities->arr, writeInd * sizeof(short));
		checkAllocation(currArraypossibilities->arr);
		currArraypossibilities->size = writeInd;
	}
}

// checks if the inserted value from the player is valid
bool isValidInput(short insertedValue, Array* ArrayCell)
{
	short i;
	for (i = 0; i < ArrayCell->size; i++)
	{
		if (insertedValue == ArrayCell->arr[i])
			return true;
	}
	return false;
}

//get valid value to currCell.
short getValueToCurrCell(Array*** possibilities, Cell* currCell)
{
	short insertedValue;
	bool validInput = false;

	while (validInput == false)
	{
		printCurrCell_PossibleValues(possibilities, currCell);
		scanf("%hi", &insertedValue);
		validInput = isValidInput(insertedValue, possibilities[currCell->rowInd][currCell->colInd]);

		if (validInput == false)
			printf("Invalid Input, choose again!!!\n");
	}

	printf("Updating cell %hi with value %hi\n\n", (currCell->rowInd) * 9 + (currCell->colInd), insertedValue);
	return insertedValue;
}

//printing relevant currCell to play.
void printCurrCell_PossibleValues(Array*** possibilities, Cell* currCell)
{
	short ind;
	unsigned short numOfPossibilities = possibilities[currCell->rowInd][currCell->colInd]->size;

	printf("Cell number [%hi,%hi] currently holds the minimum number of possible values, select one of the below:\n", currCell->rowInd, currCell->colInd);
	for (ind = 0; ind < numOfPossibilities; ind++)
		printf("%hi. %hi\n", ind + 1, possibilities[currCell->rowInd][currCell->colInd]->arr[ind]);

}

//Printing sudoki board.
void sudokoPrintBoard(short board[][SUDOKO_SIZE])
{
	short rowInd, colInd;

	printf("\n");
	printf("  |");
	for (colInd = 0; colInd < 3; colInd++)
		printf(" %hi", colInd);
	printf("|");
	for (colInd = 3; colInd < 6; colInd++)
		printf(" %hi", colInd);
	printf("|");
	for (colInd = 6; colInd < 9; colInd++)
		printf(" %hi", colInd);

	printf("\n");

	for (colInd = 0; colInd < SUDOKO_SIZE - 1; colInd++)
		printf("---");
	printf("\n");

	for (rowInd = 0; rowInd < 3; rowInd++)
	{
		printf("%hi |", rowInd);
		for (colInd = 0; colInd < 3; colInd++)
		{
			if ((board[rowInd][colInd]) == EMPTY_CELL)
			{
				printf("  ");
			}
			else
			{
				printf(" %hi", board[rowInd][colInd]);
			}
		}
		printf("|");
		for (colInd = 3; colInd < 6; colInd++)
		{
			if ((board[rowInd][colInd]) == EMPTY_CELL)
			{
				printf("  ");
			}
			else
			{
				printf(" %hi", board[rowInd][colInd]);
			}
		}
		printf("|");
		for (colInd = 6; colInd < 9; colInd++)
		{
			if ((board[rowInd][colInd]) == EMPTY_CELL)
			{
				printf("  ");
			}
			else
			{
				printf(" %hi", board[rowInd][colInd]);
			}
		}

		printf("\n");
	}

	for (colInd = 0; colInd < SUDOKO_SIZE - 1; colInd++)
		printf("---");
	printf("\n");

	for (rowInd = 3; rowInd < 6; rowInd++)
	{
		printf("%hi |", rowInd);
		for (colInd = 0; colInd < 3; colInd++)
		{
			if ((board[rowInd][colInd]) == EMPTY_CELL)
			{
				printf("  ");
			}
			else
			{
				printf(" %hi", board[rowInd][colInd]);
			}
		}
		printf("|");
		for (colInd = 3; colInd < 6; colInd++)
		{
			if ((board[rowInd][colInd]) == EMPTY_CELL)
			{
				printf("  ");
			}
			else
			{
				printf(" %hi", board[rowInd][colInd]);
			}
		}
		printf("|");
		for (colInd = 6; colInd < 9; colInd++)
		{
			if ((board[rowInd][colInd]) == EMPTY_CELL)
			{
				printf("  ");
			}
			else
			{
				printf(" %hi", board[rowInd][colInd]);
			}
		}

		printf("\n");
	}

	for (colInd = 0; colInd < SUDOKO_SIZE - 1; colInd++)
		printf("---");
	printf("\n");

	for (rowInd = 6; rowInd < 9; rowInd++)
	{
		printf("%hi |", rowInd);
		for (colInd = 0; colInd < 3; colInd++)
		{
			if ((board[rowInd][colInd]) == EMPTY_CELL)
			{
				printf("  ");
			}
			else
			{
				printf(" %hi", board[rowInd][colInd]);
			}
		}
		printf("|");
		for (colInd = 3; colInd < 6; colInd++)
		{
			if ((board[rowInd][colInd]) == EMPTY_CELL)
			{
				printf("  ");
			}
			else
			{
				printf(" %hi", board[rowInd][colInd]);
			}
		}
		printf("|");
		for (colInd = 6; colInd < 9; colInd++)
		{
			if ((board[rowInd][colInd]) == EMPTY_CELL)
			{
				printf("  ");
			}
			else
			{
				printf(" %hi", board[rowInd][colInd]);
			}
		}

		printf("\n");
	}

	printf("\n");
	printf("\n");
}

//free a single Array and set it to point NULL
void freeSingleArrayCell(Array*** possibilities, Cell* currCell)
{
	free(possibilities[currCell->rowInd][currCell->colInd]->arr);
	possibilities[currCell->rowInd][currCell->colInd] = NULL;
}


//Create starting random board functions
//Will set random board and return it to the player
void setInitRandomBoard(short sudokoBoard[][SUDOKO_SIZE], Array**** possibilities, bool* filledCorrectly)
{
	VacantCellList vacantCellLst;
	VacantCell_ListNode* currCellNode;
	Array*** resPossibilities;
	Cell* currCell;
	bool dupFlag = false;
	short i, N_cellsToFill, kIndInList, newValue, currVacantCellLst_Size = SUDOKO_SIZE * SUDOKO_SIZE;

	*filledCorrectly = true;

	//full allocating to possibilities matrix.
	resPossibilities = allocatePossiblitiesMatrix();

	//Initialize soudoBoard to be valid and empty, and also fill all possibilities in each Array in possibilities matrix.
	setInitBoardPreparations(&vacantCellLst, sudokoBoard, resPossibilities);
	N_cellsToFill = generateRandNumOfCells();

	for (i = 0; i < N_cellsToFill; i++)
	{
		//Get random cell from the empty ones.
		kIndInList = generateRand_KCell(currVacantCellLst_Size);
		currCellNode = extractVacantCellNode(&vacantCellLst, kIndInList);
		currCell = &(currCellNode->cell);


		//Update board in the relevant cell, by generating valid value.
		newValue = generateRandValueToFill(resPossibilities, currCell);
		sudokoBoard[currCell->rowInd][currCell->colInd] = newValue;
		singlePlacementProcedure(sudokoBoard, resPossibilities, currCell, newValue, &dupFlag);

		//Remove the "just inserted" cell from the Cell linked list.
		removeVacantCellNode(&vacantCellLst, currCellNode);
		currVacantCellLst_Size--;

		//in case dupliaction has found, release possibilities and cell list, and alert it to the function "createNewPlayer"
		//So a new random board will be requested.
		if (dupFlag == true)
		{
			free_ptrToArrayMat(resPossibilities);
			freeVacantCellList(&vacantCellLst);
			*filledCorrectly = false;
			return;
		}
	}

	//local to input parmam.
	*possibilities = resPossibilities;
	freeVacantCellList(&vacantCellLst);
}

//Will generate N between 1 to 20.
short generateRandNumOfCells()
{
	short res;

	srand(time(NULL));
	res = (rand() % 20 + 1);

	return res;
}

//generate a random value to insert, for a specific cell.
short generateRandValueToFill(Array*** possibilities, Cell* currCell)
{
	short* currArr;
	short resVal, resInd;
	unsigned short currArrSize;

	//For convenience
	currArr = possibilities[currCell->rowInd][currCell->colInd]->arr;
	currArrSize = possibilities[currCell->rowInd][currCell->colInd]->size;

	srand(time(NULL));
	resInd = (rand() % currArrSize);
	resVal = currArr[resInd];

	return resVal;
}
//generate valid cell to fill with value.
short generateRand_KCell(short currVacantCellLst_Size)
{
	short resKCell;

	srand(time(NULL));
	resKCell = (rand() % currVacantCellLst_Size);

	return resKCell;
}

//Initialze board and set all possibilities for each Array in possibilities matrix.
//Also, build a linked list of the vacant cells. which starting with 81 options.
void setInitBoardPreparations(VacantCellList* vacantCellLst, short board[][SUDOKO_SIZE], Array*** possibilities)
{
	makeEmpty_VacantCellList(vacantCellLst);
	setInitVacantCellList(vacantCellLst);
	setVacantBoard(board);
	setInitPossibilities(possibilities);
}

//Set all cells to be empty.
void setVacantBoard(short board[][SUDOKO_SIZE])
{
	short rowInd, colInd;

	for (rowInd = 0; rowInd < SUDOKO_SIZE; rowInd++)
		for (colInd = 0; colInd < SUDOKO_SIZE; colInd++)
			board[rowInd][colInd] = EMPTY_CELL;

}

//Set all possibilities for each Array.
void setInitPossibilities(Array*** possibilities)
{
	short rowInd, colInd;

	for (rowInd = 0; rowInd < SUDOKO_SIZE; rowInd++)
	{
		for (colInd = 0; colInd < SUDOKO_SIZE; colInd++)
		{
			possibilities[rowInd][colInd]->size = 9;
			fillAllPossiblitiesForCell(possibilities[rowInd][colInd]->arr);
		}
	}
}

//Set 81 Cells opttions which holded by a linked list
void setInitVacantCellList(VacantCellList* vacantCellLst)
{
	Cell resCell;
	short rowInd, colInd;

	//Set cells values (rowInd and colInd)
	for (rowInd = 0; rowInd < SUDOKO_SIZE; rowInd++)
	{
		for (colInd = 0; colInd < SUDOKO_SIZE; colInd++)
		{
			resCell.rowInd = rowInd;
			resCell.colInd = colInd;
			insertDataToEnd_VacantCellList(vacantCellLst, resCell);
		}
	}
}

//Recieve single Array possibilities arr, and fill 9 options, (possible value between 1-9)...
void fillAllPossiblitiesForCell(short* singleCellShortArr)
{
	short ind;

	for (ind = 0; ind < SUDOKO_SIZE; ind++)
		singleCellShortArr[ind] = ind + 1;
}

//Single Array Cell allocating
Array* fullAllocSingleArrayCell()
{
	Array* resArrayCell;

	resArrayCell = (Array*)malloc(sizeof(Array));
	checkAllocation(resArrayCell);

	resArrayCell->arr = (short*)malloc(SUDOKO_SIZE * sizeof(short));
	checkAllocation(resArrayCell->arr);

	return resArrayCell;
}

//recieve K index, and return the address of the node in place K.
VacantCell_ListNode* extractVacantCellNode(VacantCellList* vacantCellLst, short kIndInList)
{
	VacantCell_ListNode* curr;
	short cunForK = 0;

	curr = vacantCellLst->head;

	while (cunForK < kIndInList && curr != NULL)
	{
		curr = curr->next;
		cunForK++;
	}

	return curr;
}

/*allocates Array*** type, one by one, and returned it*/
Array*** allocatePossiblitiesMatrix()
{
	Array*** resPossibilities;
	short rowInd, colInd;

	resPossibilities = (Array***)malloc(SUDOKO_SIZE * sizeof(Array**));//allocates Array***
	checkAllocation(resPossibilities);

	for (rowInd = 0; rowInd < SUDOKO_SIZE; rowInd++)
	{
		resPossibilities[rowInd] = (Array**)malloc(SUDOKO_SIZE * sizeof(Array*));//allocates Array**
		checkAllocation(resPossibilities[rowInd]);

		for (colInd = 0; colInd < SUDOKO_SIZE; colInd++)
			resPossibilities[rowInd][colInd] = fullAllocSingleArrayCell();

	}

	return resPossibilities;
}


//Build tree (from ptrArr to Active Players List node arr)
ActivePlayers_TR BuildTreeFromArray(ActivePlayers_ListNode** ptrArr_ActPlayers_LNode, short ptrArrSize)
{
	ActivePlayers_TR res_ActivePlayersTree;
	makeEmpty_ActivePlayersTree(&res_ActivePlayersTree);
	res_ActivePlayersTree.root = BuildTreeFromArrayHelper(ptrArr_ActPlayers_LNode, ptrArrSize);
	return res_ActivePlayersTree;
}

ActivePlayers_TNODE* BuildTreeFromArrayHelper(ActivePlayers_ListNode** ptrArr_ActPlayers_LNode, short ptrArrSize)
{
	ActivePlayers_TNODE* res_ActivePlayer_TNODE;
	short mid, leftSize, rightSize;

	mid = (ptrArrSize / 2);
	leftSize = ptrArrSize / 2;
	rightSize = ptrArrSize - leftSize - 1;

	if (ptrArrSize <= 0)
		return NULL;

	else
	{
		res_ActivePlayer_TNODE = createNew_ActivePlayer_TNode(ptrArr_ActPlayers_LNode[mid], NULL, NULL);
		res_ActivePlayer_TNODE->left = BuildTreeFromArrayHelper(ptrArr_ActPlayers_LNode, leftSize);
		res_ActivePlayer_TNODE->right = BuildTreeFromArrayHelper(ptrArr_ActPlayers_LNode + (mid + 1), rightSize);

		return res_ActivePlayer_TNODE;
	}
}

//Running all games session. return the winners into a linked list.
WinnersList scanTreeInOrder_FullSession(ActivePlayers_TR* activePlayersTree)
{
	WinnersList resWinnersLst;
	makeEmpty_WinnersList(&resWinnersLst);
	scanTreeInOrder_FullSession_Helper(activePlayersTree->root, &resWinnersLst);
	return resWinnersLst;
}

//scan In order LDR 
void scanTreeInOrder_FullSession_Helper(ActivePlayers_TNODE* currRoot, WinnersList* newWinnersLst)
{
	int gameResult;

	if (currRoot == NULL)
		return;

	else
	{
		//Calling left sub-tree.
		scanTreeInOrder_FullSession_Helper(currRoot->left, newWinnersLst);

		//perform single game session.
		if (currRoot->single_ptrToActPlayer_LNode != NULL)
		{
			printf("\n\nGame started, Go ahead %s\n", currRoot->single_ptrToActPlayer_LNode->activePlayer.playerName);
			gameResult = FillBoard(currRoot->single_ptrToActPlayer_LNode->activePlayer.sodukuBoard, currRoot->single_ptrToActPlayer_LNode->activePlayer.ptrToArrayMat);

			if (gameResult == FINISH_SUCCESS)
			{
				insertDataToEnd_WinnersList(newWinnersLst, currRoot->single_ptrToActPlayer_LNode->activePlayer);
				printf("Congratulations!!!\n~~~~~~~~~~~~~~~~\n");
			}
			else // gameResult == FINISH_FAILURE //just free from the activePlayersList, as mentions above in the if condition
			{
				printf("Player's choice led to duplications!\n");
				printf("Maybe Next Time, %s\n~~~~~~~~~~~~~~~~\n", currRoot->single_ptrToActPlayer_LNode->activePlayer.playerName);
			}
			//function to remove ActivePlayer LNode
			removeActivePlayerListNode(currRoot->single_ptrToActPlayer_LNode);
		}

		//Update the ptrArr in the relevant index to point also to NULL, since the active player removed.
		currRoot->single_ptrToActPlayer_LNode = NULL;

		//Calling right sub-tree.
		scanTreeInOrder_FullSession_Helper(currRoot->right, newWinnersLst);
	}
}

//Printing players which complete the game successfully.
void printWinnersList(WinnersList* winnersLst)
{
	Winner_ListNode* curr;

	curr = winnersLst->head;

	printf("\nHere are the winners:\n");
	while (curr != NULL)
	{
		printf("%s\n", curr->winnerPlayer.playerName);
		sudokoPrintBoard(curr->winnerPlayer.sodukuBoard);
		printf("\n");
		curr = curr->next;
	}
	//Mention the ptrToArrayMat already freed after each player's game session
	freeWinnersList(winnersLst);

}

/*creates new linked active players list  */
ActivePlayersList createActivePlayersList(short sizeOfList)
{
	ActivePlayersList newActivePlayersList;
	makeEmpty_ActivePlayersList(&newActivePlayersList);

	short ind = 0;
	Player newPlayer;


	for (ind = 0; ind < sizeOfList; ind++)
	{
		//build "data" to node.
		//gets a player, prev* and next*
		newPlayer = createNewPlayer();
		insertDataToEnd_ActivePlayersList(&newActivePlayersList, newPlayer);
	}

	return newActivePlayersList;
}

/*creates new player struct , get the name field inside this function using gets*/
Player createNewPlayer()
{
	Player newPlayer; //creating name, short** board, Array*** ptrToArrayMat
	Array*** tmpPtrToArrayMat = NULL;
	bool boardfilledCorrectly = false;

	// will get the board and the array mat from the random board.
	printf("Enter player name: \n");
	gets(newPlayer.playerName);

	//The contidition is to cover a case the board didn't set correctly, so a new board should be created for the player.
	while (boardfilledCorrectly == false)
	{
		setInitRandomBoard(newPlayer.sodukuBoard, &tmpPtrToArrayMat, &boardfilledCorrectly);
		if (boardfilledCorrectly == false)
			printf("dup found, setInitRandomBoard AGAIN!\n");
	}

	newPlayer.ptrToArrayMat = tmpPtrToArrayMat;

	return newPlayer;
}

/*gets the linked active players list and sort it in merge sort functions*/
ActivePlayers_ListNode** createAndSort_ptrArr_ActivePlayers_LNode(ActivePlayersList* activePlayersList, short numOfPlayers)
{
	ActivePlayers_ListNode** resPtrArr;
	short resIndex = 0;

	resPtrArr = (ActivePlayers_ListNode**)malloc(numOfPlayers * sizeof(ActivePlayers_ListNode*));
	checkAllocation(resPtrArr);

	ActivePlayers_ListNode* currLNode;
	currLNode = activePlayersList->head;

	while (currLNode != NULL)
	{
		resPtrArr[resIndex] = currLNode;
		currLNode = currLNode->next;
		resIndex++;
	}

	// sorting the active players arr in the merge sort methood.
	Active_Players_ptrArr_mergeSort(resPtrArr, numOfPlayers);
	return resPtrArr;
}

//ActivePlayers_ListNode** ptrArrActivePlayers Arr Merge functions.
void Active_Players_ptrArr_mergeSort(ActivePlayers_ListNode** ptrArrActivePlayers, short ptrArrSize)
{
	//use the merge sort method to sort the ptr ActivePlayer_ListNodes array.
	ActivePlayers_ListNode** tmpPtrArr;
	short leftSize, rightSize;
	leftSize = ptrArrSize / 2;
	rightSize = ptrArrSize - leftSize;

	if (ptrArrSize <= 1)
		return;
	else
	{
		Active_Players_ptrArr_mergeSort(ptrArrActivePlayers, leftSize);
		Active_Players_ptrArr_mergeSort(ptrArrActivePlayers + (ptrArrSize / 2), rightSize);
		tmpPtrArr = merge_Active_Players_ptrArr(ptrArrActivePlayers, leftSize, ptrArrActivePlayers + (ptrArrSize / 2), rightSize);

		checkAllocation(tmpPtrArr);

		// copy values from tmpPtrArr to ptrArrActivePlayers
		copyArr(ptrArrActivePlayers, tmpPtrArr, ptrArrSize);
		free(tmpPtrArr);

	}
}

/*merges 2 ptr arrays into sorted one, as the player with the lowest num of fiiled cells atthe begining
and if we have 2 players with same amount of filled cells it will input the one with the lowes lex val*/
ActivePlayers_ListNode** merge_Active_Players_ptrArr(ActivePlayers_ListNode** ptrArrActivePlayers1, short size1, ActivePlayers_ListNode** ptrArrActivePlayers2, short size2)
{
	ActivePlayers_ListNode** resPtrArr;
	short ind1, ind2, resInd;
	short N_ofFilledCells_Player1;
	short N_ofFilledCells_Player2;
	char* player1_Name, * player2_Name;
	int resStrCmp;

	resPtrArr = (ActivePlayers_ListNode**)malloc((size1 + size2) * sizeof(ActivePlayers_ListNode*));
	checkAllocation(resPtrArr);

	ind1 = ind2 = resInd = 0;

	while ((ind1 < size1) && (ind2 < size2))
	{
		N_ofFilledCells_Player1 = numOfFilledCells(ptrArrActivePlayers1[ind1]->activePlayer.sodukuBoard);
		N_ofFilledCells_Player2 = numOfFilledCells(ptrArrActivePlayers2[ind2]->activePlayer.sodukuBoard);

		// the 1st arr board field has less filled cells
		if (N_ofFilledCells_Player1 < N_ofFilledCells_Player2)
		{
			resPtrArr[resInd] = ptrArrActivePlayers1[ind1];
			ind1++;
		}
		// the 2nd arr board field has less filled cells
		else if (N_ofFilledCells_Player1 > N_ofFilledCells_Player2)
		{
			resPtrArr[resInd] = ptrArrActivePlayers2[ind2];
			ind2++;
		}
		else //the 2 arrays boards has the same amount of unfilled cells
		{

			resStrCmp = strcmp(ptrArrActivePlayers1[ind1]->activePlayer.playerName, ptrArrActivePlayers2[ind2]->activePlayer.playerName);

			//the Lexicographic value of the first active player name is lower than the 2nd one
			if (resStrCmp < 0)
			{
				resPtrArr[resInd] = ptrArrActivePlayers1[ind1];
				ind1++;
			}
			else //the Lexicographic value of the 2nd active player name is lower than the 1st one
			{
				resPtrArr[resInd] = ptrArrActivePlayers2[ind2];
				ind2++;
			}
		}
		resInd++;
	}

	while (ind1 < size1)
	{
		resPtrArr[resInd] = ptrArrActivePlayers1[ind1];
		ind1++;
		resInd++;
	}
	while (ind2 < size2)
	{
		resPtrArr[resInd] = ptrArrActivePlayers2[ind2];
		ind2++;
		resInd++;
	}

	return resPtrArr;
}

/* counts the num of empty cells in the curr board argument*/
short numOfFilledCells(short board[][SUDOKO_SIZE])
{
	short row, col, countFilledCells = 0;
	for (row = 0; row < SUDOKO_SIZE; row++)
	{
		for (col = 0; col < SUDOKO_SIZE; col++)
		{
			if (board[row][col] != EMPTY_CELL)
				countFilledCells++;
		}
	}
	return countFilledCells;
}

// copy one arr to another(in our case one ptrArr to another)
void copyArr(ActivePlayers_ListNode** dest, ActivePlayers_ListNode** src, short size)
{
	short ind;

	for (ind = 0; ind < size; ind++)
		dest[ind] = src[ind];
}


/*adds  nulls to the end of the active players arr(the number of nulls dependes on the num of players)*/
ActivePlayers_ListNode** addNULLPlaces_ptrArrToActPlayersLNode(ActivePlayers_ListNode** ptrArr_ActivePlayers_LNode, short* ptrArrsize)
{
	short i = 0, newPtrArrSize, localPtrArrsize = *ptrArrsize;

	newPtrArrSize = calcNewPtrArrSize(localPtrArrsize);

	if (newPtrArrSize > localPtrArrsize)
	{
		ptrArr_ActivePlayers_LNode = (ActivePlayers_ListNode**)realloc(ptrArr_ActivePlayers_LNode, newPtrArrSize * sizeof(ActivePlayers_ListNode*));
		checkAllocation(ptrArr_ActivePlayers_LNode);

		//puts null in the empty places that just added to ptrArr (in the end)...
		for (i = localPtrArrsize; i < newPtrArrSize; i++)
			ptrArr_ActivePlayers_LNode[i] = NULL;
	}

	*ptrArrsize = newPtrArrSize;
	return ptrArr_ActivePlayers_LNode;
}

/*calc the expression given in the project and returns the new arr size, that will contain the nulls later*/
short calcNewPtrArrSize(short ptrArrsize)
{
	short newSize;
	double logVal;
	short powVal;

	logVal = (log((double)(ptrArrsize)+1)) / (log(2));
	powVal = ceil(logVal);
	newSize = (pow(2, powVal)) - 1;

	return newSize;
}

//Linked lists help functions
void insertDataToEnd_VacantCellList(VacantCellList* vacantCellLst, Cell cell)
{
	VacantCell_ListNode* newTail;
	newTail = createNew_VacantCellNode(cell, NULL, NULL);
	insertNodeToEnd_VacantCellList(vacantCellLst, newTail);
}

VacantCell_ListNode* createNew_VacantCellNode(Cell cell, VacantCell_ListNode* next, VacantCell_ListNode* prev)
{
	VacantCell_ListNode* res;

	res = (VacantCell_ListNode*)malloc(sizeof(VacantCell_ListNode));
	checkAllocation(res);

	res->cell = cell;
	res->next = next;
	res->prev = prev;

	return res;
}

/*linked list help functions*/
void insertNodeToEnd_VacantCellList(VacantCellList* vacantCellLst, VacantCell_ListNode* tail)
{
	if (isEmpty_VacantCellList(vacantCellLst) == true)
	{
		vacantCellLst->head = vacantCellLst->tail = tail;
	}
	else
	{
		tail->prev = vacantCellLst->tail;
		vacantCellLst->tail->next = tail;
		vacantCellLst->tail = tail;
	}
	tail->next = NULL;
}

bool isEmpty_VacantCellList(VacantCellList* vacantCellLst)
{
	if (vacantCellLst->head == NULL && vacantCellLst->tail == NULL)
		return true;
	else
		return false;
}

void makeEmpty_VacantCellList(VacantCellList* vacantCellLst)
{
	vacantCellLst->head = NULL;
	vacantCellLst->tail = NULL;
}

void removeVacantCellNode(VacantCellList* vacantCellLst, VacantCell_ListNode* currCellNode)
{
	if (vacantCellLst->head == currCellNode && vacantCellLst->tail == currCellNode)
	{
		//Do nothing. just to ensure it won't enter the other conditions.
	}
	else if (vacantCellLst->head == currCellNode)
	{
		currCellNode->next->prev = NULL;
		vacantCellLst->head = currCellNode->next;
	}
	else if (vacantCellLst->tail == currCellNode)
	{
		currCellNode->prev->next = NULL;
		vacantCellLst->tail = currCellNode->prev;
	}
	else
	{
		currCellNode->prev->next = currCellNode->next;
		currCellNode->next->prev = currCellNode->prev;
	}
	free(currCellNode);
}

void freeVacantCellList(VacantCellList* vacantCellLst)
{
	VacantCell_ListNode* curr, * newHead;

	curr = newHead = vacantCellLst->head;

	while (curr != NULL)
	{
		newHead = curr->next;
		free(curr);
		curr = newHead;
	}
}


/*linked list help functions*/
void makeEmpty_ActivePlayersList(ActivePlayersList* activePlayersLst)
{
	activePlayersLst->head = NULL;
	activePlayersLst->tail = NULL;
}

bool isEmpty_ActivePlayersList(ActivePlayersList* activePlayersLst)
{
	if (activePlayersLst->head == NULL && activePlayersLst->tail == NULL)
		return true;
	else
		return false;
}

ActivePlayers_ListNode* createNew_ActivePlayer_ListNode(Player player, ActivePlayers_ListNode* next, ActivePlayers_ListNode* prev)
{
	ActivePlayers_ListNode* res;

	res = (ActivePlayers_ListNode*)malloc(sizeof(ActivePlayers_ListNode));
	checkAllocation(res);

	res->activePlayer = player;
	res->next = next;
	res->prev = prev;
	return res;
}

void insertDataToEnd_ActivePlayersList(ActivePlayersList* activePlayersList, Player newPlayer)
{
	ActivePlayers_ListNode* newTail;
	newTail = createNew_ActivePlayer_ListNode(newPlayer, NULL, NULL);
	insertNodeToEnd_ActivePlayersList(activePlayersList, newTail);
}

void insertNodeToEnd_ActivePlayersList(ActivePlayersList* newActivePlayersList, ActivePlayers_ListNode* tail)
{
	if (isEmpty_ActivePlayersList(newActivePlayersList) == true)
	{
		newActivePlayersList->head = newActivePlayersList->tail = tail;
	}
	else
	{
		tail->prev = newActivePlayersList->tail;
		newActivePlayersList->tail->next = tail;
		newActivePlayersList->tail = tail;
	}
	tail->next = NULL;
}


/*removes a list node from the active players list and free all of his relevant fields*/
void removeActivePlayerListNode(ActivePlayers_ListNode* currNode)
{
	if (currNode->prev == NULL && currNode->next == NULL)
	{
		//Do nothing, just free the curr node at the end (after conditions).
	}
	else if (currNode->prev == NULL)//the curr node is the head of the list
		currNode->next->prev = NULL;

	else if (currNode->next == NULL)//the curr node is the tail of the list
		currNode->prev->next = NULL;

	else//the curr node is not the head or the tail
	{
		currNode->prev->next = currNode->next;
		currNode->next->prev = currNode->prev;
	}
	free_ptrToArrayMat(currNode->activePlayer.ptrToArrayMat);
	free(currNode);
}
/*frees all of the player fields*/
void free_ptrToArrayMat(Array*** ptrToArrayMat)
{
	int i, j;

	for (i = 0; i < SUDOKO_SIZE; i++)
	{
		for (j = 0; j < SUDOKO_SIZE; j++)
		{
			if (ptrToArrayMat[i][j] != NULL)
			{
				free(ptrToArrayMat[i][j]->arr); //free short arr
				free(ptrToArrayMat[i][j]); //Free *
			}
		}
		//Free **
		free(ptrToArrayMat[i]);
	}
	//Free ***
	free(ptrToArrayMat);
}

/*frees the winners list*/
void freeWinnersList(WinnersList* winnersLst)
{
	Winner_ListNode* curr, * newHead;

	curr = newHead = winnersLst->head;

	while (curr != NULL)
	{
		newHead = curr->next;
		free(curr);
		curr = newHead;
	}
}

/*linked list help functions*/
void makeEmpty_WinnersList(WinnersList* winnersLst)
{
	winnersLst->head = NULL;
	winnersLst->tail = NULL;
}

bool isEmpty_WinnersList(WinnersList* winnersLst)
{
	if (winnersLst->head == NULL)
		return true;
	else
		return false;
}

Winner_ListNode* createNew_WinnerListNode(Player newWinnerPlayer, Winner_ListNode* next, Winner_ListNode* prev)
{
	Winner_ListNode* res;

	res = (Winner_ListNode*)malloc(sizeof(Winner_ListNode));
	checkAllocation(res);

	res->winnerPlayer = newWinnerPlayer;
	res->next = next;
	res->prev = prev;
	return res;
}

void insertDataToEnd_WinnersList(WinnersList* newWinnersLst, Player newWinnerPlayer)
{
	Winner_ListNode* newTail;
	newTail = createNew_WinnerListNode(newWinnerPlayer, NULL, NULL);
	insertNodeToEnd_WinnersList(newWinnersLst, newTail);
}

void insertNodeToEnd_WinnersList(WinnersList* newWinnersLst, Winner_ListNode* tail)
{
	if (isEmpty_WinnersList(newWinnersLst) == true)
	{
		newWinnersLst->head = newWinnersLst->tail = tail;
	}
	else
	{
		tail->prev = newWinnersLst->tail;
		newWinnersLst->tail->next = tail;
		newWinnersLst->tail = tail;
	}
	tail->next = NULL;
}


//Trees help functions
void makeEmpty_ActivePlayersTree(ActivePlayers_TR* ActivePlayersTree)
{
	ActivePlayersTree->root = NULL;
}

ActivePlayers_TNODE* createNew_ActivePlayer_TNode(ActivePlayers_ListNode* ptr_ActPlayer_LNode, ActivePlayers_TNODE* left, ActivePlayers_TNODE* right)
{
	ActivePlayers_TNODE* res;
	res = (ActivePlayers_TNODE*)malloc(sizeof(ActivePlayers_TNODE));
	checkAllocation(res);
	res->single_ptrToActPlayer_LNode = ptr_ActPlayer_LNode;
	res->left = left;
	res->right = right;
	return res;
}



//Check allocations functions for all variables types.
void checkAllocation(void* ptr)
{
	if (ptr == NULL)
	{
		printf("Allocation has failed!\n");
		exit(1);
	}
}
