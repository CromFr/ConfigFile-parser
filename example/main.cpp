#include <iostream>
#include <vector>
using namespace std;

#include "../src/ConfigFile.hpp"

int main()
{

    ConfigFile conf;

    cout<<"=====> Loading and parsing ./config.cfg..."<<endl;
    enum ConfigFile::Error err = conf.Load("./config.cfg");
    if(err == ConfigFile::NO_ERROR)
    {
    	cout<<"\tFile loaded successfully !"<<endl;
    }
    else
    {
    	cout<<"\tError while loading the file : "<<ConfigFile::GetErrorString(err);
    	return 1;
    }

    cout<<"=====> Starting config :"<<endl;
    conf.Print();
    cout<<"-------------------------------"<<endl;
    cout<<endl;

    cout<<"=====> Set MySentense to \"This world is great :)\""<<endl;
	conf.SetValue("MySentense", "This world is great :)");

	cout<<"=====> Add a new variable : NewVar=68"<<endl;
	conf.SetValue("NewVar", "68");

    cout<<"=====> Modified config :"<<endl;
    conf.Print();
    cout<<"-------------------------------"<<endl;
    cout<<endl;

    cout<<"=====> The size of MyArray is "<<conf.GetArrayValue<int>("MyArray").size()<<" and the third value is "<<conf.GetValue<int>("MyArray", 2)<<endl;
    cout<<endl;
    cout<<"=====> Here is the letter :"<<endl;
    cout<<conf.GetStringValue("MyMultilineValue")<<endl;

    cout<<endl;
    cout<<"=====> Saving new configuration file into ./newconfig.cfg"<<endl;
    conf.WriteToFile("./newconfig.cfg");

    return 0;
}
