# High-Performance Autocomplete Utility

An efficient autocomplete engine implemented in C, designed to handle large datasets under memory constraints.  

## Features 
- Prefix-based autocomplete for incomplete query strings
- Custom merge sort for lexicographic ordering of the knowledge base
- Binary search for fast prefix matching
- Weight-based ranking of suggestions (top-k results)
- Optimized for repeated queries and low-latency responses 

## Technologies & Concepts
- C (manual memory management)
- File I/O
- Custom sorting algorithms (no standard library sorting)
- Binary search and string processing
- Performance-oriented design 

## Build & Run
```bash
gcc main.c -o autocomplete
./autocomplete dictionary.txt queries.txt
