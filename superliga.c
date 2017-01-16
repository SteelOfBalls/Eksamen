#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WIN  3
#define TIE  1
#define LOSS  0
#define NUMBER_OF_TEAMS 12

typedef struct date{
	int day;
	int month;
	int year;
}date;

typedef struct time{
	int hour;
	int minute;
}time;

typedef struct match{
	int round;
	char weekday[4];
	date date;
	time time;
	char teamHome[4];
	char teamAway[4];
	int goalsHome;
	int goalsAway;
	float attendees;
}match;

typedef struct team{
	char name[4];
	int winHome;
	int winAway; 
}team;

typedef struct attendAcc{
	char name[4];
	int accAttend;
}attendAcc;

typedef struct teamScore{
	char name[4];
	int matches;
	int wins;
	int ties;
	int losses;
	int goals;
	int goalsAgainst;
	int goalDifference;
	int points;
}teamScore;

match resultToStruct (char *matchResults);
int goalsFourOrMoreTied(match *matchArray, int lines, match **matchFinder);
void roundLessThanTenGoals(match *matchArray, int lines, int *roundNumber, int *roundGoals);
int moreAwayVictories(match *matchArray, int lines, team **awayVictories);
void fewestAttendees(match *matchArray, int lines, attendAcc *attend);
int startBetween(match *matchArray, int lines, char *k1, char *k2, char *u, match **matchFinder);
int leagueTable(match *matchArray, int lines, teamScore *totalTeams);
int sort_points(const void *a, const void *b);
void printGoalsFourOrMoreTied(int tiedMatches, match *tiedMatches4PlusGoals);
void printRoundLessThanTenGoals(int roundNumber, int roundGoals);
void printMoreAwayVictories(int awayTeamVictory, team *awayVictories);
void printStartBetween(int countMatchesBetween, match *matchesBetween, char *k1, char *k2, char *u);

