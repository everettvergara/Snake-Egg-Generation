A Better Way to Generate Eggs in Snake
by: Everett Gaius S. Vergara

Problem:
--
The Classic Snake is one of the shortest and easiest game to develop but most programmers fall prey to 
writing an algorithm that deteriorates the performance of generating the snake egg coordinates as the snake gets longer.

All programs I've seen are written in a manner similar to Nibbles (A 1990 Classic Snake written in QBasic).

// Pseudo code
do {
    r = rand() % n_; // n_ = Max Rows
    c = rand() % m_; // m_ = Max Cols
} while(state[r][c] != USED);
    
Supposed you have a 50(n_) x 80(m_) Snake Arena, and there are only 5 free spaces remaining for the next egg to be placed.
The chances that the next egg will fall in the 5 free spaces is 5 / 4000, or 1 in 800 repetitions of the above routine. And for each
repetition you have to get the values of row and col. 

The time complexity of this routine: 
at Best: O(1)
at Worst: O(n_ x m_)
not Best and not Worst: O({Unused_States}/(n_ x m_})


My proposed solution is to:
-- 
1) Maintain a row-major single dimensional vector of state_ so you don't have to call rand() twice for row and col.

i = r * m_ + c;
state_[i];

The free and used states are identified in the state_ vector thru a divider (div_) index.

Visualization of the state_[] vector
ix          0   1   2   3   4   5   6   7   8   9   ...
divider(5)  |<--- free  --->|   |<--- used  --------->|
value       ?   ?   ?   ?   ?   ?   ?   ?   ?   ?   ...

To get the next free egg location just do:

ix = rand() % div_ --;
egg_ix = state_[ix];

If there's a need to derive back r and c from ix, just do:

c = egg_ix % m_;
r = egg_ix / m_; where / denotes integer division

2) and Create a mapper_ to state_[ix] to check whether mapper[i] is already used or not (free) by performing mapper_[state_[i]] >= div_. The reverse means it is free.

Visualization of the mapper_[] vector
ix          0   1   2   3   4   5   6   7   8   9   ...
value       ?   ?   ?   ?   ?   ?   ?   ?   ?   ?   ...
            |
            +--> mapper_[i] points to index where mapper_[i] is located in state_[]

--

With these two methods (2), the space complexity is increased from n_ x m_, to 3 x n_ x m_ but we are able to reduce the time complexity of finding the location of the next egg from O({Used_States}/(n_ x m_}) to O(1) - a great trade-off!

I've written a FieldStateMgr Class in C++ to implement the full scope of this algorithm. 

A simple non-ECS snake in main.cpp is used to demonstrate how FieldStateMgr can be implemented in your next Snake game.
Use 'w', 'a', 's', 'd' to navigate the snake. Press space ' ' to auto play until no more possible moves are left. 
And hit 'esc' anytime to quit.
