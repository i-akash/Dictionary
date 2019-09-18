#include<iostream>
#include<fstream>
#include<sstream>

int main(int argc, char const *argv[])
{
    std::ifstream input("dictionary.csv",std::ios::in);
    std::string line,collumn;
    
    int cnt=1;
    while (getline(input,line) && cnt)
    {
        std::cout<<line<<"\n\n\n\n";
        cnt--;
        std::stringstream ss(line);
        while (getline(ss,collumn,','))
        {
            std::cout<<collumn<<" -------- ";   
        }
        std::cout<<"\n\n\n";
    }
    
    return 0;
}
