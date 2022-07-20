# Soccer-Data-Analysis

The files in this repository include: a World Cup simulator I built in 2014, a python notebook to analyze SPI performance against the betting market, and a k-Nearest Neighbors model to predict profitable bets. 

The simulator used SPI ratings for relative team strength. These were fed into a poisson distribution to calculate probabilities for each outcome for each game, and then the tournament was simulated 10000 times to predict likely outcomes.

The notebook analyzying SPI performance against the market was a research project I used as a foundation for data manipulation in python. I have reused much of this code in the CalcWinPercentage notebook.

CalcWinPercentage uses a k-Nearest Neighbors model to predict which bets are most profitable given home and away team relative strength and the implied probablity of the betting markets.