int main(const int argc, const char *argv[]){
	FILE *pMatchResults;
	char buff[255];
	char ch;
	int lines = 1;
	int i = 0;
	match *matchArray;
	match *tiedMatches4PlusGoals;
	team *awayVictories;
	int tiedMatches;
	int roundGoals;
	int roundNumber;
	int awayTeamVictory;
	attendAcc attend;
	match *matchesBetween;
	int countMatchesBetween = 0;
	char k1[] = "13:15";
	char k2[] = "14:15";
	char u[] = "Son";
	teamScore *teamTable;
	int finalStats;
	int userInterface_int = 0;

	teamTable = malloc(12 * sizeof(teamScore));

	pMatchResults = fopen("superliga-2015-2016.txt", "r");

	while(!feof(pMatchResults)){
		ch = fgetc(pMatchResults);
		if(ch == '\n'){
			lines++;
		}
	}
	rewind(pMatchResults);

	matchArray = malloc(lines * sizeof(match));

	while(!feof(pMatchResults)) {
		if (fgets(buff,255,pMatchResults)) {
			matchArray[i] = resultToStruct (buff);
			i++;
		}
	}
	
	if(argc > 1 && strcmp(argv[1], "--print") == 0){
		userInterface_int = -1;
	}else{

		/* Here the user is asked for an input, that prints the desired function*/

		printf("Hello! This program uses data from the Danish Superliga 2015-2016.\n"
			    "You now have the following options:\n"
			    "0\t - \tExit\n"
			    "1\t - \tShow tied matches with 4 or more goals\n"
			    "2\t - \tShow rounds with less than 10 goals \n"
			    "3\t - \tShow which teams have more away victories\n"
			    "4\t - \tShow which team had the fewest attendants\n"
			    "5\t - \tShow matches on a given date between two time spans\n"
			    "6\t - \tShow the final league table\n");
		printf("Please type in your desired option: ");
		scanf(" %d", &userInterface_int);
	}
	printf("\n");
	switch(userInterface_int){
		case -1:
			tiedMatches = goalsFourOrMoreTied(matchArray, lines, &tiedMatches4PlusGoals);
			printGoalsFourOrMoreTied(tiedMatches, tiedMatches4PlusGoals);
			roundLessThanTenGoals(matchArray, lines, &roundNumber, &roundGoals);
			printRoundLessThanTenGoals(roundNumber, roundGoals);
			awayTeamVictory = moreAwayVictories(matchArray, lines, &awayVictories);
			printf("The teams that had more away victories:\n\n");
			for(i = 0; i < awayTeamVictory; i++){
				printf("Team: %s  \nAway victories %d \nHome victories %d\n\n",
				awayVictories[i].name,
				awayVictories[i].winAway,
				awayVictories[i].winHome
				);
			}
			fewestAttendees(matchArray, lines, &attend);
			printf("Team with lowest attendant count:\n%s with %d attendants\n", attend.name, attend.accAttend);
			countMatchesBetween = startBetween(matchArray, lines, k1, k2, u, &matchesBetween);
			printStartBetween(countMatchesBetween, matchesBetween, k1, k2, u);
			finalStats = leagueTable(matchArray, lines, teamTable);
			printf("Pos \tHold \tK \tV \tU \tT \tMF \tP\n");
			for(i = 0; i < NUMBER_OF_TEAMS; i++){
				printf(" %d \t%s \t%d \t%d \t%d \t%d  \t%d \t%d\n",
				i + 1,
				teamTable[i].name,
				teamTable[i].matches,
				teamTable[i].wins,
				teamTable[i].ties,
				teamTable[i].losses,
				teamTable[i].goalDifference,
				teamTable[i].points
				);
			}
		break;
		case 0 :
			printf("Terminating program\n");
			exit(0);
			break;
		case 1 :
			tiedMatches = goalsFourOrMoreTied(matchArray, lines, &tiedMatches4PlusGoals);
			printGoalsFourOrMoreTied(tiedMatches, tiedMatches4PlusGoals);
			break;
		case 2 :
			roundLessThanTenGoals(matchArray, lines, &roundNumber, &roundGoals);
			printRoundLessThanTenGoals(roundNumber, roundGoals);
			break;
		case 3 :
			awayTeamVictory = moreAwayVictories(matchArray, lines, &awayVictories);
			printf("The teams that had more away victories:\n\n");
			for(i = 0; i < awayTeamVictory; i++){
				printf("Team: %s  \nAway victories %d \nHome victories %d\n\n",
				awayVictories[i].name,
				awayVictories[i].winAway,
				awayVictories[i].winHome
				);
			}
			break;
		case 4 :
			fewestAttendees(matchArray, lines, &attend);
			printf("Team with lowest attendant count:\n%s with %d attendants\n", attend.name, attend.accAttend);
			break;
		case 5 :
			countMatchesBetween = startBetween(matchArray, lines, k1, k2, u, &matchesBetween);
			printStartBetween(countMatchesBetween, matchesBetween, k1, k2, u);
			break;
		case 6 :
			finalStats = leagueTable(matchArray, lines, teamTable);
			printf("Pos \tHold \tK \tV \tU \tT \tMF \tP\n");
			for(i = 0; i < NUMBER_OF_TEAMS; i++){
				printf(" %d \t%s \t%d \t%d \t%d \t%d  \t%d \t%d\n",
				i + 1,
				teamTable[i].name,
				teamTable[i].matches,
				teamTable[i].wins,
				teamTable[i].ties,
				teamTable[i].losses,
				teamTable[i].goalDifference,
				teamTable[i].points
				);
			}
			break;
	}
	return 0;
}

/* This function copies the .txt file into an array of structs */

match resultToStruct (char *matchResults){
	match currentMatch;

	sscanf(matchResults," R%d %s %d/%d/%d %d.%d %s - %s %d - %d %f ", 
		&currentMatch.round,
		 currentMatch.weekday,
		&currentMatch.date.day,
		&currentMatch.date.month,
		&currentMatch.date.year,
		&currentMatch.time.hour,
		&currentMatch.time.minute,
		 currentMatch.teamHome,
		 currentMatch.teamAway,
		&currentMatch.goalsHome,
		&currentMatch.goalsAway,
		&currentMatch.attendees
	);

	return currentMatch;
}

/* This function finds tied matches with 4 or more goals scored */

