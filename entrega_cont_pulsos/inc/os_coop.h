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

#define OS_COOP_TASKS_LEN			4	// change this value to the amount of tasks the system has.

#define OS_COOP_TASK_PRIORITY_LOW	0
#define OS_COOP_TASK_PRIORITY_HIGH	1

#define OS_COOP_TASK_TYPE_REALTIME	0
#define OS_COOP_TASK_TYPE_NORMAL	1

/**
	os_coop_init : Initializes the operation system
	@param averageTasksDuration : Average duration of tasks in microseconds
*/
void os_coop_init(uint32_t averageTasksDuration);


/**
	os_coop_start : Starts the operating system. No tasks can be added while it is running.
*/
void os_coop_start(void);

/**
	os_coop_stop : Stops the operating system.
*/
void os_coop_stop(void);

/**
	os_coop_addTask		: Adds a task to the scheduler. This function must be called after os_coop_init and before os_coop_start

	@param functionPtr	: Pointer to task function.
	@param type			: Type of task. Posible values are :
							- OS_COOP_TASK_TYPE_REALTIME : If there is more than one task of this type, an offset will be calculated.
							- OS_COOP_TASK_TYPE_NORMAL : Fixed offset will be assigned to this task.
	@param priority 	: Task Priority. Posble values are:
							- OS_COOP_TASK_PRIORITY_HIGH : Task will be loaded at the top of the scheduler queue
							- OS_COOP_TASK_PRIORITY_LOW : Task will be loaded at the bottom of the scheduler queue
	@param period		: Task period in miliseconds.

	@return				: 1 if the task was added. 0 in case of error.

	NOTE: Tasks cannot block code execution. Each task must be a state machine.
*/
uint32_t os_coop_addTask(void (*functionPtr)(void),uint32_t type,uint32_t priority,uint32_t period);


/**
	os_coop_scheduler : This function must be called by system tick each 1 milisecond
*/
void os_coop_scheduler(void);


/** 
	os_coop_dispacher : This function must be called in the main loop.
*/
void os_coop_dispacher(void);

