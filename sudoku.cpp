/* 
    AUTHOR : Atharva bendale
*/

/*
This program prints one of the possible solutions of a given sudoku puzzle ( as there can be multiple possible soltuions).
This prints "No solution" if there are no possible solutions.
*/

/*
Input format :
Numbers in a 9x9 grid, replacing blank spaces with 0.

Output format :
A solved 9x9 sudoku puzzle grid.
*/

#include<iostream>
#include<vector>
using namespace std;


class sudoku{
    
    private:
    int board[9][9];    // Initializes a 9x9 array representing the elements of the sudoku board.
    
    public:
    
    // Constructor, takes input all the sudoku board elements
    sudoku(){
        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){
                cin >> board[i][j];
            }
        }
    }

    // Copy constructor to make a new sudoku board with same board elements
    sudoku(const sudoku& r){
        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){
                board[i][j] = r.board[i][j];
            }
        }
    }

    // A function which returns a vector array of all possible values at a blank space in sudoku board.
    // Arguments : indexes (i, j) of the blank space.
    vector<int> find_nums(int i, int j){
        bool truth_value[9]; // An array which stores 1 at index 'i' if (i+1) is not a possible number at the blank space
        for(int k=0; k<9; k++){truth_value[k]=0;}

        for(int k=0; k<9; k++){
            if(board[i][k] != 0){truth_value[board[i][k]-1] = 1;} // Checking numbers along the row
            if(board[k][j] != 0){truth_value[board[k][j]-1] = 1;} // Checking numbers along the column
        }

        
        // Checking numbers in the 3x3 subgrid
        int sq_i = (i/3)*3, sq_j = (j/3)*3;
        // sq_i *= 3; sq_j *= 3;
        for(int m=sq_i; m<sq_i+3; m++){
            for(int n=sq_j; n<sq_j+3; n++){
                if(board[m][n] != 0){truth_value[board[m][n]-1] = 1;}
            }
        }
        
        vector<int> nums;
        for(int k=0; k<9; k++){  if(!truth_value[k]){nums.push_back(k+1);}   }

        return nums;
    }
    
    // Iterates over every cell in the board and tries to fill in numbers that are uniquely determined.
    bool single_solve(int& min_i, int& min_j, int& min_choices){
        bool result = 0;
        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){
                if(board[i][j] == 0){
                    vector<int> a = find_nums(i, j);
                    if(a.size() == 1){  board[i][j] = a[0];  result = 1;}
                    else if(min_choices > a.size()){   min_choices = a.size();   min_i = i;   min_j = j;   }
                }
            }
        }
        return result;
    }

    // 'Expired' function to check whether the sudoku board is unsolvable.
    bool expired(){
        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){
                if(board[i][j]==0){
                    vector<int> a = find_nums(i, j);
                    if(a.size()==0){return 1;}                            
                }
            }
        }
        return 0;
    }

    // 'board_full' function to check if the board is already filled.
    bool board_full(){
        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){
                if(board[i][j] == 0){return 0;}
            }
        }
        return 1;
    }

    // Finds the cell indexes with minimum number of possiblities
    void find_min(int& min_i, int& min_j, int& min_choices){
        min_i=0; min_j=0; min_choices=9;
        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){
                if(board[i][j] != 0){continue;}
                vector<int> a = find_nums(i, j);
                if((min_choices > a.size())&&(a.size()>1)){min_choices = a.size(); min_i = i; min_j = j;}
            }
        }
    }

    // Executes the solving process
    bool sudo_solve(){

        if(expired()){return 0;}
        if(board_full()){return 1;}
        int min_i = 0, min_j = 0, min_choices = 9;
        bool temp = single_solve(min_i, min_j, min_choices);
        while(temp){temp = single_solve(min_i, min_j, min_choices);}
        if(board_full()){return 1;}
        
        for(int k=0; k<min_choices; k++){
            find_min(min_i, min_j, min_choices);
            vector<int> a = find_nums(min_i, min_j);
            sudoku buf_obj(*this);
            buf_obj.board[min_i][min_j] = a[k];
            bool awol = buf_obj.sudo_solve();
            if(awol){   *this = buf_obj;   return 1;   }

        }
        return 0;
    }

    // overloading = operator.
    sudoku operator=(const sudoku& r){
        for(int i=0; i<9; i++){    for(int j=0; j<9; j++){ board[i][j] = r.board[i][j]; }    }
        return *this;
    }
    
    friend ostream& operator << (ostream&, const sudoku&);
};

// overloading cout(<<) operator.
ostream& operator << (ostream& os, const sudoku& r){
    os << endl;
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            os << r.board[i][j] << " ";
        }   os << endl;
    }
}


int main(){
    sudoku obj;
    bool a = obj.sudo_solve();
    if(!a){cout << "No solution";}
    else{cout << obj;}
}