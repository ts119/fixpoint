#ifndef __FILE_HPP__
#define __FILE_HPP__


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>


//ファイルの読み込みのためのクラス
class File{
    public:
    std::vector<std::vector<std::string>> log;

    File(const char* filename){
        std::ifstream ifs(filename, std::ios::in);
        file_read(ifs);
    }

    File(){
        std::ifstream ifs("example.csv", std::ios::in);
        file_read(ifs);
    }

    void print(){
        for(auto log_line : log){
            for(auto log_element:log_line){
                std::cout<<log_element<<" ";
        }
        std::cout<<"\n";
    }
    }

    private:
    void file_read(std::ifstream& ifs){
        if(ifs.fail()){
            std::cout<<"This File is not open.\n";
            return;
        }
        std::string line, element;
        while(std::getline(ifs, line)){
            std::istringstream line_stream(line);
            std::vector<std::string> line_buf;
            while(std::getline(line_stream, element,',')){
                line_buf.push_back(element);
            }
            log.push_back(line_buf);
        }
    }
    
};

#endif