
#include "ConfigFile.hpp"

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
ConfigFile::ConfigFile()
{

}

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
ConfigFile::ConfigFile(const char* cPath, bool bCreateIfNotExists)
{
    Load(cPath);
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
			return FILE_NOT_FOUND;
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
				struct ConfigLine CurrConfigLine;

				//Parsing var name
				while(c!=' ' && c!='\t')
				{
					CurrConfigLine.name+=c;
					File.get(c);
				}

				//Parsing var value
				File.get(c);
				while(c!=';')
				{
					CurrConfigLine.value+=c;
					File.get(c);
				}

				m_Config.push_back(CurrConfigLine);

			}

		}
    }while(!File.eof());

    m_sPath=cPath;
	return NO_ERROR;
}




/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
string ConfigFile::GetStringValue(const char* cName)const
{
    for(unsigned int i=0 ; i<m_Config.size() ; i++)
    {
        if(m_Config[i].name == cName)
            return m_Config[i].value;
    }
    return "";
}

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
void ConfigFile::SetValue(const char* cName, const char* cValue)
{
    //Find occurences in the config
    unsigned int n;
    for(n=0 ; n<m_Config.size() ; n++)
    {
        if(m_Config[n].name == cName)
        {
        	//Set value
        	m_Config[n].value = cValue;
            return;
        }
    }

	//Add new value
	struct ConfigLine ConfigLine;
	ConfigLine.name = cName;
	ConfigLine.value = cValue;

	m_Config.push_back(ConfigLine);
}

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
enum ConfigFile::Error ConfigFile::WriteToFile(const char* cPath)
{
	ifstream File(m_sPath.c_str());

    if(!File)
    	return FILE_NOT_FOUND;


    string sNewFile("");

    bool* bWritten;
    bWritten = new bool[m_Config.size()];
    for(unsigned int i=0 ; i<m_Config.size() ; i++)
		bWritten[i]=false;

    char c;
    do
    {
    	//Go to first interesting char
    	do
    	{
    		File.get(c);
    		if(c=='\n' || c=='\t' || c==' ')
    			sNewFile+=c;
    	}while((c=='\n' || c=='\t' || c==' ') && !File.eof());

		if(!File.eof())
		{
			//It is a comment
			if(c=='#')
			{
				sNewFile+=c;
				do
				{
					File.get(c);
    				sNewFile+=c;
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

				sNewFile+=sVarName;

				//rewrite whitespace
				sNewFile+=c;

				//Ignore var value
				File.get(c);
				while(c!=';')
				{
					File.get(c);
				}
				//Set var value
				for(unsigned int i=0 ; i<m_Config.size() ; i++)
				{
					if(m_Config[i].name == sVarName)
					{
						sNewFile+=m_Config[i].value+c;
						bWritten[i]=true;
					}
				}

			}

		}
    }while(!File.eof());
    File.close();

    //Write unwritten variables
	for(unsigned int i=0 ; i<m_Config.size() ; i++)
	{
		if(bWritten[i]==false)
		{
			sNewFile+=m_Config[i].name+" "+m_Config[i].value+";\n";
		}
	}

	//Writing the new file
	ofstream FileW(cPath);
	FileW<<sNewFile;
	FileW.close();

	return NO_ERROR;
}

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
void ConfigFile::Print()const
{
    for(unsigned int i=0 ; i<m_Config.size() ; i++)
    {
        cout<<m_Config[i].name<<"   \t\t"<<m_Config[i].value<<"   \t;"<<endl;
    }
}







/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
									PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/






