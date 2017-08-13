#include<iostream>
#include<bitset>
#include<algorithm>
#include<string>
#include<cmath>
using namespace std;

int stoint(string str)
{
	int i=0,value=0;
	for(i=5;i>=0;i--)
	{
		value += (str[i]-'0')*pow(2,(5-i));
	}
	return value;
}

string keysch(char *tempkey,int *PC2)
{
	//left shift in right part and left part of tempkey
	char key[49];
	int i;
	char _0;

	_0 = tempkey[0];
	
	for(i=0;i<56;i++) tempkey[i]=tempkey[i+1];
	*(tempkey+55)=*(tempkey+27);
	*(tempkey+27)=_0;
	
	for(i=0;i<48;i++) key[i]=tempkey[*(PC2+i)];
	key[48]='\0'; 	//set null character
	
	return key;
	
}

string func(string str,string key,int *sbox,int *P,int signal) //signal = -1 in decipher and 31 in cipher
{
	char result[33];
	char R1[33];
	char ext[49];
	string tempext;
	
	int r=0;
	int i;
	int expansion[48]={32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
	
	for(i=0;i<48;i++) ext[i]=str[expansion[i]+signal]; //expand last 32 bits or first 32 bits
	ext[i]='\0';
	
	//xor;
	for(i=0;i<48;i++) ext[i] = (ext[i]==key[i]) ? '0':'1'; 
	tempext=ext;
	
	//sbox
	for(i=0;i<48;i++)
	{
		if((i%6)==0)
		{
			int index = stoint(tempext.substr(i,6));
			int value = sbox[index];
			string binary = bitset<4>(value).to_string();
			R1[r]=binary[0];r++;
			R1[r]=binary[1];r++;
			R1[r]=binary[2];r++;
			R1[r]=binary[3];r++;
		}
	}
	R1[r]='\0'; //set end of string
	
	//Permutaion
	for(i=0;i<32;i++) result[i] = R1[P[i]];
	result[i]='\0'; //set end of string
	
	return result;
}

int main()
{
	
	
	string mainkey;
	string mainkey1;
	string mainkey2;
	string tempkey;
	string key;
	string plaintext;
	string tempplain;
	string R1;
	
	int i;
	int sbox[64];
	int PC1[56];	//PC1 for key scheduling
	int *PC2;	//PC2 for key scheduling
	int P[32];
	int IP[64]={58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7};
	int invIP[64]={40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25};
	//creating sbox
	for(i=0;i<64;i++) sbox[i]=rand()%16;
	
	//creating PC1 for our use;
	for(i=0;i<56;i++) PC1[i]=i;
	random_shuffle(PC1,PC1+56);
	
	//creating PC2 for our use;
	PC2=PC1+5; //PC2 points at PC1[5] 
	
	//creating P for our use
	for(i=0;i<32;i++) P[i]=i;
	random_shuffle(P,P+32);

	//getting key and storing in mainkey;
	cout<<"Tell the key(only first 112 bits will be considered)\n";
	cin>>mainkey;
	mainkey1=mainkey.substr(0,56);	//take first 56 bits
	mainkey2=mainkey.substr(56,56);	//take last 56 bits
	
	//getting plain text and storing in plaintext
	cout << "Tell the plaintext(only 64 bits will be used) \n";
	cin >> plaintext;
	plaintext=plaintext.substr(0,64);
	
	
	
	//------------------------------------------------------------DES-cipher-------------------------------------------------------------------------------------
	
	
	
	//applying PC1;
	tempkey=mainkey1; //to set string size actually
	for(i=0;i<56;i++) tempkey[i]=mainkey1[PC1[i]];
	
	//applying Initial permutation;
	tempplain=plaintext;
	for(i=0;i<64;i++)tempplain[i]=plaintext[IP[i]-1];
	plaintext=tempplain;
	
	
	//rounds----------------------------------------------------------------------------------
	
	for(i=0;i<16;i++)
	{		
		key=keysch(&tempkey[0],PC2);
		
		//f function
		
		tempplain=plaintext;
		R1=func(tempplain,key,sbox,P,31); //f function in DES
		
		for(i=0;i<32;i++)tempplain[i+32]=tempplain[i];
		for(i=0;i<32;i++)tempplain[i]=(plaintext[i+32]==R1[i]) ? '0':'1';
		
		plaintext=tempplain;
		
	}

	//-----------------------------------------------------------------------------------------------------------
	
	//applying INverse Initial permutation;
	tempplain=plaintext;
	for(i=0;i<64;i++)tempplain[i]=plaintext[invIP[i]-1];
	plaintext=tempplain;
	
	
	
	
		//------------------------------------------------------------DES-decipher-------------------------------------------------------------------------------------
	
	
	
	//applying PC1;
	tempkey=mainkey2; //to set string size actually
	for(i=0;i<56;i++) tempkey[i]=mainkey1[PC1[i]];
	
	//applying Initial permutation;
	tempplain=plaintext;
	for(i=0;i<64;i++)tempplain[i]=plaintext[invIP[i]-1];
	plaintext=tempplain;
	
	
	//rounds----------------------------------------------------------------------------------
	
	for(i=0;i<16;i++)
	{		
		key=keysch(&tempkey[0],PC2);
		
		//f function
		
		tempplain=plaintext;
		R1=func(tempplain,key,sbox,P,-1); //f function in DES
		
		for(i=0;i<32;i++)tempplain[i]=tempplain[i+32];
		for(i=0;i<32;i++)tempplain[i+32]=(plaintext[i]==R1[i]) ? '0':'1';
		
		plaintext=tempplain;
		
	}

	//-----------------------------------------------------------------------------------------------------------
	
	//applying INverse Initial permutation;
	tempplain=plaintext;
	for(i=0;i<64;i++)tempplain[i]=plaintext[IP[i]-1];
	plaintext=tempplain;
	
	
	
	
	//------------------------------------------------------------DES-cipher-------------------------------------------------------------------------------------
	
	
	
	//applying PC1;
	tempkey=mainkey1; //to set string size actually
	for(i=0;i<56;i++) tempkey[i]=mainkey1[PC1[i]];
	
	//applying Initial permutation;
	tempplain=plaintext;
	for(i=0;i<64;i++)tempplain[i]=plaintext[IP[i]-1];
	plaintext=tempplain;
	
	
	//rounds----------------------------------------------------------------------------------
	
	for(i=0;i<16;i++)
	{		
		key=keysch(&tempkey[0],PC2);
		
		//f function
		
		tempplain=plaintext;
		R1=func(tempplain,key,sbox,P,31); //f function in DES
		
		for(i=0;i<32;i++)tempplain[i]=tempplain[i+32];
		for(i=0;i<32;i++)tempplain[i+32]=(plaintext[i]==R1[i]) ? '0':'1';
		
		plaintext=tempplain;
		
	}

	//-----------------------------------------------------------------------------------------------------------
	
	//applying INverse Initial permutation;
	tempplain=plaintext;
	for(i=0;i<64;i++)tempplain[i]=plaintext[invIP[i]-1];
	plaintext=tempplain;
	
	cout<<"\n Cipher text is : "<<plaintext;
}
