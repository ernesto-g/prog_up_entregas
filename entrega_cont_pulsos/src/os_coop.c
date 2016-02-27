/*
	Cooperative Operating System
    Copyright (C) <2016>  <Ernesto Gigliotti>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdint.h"
#include "os_coop.h"

struct S_taskData{
	uint32_t priority;
	uint32_t type;
	void (*functionPtr)(void);
	uint32_t period;
	uint32_t periodValue;
	uint32_t offset;
	uint32_t flagExecute;
	uint32_t flagEmpty;
};
typedef struct S_taskData	OsCoopTaskData;

struct S_osCoopData{
	uint32_t flagRun;
	uint32_t averageTasksDuration;

};
typedef struct S_osCoopData	OsCoopData;




static void recalculateTasksOffsets(void);

static volatile OsCoopTaskData tasksData[OS_COOP_TASKS_LEN];
static volatile OsCoopData osCoopData;

void os_coop_init(uint32_t averageTasksDuration)
{
	osCoopData.flagRun = 0;

	uint32_t i;
	for(i=0; i<OS_COOP_TASKS_LEN; i++)
		tasksData[i].flagEmpty=1;

	osCoopData.averageTasksDuration = averageTasksDuration;
}
void os_coop_start(void)
{
	osCoopData.flagRun = 1;
}
void os_coop_stop(void)
{
	osCoopData.flagRun = 0;
}

uint32_t os_coop_addTask(void (*functionPtr)(void),uint32_t type,uint32_t priority,uint32_t period)
{
	uint32_t i;
	uint32_t freeIndex=-1;
	if(priority==OS_COOP_TASK_PRIORITY_HIGH)
	{
		for(i=0; i<OS_COOP_TASKS_LEN; i++)
		{
			if(tasksData[i].flagEmpty==1)
			{
				freeIndex=i;
				break;
			}
		}
	}
	else
	{
		for(i=OS_COOP_TASKS_LEN-1; i>0; i--)
		{
			if(tasksData[i].flagEmpty==1)
			{
				freeIndex=i;
				break;
			}
		}
	}
	if(freeIndex==-1)
		return 0; // error

	tasksData[freeIndex].type = type;
	tasksData[freeIndex].priority = priority;
	tasksData[freeIndex].period = period;
	tasksData[freeIndex].functionPtr = functionPtr;
	tasksData[freeIndex].flagExecute = 0;
	tasksData[freeIndex].periodValue = period;
	tasksData[freeIndex].flagEmpty = 0;

	recalculateTasksOffsets();

	return 1;
}

void os_coop_scheduler(void) // each milisecond
{
	if(osCoopData.flagRun==1)
	{
		uint32_t i;
		for(i=0; i<OS_COOP_TASKS_LEN; i++)
		{
			if(tasksData[i].flagEmpty==0)
			{	
				if(tasksData[i].offset>0)
				{
					tasksData[i].offset--;
					continue;
				}
				else
				{
					if(tasksData[i].periodValue>0)
					{
						tasksData[i].periodValue--;
						if(tasksData[i].periodValue==0)
						{
							tasksData[i].flagExecute=1;
							tasksData[i].periodValue = tasksData[i].period;
						}
					}
					else if(tasksData[i].period==0)
						tasksData[i].flagExecute=1;
				}
			}
		}
	}
}


void os_coop_dispacher(void)
{
		uint32_t i;
		for(i=0; i<OS_COOP_TASKS_LEN; i++)
		{
			if(tasksData[i].flagEmpty==0)
			{
				if(tasksData[i].flagExecute==1)
				{
					tasksData[i].functionPtr();
					if(tasksData[i].period!=0)
						tasksData[i].flagExecute=0;
				}				
			}
		}	
}


static void recalculateTasksOffsets(void)
{
	uint32_t i;
	for(i=0; i<OS_COOP_TASKS_LEN; i++)
	{
		if(tasksData[i].flagEmpty==0)
		{
			tasksData[i].offset=0; // TODO
		}
	}
}
