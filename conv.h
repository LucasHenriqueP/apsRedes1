#ifndef _CONV_H
#define _CONV_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int tabela(char c){
  switch (c) {
    case 48:
      return 0;
      break;
    case 49:
      return 1;
      break;
    case 50:
      return 2;
      break;
    case 51:
      return 3;
      break;
    case 52:
      return 4;
      break;
    case 53:
      return 5;
      break;
    case 54:
      return 6;
      break;
    case 55:
      return 7;
      break;
    case 56:
      return 8;
      break;
    case 57:
      return 9;
      break;
    case 65:
      return 10;
      break;
    case 66:
      return 11;
      break;
    case 67:
      return 12;
      break;
    case 68:
      return 13;
      break;
    case 69:
      return 14;
      break;
    case 70:
      return 15;
      break;
    default:
      return 88;
  }
}

int strToInt(char* str){

  if(strlen(str) == 1){
    str[2] = str[0];
    str[0] = '0';
    str[1] = '0';
    str[3] = '\0';
  }
  else if(strlen(str) == 2){
    str[2] = str[1];
    str[1] = str[0];
    str[0] = '0';
    str[3] = '\0';
  }
  int i = 0;
  int res = 0;
  int ac = 0;
  int t = strlen(str);
  if (t>3 || t < 0) return -1;
  for(i = 0; i<t; i++){
    res = tabela(str[i]);

    switch (i) {
      case 0:
        ac += res*100;
        break;
      case 1:
        ac += res*10;
        break;
      case 2:
        ac += res*1;
        break;
      default:
        break;
    }

  }
  return ac;

}


int strHexToInt(char* str){
  if(str[0] > 64) str[0] = toupper(str[0]);
  if(str[1] > 64) str[1] = toupper(str[1]);
  int oc1 = tabela(str[0]);
  int oc2 = tabela(str[1]);
  oc1 = oc1 << 4;
  return oc1 | oc2;
}

int* getIP(char* ip){
  int* ret;
  char aux [6];
  ret = (int*) calloc(4, sizeof(int));
  int j, i, pos =0;
  j = strlen(ip);
  ip[j] = '.';
  ip[j+1] = '\0';

  j=0;

  for(i = 0; i < strlen(ip); i++){
    if(ip[i] != '.'){

      aux[pos] = ip[i];
      pos++;
    }
    else{
      aux[pos] = '\0';
      ret[j] = strToInt(aux);
      j++;
      pos = 0;
    }

  }
  return ret;


}
int* getMac(char* mac){
  int* ret;
  char aux [6];
  ret = (int*) calloc(6, sizeof(int));
  int j, i, pos = 0;
  j = strlen(mac);
  mac[j] = ':';
  mac[j+1] = '\0';
  j=0;

  for(i = 0; i < strlen(mac); i++){
    if(mac[i] != ':'){

      aux[pos] = mac[i];
      pos++;
    }
    else{
      aux[pos] = '\0';
      ret[j] = strHexToInt(aux);
      j++;
      pos = 0;
    }

  }
  return ret;

}


#endif
