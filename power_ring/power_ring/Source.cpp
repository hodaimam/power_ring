#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

int main () 
{
	ifstream myReadFile;
	ifstream myReadFile_lef;
	ofstream myWritefile;

	
	char output[1000];         
	string lineString;
	string outputString;
	int core[4];
	map<int, string> row;
	bool core_flag=0;
	bool nets_flag=0;
	bool ref1=0;
	bool ref2=0;
	bool pins_flag=0; 
	bool row_flag=0;
	bool taper_flag=0;
	int nets=0;
	int z=0;
	string metal1="metal1";
	string metal2="metal2";
	string via="M2_M1";
	string def_path;
	string output_path;
	int width=80;
	int distance=20;
	int n=0;
	int t=0;
	
	cout<<"Give def path"<<endl;
	cin>>def_path;
	cout<<"Give output path"<<endl;
	cin>>output_path;
	cout<<"Give metal1 type"<<endl;
	cin>>metal1;
	cout<<"Give metal2 type"<<endl;
	cin>>metal2;
	cout<<"Give via type"<<endl;
	cin>>via;
	cout<<"Give width"<<endl;
	cin>>width;
	cout<<"Give distance away from core area"<<endl;
	cin>>distance;

	myReadFile.open(def_path);
	myWritefile.open(output_path);
	myReadFile_lef.open("osu035_stdcells.lef");

 

	if (myReadFile.is_open())
	{
		while (!myReadFile.eof()) 
		{   
			getline(myReadFile,lineString);
		//	cout<<lineString<<endl;
			strcpy(output, lineString.c_str());
			int commentFlag = 0 ;                          //flag used to indicate that all coming charcters are comments
			for(int i = 0; i<lineString.length(); i++)
			{
				
				if(output[i]==' ' ||  output[i]=='\t' || output[i]=='\f' || output[i]=='\v' || output[i]=='\n' || output[i]=='\r' || output[i]=='(' || output[i]==')')
				{
					myWritefile<<output[i];
					//cout<<output[i];
					continue;
					
				}
		
				else if(taper_flag==1)
				{
					//cout<<"m";
					if(isspace(output[i+1]) || output[i+1]==NULL)
					{
			            outputString +=output[i];
						if(outputString==";")
						{
							taper_flag=0;
							myWritefile<<outputString<<endl;
							myWritefile<<endl<<"NONDEFAULTRULES 1 ;"<<endl;
							myWritefile<<"- doubleSpaceRule"<<endl; 
							myWritefile<<"+ LAYER "<<metal1<<" WIDTH "<<width<<endl;   
							myWritefile<<"+ LAYER "<<metal2<<" WIDTH "<<width<<endl;
							myWritefile<<" ;"<<endl;
							myWritefile<<"END NONDEFAULTRULES"<<endl;
						}
				        outputString = "";	
					}	
					else 
					outputString +=output[i];
					
				}
				
				else if(core_flag==1)
				{
					//cout<<"m";
					if(output[i+1]==' ' ||  output[i+1]=='\t' || output[i+1]=='\f' || output[i+1]=='\v' || output[i+1]=='\n' || output[i+1]=='\r' || output[i+1]=='(' || output[i+1]==')')
					{
			            outputString +=output[i];
						core[n]=stoi(outputString);
						//cout<<outputString<<endl;
						n++;
						if(n==4)
						{
							core_flag=0;
							taper_flag=1;
						}
						
				        outputString = "";	
					}	
					else 
					outputString +=output[i];
					myWritefile<<output[i];	
					
				}
				
				else if(row_flag==1)
				{
					//cout<<"m";
					if(output[i+1]==' ' ||  output[i+1]=='\t' || output[i+1]=='\f' || output[i+1]=='\v' || output[i+1]=='\n' || output[i+1]=='\r')
					{
			            outputString +=output[i];
						//cout<<outputString<<endl;
						if(z==1)
						{
						row[t]=outputString;
						
						t++;
						row_flag=0;
						//cout<<"l";
						}
						z++;
						outputString = "";
					}	
					else 
					outputString +=output[i];
					myWritefile<<output[i];	
					//cout<<output[i];	
					
				}

				else if(pins_flag==1 && ref1==0)
				{
					//cout<<"j";
					if(isspace(output[i+1]) || output[i+1]==NULL)
					{
			            outputString +=output[i];
						
						if(outputString == ";")
						{   
							myWritefile<<";"<<endl;
							myWritefile<<"-  VDD + NET vdd_ring"<<endl; 
							myWritefile<<"+ LAYER "<< metal1<<" ( -20 0 ) ( 25 50 )"<<endl;
							myWritefile<<"+ PLACED ( "<<core[0]+0.5*width+distance<<" "<<core[1]+0.5*width+distance<<" ) N ;"<<endl;
							myWritefile<<"-  ground + NET ground_ring"<<endl; 
							myWritefile<<"+ LAYER "<< metal1<<" ( -20 0 ) ( 25 50 )"<<endl;
							myWritefile<<"+ PLACED ( "<<core[0]+1.5*width+distance<<" "<<core[1]+1.5*width+distance<<" ) N ;"<<endl;
							pins_flag=0;
							ref1=1;
							//cout<<"h";

						}
						else
						{
							//cout<<outputString;
						int pins=stoi(outputString)+2;
						myWritefile<<pins;	
						}

						outputString = "";
						
					}	
					else 
					{
						
					outputString +=output[i];
					//cout<<output[i];
					}
					
					
				}
				else if(nets_flag==1 && ref2==0)
				{
					
					if(isspace(output[i+1]) || output[i+1]==NULL)
					{  //cout<<"g";
			            outputString +=output[i];
						
						if(outputString==";")
						{
							myWritefile<<";"<<endl;
							myWritefile<<"- vdd_ring"<<endl; 
							myWritefile<<"( PIN VDD ) "<<endl;
							myWritefile<<"+ ROUTED "<<metal1<<" TAPERRULE doubleSpaceRule ( "<<core[0]+0.5*width+distance<<" "<<core[1]+0.5*width+distance<<" ) ( "<<core[2]-0.5*width-distance<<" * ) "<<via<<endl;
							myWritefile<<"+ ROUTED "<<metal2<<" TAPERRULE doubleSpaceRule ( "<<core[2]-0.5*width-distance<<" "<<core[1]+0.5*width+distance<<" ) ( * "<<core[3]-0.5*width-distance<<" ) "<<via<<endl;
							myWritefile<<"+ ROUTED "<<metal1<<" TAPERRULE doubleSpaceRule ( "<<core[2]-0.5*width-distance<<" "<<core[3]-0.5*width-distance<<" ) ( "<<core[0]+0.5*width+distance<<" * ) "<<via<<endl;
							myWritefile<<"+ ROUTED "<<metal2<<" TAPERRULE doubleSpaceRule ( "<<core[0]+0.5*width+distance<<" "<<core[3]-0.5*width-distance<<" ) ( * "<<core[1]+0.5*width+distance<<" ) "<<via<<endl;
						
							map< int,string>::iterator itn1;
							itn1 = row.begin();
							if(itn1!=row.end())
							{
							while(itn1!=row.end())
							{
								myWritefile<<"+ ROUTED "<<metal2<<" TAPERRULE doubleSpaceRule ( "<<core[0]+0.5*width+distance<<" "<<itn1->second<<" ) "<<via<<" ( "<<core[0]+0.5*width+distance<<" "<<itn1->second<<" ) ( "<<core[2]-0.5*width-distance<<" * ) "<<via<<endl;
		                        advance(itn1,1);
								if(itn1==row.end())
								break;
								else
									advance(itn1,1);
							}
							}
							advance(itn1,-1);
							map< int,string>::iterator itn3=itn1;
							advance(itn3,-1);
							myWritefile<<"+ ROUTED "<<metal2<<" TAPERRULE doubleSpaceRule ( "<<core[0]+0.5*width+distance<<" "<<stoi(itn1->second)+stoi(itn1->second)-stoi(itn3->second)<<" ) "<<via<<" ( "<<core[0]+0.5*width+distance<<" "<<stoi(itn1->second)+stoi(itn1->second)-stoi(itn3->second)<<" ) ( "<<core[2]-0.5*width-distance<<" * ) "<<via<<endl;
							myWritefile<<";"<<endl;


							myWritefile<<"- ground_ring"<<endl; 
							myWritefile<<"( PIN ground ) "<<endl;
							myWritefile<<"+ ROUTED "<<metal1<<" TAPERRULE doubleSpaceRule ( "<<core[0]+1.5*width+distance<<" "<<core[1]+1.5*width+distance<<" ) ( "<<core[2]-1.5*width-distance<<" * ) "<<via<<endl;
							myWritefile<<"+ ROUTED "<<metal2<<" TAPERRULE doubleSpaceRule ( "<<core[2]-1.5*width-distance<<" "<<core[1]+1.5*width+distance<<" ) ( * "<<core[3]-1.5*width-distance<<" ) "<<via<<endl;
							myWritefile<<"+ ROUTED "<<metal1<<" TAPERRULE doubleSpaceRule ( "<<core[2]-1.5*width-distance<<" "<<core[3]-1.5*width-distance<<" ) ( "<<core[0]+1.5*width+distance<<" * ) "<<via<<endl;
							myWritefile<<"+ ROUTED "<<metal2<<" TAPERRULE doubleSpaceRule ( "<<core[0]+1.5*width+distance<<" "<<core[3]-1.5*width-distance<<" ) ( * "<<core[1]+1.5*width+distance<<" ) "<<via<<endl;
						

							map< int,string>::iterator itn2;
							itn2 = row.begin();
							itn2++;
							if(itn2!=row.end())
							{
							while(itn2!=row.end())
							{
								myWritefile<<"+ ROUTED "<<metal2<<" TAPERRULE doubleSpaceRule ( "<<core[0]+1.5*width+distance<<" "<<itn2->second<<" ) "<<via<<" ( "<<core[0]+1.5*width+distance<<" "<<itn2->second<<" ) ( "<<core[2]-1.5*width-distance<<" * ) "<<via<<endl;
								//cout<<itn2->second<<endl;
								advance(itn2,1);

								if(itn2==row.end())
								break;
								else
									advance(itn2,1);
							}
							}
							
							myWritefile<<";"<<endl;
							nets_flag=0;
							ref2=1;
						}
						else
						{
						int pins=stoi(outputString)+2;
					
						myWritefile<<pins;	
						}
						outputString = "";
						
					}	
					else 
					outputString +=output[i];
					
					
				}
				
				else if((output[i]>='a' && output[i]<='z')||(output[i]>='A' && output[i]<='Z'))
				{
					outputString +=output[i];
					if((output[i+1]>='a' && output[i+1]<='z')||(output[i+1]>='A' && output[i+1]<='Z'))
					{

						continue;
					}
					
					if(outputString == "DIEAREA")
					{
					    myWritefile<<outputString;	
						core_flag=1;
						
					}
					else if(outputString == "core" && output[i+1]!='_')
					{
						
 					myWritefile<<outputString;	
					//cout<<"jj";
						row_flag=1;
						z=0;
					}
					else if(outputString == "PINS")
					{
 					myWritefile<<outputString;	
					//cout<<output[i+2]<<endl;
					//cout<<"jj";
						pins_flag=1;
						
					}
					else if(outputString == "NETS")
					{
 					myWritefile<<outputString;	

						nets_flag=1;
						
					}
					else
						//cout<<outputString;
						myWritefile<<outputString;	
					

					outputString = "";
				
				}

				else
					//cout<<outputString<<endl;
					myWritefile<<output[i];
				
			
				
				
			}
			myWritefile<<endl;
		}
		
	

	}

	map< int,string>::iterator itn;
		for ( itn = row.begin(); itn != row.end(); itn++ )
		{
			//cout<<itn->second<<endl;
			
	
		}
	myReadFile.close();
	myWritefile.close();
	return 0;
}