int goalsFourOrMoreTied(match *matchArray, int lines, match **matchFinder){
	int i;
	int j=0;
	int *indexes;
	indexes = malloc(lines*sizeof(int));

	for(i = 0; i < lines; i++){
		if(matchArray[i].goalsHome == matchArray[i].goalsAway && (matchArray[i].goalsHome + matchArray[i].goalsAway) >= 4 ){
			indexes[j++] = i; 
		}
	}

	(*matchFinder) = (match*) malloc(j*sizeof(match));

	for(i = 0; i < j; i++){
		(*matchFinder)[i] = matchArray[indexes[i]];
	}

	free(indexes);

	return j;
}

/* This function finds rounds with less than 10 goals scored */

void roundLessThanTenGoals(match *matchArray, int lines, int *roundNumber, int *roundGoals){
	int i;
	int totalGoals = 0;
	int currentRound = 0;

	for (i = 0; i < lines; i++){
		if(currentRound == matchArray[i].round){
			totalGoals -= matchArray[i].goalsHome + matchArray[i].goalsAway;
			
		}else{
			if(totalGoals > 0){
				*roundGoals = 10 - totalGoals;
				*roundNumber = currentRound;
				return;
			}
			currentRound = matchArray[i].round;
			totalGoals = 10;
			i--;
		}
	}
}

/* This function finds which teams have won more away matches */

int moreAwayVictories(match *matchArray, int lines, team **awayVictories){
	int i, j;
	int isFound1;
	int isFound2;
	int *indexes;
	int k = 0;
	int homeIndex = 0;
	int awayIndex = 0;
	int foundTeams = 0;
	int numberOfTeams = (lines / matchArray[lines - 1].round)*2;
	team *totalTeams;

	totalTeams = calloc(numberOfTeams, sizeof(team));
	indexes = calloc(numberOfTeams, sizeof(int));

	for(i = 0; i < lines; i++){
		for(k = 0; k < foundTeams; k++){
			if(strcmp(totalTeams[k].name, matchArray[i].teamHome) == 0){
				isFound1 = 1;
				homeIndex = k;			
			}	
			if(strcmp(totalTeams[k].name, matchArray[i].teamAway) == 0){
				isFound2 = 1;
				awayIndex = k;		
			}	
		}

		if(!isFound1){
			strcpy(totalTeams[foundTeams].name, matchArray[i].teamHome);
			homeIndex = foundTeams;
			totalTeams[foundTeams].winHome = 0;
			totalTeams[foundTeams].winAway = 0;
			foundTeams++;
		}
		if(!isFound2){
			strcpy(totalTeams[foundTeams].name, matchArray[i].teamAway);
			awayIndex = foundTeams;
			totalTeams[foundTeams].winHome = 0;
			totalTeams[foundTeams].winAway = 0;
			foundTeams++;
		}

		if(matchArray[i].goalsHome == matchArray[i].goalsAway){
			continue;
		}else if(matchArray[i].goalsAway > matchArray[i].goalsHome){
			totalTeams[awayIndex].winAway += 1;	
		}else{
			totalTeams[homeIndex].winHome += 1;
		}
	}

	k=0;

	for (j = 0; j < foundTeams; j++){
		if(totalTeams[j].winAway > totalTeams[j].winHome){
			indexes[k] = j;
			k++;
		}
	}

	(*awayVictories) = (team *) malloc(k * sizeof(team));

	for(i = 0; i < k; i++){
		(*awayVictories)[i] = totalTeams[indexes[i]];
	}

	return k;
}

/* This function finds which team had the fewest attendants */

void fewestAttendees(match *matchArray, int lines, attendAcc *attend){
	int i, j, k;
	attendAcc *totalTeams;
	int foundTeams = 0;
	int numberOfTeams = (lines / matchArray[lines - 1].round)*2;
	int isFound = 0;
	int lowest = 99999999;

	totalTeams = calloc(numberOfTeams, sizeof(attendAcc));

	for(i = 0; i < lines; i++){
		if(matchArray[i].date.year == 2015){
			for(k = 0; k < foundTeams; k++){
				if(strcmp(totalTeams[k].name, matchArray[i].teamHome) == 0){
					isFound = 1;
					j = k;
					break;
				}	
			}
			if(!isFound){
				strcpy(totalTeams[foundTeams].name, matchArray[i].teamHome);
				totalTeams[foundTeams].accAttend += (matchArray[i].attendees * 1000);
				foundTeams++;
			}else{
				totalTeams[j].accAttend += (matchArray[i].attendees * 1000);
			}
		}

		isFound = 0;
	}



	for (i = 0; i < foundTeams; i++){
		if(totalTeams[i].accAttend < lowest){
			lowest = totalTeams[i].accAttend;
			*attend = totalTeams[i];
		}
	}
}

