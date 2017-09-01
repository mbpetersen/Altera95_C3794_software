
//#include <fcntl.h>
//#include <unistd.h>
//#include <errno.h>
#include <string.h>
#include "gigle.h"
//#include "bool_check.h"
//#include "helpers.h"
//#include "elem.h"


unsigned int littleEndianizeWord(char *input)
{
  unsigned int ret_val=0;
  
  ret_val = (unsigned int)(((input[0]<<24)&0xff000000)|
                           ((input[1]<<16)&0x00ff0000)|
                           ((input[2]<<8)&0x0000ff00) |
                           (input[3]&0x000000ff));   
  
  return(ret_val);  
}

unsigned int bigEndianizeWord(char *input)
{
  unsigned int ret_val=0;
  
  ret_val = (unsigned int)(((input[0]<<24)&0xff000000)|
                           ((input[1]<<16)&0x00ff0000)|
                           ((input[2]<<8)&0x0000ff00) |
                           (input[3]&0x000000ff));   
  
  return(ret_val);  
}

int convertHexToString(char * output, char *input, int numberBits)
{
    int count=0;
    int num_output=0;
    int num_bits_left=0;
    char nibble;
        
    /*
     * convert bits to whole nibbles, and round up
     */
    num_output = numberBits/4;
    if((num_bits_left = (numberBits%4)))
        num_output++;

    for(count=0;count<num_output;count++,output++)
    {          
        if(count%2 == 0)
            nibble = (*input & 0xf0)>>4;  
        else
        {
            nibble = (*input & 0x0f);
            input++;
        }

        if(num_bits_left)
        {
            nibble &= (0x0f >> (4-num_bits_left));
            num_bits_left=0;   
        }
        
        if((0<=nibble)&&(nibble<=9))
            *output = '0'+nibble;
        else
            *output = 'a'+(nibble-10);

    }
    *output = 0;  
    return(count);  
}

unsigned int convertStringToHex(char * output, char *input, int numberBits)
{
    int num_output=0;
    int num_bits_left=0;
    int count=0;
    int num_input=0;
    char nibble;
    unsigned int result=0;
    
    /*
     * convert bits to whole nibbles, and round up
     */
    num_output = numberBits/4;
    if((num_bits_left = (numberBits%4)))
        num_output++;
    num_input=strlen(input);
    count = num_output;
    
    /* pad if the input is less than number of bits to process */
    while(count > num_input)
    {
      *output = 0x00;   
      count-=2;

      output++;        
      if(count < num_input)
        output--;      
    }
    count = num_input;   

    
    /* handle rest of string */
    count = num_output - count;
    while(num_output)
    {
      if((*input>='0')&&(*input<='9'))
        nibble = *input - '0';
      else if((*input>='a')&&(*input<='f'))
        nibble = *input - 'a'+10;
      else if((*input>='A')&&(*input<='F'))  
        nibble = *input - 'A'+10;
      else
        break;   
        
      if(num_bits_left)
      {
        char shift = (0x0f>>(4-num_bits_left));
        nibble = nibble & shift;  
        num_bits_left=0;
      }
        
      if((count%2)==0)
        *output = ((nibble<<4) & 0xF0);
      else
      {
        *output |= (nibble&0x0F);  
        output++;
      }
      input++;
      count++;
    }
    return result;
}

void stripLeadingHex(char ** input)
{
  if(strncmp(*input, "0x", 2)==0)
  {
    (*input)+=2; 
  }    
}
int validateHexString(char * input)
{
  int i=0;
  char n=0;

  if(strncmp(input, "0x", 2)==0)
    i=2;
  else 
    return FALSE;

  for(;i<strlen(input);i++)
  {
    n=*(input+i);
    if(((('0'<=n)&&(n<='9')) || 
        (('a'<=n)&&(n<='f')) ||
        (('A'<=n)&&(n<='F')))==FALSE) 
        return FALSE;
  }
  return TRUE;
    
}

/***
void formatFloatUnits(char * string, float number, int decimals)
{
    int count=0;
    int pos=0;
    char convExp[32];
    
    pos=sprintf(convExp, "%s%d%s", "%.", decimals, "f");
        
    while(1)
    {
        if(number >= 1000)
        {
            number /= 1000;            
            count+=3;    
        }
        else            
            break;
    }
        
    if(count == 0)
        sprintf(string, convExp, number); 
    else    
    {
        pos=sprintf(string, convExp, number);
        sprintf(string+pos, "x10^%d", count);
    }
}


void formatUnsignedUnits(char * string, unsigned int number, int decimals)
{
    int count=0;
    int pos=0;

    while(1)
    {
        if(number >= 1000)
        {
            number /= 1000;            
            count+=3;    
        }
        else            
            break;
    }
        
    if(count == 0)
        sprintf(string, "%u", number); 
    else    
    {
        pos=sprintf(string, "%u", number);
        sprintf(string+pos, "x10^%d", count);
    }    
}
***********/
