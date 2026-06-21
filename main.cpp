#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <climits>
#include <iterator>
#include <list>
#include <chrono>
#include <queue>
#include <windows.h>

using namespace std;

void setColor(int color)
{
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        color
    );
}

struct Document
{
    int id;
    string fileName;
    string content;
    int wordCount;
};
class TrieNode
{
public:

    bool isEnd;

    unordered_map<char, TrieNode*> children;

    TrieNode()
    {
        isEnd = false;
    }
};

class Trie
{
private:

    TrieNode* root;

    void collectWords(
        TrieNode* node,
        string currentWord,
        vector<string>& results)
    {
        if (node->isEnd)
        {
            results.push_back(currentWord);
        }

        for (auto child : node->children)
        {
            collectWords(
                child.second,
                currentWord + child.first,
                results);
        }
    }

public:

    Trie()
    {
        root = new TrieNode();
    }

    void insert(string word)
    {
        TrieNode* current = root;

        for (char ch : word)
        {
            if (current->children.find(ch)
                == current->children.end())
            {
                current->children[ch]
                    = new TrieNode();
            }

            current =
                current->children[ch];
        }

        current->isEnd = true;
    }

    vector<string> getSuggestions(string prefix)
    {
        TrieNode* current = root;

        for (char ch : prefix)
        {
            if (current->children.find(ch)
                == current->children.end())
            {
                return {};
            }

            current =
                current->children[ch];
        }

        vector<string> suggestions;

        collectWords(
            current,
            prefix,
            suggestions);

        return suggestions;
    }
};

class LRUCache
{

private:

    int capacity;


    list<pair<string, vector<string>>> cacheList;


    unordered_map<
        string,
        list<pair<string, vector<string>>>::iterator
    > cacheMap;



public:


    int hits = 0;

    int misses = 0;



    LRUCache(int size)
    {
        capacity = size;
    }



    bool contains(string key)
    {

        if (cacheMap.find(key)
            != cacheMap.end())
        {
            hits++;
            return true;
        }


        misses++;

        return false;
    }




    vector<string> get(string key)
    {

        auto position =
            cacheMap[key];


        cacheList.splice(
            cacheList.begin(),
            cacheList,
            position
        );


        return position->second;
    }





    void put(string key, vector<string> value)
    {


        if (cacheMap.find(key)
            != cacheMap.end())
        {

            auto position =
                cacheMap[key];


            position->second = value;


            cacheList.splice(
                cacheList.begin(),
                cacheList,
                position
            );


            return;
        }




        if (cacheList.size() == capacity)
        {

            auto last =
                cacheList.back();


            cacheMap.erase(
                last.first
            );


            cacheList.pop_back();

        }




        cacheList.push_front(
            {
                key,
                value
            }
        );


        cacheMap[key] =
            cacheList.begin();


    }




    void displayCache()
    {

        cout << "\n====================================\n";
        cout << "LRU SEARCH CACHE\n";
        cout << "====================================\n";


        int count = 1;


        for (auto item : cacheList)
        {

            cout
                << count++
                << ". "
                << item.first
                << endl;

        }



        cout << "\nCache Hits   : "
            << hits
            << endl;


        cout << "Cache Misses : "
            << misses
            << endl;


    }

};

class SearchEngine
{
private:

    LRUCache cache = LRUCache(5);

    Trie trie;

    set<string> vocabulary;

    vector<Document> documents;

    unordered_map<string, unordered_map<int, int>> invertedIndex;

    unordered_map<string, int> keywordFrequency;

    vector<string> searchHistory;

    unordered_map<string, int> searchFrequency;

    vector<string> lastSearchResults;

    double totalSearchTime = 0;

    int totalSearches = 0;

public:

    string cleanWord(string word)
    {
        string result;

        for (char ch : word)
        {
            if (isalpha(ch))
            {
                result += tolower(ch);
            }
        }

        return result;
    }

