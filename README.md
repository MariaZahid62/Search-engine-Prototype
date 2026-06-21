## Search Engine Prototype <br>
A DSA-based intelligent search engine prototype built in **C++** that demonstrates how modern search engines process, index, retrieve, and rank information efficiently.

The project implements advanced **Data Structures, Algorithms, Object-Oriented Programming, File Handling, and Information Retrieval techniques** to create a functional search system.

The system allows users to import documents, build indexes, perform intelligent searches, receive auto-complete suggestions, correct spelling mistakes, rank results based on relevance, analyze search behavior, and export reports.

---

## Features

## 1. Document Import & File Indexing

- Import multiple `.txt` documents using file handling.
- Read and process document content automatically.
- Extract and preprocess words:
  - Convert text to lowercase
  - Remove punctuation
  - Tokenize words
- Store document information:
  - Document ID
  - File name
  - Word count
  - Indexed data

**Concepts Used:**
- File Handling
- Strings
- Vectors
- Data Preprocessing

---

## 2. Inverted Index Search Engine

Implemented an inverted index structure:
Features:

- Fast keyword lookup
- Efficient document retrieval
- Avoids scanning every document during search

**DSA Used:**

- Hash Tables
- Maps
- Vectors

---

## 3. Trie-Based Auto Complete

Provides real-time search suggestions while typing.
Features:

- Prefix-based searching
- Fast word suggestions
- Improved search experience

**DSA Used:**

- Trie Tree

---

## 4. TF-IDF Search Ranking

Ranks search results according to relevance.

Instead of displaying random matches, results are scored using:

- Term Frequency (TF)
- Inverse Document Frequency (IDF)


**Concepts Used:**

- Information Retrieval
- Mathematical Ranking
- Sorting Algorithms

---

## 5. Boolean Search Queries

Supports advanced search operations:
Operations:

- AND → Documents containing all keywords
- OR → Documents containing any keyword
- NOT → Removes unwanted results

**DSA Used:**

- Sets
- Set Operations

---

## 6. Spell Correction System

Detects incorrect search queries and suggests corrections.
Features:

- Finds closest matching words
- Improves search accuracy

**Algorithm Used:**

- Levenshtein Distance
- Dynamic Programming

---

## 7. Fuzzy Search

Allows searching even with typing mistakes.
Features:

- Similar word matching
- Error-tolerant searching

**Algorithm Used:**

- Edit Distance

---

## 8. LRU Search Cache

Optimizes repeated searches.
Features:

- Stores recently searched queries
- Returns cached results instantly
- Removes least recently used data automatically

Implementation:

- Hash Map
- Doubly Linked List

Benefits:

- Faster response time
- Improved performance

---

## 9. Search Analytics Dashboard

Tracks search engine activity and performance.

Displays:
-Total Documents Indexed
- Total Searches
- Most Searched Keyword
- Average Search Time
- Cache Hit Rate:
  
Tracks:

- Search history
- Popular keywords
- Query frequency
- Performance statistics

**DSA Used:**

- Hash Maps
- Priority Queue

---

## 10. Import / Export Data System

Allows saving and restoring search engine data.

## Import Features:

- Load document datasets
- Restore indexed data
- Continue previous sessions


## Export Features:

Generates:

- Search result reports
- Search history
- Analytics reports
- Keyword frequency files

---  

##Performance Goals

The system focuses on:

Fast document indexing
Efficient keyword retrieval
Relevance-based search results
Optimized repeated queries using caching
Scalable search architecture

---

## Author
Maria Zahid
- Student – BS Computer Science, Bahria University
- www.linkedin.com/in/maria-zahid-056a87327
- mariazahid404@gmail.com

## License
This project is open-source and available under the MIT License.









