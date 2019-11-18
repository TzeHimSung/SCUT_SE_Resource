#include "sim_foundation.h"
#include "mess_queue.h"
#include "SStd.h"
#include <string>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>

//***************************************************************************//
//***************************************************************************//
sim_foundation * sim_foundation::s_f_ = 0;
void init_file(ifstream & inFile_);
//***************************************************************************//
sim_foundation::sim_foundation(): 
	inter_network_(),
	ary_size_(0),
	cube_size_(0),
	router_counter_(0),
	packet_counter_(0),
	inFile_()

{
	s_f_ = this;
	ary_size_ = configuration::ap().ary_number();
	cube_size_ = configuration::ap().cube_number();
	long vc_size = configuration::ap().virtual_channel_number();
	long buff_size = configuration::ap().buffer_size();
	long outbuff_size = configuration::ap().outbuffer_size();
	long flit_size = configuration::ap().flit_size();

	// + 1 means, one for injection
	long phy_ports_t = cube_size_ * 2 + 1;
	router_counter_ = ary_size_;
	for(long i = 0; i < cube_size_ - 1; i++) {
		router_counter_ = router_counter_ * ary_size_;
	}
	add_type add_t;
	add_t.resize(cube_size_, 0);
	for(long i = 0; i < router_counter_; i++) {
		inter_network_.push_back(sim_router_template
			(phy_ports_t, vc_size, buff_size, outbuff_size, add_t, 
			 ary_size_, flit_size)); 
		//assign the address of the router
		add_t[cube_size_ - 1]++;
		for(long j = cube_size_ -1; j > 0; j--) {
			if(add_t[j] == ary_size_) {
				add_t[j] = 0;
				add_t[j-1]++;
			}
		}
			
	}
	init_file();
}

//***************************************************************************//
void sim_foundation::init_file()
{
	inFile_.open(configuration::wap().trace_fname().c_str());
    if(!inFile_) {
       cerr<<"Can not open source file."<<endl;
       assert(0);
    }
    time_type event_time_t;
	inFile_ >> event_time_t;
	mess_queue::wm_pointer().add_message(mess_event(
						event_time_t, EVG_));
}
//***************************************************************************//
ostream& operator<<(ostream& os, const sim_foundation& sf)
{
	os <<"************Router list*************"<<endl;
	vector<sim_router_template>::const_iterator first = (sf.inter_network_)
																	.begin();
	vector<sim_router_template>::const_iterator last = (sf.inter_network_)
																	  .end();
	for(; first != last; first++) {
		os<<(*first);
	}
	return os;
}

//***************************************************************************//
sim_router_template & sim_foundation::router(const add_type & a) 
{
	add_type::const_iterator first = a.begin();
	add_type::const_iterator last = a.end();
	long i = (* first); first++;
	for(; first!= last; first++) {
		i = i * ary_size_ + (*first);
	}
	return (inter_network_[i]);
}

//***************************************************************************//
const sim_router_template & sim_foundation::router(const 
			add_type & a) const 
{
	add_type::const_iterator first = a.begin();
	add_type::const_iterator last = a.end();
	long i = (* first); first ++;
	for(; first!= last; first++) {
		i = i * ary_size_ + (*first);
	}
	return (inter_network_[i]);
}

//***************************************************************************//
void sim_foundation::receive_EVG_message(mess_event mesg)
{
	//first, inject the flits 
    add_type sor_addr_t;
    add_type des_addr_t;
    long pack_size_t;
    for(long i = 0; i < cube_size_; i++) {
		long t; inFile_ >> t;
		Sassert(!inFile_.eof());
		sor_addr_t.push_back(t);
	}
    for(long i = 0; i < cube_size_; i++) {
		long t; inFile_ >> t;
		Sassert(!inFile_.eof());
		des_addr_t.push_back(t);
	}
	inFile_ >> pack_size_t;
	Sassert(!inFile_.eof());
	router(sor_addr_t).receive_packet();
	packet_counter_++;

    if(!inFile_.eof()) {
        time_type event_time_t;
		inFile_ >> event_time_t;
		if(!inFile_.eof()) {
			mess_queue::wm_pointer().add_message(mess_event(
						event_time_t, EVG_));
		}
	}

}
//***************************************************************************//
//evaluate the address
bool sim_foundation::valid_address(const add_type & a) const {
	if(a.size() != cube_size_) {
		return false;
	}
	for(long i = 0; i < a.size(); i++) {
		if((a[i] >= ary_size_) || (a[i] < 0)) {
			return false;
		}
	}
	return true;
}