    int levenshteinDistance(string a, string b)
    {
        int n = a.length();
        int m = b.length();

        vector<vector<int>> dp(
            n + 1,
            vector<int>(m + 1)
        );

        for (int i = 0; i <= n; i++)
            dp[i][0] = i;

        for (int j = 0; j <= m; j++)
            dp[0][j] = j;

        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= m; j++)
            {
                if (a[i - 1] == b[j - 1])
                {
                    dp[i][j] =
                        dp[i - 1][j - 1];
                }
                else
                {
                    dp[i][j] =
                        1 +
                        min(
                            dp[i - 1][j],
                            min(
                                dp[i][j - 1],
                                dp[i - 1][j - 1]
                            )
                        );
                }
            }
        }

        return dp[n][m];
    }

    string getClosestWord(string input)
    {
        int bestDistance = INT_MAX;

        string closestWord = "";

        for (string word : vocabulary)
        {
            int distance =
                levenshteinDistance(
                    input,
                    word
                );

            if (distance < bestDistance)
            {
                bestDistance = distance;
                closestWord = word;
            }
        }

        return closestWord;
    }

    void fuzzySearch()
    {
        string keyword;

        cout
            << "\nEnter Keyword: ";

        cin >> keyword;

        keyword = cleanWord(keyword);

        if (invertedIndex.find(keyword)
            != invertedIndex.end())
        {
            searchKeywordTFIDF(keyword);
            return;
        }

        string suggestion =
            getClosestWord(keyword);

        cout
            << "\nNo Exact Match Found.\n";

        cout
            << "Did you mean: "
            << suggestion
            << " ?\n";

        char choice;

        cout
            << "\nSearch using suggested word? (Y/N): ";

        cin >> choice;

        if (choice == 'Y' || choice == 'y')
        {
            searchKeywordTFIDF(suggestion);
        }
    }

    void booleanSearch()
    {
        string word1;
        string op;
        string word2;

        cout << "\nEnter Query (Example: data AND structures)\n";
        cin >> word1 >> op >> word2;

        word1 = cleanWord(word1);
        word2 = cleanWord(word2);

        set<int> result;

        set<int> docs1;
        set<int> docs2;

        if (invertedIndex.find(word1) != invertedIndex.end())
        {
            for (auto item : invertedIndex[word1])
            {
                docs1.insert(item.first);
            }
        }

        if (invertedIndex.find(word2) != invertedIndex.end())
        {
            for (auto item : invertedIndex[word2])
            {
                docs2.insert(item.first);
            }
        }

        if (op == "AND" || op == "and")
        {
            set_intersection(
                docs1.begin(),
                docs1.end(),
                docs2.begin(),
                docs2.end(),
                inserter(result, result.begin())
            );
        }

        else if (op == "OR" || op == "or")
        {
            set_union(
                docs1.begin(),
                docs1.end(),
                docs2.begin(),
                docs2.end(),
                inserter(result, result.begin())
            );
        }

        else if (op == "NOT" || op == "not")
        {
            set_difference(
                docs1.begin(),
                docs1.end(),
                docs2.begin(),
                docs2.end(),
                inserter(result, result.begin())
            );
        }

        else
        {
            cout << "\nInvalid Operator!\n";
            return;
        }

        if (result.empty())
        {
            cout << "\nNo Results Found!\n";
            return;
        }

        cout << "\n====================================\n";
        cout << "BOOLEAN SEARCH RESULTS\n";
        cout << "====================================\n";

        for (int docID : result)
        {
            cout << "\nFile : "
                << documents[docID].fileName
                << endl;

            string snippet =
                documents[docID].content.substr(0, 80);

            cout << "Snippet : "
                << snippet
                << "...\n";
        }
    }

    void autoComplete()
    {
        string prefix;

        cout
            << "\nEnter Prefix: ";

        cin >> prefix;

        prefix = cleanWord(prefix);

        vector<string> suggestions =
            trie.getSuggestions(prefix);

        if (suggestions.empty())
        {
            cout
                << "\nNo Suggestions Found.\n";

            return;
        }

        cout
            << "\n====================================\n";

        cout
            << "AUTO COMPLETE SUGGESTIONS\n";

        cout
            << "====================================\n";

        int count = 1;

        for (string word : suggestions)
        {
            cout
                << count++
                << ". "
                << word
                << endl;

            if (count > 10)
            {
                break;
            }
        }
    }

    void analyticsDashboard()
    {

        setColor(13);
        cout << "\n====================================\n";
        cout << "       SEARCH ANALYTICS DASHBOARD\n";
        cout << "====================================\n";
        setColor(15);


        cout
            << "Total Documents Indexed : "
            << documents.size()
            << endl;



        cout
            << "Total Searches          : "
            << totalSearches
            << endl;




        set<string> unique;


        for (string s : searchHistory)
        {
            unique.insert(s);
        }



        cout
            << "Unique Searches         : "
            << unique.size()
            << endl;





        // MOST SEARCHED TERM


        string popular = "";

        int highest = 0;



        for (auto item : searchFrequency)
        {

            if (item.second > highest)
            {

                highest = item.second;

                popular = item.first;

            }

        }



        cout
            << "Most Searched Term      : "
            << popular
            << endl;





        double averageTime = 0;



        if (totalSearches > 0)
        {

            averageTime =
                totalSearchTime / totalSearches;

        }



        cout
            << "Average Search Time     : "
            << fixed
            << setprecision(5)
            << averageTime
            << " sec"
            << endl;






        // CACHE PERFORMANCE


        double hitRate = 0;



        int totalCacheOperations =
            cache.hits + cache.misses;



        if (totalCacheOperations > 0)
        {

            hitRate =
                ((double)cache.hits /
                    totalCacheOperations)
                * 100;

        }



        cout
            << "Cache Hit Rate          : "
            << fixed
            << setprecision(2)
            << hitRate
            << "%"
            << endl;







        // PRIORITY QUEUE


        priority_queue<
            pair<int, string>
        > ranking;



        for (auto item : searchFrequency)
        {

            ranking.push(
                {
                item.second,
                item.first
                });

        }




        cout << "\nTop Search Keywords\n";

        cout << "------------------------------------\n";



        int count = 1;


        while (!ranking.empty()
            && count <= 5)
        {


            cout
                << count++
                << ". "
                << ranking.top().second
                << " ("
                << ranking.top().first
                << " searches)"
                << endl;


            ranking.pop();


        }



    }

    void exportSearchResults()
    {
        ofstream file("search_results.txt");

        if (!file)
        {
            cout << "\nExport Failed!\n";
            return;
        }

        for (string result : lastSearchResults)
        {
            file << result << endl;
        }

        file.close();

        cout
            << "\nsearch_results.txt exported successfully!\n";
    }

    void importDocument(string fileName)
    {
        ifstream file(fileName);

        if (!file)
        {
            setColor(12);
            cout << "\nFile Not Found!\n";
            setColor(15);
            return;
        }

        string line;
        string content;

        int words = 0;

        Document doc;

        doc.id = documents.size();
        doc.fileName = fileName;

        while (getline(file, line))
        {
            content += line + " ";

            stringstream ss(line);

            string word;

            while (ss >> word)
            {
                word = cleanWord(word);

                if (word.empty())
                    continue;

                words++;

                invertedIndex[word][doc.id]++;

                keywordFrequency[word]++;

                trie.insert(word);

                vocabulary.insert(word);
            }
        }

        doc.content = content;
        doc.wordCount = words;

        documents.push_back(doc);

        setColor(10);
        cout << "\nDocument Indexed Successfully!\n";
        setColor(15);
    }

    void exportSearchHistory()
    {
        ofstream file("search_history.txt");

        if (!file)
        {
            cout << "\nExport Failed!\n";
            return;
        }

        for (string query : searchHistory)
        {
            file << query << endl;
        }

        file.close();

        cout << "\nsearch_history.txt exported successfully!\n";
    }

    void exportKeywordReport()
    {
        ofstream file("keyword_frequency.csv");

        if (!file)
        {
            cout << "\nExport Failed!\n";
            return;
        }

        file << "Keyword,Frequency\n";

        for (auto item : keywordFrequency)
        {
            file
                << item.first
                << ","
                << item.second
                << "\n";
        }

        file.close();

        cout
            << "\nkeyword_frequency.csv exported successfully!\n";
    }

    void exportAnalyticsReport()
    {
        ofstream file("analytics_report.csv");

        if (!file)
        {
            cout << "\nExport Failed!\n";
            return;
        }

        set<string> unique;

        for (string query : searchHistory)
        {
            unique.insert(query);
        }

        string popular = "";

        int highest = 0;

        for (auto item : searchFrequency)
        {
            if (item.second > highest)
            {
                highest = item.second;
                popular = item.first;
            }
        }

        double avgTime = 0;

        if (totalSearches > 0)
        {
            avgTime =
                totalSearchTime /
                totalSearches;
        }

        double hitRate = 0;

        int totalCache =
            cache.hits +
            cache.misses;

        if (totalCache > 0)
        {
            hitRate =
                ((double)cache.hits /
                    totalCache) * 100;
        }

        file << "Metric,Value\n";

        file << "Documents Indexed,"
            << documents.size()
            << "\n";

        file << "Total Searches,"
            << totalSearches
            << "\n";

        file << "Unique Searches,"
            << unique.size()
            << "\n";

        file << "Most Searched,"
            << popular
            << "\n";

        file << "Average Search Time,"
            << avgTime
            << "\n";

        file << "Cache Hit Rate,"
            << hitRate
            << "\n";

        file.close();

        cout
            << "\nanalytics_report.csv exported successfully!\n";
    }

    void importSearchHistory()
    {
        ifstream file("search_history.txt");

        if (!file)
        {
            cout
                << "\nsearch_history.txt not found!\n";

            return;
        }

        searchHistory.clear();

        string line;

        while (getline(file, line))
        {
            searchHistory.push_back(line);

            searchFrequency[line]++;
        }

        file.close();

        cout
            << "\nSearch History Restored Successfully!\n";
    }

    void dataManagement()
    {
        int choice;

        cout << "\n====================================\n";
        cout << "        DATA MANAGEMENT\n";
        cout << "====================================\n";
        cout << "1. Export Search Results\n";
        cout << "2. Export Search History\n";
        cout << "3. Export Keyword Report\n";
        cout << "4. Export Analytics Report\n";
        cout << "5. Import Search History\n";
        cout << "6. Back\n";
        cout << "====================================\n";

        cout << "\nEnter Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            exportSearchResults();
            break;

        case 2:
            exportSearchHistory();
            break;

        case 3:
            exportKeywordReport();
            break;

        case 4:
            exportAnalyticsReport();
            break;

        case 5:
            importSearchHistory();
            break;

        case 6:
            return;

        default:
            cout << "\nInvalid Choice!\n";
        }
    }

    void importMultipleDocuments()
    {
        int n;

        cout << "\nHow Many Files? ";
        cin >> n;

        for (int i = 0; i < n; i++)
        {
            string fileName;

            cout << "\nEnter File Name: ";
            cin >> fileName;

            importDocument(fileName);
        }
    }

    void searchKeywordTFIDF(string keyword)
    {
        auto start =
            chrono::high_resolution_clock::now();

        keyword = cleanWord(keyword);

        searchHistory.push_back(keyword);

        searchFrequency[keyword]++;

        totalSearches++;

        if (cache.contains(keyword))
        {
            setColor(10);
            cout << "\n[ CACHE HIT ] Loading previous result...\n";
            setColor(15);

            vector<string> oldResults =
                cache.get(keyword);

            cout << "\n====================================\n";
            cout << "CACHED SEARCH RESULTS\n";
            cout << "====================================\n";

            for (string result : oldResults)
            {
                cout << result << endl;
            }

            return;
        }

        setColor(14);
        cout << "\n[ CACHE MISS ] Searching database...\n";
        setColor(15);

        if (invertedIndex.find(keyword)
            == invertedIndex.end())
        {
            cout << "\nNo Results Found!\n";
            return;
        }

        vector<pair<double, int>> rankedResults;

        int totalDocs =
            documents.size();

        int docsContainingWord =
            invertedIndex[keyword].size();

        double idf =
            log(
                (double)(totalDocs + 1) /
                (docsContainingWord + 1)
            ) + 1;

        for (auto item : invertedIndex[keyword])
        {
            int docID =
                item.first;

            int frequency =
                item.second;

            double tf =
                (double)frequency /
                documents[docID].wordCount;

            double score =
                tf * idf;

            rankedResults.push_back(
                {
                    score,
                    docID
                }
            );
        }

        sort(
            rankedResults.begin(),
            rankedResults.end(),
            greater<pair<double, int>>()
        );

        vector<string> cacheResults;

        lastSearchResults.clear();

        cout << "\n====================================\n";
        cout << "TF-IDF SEARCH RESULTS\n";
        cout << "====================================\n";

        int rank = 1;

        for (auto result : rankedResults)
        {
            int docID =
                result.second;

            string output =
                "\nRank #" +
                to_string(rank++) +
                "\nFile : " +
                documents[docID].fileName +
                "\nScore : " +
                to_string(result.first) +
                "\n";

            setColor(11);
            cout << output;
            setColor(15);

            cacheResults.push_back(output);
            lastSearchResults.push_back(output);
        }

        cache.put(
            keyword,
            cacheResults
        );

        auto end =
            chrono::high_resolution_clock::now();

        chrono::duration<double> duration =
            end - start;

        totalSearchTime += duration.count();
    }

    void viewDocuments()
    {
        cout << "\n====================================\n";
        cout << "INDEXED DOCUMENTS\n";
        cout << "====================================\n";

        for (Document doc : documents)
        {
            cout
                << "ID: "
                << setw(3)
                << doc.id
                << " | "
                << doc.fileName
                << " | Words: "
                << doc.wordCount
                << endl;
        }
    }

    void viewSearchHistory()
    {
        cout << "\n====================================\n";
        cout << "SEARCH HISTORY\n";
        cout << "====================================\n";

        if (searchHistory.empty())
        {
            cout << "\nNo Searches Yet.\n";
            return;
        }

        for (int i = 0; i < searchHistory.size(); i++)
        {
            cout
                << i + 1
                << ". "
                << searchHistory[i]
                << endl;
        }
    }

    void viewTopKeywords()
    {
        cout << "\n====================================\n";
        cout << "KEYWORD FREQUENCY REPORT\n";
        cout << "====================================\n";

        for (auto item : keywordFrequency)
        {
            cout
                << left
                << setw(20)
                << item.first
                << item.second
                << endl;
        }
    }

    void dashboard()
    {
        cout
            << "\n====================================\n";

        cout
            << "SEARCH ENGINE DASHBOARD\n";

        cout
            << "====================================\n";

        cout
            << "Documents Indexed : "
            << documents.size()
            << endl;

        cout
            << "Total Searches    : "
            << searchHistory.size()
            << endl;

        cout
            << "Unique Keywords   : "
            << keywordFrequency.size()
            << endl;

        int highest = 0;

        string popularWord;

        for (auto item : keywordFrequency)
        {
            if (item.second > highest)
            {
                highest = item.second;
                popularWord = item.first;
            }
        }

        cout
            << "Most Common Word  : "
            << popularWord
            << endl;
    }
   
    void showCache()
    {

        cache.displayCache();

    }
};

