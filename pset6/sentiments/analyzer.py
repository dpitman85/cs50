import nltk
from nltk.tokenize import TweetTokenizer
import io

#Define lists to store words for analysis


class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        #Open files
        p = open(positives, 'r')
        n = open(negatives, 'r')

        #init lists
        self.positives = []
        self.negatives = []

        #Populate lists
        for line in p:
            if not line.startswith((';',' ','\n')):
                self.positives.append(line[:-1])
        for line in n:
            if not line.startswith((';',' ','\n')):
                self.negatives.append(line[:-1])


    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        #Init score counter
        score = 0

        #tokenize text
        tokenizer = TweetTokenizer()
        tokens = tokenizer.tokenize(text)

        #loop through words
        for word in tokens:
            #find word in list and adjust score
            if word.lower() in self.positives:
                score += 1
            elif word.lower() in self.negatives:
                score -= 1

        return score
