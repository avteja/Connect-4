# Connect-4

Connect Four (also known as Captain's Mistress, Four Up, Plot Four, Find Four, Fourplay, Four in a Row and Four in a Line) is a two-player connection game in which the players first choose a color and then take turns dropping colored discs from the top into a seven-column, six-row vertically suspended grid. The pieces fall straight down, occupying the next available space within the column. The objective of the game is to connect four of one's own discs of the same color next to each other vertically, horizontally, or diagonally before your opponent. Connect Four is a strongly solved game. The first player can always win by playing the right moves.
( Source: http://en.wikipedia.org/wiki/Connect_Four )

Our idea is to make a Connect4 Bot, which can play intelligently against a human player. The bot can gauge the given situation to play the most suitable move to ensure a greater chance of victory.


Heuristic Function (also called an Evaluation Function) is the scoring function that determines the score of a given state by using specific features of the state of the game. In our case of Connect-4, we have designed our heuristic function as follows:
1. The heuristic function is proportional to the power of number of chips that are continuous in a row/column/diagonal. This is added up for every row,column and diagonal.
Note: The value calculated for each row/column/diagonal has a positive or negative sign depending on which coloured chips are present consecutively in that row or column.
2. The heuristic function gives a very high value(like INT_MAX) to 4 chips consecutively in a row/column/diagonal.
'Max' and 'Min' can be considered to be the two players of the game(considering two-player games only) . 'Max' will try to maximize the heuristic function at every Max move and 'Min' will try to minimize the heuristic at every Min move.
MinMax (also known as minimax) algorithm is an algorithm that can be used to find the best possible move/action for each player(Min and Max). Further details about the MinMax Algorithm can be found at http://web.stanford.edu/~msirota/soco/minimax.html .
Alpha-Beta pruning is a way of avoiding subtrees of searches which won't be selected. Further details can be found at http://cs.ucla.edu/~rosen/161/notes/alphabeta.html .
MinMax algorithm with alpha-beta pruning is used to get a depth of 10 (at reasonable speeds) for Connect4 game.
