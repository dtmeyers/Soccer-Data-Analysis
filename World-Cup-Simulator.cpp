#include <iostream>
#include <string>
#include <cmath>
#include <time.h>
#include <algorithm>
#include <iomanip>

using namespace std;

//Initialize team names in group order
static const string teamName[32] = {"Brazil", "Croatia", "Mexico", "Cameroon",
                            "Spain", "Netherlands", "Chile", "Australia",
                            "Colombia", "Greece", "Ivory Coast", "Japan",
                            "Uruguay", "Costa Rica", "England", "Italy",
                            "Switzerland", "Ecuador", "France", "Honduras",
                            "Argentina", "Bosnia", "Iran", "Nigeria",
                            "Germany", "Portugal", "Ghana", "United States",
                            "Belgium", "Algeria", "Russia", "South Korea"};
//The first entry is exp goals scored, the second is exp goals allowed, the third is points accrued in the group stage. Stats based on ESPN's SPI
float teamStats[32][3] = {3.4, .5, 0, 1.7, .9, 0, 1.6, .7, 0, 1.5, 1, 0,
                        2.8, .5, 0, 2.4, .9, 0, 2.8, .7, 0, 1.7, 1.3, 0,
                        2.2, .4, 0, 1.4, .6, 0, 2.3, 1, 0, 1.9, 1.2, 0,
                        2.4, .7, 0, 1.4, .6, 0, 2.2, .7, 0, 2.1, .8, 0,
                        2, 1, 0, 2, .6, 0, 2.4, .6, 0, 1.7, 1, 0,
                        2.9, .4, 0, 2.3, 1, 0, 1.4, 1, 0, 1.7, .9, 0,
                        3.1, .7, 0, 2.1, .9, 0, 1.9, 1, 0, 2.1, 1, 0,
                        2.1, .8, 0, 1.1, 1.2, 0, 1.7, .7, 0, 1.7, 1, 0};

static const double e = 2.71828183;
static const float avgGoals = 1.44219;

int factorial (int number)
{
    int result = 1;
    if (number > 0){
        for (int i = number; i > 1; i--){
            result *= i;
        }
    }
    return (result);
}

//Function to calculate win percentages for group games
void calculateGameOdds (int teamA, int teamB, double& teamAWin, double& teamBWin, const float& averageGoals = avgGoals){
    int i, j;
    float scoreChance;

    float teamAAttack, teamBAttack;
    teamAWin = 0;
    teamBWin = 0;

    //Adjust for Brazil's home field advantage
    //if ((teamName[0] != teamName[teamA]) and (teamName[0] != teamName[teamB])){
        teamAAttack = teamStats[teamA][0] + teamStats[teamB][1] - avgGoals;
        teamBAttack = teamStats[teamB][0] + teamStats[teamA][1] - avgGoals;
    //}
    /*else{
        if (teamName[0] == teamName[teamA]){
            teamAAttack = 1.868 + (teamStats[teamA][0] - avgGoals) - (avgGoals - teamStats[teamB][1]);
            teamBAttack = .897 + (teamStats[teamB][0] - avgGoals) - (avgGoals - teamStats[teamA][1]);
        }
        else{
            teamAAttack = .897 + (teamStats[teamA][0] - avgGoals) - (avgGoals - teamStats[teamB][1]);
            teamBAttack = 1.868 + (teamStats[teamB][0] - avgGoals) - (avgGoals - teamStats[teamA][1]);
        }
    }*/


    for (i = 0; i < 8; i++){
        for (j = 0; j < 8; j++){
            scoreChance = pow(teamAAttack, i) * pow(teamBAttack, j) * exp (-(teamAAttack + teamBAttack)) / (factorial(i) * factorial(j));
            if (i > j){
                teamAWin += scoreChance;
            }
            if (j > i){
                teamBWin += scoreChance;
            }
        }
    }
}

