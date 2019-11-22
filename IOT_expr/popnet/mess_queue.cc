#include "mess_queue.h"
#include "sim_foundation.h"
#include <string>
#include <iostream>
#include <strstream>
#include <unistd.h>


bool operator<(const mess_event & a, const mess_event & b) {
	return a.event_start() < b.event_start();
}

mess_queue * mess_queue::m_pointer_ = 0;

mess_queue::mess_queue(time_type start_time):
    current_time_(0),
	last_time_(0),
	mess_counter_(0),
	m_q_(),
	total_finished_(0)
{
	current_time_ = start_time;
	m_pointer_ = this;
	add_message(mess_event(0, ROUTER_));
}

string mess_queue:: mess_error_ = 
	string("This message type is not supported.\n");

void mess_queue::simulator() {

	time_type report_t = 0;
	long total_incoming = 0;
	//when mess queue is not empty and simulation deadline has not reach
	while(m_q_.size() > 0 && (current_time_ <= (configuration
					::ap().sim_length()))) {
		mess_event current_message = * get_message();
		remove_top_message();
		Sassert(static_cast<bool>(current_time_ <= ((current_message.
					event_start()) + S_ELPS_)));
		current_time_ = current_message.event_start();

		if(current_time_ > report_t) {
		   cout<<"Current time: "<<current_time_<<" Incoming packets"
			   <<total_incoming<<" Finished packets"<<total_finished_<<endl;
		   sim_foundation::wsf().simulation_results();
		   report_t += REPORT_PERIOD_;
		}
		
		switch(current_message.event_type()) {

			case EVG_ :
				sim_foundation::wsf().receive_EVG_message(current_message);
				total_incoming ++;
			break;

			case ROUTER_ :
				sim_foundation::wsf().receive_ROUTER_message(current_message);
			break;

			case WIRE_ :
				sim_foundation::wsf().receive_WIRE_message(current_message);
			break;

			case CREDIT_ :
				sim_foundation::wsf().receive_CREDIT_message(current_message);
			break;

			default:
				throw pro_error(mess_error_);
			break;
		} 
	}
}

