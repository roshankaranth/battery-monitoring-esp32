#include<bits/stdc++.h>
using namespace std;

map<double,double> cleanData(vector<pair<double,double>> &dataset){
    map<double,double> data;

    for(int i = 0; i  < dataset.size(); i++){
        data[dataset[i].first] = dataset[i].second;
    }

    return data;
}

void writeToFile(map<double,double> &data, string filename){
    ofstream myFile(filename + "_clean");
    for(auto it : data){
        myFile<<it.first<<" "<<it.second<<"\n";
    }

    myFile.close();
}

vector<pair<double,double>> readFromFile(string filename){
    string str;
    vector<pair<double,double>> dataset;
    stringstream S;
    ifstream read(filename);
    string x,y;
    while(getline(read,str)){
        S<<str;
        S>>x;
        S>>y;
        dataset.push_back({stod(x),stod(y)});
        S.clear();
        
    }

    read.close();
    return dataset;
}

int main(){
    vector<string> files = {"Black Line.csv", "Red Line.csv", "Cyan Line.csv", "Green Line.csv", "Magenta Line.csv"};
    string source = "V_vs_Capacity_data/";
    string dest = "V_vs_Capacity_data_clean/";
    for(auto file : files ){
        vector<pair<double,double>> dataset = readFromFile(source + file);
        map<double,double> data = cleanData(dataset);
        writeToFile(data,dest + file);
    }
}



