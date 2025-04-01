#!/usr/bin/python3

import random
import argparse


words = []
with open('/usr/share/dict/words', 'r') as word_list:
    for word in word_list:
        words.append(word[:-1])

parser = argparse.ArgumentParser()
parser.add_argument('-c','--files-count', type=int, required=True, help='How many files to generate', dest='files_count')
parser.add_argument('-m','--max-word-count', type=int, default=1000, help='Max number of words in file', dest='max_word_count')
args = parser.parse_args()

print(f'Generating {args.files_count} files')

random.seed()
for i in range(1, args.files_count + 1):
    expected_output = {}
    max_number_of_words = args.max_word_count
    while max_number_of_words >= 1:
        word_count = random.randint(1, max_number_of_words)
        word = ''.join(filter(str.isalnum, random.choice(words)))
        expected_output[word_count] = word
        max_number_of_words = max_number_of_words - word_count
    
    input = []
    for count, word in expected_output.items():
        for j in range(0, count):
            input.append(word)
    random.shuffle(input)
    with open(f'TestData/inputs/{i}.input', 'w') as input_file:
        input_file.write(' '.join(input))
    with open(f'TestData/expected_outputs/{i}.output', 'w+') as expected_output_file:
        for key, value in sorted(expected_output.items(), reverse=True):
            expected_output_file.write(f'{key} {value.lower()}\n')
