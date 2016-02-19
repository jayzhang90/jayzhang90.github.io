/*
The goal is to match product listings from listings.txt file.
The program consumes 2files, products.txt and listings.txt(default). Firstly, the program creates a map(called table) which the key is the manufacturer of the product,
the value is a list of productInfo(a structure contains the product's model information and so on.). Then the program will go through the listings.txt.
if the manufacturer in listings.txt exactly matches manufacturer in the table, then check the model, if it also matches, then add the whole listing into the list of the productInfo struct.
At the end, the progam outputs all results into a file called result.txt
*/
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

struct productInfo{
  string manufacturer;
  string model;
  string product_name;
  vector<string> list;
};

int main(int argc, const char* argv[]){
  map<string,vector<productInfo*> > table;   //key: manufacturer, value is vector of productInfo
  string input;
  ifstream infile1,infile2;
  infile1.open("products.txt");
  infile2.open("listings.txt");
 ///////////////////////////////// product file
  while(getline(infile1,input)){
      productInfo *new_productInfo=new productInfo();
      size_t found1,found2,found3,found4;           // found1=loc of :, found2= loc of ,
      string info,info_manufacturer;

      found1=input.find("\"product_name\":");
      found2=input.find("\"manufacturer\":");
      found3=input.find("\"model\":");

      found1=found1+14+1+1;                      //14=length of "product_name", 1 = :, 1= "
      info=input.substr(found1,found2-found1-2); //product_name(why minus 2? 1 for " 1 for,)
      new_productInfo->product_name=info;

      found2=found2+14+1+1;
      info_manufacturer=input.substr(found2,found3-found2-2);
      for(int i=0;i<info_manufacturer.length();i++){
          info_manufacturer[i]=tolower(info_manufacturer[i]);                  //lowercase of the manufacturer
      }
      new_productInfo->manufacturer=info_manufacturer;                     //manufacturer of the product;

      found3=found3+7+1+1;                                              //7=length of "model"  1=: 1="
      if(input.find("\"family\"")!= string::npos){
          found4=input.find("\"family\":");
      }
      else{
          found4=input.find("\"announced-date\":");
      }
      info=input.substr(found3,found4-found3-2);                     //model of the product
      new_productInfo->model=info;

      table[info_manufacturer].push_back(new_productInfo);
  }

  ///////////////////////////////// listing file
  while(getline(infile2,input)){
    size_t found1,found2,found3;
    string manufacturer,title;
    found1=input.find("\"title\":");
    found2=input.find("\"manufacturer\":");
    found3=input.find("\"currency\":");

    found1=found1+7+1+1;                         // 7=length of "title"
    title=input.substr(found1,found2-found1-2);

    found2=found2+14+1+1;                       //14=length of "manufacturer"  1 = :  1= "
    manufacturer=input.substr(found2,found3-found2-2);
    for(int i=0;i<manufacturer.length();i++){
        manufacturer[i]=tolower(manufacturer[i]);                  //lowercase of the manufacturer(used for comparison)
    }
    for (std::map<string,vector<productInfo*> >::iterator it=table.begin(); it!=table.end(); ++it){ //go through the table to check if the manufacturer is covered by table
        string temp=it->first;
        size_t loc=manufacturer.find(temp);
        if(loc!= string::npos  && (manufacturer[loc-1]==' ' || manufacturer[loc-1]=='\"' || loc==0)&& (manufacturer[loc+temp.length()]==' ' || manufacturer[loc+temp.length()]=='\"' || loc+temp.length()==manufacturer.length())){               // the manufacturer is covered, check if the model is identical
               cout<<manufacturer<<" , "<<temp<<endl;
             for(int i=0;i<it->second.size();i++){
                  productInfo * temp_productInfo=it->second.at(i);
                  string temp_model=temp_productInfo->model;
                  temp_model=" "+temp_model+" ";                     // exact model number
                  if(title.find(temp_model)!= string::npos){         //the model number is in the title.
                        temp_productInfo->list.push_back(input);            //push back the list to the corresponding product
                  }
             }
             break;
        }
    }
  }

  ofstream myfile;
  myfile.open ("result.txt");
  for (std::map<string,vector<productInfo*> >::iterator it=table.begin(); it!=table.end(); ++it){
      for(int i=0;i<it->second.size();i++){
         productInfo * temp=it->second.at(i);
         myfile<<"{"<<"\"product_name\":\""<<temp->product_name<<"\""<<",\"listings\":"<<"[";                 //print as JSON format, which means  {"product_name" : product_name,"listings":[listing]}
         for(int i=0;i<temp->list.size();i++){
            myfile<<temp->list.at(i);
            if(i != temp->list.size()-1){
                myfile<<",";
            }
          }
          myfile<<"]"<<"}"<<endl;
      }
  }
  infile1.close();
  infile2.close();
  myfile.close();
  cout<<"The result is stored in result.txt"<<endl;
  return 0;
}
