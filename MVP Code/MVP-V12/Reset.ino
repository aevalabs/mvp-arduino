/**********************************************************************;
* Project           : Aeva Labs Demo Unit
*
* Program name      : Default.ino (Part of the MVP-V7 code)
*
* Author            : wdcosta
*
* Date created      : 06/28/2018
*
* Purpose           : This code clears all the variables used
*
|**********************************************************************/

void Clear_Array(void)
{
  temp=0;
  temp2 = 0;
  P1_Hours=0;
  P1_Total_Minutes=0;
  P1_Minutes1=0;
  P1_Minutes2=0;
  P1_TotalTime=0;
  
  P2_Hours=0;
  P2_Total_Minutes=0;
  P2_Minutes1=0;
  P2_Minutes2=0;
  P2_TotalTime=0;
  
  minutes1 = 0;
  minutes2 = 0;
  hours = 0;
  minutes3 = 0;
  minutes4 = 0;
  hours2 = 0;
  Max_minutes = 0;
  Max_minutes2 = 0;
  Max_hours = 0;
  Max_minutes3 = 0;
  Max_minutes4 = 0;
  Max_hours2 = 0;
  memset(newtemp,0,sizeof(newtemp));
  memset(newtemp2,0,sizeof(newtemp2));
  memset(newmins1,0,sizeof(newmins1));
  memset(newmins2,0,sizeof(newmins2));
  memset(newhours,0,sizeof(newhours));
  memset(newmins3,0,sizeof(newmins3));
  memset(newmins4,0,sizeof(newmins4));
  memset(newhours2,0,sizeof(newhours2));
  memset(Max_newmins1,0,sizeof(Max_newmins1));
  memset(Max_newmins2,0,sizeof(Max_newmins2));
  memset(Max_newhours,0,sizeof(Max_newhours));
  memset(Display_Current_Temp,0,sizeof(Display_Current_Temp));
  
}

void Reset_Values_Extraction(void)
{
  Max_minutes = 0;
  Max_minutes2 = 0;
  Max_hours = 0;
  minutes1 = 0;
  minutes2 = 0;
  hours = 0;
  temp = 0;
}

void Reset_Values_Maturity(void)
{
  minutes3 = 0;
  minutes4 = 0;
  hours2 = 0;
  Max_minutes3 = 0;
  Max_minutes4 = 0;
  Max_hours2 = 0;
  temp2 = 0;
}



