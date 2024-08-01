#include "misc.h"
#include <fstream>
#include <iterator>
#include <sstream>
using namespace std;

map<string, string> read_config(string config_path){
    map<string, string> config;
    
    ifstream ifs(config_path);
    string config_raw(
        (istreambuf_iterator<char>(ifs)),
        (istreambuf_iterator<char>()) 
    );

    istringstream is_file(config_raw);

    string line;
    while( getline(is_file, line) ) {
        istringstream is_line(line);
        string key;
        if( getline(is_line, key, '=') ) {
            string value;
            if( getline(is_line, value) ) 
                config.insert_or_assign(key, value);
        }
    }
    return config;
}
