#ifndef CONFIGFILE_HPP_INCLUDED
#define CONFIGFILE_HPP_INCLUDED


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

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;



class ConfigFile
{
    public:

	/**
	@brief Errors casted by the ConfigFile
	@note You can have a reprentative string whith @see string GetErrorString(enum Error e)
	**/
    enum Error
    {
    	NO_ERROR,
    	FILE_NOT_FOUND,
    	FILE_NOT_OPENED
    };

    /**
    @brief Get a human readable string for a given error
    @param e The error
    **/
    static string GetErrorString(enum Error e)
    {
    	if(e==NO_ERROR)return "Success";
    	else if(e==FILE_NOT_FOUND)return "File not found";
    	else if(e==FILE_NOT_OPENED)return "The file needs to be opened";
    	return "Unknown error :(";
	}


//================================================================================
    /**
    @brief Default constructor
    @warning You must load the configfile manually before using it
    **/
    ConfigFile();
//================================================================================
    /**
    @brief Construct and load the config file
    @param cPath Path to the config file
    @param bCreateIfNotExists If true, an empty file will be created if it doesn't exist. If false and the config file doesn't exist, the file will not be loaded.
    @note You may prefer to use Load(...) to get the generated error
    **/
    ConfigFile(const char* cPath, bool bCreateIfNotExists=false);
//================================================================================
	/**
	@brief Load the config file
    @param cPath Path to the config file
    @param bCreateIfNotExists If true, an empty file will be created if it doesn't exist. If false and the config file doesn't exist, will return FILE_NOT_FOUND.
	**/
	enum Error Load(const char* cPath, bool bCreateIfNotExists=false);
//================================================================================
    /**
    @brief Save the current config to the config file
    @arg sNewFile The path to the config file to be written. If void, will save to the config file that has been used in the last Load()
    @warning if cNewFile="" (default), the file used in last Loading MUST BE OPENED !
    **/
    enum Error WriteToFile(const char* cNewFile=0);
//================================================================================
//================================================================================
    /**
    @brief Return the string that reprensents the value of the config line. It can be
    @param cName The name of the variable in the config file
    @return examples: "10" or "Thibaut CHARLES" or "10.5 18.0 80.6" for a vector. Returns "" if the row does not exist
    **/
    string GetStringValue(const char* cName)const;
//================================================================================
	template<typename T>
	/**
	@brief Get a parsed value from the config file
	@param cName The name of the variable in the config file
	@param n The position of the value in a multivalue field.
	@example For "MyArray 10 56 38 52 46;",  GetValue<int>("MyArray", 3) will return 52. (the first value is at n=0)
	**/
    T GetValue(const char* cName, int n=0)const
	{
		string sValue(GetStringValue(cName)+" ");
		string sCurrValue;

		int nPos=0;
		for(unsigned int i=0 ; i<sValue.size() ; i++)
		{
			if(sValue[i]!=' ' && sValue[i]!='\t')
			{
				sCurrValue+=sValue[i];
			}
			else
			{
				if(nPos==n)
				{
					stringstream ss(sCurrValue);
					T a;
					ss>>a;
					return a;
				}
				else
				{
					sCurrValue="";
					nPos++;
				}
			}
		}
		return 0;
	}
//================================================================================
	template<typename T>
	/**
	@brief Get an array of parsed values from the config file
	@param cName The name of the variable in the config file
	**/
    vector<T> GetArrayValue(const char* cName)const
	{
		vector<T> Return;
		string sValue(GetStringValue(cName)+" ");

		string sCurrValue;
		for(unsigned int i=0 ; i<sValue.size() ; i++)
		{
			char c = sValue[i];
			if(c!=' ')
			{
				sCurrValue+=c;
			}
			else
			{
				stringstream ss(sCurrValue);
				T a;
				ss>>a;
				Return.push_back(a);

				sCurrValue="";
			}
		}
		return Return;
	}
//================================================================================
    /**
    @brief Set a value in the loaded config. If the name of the var doesn't exist, it will add it.
    @param cName Name of the variable to set
    @param cValue string value of the variable to set.
    **/
    void SetValue(const char* cName, const char* cValue);
//================================================================================
//================================================================================
    /**
    @brief Prints the config to cout. Useful for debugging ;)
    **/
    void Print()const;





    private:

    struct ConfigLine
    {
        string name;
        string value;
    };

    string m_sPath;
    vector<struct ConfigLine> m_Config;


};

#endif // CONFIGFILE_HPP_INCLUDED