//***************************************************************************//
//router pipeline stage
void sim_foundation::receive_ROUTER_message(mess_event mesg)
{
	mess_queue::wm_pointer().add_message(mess_event(
				mesg.event_start() + PIPE_DELAY_, ROUTER_));

	for(long i = 0; i < router_counter_; i++) {
		inter_network_[i].router_sim_pwr();
	}
}

//***************************************************************************//
void sim_foundation::receive_WIRE_message(mess_event mesg)
{
	add_type des_t = mesg.des();
	long pc_t = mesg.pc();
	long vc_t = mesg.vc();
	flit_template & flits_t = mesg.get_flit();
	router(des_t).receive_flit(pc_t, vc_t, flits_t);
}

//***************************************************************************//
void sim_foundation::receive_CREDIT_message(mess_event mesg)
{
	add_type des_t = mesg.des();
	long pc_t = mesg.pc();
	long vc_t = mesg.vc();
	router(des_t).receive_credit(pc_t, vc_t);
}

//***************************************************************************//
void sim_foundation::simulation_results()
{
	vector<sim_router_template>::const_iterator first = 
							inter_network_.begin();
	vector<sim_router_template>::const_iterator last = 
							inter_network_.end();
	double total_delay = 0;
	//calculate the total delay
	first = inter_network_.begin();
	for(; first != last; first++) {
		total_delay += first->total_delay();
	}
	long tot_f_t = mess_queue::wm_pointer().total_finished();

	double total_mem_power = 0;
	double total_crossbar_power = 0;
	double total_arbiter_power = 0;
	double total_power = 0;
	double total_link_power = 0;
	vector<sim_router_template>::iterator ffirst = 
							inter_network_.begin();
	vector<sim_router_template>::iterator llast = 
							inter_network_.end();
	ffirst = inter_network_.begin();
	for(; ffirst != llast; ffirst++) {
		total_mem_power += ffirst->power_buffer_report();
		total_crossbar_power += ffirst->power_crossbar_report();
		total_arbiter_power += ffirst->power_arbiter_report();
		total_link_power += ffirst->power_link_report();
	}
	time_type curr_time = mess_queue::m_pointer().current_time();
	total_mem_power /= curr_time;
	total_crossbar_power /= curr_time;
	total_link_power /= curr_time;
	total_arbiter_power /= curr_time;
	total_power = total_mem_power + total_crossbar_power + total_arbiter_power +
		total_link_power;

	cout.precision(6);
	cout<<"**************************************************"<<endl;
	cout<<"total finished:       "<<tot_f_t<<endl;
	cout<<"average Delay:        "<< total_delay/tot_f_t<<endl;
	cout<<"total mem power:      "<<total_mem_power * POWER_NOM_<<endl;
	cout<<"total crossbar power: "<<total_crossbar_power * POWER_NOM_<<endl;
	cout<<"total arbiter power:  "<<total_arbiter_power * POWER_NOM_<<endl;
	cout<<"total link power:     "<<total_link_power * POWER_NOM_<<endl;
	cout<<"total power:          "<<total_power * POWER_NOM_<<endl;
	cout<<"**************************************************"<<endl;
}

//***************************************************************************//
//Check if the network is back to the inital state.
void sim_foundation::simulation_check()
{
	vector<sim_router_template>::const_iterator first = 
					inter_network_.begin();
	vector<sim_router_template>::const_iterator last = 
					inter_network_.end();
	for(; first != last; first++) {
		first->empty_check();
	}
	cout<<"simulation empty check is correct.\n";
}

//***************************************************************************//
string sim_foundation:: file_name_ =
 string("Invaid file name.\n");

//***************************************************************************//
