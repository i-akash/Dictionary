#include<iostream>
#include<vector>
#include<stack>
#include<utility>
#include<string>
#include<fstream>
#include<queue>
#include<sstream>
#include<string.h>

class Dictionary
{
    private:
        Dictionary *children[30];
        int childs;    
        bool isEnd;
        int autoCompLim;
        std::string meaning;

    public:
        Dictionary();
        void insert(const char *word,std::string wordMeaning);
        bool search(const char *query);
        int possiblitiesInThisWay(const char *query); 
        std::vector<std::pair<std::string,std::string>> autoCompletion(const char *query);
        std::vector<std::pair<std::string,std::string>> traverse(Dictionary *start);
        void saveDictionary();
        void loadDictionary();
        ~Dictionary();
};

Dictionary::Dictionary()
{
    autoCompLim=20;
    isEnd=false;
    childs=0;
    meaning="";
    for(int i=0;i<27;i++){
        children[i]=nullptr;
    }
}

void Dictionary::insert(const char *word,std::string wordMeaning){
    Dictionary *rootIterator=this;

    for(int i=0;word[i]!='\0';i++){
        int index=word[i]-'a';
        if(index<0 || index>25)continue;

        if(!rootIterator->children[index])
            rootIterator->children[index]=new Dictionary;
        rootIterator->childs=rootIterator->childs+1;    
        rootIterator=rootIterator->children[index];
    }

    rootIterator->meaning=wordMeaning;
    rootIterator->isEnd=true;
}


bool Dictionary::search(const char *query){
    Dictionary *rootIterator=this;

    for(int i=0;query[i]!='\0';i++){
        int index=query[i]-'a';

        if(index<0 || index>25)continue;
        
        if(!rootIterator->children[index])
            return false;
        rootIterator=rootIterator->children[index];
    }

    return rootIterator->isEnd;
}



int Dictionary::possiblitiesInThisWay(const char * query){
    Dictionary *rootIterator=this;

    for(int i=0;query[i]!='\0';i++){
        int index=query[i]-'a';
        if(!rootIterator->children[index])
            return 0;
        rootIterator=rootIterator->children[index];
    }

    return rootIterator->childs;
}


std::vector<std::pair<std::string,std::string> > Dictionary::autoCompletion(const char *query){
    Dictionary *rootIterator=this;
    std::vector<std::pair<std::string,std::string>> result;

    for(int i=0;query[i]!='\0';i++){
        int index=query[i]-'a';
        if(!rootIterator->children[index]){
                return result;
            }
        rootIterator=rootIterator->children[index];
    }
    result=this->traverse(rootIterator);

    return result;   
}

std::vector< std::pair <std::string,std::string> > Dictionary::traverse(Dictionary *start){
    std::stack< std::pair<Dictionary *,std::string > >dfs;

    dfs.push({start,""});
    std::vector<std::pair<std::string,std::string>>result;

    while (!dfs.empty() && result.size()<autoCompLim)
    {
        auto node=dfs.top();
        Dictionary *curr=node.first;
        std::string currString=node.second;
        dfs.pop();

        for(int child=0;child<26;child++){
                    char cl='a'+child;
                    if(curr->children[child])
                        dfs.push({curr->children[child],currString+cl});
            }
        if(curr->isEnd){
            result.push_back({currString,std::string(curr->meaning)});
        }

    }
    
    return result;
}

void Dictionary::saveDictionary(){
    std::queue<Dictionary *>Queue;
    Queue.push(this);
    std::ofstream output("out.txt",std::ios::out);

    while (!Queue.empty())
    {
        Dictionary *currentNode=Queue.front();
        Queue.pop();

        for(int childIndex=0;childIndex<26;childIndex++){
            
            if(currentNode->children[childIndex]){
                    output<<childIndex<<" "<<currentNode->children[childIndex]->isEnd<<" "<<currentNode->children[childIndex]->childs<<"\n"<<currentNode->children[childIndex]->meaning<<"\n";
                    Queue.push(currentNode->children[childIndex]);
            }
        }
        output<<-1<<" "<<-1<<" "<<-1<<"\n"<<-1<<"\n";
    }
    
}

void Dictionary::loadDictionary(){
    std::queue<Dictionary *>Queue;
    Queue.push(this);
    std::ifstream input("out.txt",std::ios::in);
    while (!Queue.empty())
    {
        Dictionary *currentNode=Queue.front();
        Queue.pop();

        while (true)
        {
            Dictionary *child=new Dictionary;
            int index,end,way;
            std::string path,wordMeaning;
            getline(input,path);
            getline(input,wordMeaning);

            std::stringstream ss(path);
            ss>>index;
            ss>>end;
            ss>>way;

            if(index<0)break;
            
            child->isEnd=end;
            child->childs=way;
            child->meaning=wordMeaning;
            currentNode->children[index]=child;
            Queue.push(child);
        }
        
    }
    
}

Dictionary::~Dictionary()
{

}


int main(int argc, char const *argv[])
{
    
    Dictionary dictionary;



    // std::ifstream input("dictionary.txt");
    // std::string line;
    // while(getline(input,line))
    // {
    //     std::vector<std::string> brokenwords;
    //     char *charline=(char *)line.c_str();
    //     char *word;
        
    //     word=strtok(charline,"$");
        
    //     while (word!=NULL)
    //     {
    //         brokenwords.push_back(std::string(word));
    //         word=strtok(NULL,"$");
    //     }
        
    //     int length=brokenwords.size();
    //     const char* dword=brokenwords[0].c_str();
    //     const char* mean=brokenwords[length-1].c_str();
    //     dictionary.insert(dword,mean);
           
    // }

    // dictionary.saveDictionary();
    
    dictionary.loadDictionary();
    
    if(argc<2)return 0;
    
    int possiblities=dictionary.possiblitiesInThisWay(argv[1]);
    std::cout<<possiblities<<"\n";
    std::vector<std::pair<std::string,std::string>> results=dictionary.autoCompletion(argv[1]);
    for(std::pair<std::string,std::string> word:results)
        std::cout<<argv[1]+word.first<<"--> "<<word.second<<"\n\n";


    return 0;
}
