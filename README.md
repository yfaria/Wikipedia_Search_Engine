# Wikipedia_Search_Engine-EDA
Project of Data Structures and Algorithms (EDA)

## Alunos:
* Danillo Souza Fiorenza
* Renan Ferreira Moura
* Yuri Luis Faria Silva

You can compile the engine by yourself; it's the file engine.cpp

A video explaining the work is [this](https://youtu.be/Oe5BwuJcyUU)

To use, you must unzip [this](https://drive.google.com/open?id=1KO26qevOJV0zXWZuFsWTu5Gd8jL8vuLm) on the folder with all those files; it's important to maint it there and it's name. Additionally, you should put [this] (https://drive.google.com/open?id=1vBAM9ogjZvkheHhHvxF5BqH_JsFg3Oq6) on the folder too

```
g++ engine.cpp -std=c++11 -o engine
```
may compile it just fine; but we put the executable there.

To use it properly, you have to wait for the engine to load (it takes some minutes and some RAM).
After loading, you enter queries you wish. When you do it, it displays the number of results that match with your search, the time that the program took to find them and the first 20 results. To see the other 20 results, input 'm'; to go back and see the previous results, enter 'v'; to make another query, input 'n'; to open an article in the list of the 20, enter its number.

The search of compound terms do not return the articles with these terms adjacents; just means that all words in the search are in the article.
