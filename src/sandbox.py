grid = [['','',''],
        ['','',''],
        ['','','']]
game_state = True
count_moves = 0
player = 1
play = 'X'
while game_state:
    #WHILE the user doesn't give us a valid position, we have to keep on asking for more positions
    move_state = True
    while move_state:
        player1_coordinate1 = int(input(f"Player {player} enter your move: "))
        player1_coordinate2 = int(input(f"Player {player} enter your move: "))
        if player1_coordinate1 in range(0, 3):
            if player1_coordinate2 in range(0, 3):
                if grid[player1_coordinate1][player1_coordinate2] == '':
                    grid[player1_coordinate1][player1_coordinate2] = play
                    count_moves += 1
                    move_state = False
                else:
                    print("That spot is taken try another: ")
            else:
                print(f"{player} 1 select a valid coordinate")
        else:
            print(f"{player} 1 select a valid coordinate")

    for i in grid:
        print(i)

    for i in range(0, len(grid)):

        if play == grid[i][0] and play == grid[i][1] and play == grid[i][2]:

            print(f'Player {player} horizonal win')
            game_state = False

    if play != grid[i][0] and play != grid[i][1] and play != grid[i][2]:
        for _ in range(0, len(grid)):
            if play == grid[_][1] and play == grid[_][1] and play == grid[_][2]:
                print(f'Player {player} vertical win! {play}')

    if play == grid[0][0] and play == grid[1][1] and play == grid[2][2]:
        print(f'{player} wins!')
        break
    if play == grid[0][2] and play == grid[1][1] and play == grid[2][0]:
        print(f'{player} wins!')
        break
    else:
        if count_moves == 9:
            print("It's a draw!")
            game_state = False

    if play == 'X':
        play = 'O'
        player = 2
    else:
        play = 'X'
        player = 1
