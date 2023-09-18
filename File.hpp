#ifndef __FILE_HPP__
#define __FILE_HPP__


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class File{
    public:
    vector<vector<string>> log;

    File(const char* filename){
        ifstream ifs(filename, ios::in);
        file_read(ifs);
    }

    File(){
        ifstream ifs("example.csv", ios::in);
        file_read(ifs);
    }

    void print(){
        for(auto log_line : log){
            for(auto log_element:log_line){
                cout<<log_element<<" ";
        }
        cout<<"\n";
    }
    }

    private:
    void file_read(ifstream& ifs){
        if(ifs.fail()){
            cout<<"This File is not open."<<endl;
            return;
        }
        string line, element;
        while(getline(ifs, line)){
            std::istringstream line_stream(line);
            vector<string> line_buf;
            while(getline(line_stream, element,',')){
                line_buf.push_back(element);
            }
            log.push_back(line_buf);
        }
    }
    
};

#endif