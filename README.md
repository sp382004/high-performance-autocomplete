High-Performance Autocomplete Utility

An efficient autocomplete engine implemented in C, designed to handle large datasets under memory constraints.  

Features 
- Prefix-based autocomplete 
- Custom merge sort for lexicographic ordering 
- Binary search for prefix matching 
- Weight-based ranking of suggestions  

Technologies 
- C 
- File I/O 
- Algorithms and Data Structures  

## Usage 
```bash 
gcc -o autocomplete main.c 
./autocomplete dictionary.txt queries.txt