/* This function finds matches between two time spans on a set date */

int startBetween(match *matchArray, int lines, char *k1, char *k2, char *u, match **matchFinder){
	char tempTime[6];
	int i;
	int j = 0;
	int *indexes;

	indexes = malloc(lines*sizeof(int));

	for(i = 0; i < lines; i++){

		sprintf(tempTime, "%2.d:%2.d", matchArray[i].time.hour, matchArray[i].time.minute);
		if(strcmp(matchArray[i].weekday, u) == 0 && strcmp(tempTime, k1) >= 0 && strcmp(tempTime, k2) <= 0) {
			indexes[j++] = i; 
		}
	}

	(*matchFinder) = (match*) malloc(j*sizeof(match));

	for(i = 0; i < j; i++){
		(*matchFinder)[i] = matchArray[indexes[i]];
	}

	free(indexes);

	return j;
}

/* This function finds and sorts the scoreboard after every match has been played, thus displaying the final result of the league */

int leagueTable(match *matchArray, int lines, teamScore *totalTeams){
	int i, j;
	int isFound1=0;
	int isFound2=0;
	int homeIndex;
	int awayIndex;
	int k = 0;
	int foundTeams = 0;

	for(i = 0; i < lines; i++){
		for(k = 0; k < foundTeams; k++){
			if(strcmp(totalTeams[k].name, matchArray[i].teamHome) == 0){
				isFound1 = 1;
				homeIndex = k;			
			}	
			if(strcmp(totalTeams[k].name, matchArray[i].teamAway) == 0){
				isFound2 = 1;
				awayIndex = k;		
			}
		}

		if(!isFound1){
			strcpy(totalTeams[foundTeams].name, matchArray[i].teamHome);
			homeIndex = foundTeams;
			totalTeams[foundTeams].matches = 0;
			totalTeams[foundTeams].wins = 0;
			totalTeams[foundTeams].ties = 0;
			totalTeams[foundTeams].losses = 0;
			totalTeams[foundTeams].goals = 0;
			totalTeams[foundTeams].goalsAgainst = 0;
			totalTeams[foundTeams].goalDifference = 0;
			totalTeams[foundTeams].points = 0;
			foundTeams++;
		}
		if(!isFound2){
			strcpy(totalTeams[foundTeams].name, matchArray[i].teamAway);
			awayIndex = foundTeams;
			totalTeams[foundTeams].matches = 0;
			totalTeams[foundTeams].wins = 0;
			totalTeams[foundTeams].ties = 0;
			totalTeams[foundTeams].losses = 0;
			totalTeams[foundTeams].goals = 0;
			totalTeams[foundTeams].goalsAgainst = 0;
			totalTeams[foundTeams].goalDifference = 0;
			totalTeams[foundTeams].points = 0;
			foundTeams++;
		}
		if(matchArray[i].goalsHome == matchArray[i].goalsAway){
			totalTeams[homeIndex].goals += matchArray[i].goalsHome;
			totalTeams[awayIndex].goals += matchArray[i].goalsAway;

			totalTeams[homeIndex].goalsAgainst += matchArray[i].goalsAway;
			totalTeams[awayIndex].goalsAgainst += matchArray[i].goalsHome;
			
			totalTeams[homeIndex].ties += 1;
			totalTeams[awayIndex].ties += 1;

			totalTeams[homeIndex].points += TIE;
			totalTeams[awayIndex].points += TIE;
			
			totalTeams[awayIndex].matches += 1;
			totalTeams[homeIndex].matches += 1;

		}else if(matchArray[i].goalsAway > matchArray[i].goalsHome){
			totalTeams[awayIndex].wins += 1;
			totalTeams[awayIndex].points += WIN;

			totalTeams[homeIndex].goalsAgainst += matchArray[i].goalsAway;
			totalTeams[awayIndex].goalsAgainst += matchArray[i].goalsHome;

			totalTeams[homeIndex].goals += matchArray[i].goalsHome;
			totalTeams[awayIndex].goals += matchArray[i].goalsAway;

			totalTeams[homeIndex].losses += 1;
			totalTeams[homeIndex].points += LOSS;

			totalTeams[awayIndex].matches += 1;
			totalTeams[homeIndex].matches += 1;
		}else{
			totalTeams[homeIndex].wins += 1;
			totalTeams[homeIndex].points += WIN;

			totalTeams[homeIndex].goalsAgainst += matchArray[i].goalsAway;
			totalTeams[awayIndex].goalsAgainst += matchArray[i].goalsHome;

			totalTeams[homeIndex].goals += matchArray[i].goalsHome;
			totalTeams[awayIndex].goals += matchArray[i].goalsAway;

			totalTeams[awayIndex].losses += 1;
			totalTeams[awayIndex].points += LOSS;

			totalTeams[awayIndex].matches += 1;
			totalTeams[homeIndex].matches += 1;
		}

		isFound1 = 0;
		isFound2 = 0;
	}

	for (j = 0; j < foundTeams; j++){
		totalTeams[j].goalDifference = totalTeams[j].goals - totalTeams[j].goalsAgainst;
	}

	qsort(totalTeams, foundTeams, sizeof(teamScore), sort_points);

	return foundTeams;
}

