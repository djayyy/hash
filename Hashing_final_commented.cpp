#include <bits/stdc++.h>
#include<iostream>
#include<cstring>
#include<dirent.h>

//limited the number of words that the program can handle
#define MAX_SIZE 10000

//limited the number of files the program can handle
#define MAX_FILES 10


DIR *dpdf;
struct dirent *epdf;

int words=0,currentfile=0;

using namespace std;

//function used for sorting ====================================================
int comparator(const void *p,const void *q)
{
	const int *a = (const int *)p;
	const int *b = (const int *)q;
	return(a[1]-b[1]);
}
//==============================================================================


//basic structure of the linked list ===========================================
typedef struct node{
	char *data;
	int freq[MAX_FILES][2];// 0 index for file , 1 for frequency
	struct node *next;
}node;
//==============================================================================


//hash function ================================================================
long int hash(char *st)
{
	long int hashvalue=0;
	int c;
	char *str = &st[0];
	while(c=*str)
	{
		hashvalue = c + hashvalue<<6 + hashvalue<<16 - hashvalue;
		str++;
	}
	return hashvalue%10003;
}
//==============================================================================
//main function to execute the program =========================================
int main()
{
	//int num = filenames.size();
	int i,val,len,j,num;
	string locationname[MAX_FILES];
	string userstring;
	char *pointertostring,c;
	std::cout<<"Getting all files in the desired directory.....\n";
	//opening the current directory
	dpdf = opendir("./");
	vector<string> tobeopen;
	vector<string> filenames;
	
//storing all file names in a vector ===========================================
	if (dpdf != NULL)
	{
		//cout<<"gg"<<endl;
   		while (epdf = readdir(dpdf))
   		{
     	 	filenames.push_back(epdf->d_name);
     	 	//cout<<epdf->d_name<<endl;
   		}
	}
//==============================================================================
	
	cout<<tobeopen.size()<<endl;
	num = filenames.size();
	i=0;
	
//storing file names having '.txt' format in another vector ====================
	while(i<num)
	{
		string fname = filenames[i];
		len = strlen((char *)&fname[0]);
		j=0;
		while(j+3<len)
		{
			if(fname[j]=='.'&&fname[j+1]=='t'&&fname[j+2]=='x'&&fname[j+3]=='t'&&fname[j+4]!='~')
			{
				tobeopen.push_back(fname);
			}
			j++;
		}
		i++;
	}
//==============================================================================
	
	
	//cout<<(tobeopen.size())<<endl;
	node table[MAX_SIZE];
	string stored[MAX_SIZE];
	for(i=0;i<MAX_SIZE;i++) table[i].next=NULL;
	
//==============================================================================


//******************************************************************************
	//start of while loop to read files one by one from directory ==============
	while(currentfile<(tobeopen.size()))
	{		// for reading file from the directory vector: tobeopen is used
		string finame = tobeopen[currentfile];
		cout<<endl<<finame<<"   ";
		ifstream fin;
		fin.open(finame.c_str(),ios::in);
		bool check=false;
		//while loop for reading the characters in a file ----------------------
		while(!fin.eof())
		{
			string g;
			fin.get(c);
			while((c>=97&&c<=122)||(c>=65&&c<=90)||(c>=48&&c<=57))
			{
				if(c>=65&&c<=90) c+=32;
				check=true;
				g+=c;
				fin.get(c);
			}
			if(check)//while(fin>>g&&fin.is_open())
			{
				g+='\0';
				//cout<<g<<" ";
				stored[words]=g;
				len = strlen(&g[0]);
				val = hash((char *)&g[0])%7;
				//cout<<val<<"  "<<g<<endl;
				pointertostring = (char *)&stored[words][0];
				
				//form a new node if the place corresponding to the hash value 
				//obtained for the word is empty
				if(table[val].next==NULL)
				{
					table[val].freq[currentfile][0] = currentfile;
					table[val].freq[currentfile][1] = 1;
					table[val].data = pointertostring;
					//table[val].freq = 1;
					node* temp = new node;
					temp->next=NULL;
					table[val].next=temp;
					words++;		//go to next word of the file
				}
				
				//go to the corresponding node of the linked list if there 
				//already exists a word corresponding to the obtained hash value
				else
				{
					node *f;
					bool check=true;
					f = &table[val];
					
					//check if the word is already present in the linked list 
					//by traversing it from the root
					
					//if the is word already present in the linked list 
					//corresponding to the hash value, increase the frequency
					//corresponding to the file name that is being traversed
					while(f->next!=NULL)
					{
						if(strcmp(f->data,pointertostring)==0)
						{
							f->freq[currentfile][0]= currentfile;
							f->freq[currentfile][1]++;
							//(f->freq)++;
							check = false;
						}
						f=f->next;
					}
					
					//if the word is not present then create another node in 
					//the same linked list with the new word and update its
					//frequency corresponding to the file being traversed
					if(check)
					{
						f->data = strdup(pointertostring);
						//f->freq = 1;
						f->freq[currentfile][0] = currentfile;
						f->freq[currentfile][1] = 1;
						node* temp = new node;
						temp->next=NULL;
						f->next = temp;
						words++;			//go to next word of the file
					}
				}
			}
		}
		// end of while loop for reading characters ----------------------------
		
		//go to next file of the directory
		currentfile++;
	}
	//end of while loop for reading files from the directory ====================

//******************************************************************************	
	
	cout<<" size of total files "<<currentfile<<endl;
	char choice;
	bool check;
	
//==============================================================================
	while(1)
	{
		cout<<"Want to enter string?\n";
		cin>>choice;
		if(choice=='n'||choice=='N') 
			break;
			
		//getting keyword from user
		cout<<"Enter a string to search for"<<endl;
		cin>>userstring;
		
		//assigning a pointer to the variable in which user input is stored
		pointertostring = (char *)(&userstring[0]);
		val = hash(pointertostring)%7;
		node *f = &table[val];
		check = true;
		while(f->next!=NULL)
		{
			if(strcmp(f->data,pointertostring)==0)
			{
				cout<<"found  ";
				qsort(f->freq,currentfile,2*sizeof(int),comparator);
				for(i=0;i<currentfile;i++)
				{
					if(f->freq[i][1]>0)
						cout<<"File name is "<<tobeopen[f->freq[i][0]]<<"  Frequency is  "<<f->freq[i][1]<<endl;
				}
				check = false;
				break;
			}
			f=f->next;
		}
		
		//give an output when no file containes the searched keyword
		if(check)
		{
			cout<<"Not found \n";
		}
	}
//==============================================================================
	return 0;
}

/*############################## END OF CODE #################################*/


