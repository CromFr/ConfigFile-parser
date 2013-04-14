
/**
ConfigFile handler

Original code by Thibaut CHARLES (crom29 @ hotmail.fr)


This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.





Configuration file example:
________________________________
#Custom comment

#the syntax is :
#	[var name][space][value(s)];
#
#[var name] : Name of the var, without any whitespaces into
#[space] : must be a ' ' or a '\t'
#[value(s)] : the value(s) field
#The ';' is very important !


#Examples:
MyVar 10;
MySentense Hello world !;
MyArray 10 56 38 52 46;

#A value can contain any character excepting ';'
# It can even contain '\n' !

MyMultilineValue Dear sir,
This a message for you

Yours faithfully,
Thibaut CHARLES;

________________________________

**/




#include "ConfigFile.hpp"

#include <iostream>
#include <fstream>
#include <map>

using namespace std;


/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
const string ConfigFile::GetErrorString(enum Error e)
{
	if(e==no_error)return "Success";
	else if(e==file_not_found)return "File not found";
	else if(e==file_not_opened)return "The file needs to be opened";
	else if(e==configfile_not_loaded)return "The ConfigFile must be loaded first";
	return "Unknown error :(";
}

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
ConfigFile::ConfigFile()
{

}

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
enum ConfigFile::Error ConfigFile::Load(const char* cPath, bool bCreateIfNotExists)
{
	//Open the file
    ifstream File(cPath);

	if(!File)
	{
		if(bCreateIfNotExists)
		{
			//Create the file
			ofstream ofFile(cPath);
			ofFile.close();
		}
		else
		{
			return file_not_found;
		}
	}

    char c;
    do
    {
    	//Go to first interesting char
    	do
    	{
    		File.get(c);
    	}while((c=='\n' || c=='\t' || c==' ') && !File.eof());

		if(!File.eof())
		{
			//It is a comment
			if(c=='#')
			{
				do
				{
					File.get(c);
				}while(c!='\n' && !File.eof());
			}
			//It is a var
			else
			{
				string sName(""), sValue("");

				//Parsing var name
				while(c!=' ' && c!='\t' && !File.eof())
				{
					sName+=c;
					File.get(c);
				}

				//Parsing var value
				File.get(c);
				while(c!=';' && !File.eof())
				{
					sValue+=c;
					File.get(c);
				}

                //insert or update
                pair<unordered_map<string,string>::iterator,bool> p = m_Config.insert({sName, sValue});
                if(p.second == false)
                    p.first->second.assign(sValue);
			}

		}
    }while(!File.eof());

    m_sPath=cPath;
	return no_error;
}



/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
enum ConfigFile::Error ConfigFile::WriteToFile(const char* cPath)
{
    if(m_sPath.empty())return configfile_not_loaded;

	ifstream File(m_sPath.c_str());

    string sNewFileData("");

    bool bWritten[m_Config.size()];
    for(unsigned int i=0 ; i<m_Config.size() ; i++)
		bWritten[i]=false;

    if(File)
    {
        char c;
        do
        {
            //Go to first interesting char
            do
            {
                File.get(c);
                if(c=='\n' || c=='\t' || c==' ')
                    sNewFileData+=c;
            }while((c=='\n' || c=='\t' || c==' ') && !File.eof());

            if(!File.eof())
            {
                //It is a comment
                if(c=='#')
                {
                    sNewFileData+=c;
                    do
                    {
                        File.get(c);
                        sNewFileData+=c;
                    }while(c!='\n' && !File.eof());
                }
                //It is a var
                else
                {
                    string sVarName("");

                    //get & rewrite var name
                    while(c!=' ' && c!='\t')
                    {
                        sVarName+=c;
                        File.get(c);
                    }

                    sNewFileData+=sVarName;

                    //rewrite whitespace
                    sNewFileData+=c;

                    //Ignore var value
                    File.get(c);
                    while(c!=';')
                    {
                        File.get(c);
                    }

                    //Set var value
                    auto it = m_Config.find(sVarName);
                    if(it!=m_Config.end())
                    {
                        sNewFileData+=it->second+c;
                        bWritten[distance(m_Config.begin(), it)]=true;
                    }

                }

            }
        }while(!File.eof());
        File.close();
    }


    //Write unwritten variables
    for(auto it=m_Config.cbegin() ; it!=m_Config.cend() ; it++)
    {
        if(bWritten[distance(m_Config.cbegin(), it)]==false)
		{
			sNewFileData+="\n"+it->first+" "+it->second+";";
		}

    }

	//Writing the new file
    ofstream FileW;
    if(cPath==nullptr)
        FileW.open(m_sPath.c_str());
    else
        FileW.open(cPath);

	FileW<<sNewFileData;
	FileW.close();

	return no_error;
}



/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
const string& ConfigFile::GetValueString(const char* cName)const
{
    auto it = m_Config.find(cName);
    if(it!=m_Config.end())
    {
        return it->second;
    }

    //return empty string
    return m_Config.end()->second;
}



/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
void ConfigFile::SetValueString(const char* cName, const char* cValue)
{
    //Find occurences in the config
    auto it = m_Config.find(cName);
    if(it!=m_Config.end())
    {
        //change the value
        it->second.assign(cValue);
    }
    else
    {
        //Add new value
        m_Config.insert({cName, cValue});
    }
}



/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
void ConfigFile::Print(bool bOrderByName)const
{
    cout<<">>>>>>>>>>>>>> ConfigFile >>>>>>>>>>>>>>"<<endl;
    cout<<"Loaded from '"<<m_sPath<<"'"<<endl;
    cout<<"Data:"<<endl;
    if(bOrderByName)
    {
        map<string, string> ordered;
        for(auto it : m_Config)
        {
            ordered.insert({it.first, it.second});
        }

        //Print
        for(auto it : ordered)
        {
            cout<<it.first<<"  ";
            for(int i=it.first.length()+2 ; i<28 ; i++)
                cout<<"-";
            cout<<"  "<<it.second<<""<<endl;
        }
    }
    else
    {
        //Print
        for(auto it : m_Config)
        {
            cout<<it.first<<"  ";
            for(int i=it.first.length()+2 ; i<28 ; i++)
                cout<<"-";
            cout<<"  "<<it.second<<""<<endl;
        }
    }
    cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
}


