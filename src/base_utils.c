#include "base_utils.h"


char digit_to_char(int n){
    char digit_char[]="0123456789";
    return digit_char[n];
}
long expo(int n, int p){


    long el = 1;

    for(int i=0;i<p;i++){
        el *= n;
    }
    return el;
}
void reverse(char tab[]){
    int n= strlen(tab);
    for (int i=0;i<n/2;i++){
        int c = tab[n-i-1];
        tab[n-i-1] = tab[i];
        tab[i]=c;

    }
}
int int_to_char(int n, char int_[]){
    
    int i = 0;
    int residu = n;
    int el, p;
    while(residu!=0){
        i++;
        p = expo(10,i);
        el = residu % p;
        residu -= el;
        el = el/ ( p/10 );
        int_[i-1] = digit_to_char(el);  
    }
    int_[i] = '\0';
    reverse(int_);
    return i;
}
int long_to_char(long long n, char int_[]){
    
    int i = 0;
    long long residu = n;
    long long el, p;
    while(residu!=0){
        i++;
        p = expo(10,i);
        el = residu % p;
        residu -= el;
        el = el/ ( p/10 );
        int_[i-1] = digit_to_char(el);  
    }
    int_[i] = '\0';
    reverse(int_);
    return i;
}

char *replace(char *before, char *oldsub, char *newsub){
  
  int old_length = strlen(oldsub);
  int new_length = strlen(newsub);
  int before_length = strlen(before);
  char *after;
  if (old_length == new_length)
  { 
    after = (char *)malloc((before_length + 1) * sizeof(char));
  }
  else
  {
    int occurrences = 0;
    int i = 0;
    while (i < before_length)
    {
      if (strstr(&before[i], oldsub) == &before[i])
      { 
        occurrences++;
        i += old_length;
      } 
      else i++;
    }
    int sub_diff = new_length - old_length;
    int after_length = before_length;
    after_length += occurrences * sub_diff;   
    after =  (char *)malloc((after_length + 1) * sizeof(char));
  }  
  int i = 0;
  int j = 0;
  while (i < (int)strlen(before))
  {
    if (strstr(&before[i], oldsub) == &before[i])
    {   
      strcpy(&after[j], newsub); 
      i += old_length;
      j += new_length;
    }
    else
    {
      after[j] = before[i];
      i++;
      j++;
    }
  }
  j--;
  after[j] = '\0';
  return after;
}