//Function to calculate to results of each group
void groupStage (int groupName[8][4]){
    double teamAWin, teamBWin;
    int i, j, k, game;
    for (k = 0; k < 8; k++){
        for (i = 0; i < 3; i++){
            for (j = i+1; j < 4; j++){
                calculateGameOdds(groupName[k][i], groupName[k][j], teamAWin, teamBWin);
                game = rand() % 100;
                if (game < (teamAWin * 100)){
                    teamStats[groupName[k][i]][2] += 3;
                }
                else{
                    if (game < ((teamAWin + teamBWin) * 100)){
                        teamStats[groupName[k][j]][2] += 3;
                    }
                    else{
                        teamStats[groupName[k][i]][2] += 1;
                        teamStats[groupName[k][j]][2] += 1;
                    }
                }
            }
        }
    }
}

//Function to simulate the World Cup
void simulator(int rounds, const float& averageGoals = avgGoals)
{
    //Variables for how many times each team is eliminated at each stage of the competition
    int group[32];
    int octos[32];
    int quarters[32];
    int semis[32];
    int finals[32];
    int winners[32];

    //Variables for each stage
    int octofinalists[16], quarterfinalists[8], semifinalists[4], finalists[2], champion;
    int firstRoundGroups[8][4] = {0, 1, 2, 3, 4, 5, 6, 7,
                                8, 9, 10, 11, 12, 13, 14, 15,
                                16, 17, 18, 19, 20, 21, 22, 23,
                                24, 25, 26, 27, 28, 29, 30, 31};

    //Declare variables to calculate win probabilities
    int i, j, k, l, m, temp, team1, team2, game;
    int counter = 10000;
    string stringTeam1;
    bool groupSort = false;
    double teamAWin, teamBWin, draw;

    for (i = 0; i < 32; i++){
        group[i] = 0;
        octos[i] = 0;
        quarters[i] = 0;
        semis[i] = 0;
        finals[i] = 0;
        winners[i] = 0;
    }

    //Begin simulation
    for (i = 0; i < counter; i++){
        groupStage(firstRoundGroups);

        /*test loop: will print total points accrued during the group stage for each team
        for (j = 0; j < 8; j++){
            for (k = 0; k < 4; k++){
                cout << teamName[firstRoundGroups[j][k]] << ": " << teamStats[firstRoundGroups[j][k]][2] << "   ";
            }
            cout << endl;
        }*/

        //Loop to sort each group and determine who advances
        for (j = 0; j < 8; j++){
            groupSort = false;
            while (groupSort == false){
                groupSort = true;
                for (k = 0; k < 3; k++){  //k < 3 because each comparison is to the next element, so k = 3 forces comparison to k = 4 which doesn't exist
                    if (teamStats[firstRoundGroups[j][k]][2] < teamStats[firstRoundGroups[j][k+1]][2]){
                        groupSort = false;
                        temp = firstRoundGroups[j][k];
                        firstRoundGroups[j][k] = firstRoundGroups[j][k+1];
                        firstRoundGroups[j][k+1] = temp;
                    }
                }
            }
            //At this point, the group is sorted, but ties have not been broken.
            //Loop to break ties and declare group winner and runner-up
            for (k = 0; k < 3; k++){
                if (teamStats[firstRoundGroups[j][k]][2] == teamStats[firstRoundGroups[j][k+1]][2]){
                    calculateGameOdds(firstRoundGroups[j][k], firstRoundGroups[j][k+1], teamAWin, teamBWin);
                    if (teamAWin < teamBWin){ //if teamAWin > teamBWin, I can leave them as they are
                        temp = firstRoundGroups[j][k];
                        firstRoundGroups[j][k] = firstRoundGroups[j][k+1];
                        firstRoundGroups[j][k+1] = temp;
                    }
                }
            }
        }

        //Increment counter for each team that was eliminated, and advance the group winner/runner up to octofinals
        for (j = 0; j < 8; j++){
            if (j % 2 == 0){
                octofinalists[j] = firstRoundGroups[j][0];
                octofinalists[j+8] = firstRoundGroups[j][1];
            }
            else{
                octofinalists[j] = firstRoundGroups[j][1];
                octofinalists[j+8] = firstRoundGroups[j][0];
            }
            group[firstRoundGroups[j][2]]++;
            group[firstRoundGroups[j][3]]++;
        }

        //Loop to reset each team's point total for each simulation
        for (k = 0; k < 32; k++){
            teamStats[k][2] = 0;
        }
        //Group stage is complete. Next step is moving through the knockout rounds.
        for (j = 0; j < 16; j += 2){
            calculateGameOdds(octofinalists[j], octofinalists[j+1], teamAWin, teamBWin);
            game = rand() % 100;
            if (game < (teamAWin * 100)){
                octos[octofinalists[j+1]]++;
                quarterfinalists[j/2] = octofinalists[j];
            }
            else {
                if (game < ((teamAWin + teamBWin) * 100)){
                    octos[octofinalists[j]]++;
                    quarterfinalists[j/2] = octofinalists[j+1];
                }
                else{
                    game = rand() % 2;
                    if (game == 0){
                        octos[octofinalists[j+1]]++;
                        quarterfinalists[j/2] = octofinalists[j];
                    }
                    else{
                        octos[octofinalists[j]]++;
                        quarterfinalists[j/2] = octofinalists[j+1];
                    }
                }
            }
        }
        //Quarter finals loop
        for (j = 0; j < 8; j += 2){
            calculateGameOdds(quarterfinalists[j], quarterfinalists[j+1], teamAWin, teamBWin);
            game = rand() % 100;
            if (game < (teamAWin * 100)){
                quarters[quarterfinalists[j+1]]++;
                semifinalists[j/2] = quarterfinalists[j];
            }
            else{
                if (game < ((teamAWin + teamBWin) * 100)){
                    quarters[quarterfinalists[j]]++;
                    semifinalists[j/2] = quarterfinalists[j+1];
                }
                else{
                    game = rand() % 2;
                    if (game == 0){
                        quarters[quarterfinalists[j+1]]++;
                        semifinalists[j/2] = quarterfinalists[j];
                    }
                    else{
                        quarters[quarterfinalists[j]]++;
                        semifinalists[j/2] = quarterfinalists[j+1];
                    }
                }
            }
        }
        //Semi finals loop
        for (j = 0; j < 4; j += 2){
            calculateGameOdds(semifinalists[j], semifinalists[j+1], teamAWin, teamBWin);
            game = rand() % 100;
            if (game < (teamAWin * 100)){
                semis[semifinalists[j+1]]++;
                finalists[j/2] = semifinalists[j];
            }
            else{
                if (game < ((teamAWin + teamBWin) * 100)){
                    semis[semifinalists[j]]++;
                    finalists[j/2] = semifinalists[j+1];
                }
                else{
                    game = rand() % 2;
                    if (game == 0){
                        semis[semifinalists[j+1]]++;
                        finalists[j/2] = semifinalists[j];
                    }
                    else{
                        semis[semifinalists[j]]++;
                        finalists[j/2] = semifinalists[j+1];
                    }
                }
            }
        }
        //Finals
        calculateGameOdds(finalists[0], finalists[1], teamAWin, teamBWin);
        game = rand() % 100;
        if (game < (teamAWin * 100)){
            finals[finalists[1]]++;
            champion = finalists[0];
            winners[champion]++;
        }
        else{
            if (game < ((teamAWin + teamBWin) * 100)){
                finals[finalists[0]]++;
                champion = finalists[1];
                winners[champion]++;
            }
            else{
                game = rand() % 2;
                if (game == 0){
                    finals[finalists[1]]++;
                    champion = finalists[0];
                    winners[champion]++;
                }
                else{
                    finals[finalists[0]]++;
                    champion = finalists[1];
                    winners[champion]++;
                }
            }
        }
    }

    //Results output
    cout << "Team Name" << setw(11) << "Groups" << setw(10) << "Octos" << setw(10) << "Quarters";
    cout << setw(10) << "Semis" << setw(10) << "Finals" << setw(10) << "Winners" << endl;
    for (j = 0; j < 32; j++){
        stringTeam1 = teamName[j];
        cout << stringTeam1 << setw(20 - stringTeam1.length()) << setprecision(3) << ((group[j] + 0.0) / (counter + 0.0)) << setw(10);
        cout << ((octos[j] + 0.0) / (counter + 0.0)) << setw(10) << ((quarters[j] + 0.0) / (counter + 0.0)) << setw(10);
        cout << ((semis[j] + 0.0) / (counter + 0.0)) << setw(10) << ((finals[j] + 0.0) / (counter + 0.0)) << setw(10);
        cout << ((winners[j] + 0.0) / (counter + 0.0)) << endl;
        if ((j+1) % 4 == 0){
            cout << endl;
        }
    }
    cout << endl;
}

