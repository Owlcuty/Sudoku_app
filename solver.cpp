#include <stdio.h>
#include <assert.h>

const int NMax = 9;
const int groups[NMax][NMax]	    =       {{1, 1, 1, 2, 2, 2, 3, 3, 3},
                                             {1, 1, 1, 2, 2, 2, 3, 3, 3},
                                             {1, 1, 1, 2, 2, 2, 3, 3, 3},
                                             {4, 4, 4, 5, 5, 5, 6, 6, 6},
                                             {4, 4, 4, 5, 5, 5, 6, 6, 6},
                                             {4, 4, 4, 5, 5, 5, 6, 6, 6},
                                             {7, 7, 7, 8, 8, 8, 9, 9, 9},
                                             {7, 7, 7, 8, 8, 8, 9, 9, 9},
                                             {7, 7, 7, 8, 8, 8, 9, 9, 9}};

long int counter = 0;
long int cnt = 0;

bool empty(int x) {
	return(!x);
}

bool checker(int sudoku[][NMax]) {
     int by_grps[NMax][NMax] = {};
     for (int y = 0; y < NMax; y ++) {
          for (int x = 0; x < NMax; x ++) {
	       if (empty(sudoku[y][x])) continue;
	       if (by_grps[groups[y][x] - 1][sudoku[y][x] - 1]) {
		    return false;
	       }
	       by_grps[groups[y][x] - 1][sudoku[y][x] - 1] = true;
	       for (int x1 = 0; x1 < NMax; x1 ++) {
		    if (x1 == x) continue;
		    if (sudoku[y][x] == sudoku[y][x1]) {
			return false;
		    }
	       }
	       for (int y1 = 0; y1 < NMax; y1 ++) {
		    if (y1 == y) continue;
		    if (sudoku[y][x] == sudoku[y1][x]) {
			return false;
		    }
	       }
	  }
     }
     return true;
}

void check_allowed(int sudoku [][NMax], int allowed[][NMax][NMax]) {

	for (int y = 0; y < NMax; y ++) {
		for (int x = 0; x < NMax; x ++) {
			if (!empty(sudoku[y][x])) {
				for (int k = 0; k < NMax; k ++) allowed[y][x][k] = false;
				continue;
			}
			for (int y1 = 0; y1 < NMax; y1++) {
				for (int x1 = 0; x1 < NMax; x1 ++) {
					if (y == y1 && x == x1) continue;
					if (!empty(sudoku[y1][x1])) {
						if(groups[y][x] == groups[y1][x1] || x == x1 || y == y1) {
							allowed[y][x][sudoku[y1][x1] - 1] = false;
						}
					}
				}
			}
		}
	}

}

void copy_array(int arr[][NMax], int copy_arr[][NMax]) {
	for (int y = 0; y < NMax; y ++) {
		for (int x = 0; x < NMax; x ++) {
			copy_arr[y][x] = arr[y][x];
		}
	}
}

void solver(int sudoku[][NMax], int allowed[][NMax][NMax], int residue, int used[NMax][NMax]) {

	if (!residue) {
       		counter ++;
		if (!checker(sudoku)) {
			return;
		}
		printf("%i:: Num. %i" "\n", __LINE__, counter);
		for (int y = 0; y < NMax; y ++) {
			for (int x = 0; x < NMax; x++) {
				printf("%i ", sudoku[y][x]);
			}
			printf("\n");
		}
		printf("\n");
		return;
	}
	int copy_sudoku[NMax][NMax];
	int copy_used[NMax][NMax];
	for (int y = 0; y < NMax; y ++) {
		for (int x = 0; x < NMax; x++) {
			if (used[y][x]) continue;
			for (int i = 0; i < NMax; i ++) {
				if (!allowed[y][x][i]) continue;
				copy_array(sudoku, copy_sudoku);
				copy_array(used, copy_used);
				copy_sudoku[y][x] = i + 1;
				copy_used[y][x] = 1;
				//cnt ++;
				/*if (residue < 2) {
					printf("%i:: cnt = %i" "\n", __LINE__, cnt);
				       for (int y1 = 0; y1 < NMax; y1 ++) {
				            for (int x1 = 0; x1 < NMax; x1 ++) {
                                        	printf("%i ", sudoku[y1][x1]);
					    }
					    printf("\n");
				       }
				       printf("\n");
				}*/
				solver(copy_sudoku, allowed, residue - 1, copy_used);
			}
		}
	}

}

int main() {
/*
     int sudoku[NMax][NMax] = {{8, 0, 1, 0, 5, 2, 0, 4, 3},
	     		       {7, 2, 6, 9, 4, 0, 5, 8, 1},
			       {3, 4, 5, 1, 7, 8, 0, 9, 6},
			       {5, 3, 8, 0, 6, 7, 1, 0, 9},
			       {1, 6, 2, 8, 9, 5, 3, 7, 4},
			       {4, 7, 9, 2, 3, 1, 8, 6, 5},
			       {9, 1, 3, 7, 2, 4, 0, 5, 8},
			       {6, 8, 7, 5, 1, 9, 4, 3, 2},
			       {2, 5, 4, 3, 8, 6, 9, 1, 7}};
*/
     int sudoku[NMax][NMax] = {{8, 0, 1, 0, 5, 2, 0, 4, 3},
	     		       {7, 0, 6, 0, 4, 0, 5, 0, 1},
			       {3, 0, 5, 1, 7, 8, 0, 9, 0},
			       {0, 3, 8, 0, 6, 7, 1, 0, 9},
			       {1, 0, 0, 8, 9, 0, 3, 7, 4},
			       {4, 7, 9, 2, 3, 1, 8, 6, 5},
			       {0, 1, 0, 7, 2, 4, 0, 5, 8},
			       {6, 0, 7, 0, 1, 9, 0, 3, 2},
			       {2, 5, 4, 3, 8, 6, 9, 1, 7}};


     int allowed[NMax][NMax][NMax];
     int residue = 0;

     int used[NMax][NMax];
     for (int y = 0; y < NMax; y ++) {
	    for (int x = 0; x < NMax; x ++) {
		   used[y][x] = 0;
		   if (empty(sudoku[y][x])) residue ++;
		   else used[y][x] = 1; 
		   for (int i = 0; i < NMax; i ++) {
			  allowed[y][x][i] = true;
		   }
	    }
     }
     printf("%i:: residue = %i" "\n", __LINE__, residue);
    int now = 10; 
     if (checker(sudoku)) {
	check_allowed(sudoku, allowed);
        for (int y = 0; y < NMax; y ++) {
                for (int x = 0; x < NMax; x ++) {
                        now = 10;
			printf("[ ");
                        for (int k = 0; k < NMax; k ++) {
                                if (allowed[y][x][k]) {printf("%i ", k + 1); now -= 2;}
                        }
			for (int pass = 0; pass < now; pass ++) printf(" ");
                        printf("] ");
                }
                printf("\n");
        }
     	solver(sudoku, allowed, residue, used);
     }
     else
	printf("%i:: Oops!!!" "\n", __LINE__);
     for (int y = 0; y < NMax; y ++) {
	     for (int x = 0 ; x < NMax; x ++) {
		     printf("%i ", sudoku[y][x]);
	     }
	     printf("\n");
     }
     return 0;
}