/* This function is used for sorting the teams in the function "leagueTable" */

int sort_points(const void *a, const void *b){
	teamScore *ta = (teamScore*) a;
	teamScore *tb = (teamScore*) b;

	return (tb->points - ta->points);
}

/* This function prints goalsFourOrMoreTied */

void printGoalsFourOrMoreTied(int tiedMatches, match *tiedMatches4PlusGoals){
	int i;

	printf("Tied matches:\n");
	for(i = 0; i < tiedMatches; i++){

		printf("%d: \tRound: %d \tDay of Week: %s \tDate: %d/%d/%d  \tTime: %.2d.%.2d \tTeams: %s - %s \tResult: %d - %d\n",
			i + 1,
			tiedMatches4PlusGoals[i].round,
			tiedMatches4PlusGoals[i].weekday,
			tiedMatches4PlusGoals[i].date.day,
			tiedMatches4PlusGoals[i].date.month,
			tiedMatches4PlusGoals[i].date.year,
			tiedMatches4PlusGoals[i].time.hour,
			tiedMatches4PlusGoals[i].time.minute,
			tiedMatches4PlusGoals[i].teamHome,
			tiedMatches4PlusGoals[i].teamAway,
			tiedMatches4PlusGoals[i].goalsHome,
			tiedMatches4PlusGoals[i].goalsAway
		);

	}
}

/* This function prints roundLessThanTenGoals */

void printRoundLessThanTenGoals(int roundNumber, int roundGoals){

	printf("Rounds containing less than ten goals\nRound number: %d  Goals: %d\n", roundNumber, roundGoals);

}

/* This function prints moreAwayVictories */

void printMoreAwayVictories(int awayTeamVictory, team *awayVictories){
	int i;

	printf("The teams that had more away victories:\n\n");
	for(i = 0; i < awayTeamVictory; i++){

		printf("Team: %s  \nAway victories %d \nHome victories %d\n\n",
			awayVictories[i].name,
			awayVictories[i].winAway,
			awayVictories[i].winHome
		);
	}
}

/* This function prints startBetween */

void printStartBetween(int countMatchesBetween, match *matchesBetween, char *k1, char *k2, char *u){
	int i;

	printf("Matches between %s - %s on %s: \n", k1, k2, u );

	for(i = 0; i < countMatchesBetween; i++){

		printf("%d: \tRound: %d \tDay of Week: %s \tDate: %d/%d/%d  \tTime: %.2d.%.2d  \tTeams: %s - %s \tResult: %d - %d\n",
			i + 1,
			matchesBetween[i].round,
			matchesBetween[i].weekday,
			matchesBetween[i].date.day,
			matchesBetween[i].date.month,
			matchesBetween[i].date.year,
			matchesBetween[i].time.hour,
			matchesBetween[i].time.minute,
			matchesBetween[i].teamHome,
			matchesBetween[i].teamAway,
			matchesBetween[i].goalsHome,
			matchesBetween[i].goalsAway
		);
	}
}

/* Notér venligst denne kode, som skal angives på formularen:  1480590002 */