int main()
{
    bool exitprogram = false;
    int groupGame;
    int choice1, choice2, team1, team2;
    double teamAWin, teamBWin, draw;
    int i, j, k;
    float averageGoals = 0;

    srand(time(NULL));

    for (i = 0; i < 32; i++){
        averageGoals += teamStats[i][0] + teamStats[i][1];
    }
    averageGoals = averageGoals / 64;

    while (exitprogram == false){
        cout << "(1) Simulate World Cup" << endl;
        cout << "(2) Simulate single game" << endl;
        cout << "Anything else to exit" << endl;
        cin >> choice1;
        switch (choice1)
        {
        case 1:
            cout << "How many rounds do you want to simulate?" << endl;
            cout << "(1) for Group Stage only" << endl;
            cout << "(2) for first knockout round" << endl;
            cout << "(3) for quarter finals" << endl;
            cout << "(4) for semi finals" << endl;
            cout << "(5) for complete tournament" << endl;
            cin >> choice2;

            if (choice2 > 0 and choice2 < 6){
                simulator(choice2, averageGoals);
            }
            else {
                cout << "Invalid entry, please try again" << endl << endl;
            }
            break;
        case 2:
            cout << "Is the game a group stage game (1 for yes, 0 for no)" << endl;
            cin >> groupGame;

            cout << "Which teams do you want to simulate a game between?" << endl;
            cout << "(0) Brazil, (1) Croatia, (2) Mexico, (3) Cameroon" << endl;
            cout << "(4) Spain, (5) Netherlands, (6) Chile, (7) Australia" << endl;
            cout << "(8) Colombia, (9) Greece, (10) Ivory Coast, (11) Japan" << endl;
            cout << "(12) Uruguay, (13) Costa Rica, (14) England, (15) Italy" << endl;
            cout << "(16) Switzerland, (17) Ecuador, (18) France, (19) Honduras" << endl;
            cout << "(20) Argentina, (21) Bosnia-Herzegovina, (22) Iran, (23) Nigeria" << endl;
            cout << "(24) Germany, (25) Portugal, (26) USA, (27) Ghana" << endl;
            cout << "(28) Belgium, (29) Algeria, (30) Russia, (31) South Korea" << endl;
            cout << "Team 1: ";
            cin >> team1;
            cout << "Team 2: ";
            cin >> team2;
            if (team1 >= 0 and team1 < 32 and team2 >= 0 and team2 < 32){
                calculateGameOdds(team1, team2, teamAWin, teamBWin, averageGoals);
                if (groupGame != 1){
                    draw = 1 - teamAWin - teamBWin;
                    teamAWin += draw / 2;
                    teamBWin += draw / 2;
                }
                cout << teamName[team1] << ":" << teamAWin << ", " <<teamName[team2] << ":" << teamBWin << endl << endl;
            }
            else {
                cout << "Invalid entry, please try again" << endl << endl;
            }
            break;
        default:
            exitprogram = true;
            break;
        }
    }
    return 0;
}