int main()
{
    SearchEngine engine;

    int choice;

    while (true)
    {
        setColor(11);
        cout << "\n";
        cout << "=========================================\n";
        cout << "       SEARCH ENGINE PROTOTYPE\n";
        cout << "=========================================\n";
        setColor(15);
        cout << "1.  Import Single Document\n";
        cout << "2.  Import Multiple Documents\n";
        cout << "3.  Search Keyword\n";
        cout << "4.  Boolean Search\n";
        cout << "5.  Auto Complete\n";
        cout << "6.  Fuzzy Search\n";
        cout << "7.  View Documents\n";
        cout << "8.  View Search History\n";
        cout << "9.  View Keyword Report\n";
        cout << "10. Dashboard\n";
        cout << "11. View Cache Statistics\n";
        cout << "12. Analytics Dashboard\n";
        cout << "13. Data Management\n";
        cout << "14. Exit\n";
        setColor(11);
        cout << "=========================================\n";
        setColor(15);

        cout << "\nEnter Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string fileName;

            cout << "\nEnter File Name: ";
            cin >> fileName;

            engine.importDocument(fileName);

            break;
        }

        case 2:
            engine.importMultipleDocuments();
            break;

        case 3:
        {
            string keyword;

            cout << "\nEnter Keyword: ";
            cin >> keyword;

            engine.searchKeywordTFIDF(keyword);

            break;
        }

        case 4:
            engine.booleanSearch();
            break;

        case 5:
            engine.autoComplete();
            break;

        case 6:
            engine.fuzzySearch();
            break;

        case 7:
            engine.viewDocuments();
            break;

        case 8:
            engine.viewSearchHistory();
            break;

        case 9:
            engine.viewTopKeywords();
            break;

        case 10:
            engine.dashboard();
            break;

        case 11:
            engine.showCache();
            break;

        case 12:
            engine.analyticsDashboard();
            break;

        case 13:
            engine.dataManagement();
            break;

        case 14:
            cout << "\nThank You!\n";
            return 0;

        default:
            cout << "\nInvalid Choice!\n";
        }
    }
}