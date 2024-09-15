// Caden Roberts
#include <iostream>
using namespace std;
class Board {
public:
    Board() {
        for (int i = 0; i < 100; i++) board[i] = 0;
    }
    
    void addLadder(int start, int end) {
        board[start-1] = end - start;
    }
    
    void addSnake(int start, int end) {
        board[start-1] = end - start;
    }
    
    void minmovesguider( int moves=0, int position=1) {
        if (position<95){
            if (position>1) cout << "Turn end at " << position << ".\n";
            int best[6]{board[position]+1, board[position+1]+2, board[position+2]+3, board[position+3]+4, board[position+4]+5, board[position+5]+6};
            return minmovesguider(moves+1, position+*max_element(best, best+6));
        } else if (position!=100) {
            cout << "Turn end at " << position << ".\n";
            return minmovesguider(moves+1, 100);
        } else cout << "Reached final square 100.\nMinimum moves for the board is " << moves << ".\n";
        
    }
    
private:
    int board[100];
};

int main () {
    Board Board;
    Board.addLadder(3,21);
    Board.addLadder(8,30);
    Board.addLadder(28,84);
    Board.addLadder(58,77);
    Board.addLadder(75,86);
    Board.addLadder(80,100);
    Board.addLadder(90,91);
    Board.addSnake(17, 13);
    Board.addSnake(88, 18);
    Board.addSnake(62,22);
    Board.addSnake(52, 29);
    Board.addSnake(57,40);
    Board.addSnake(95,51);
    Board.addSnake(97,79);
    
    Board.minmovesguider(0, 0);
    
    return 0;
}
