#include <stdio.h>
#include "utils.h"

int 
record_delay_time = 2 ; 


int 
replay_delay_time = 10 ; 

int action_mode = TG_REPLAY; 


void 
delay(void)
{
  if (action_mode==TG_REPLAY)
    {
      sleep(replay_delay_time);
    }
  return ;
}


void 
rec_delay(void)
{
  if (action_mode==TG_RECORD)
    {
      sleep(record_delay_time);
    }
  return ;
}


void 
set_mode(int mode)
{
  action_mode=mode;
  return ;
}

