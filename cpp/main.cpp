/*
 * Copyright (C) 2020 Maria Santamaria
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;


void computeShanonEntropy(string filename, int wordLen)
{
  ifstream binFile(filename, ifstream::binary);
  binFile.seekg(0, binFile.end);
  long length = binFile.tellg();
  binFile.seekg (0, binFile.beg);

  char* buffer = new char[length];
  binFile.read(buffer, length);
  binFile.close();

  map<uint64_t, uint64_t> wordCount;

  long idx = 0;
  uint64_t word = 0;

  for (int b = 0; b < length; b++)
  {
    for (int i = 7; i >= 0; i--)
    {
      int bit = ((buffer[b] >> i) & 1) << (wordLen - idx - 1);
      word |= bit;
      idx++;

      if (idx == wordLen)
      {
        if (wordCount.find(word) == wordCount.end())
        {
          wordCount[word] = 1;
        }
        else
        {
          wordCount[word]++;
        }
        idx = 0;
        word = 0;
      }
    }
  }

  delete[] buffer;

  long totalWords = length * 8 / wordLen;
  double entropy = 0;

  cerr << "PMT: " << endl;

  map<uint64_t, uint64_t>::iterator it = wordCount.begin();
  double probability = 0;
  for (auto element: wordCount)
  {
    probability = (double)element.second / (double)totalWords;
    cerr << "Word: ";
    for (int j = wordLen - 1; j >= 0; j--)
    {
      cerr << ((element.first >> j) & 1);
    }
    cerr << "; Probability: " << probability << endl;
    entropy -= probability == 0 ? 0 : probability * log2(probability);
  }

  double minSize = totalWords * entropy;

  cout << endl << "Entropy: " << entropy << endl << "Min size: " << minSize << endl;
}


int main (int argc, char* argv[])
{
  string filename = string(argv[1]);
  int wordLen = atoi(argv[2]);
  computeShanonEntropy(filename, wordLen);

  return 0;
}
