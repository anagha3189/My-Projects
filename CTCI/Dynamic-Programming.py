import sys
class Solution(object):
	def numberOfWaysFinalScore(self, final_score, individual_scores):
		row = len(individual_scores)
		col = final_score + 1
		numberOfWays = [[0 for x in range(row)] for y in range(col)]
		
		for i in range(len(individual_scores)):
			numberOfWays[i][0] = 1
			for j in range(1, final_score+1):
				if(i >= 1):
					without_this_play = numberOfWays[i-1][j] 
				else:
					without_this_play = 0
		
				if(j >= individual_scores[i]):
					with_this_play = numberOfWays[i][j-individual_scores[i]]
				else:
					with_this_play = 0

		return numberOfWays[row-1][col-1]
			

if __name__ == "__main__":
	final_score = 9
	print (Solution().numberOfWaysFinalScore(final_score , [2, 3, 7]))
