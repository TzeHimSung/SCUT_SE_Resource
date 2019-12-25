#include <math.h>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include "sim_router.h"
#include "sim_foundation.h"
#include "mess_queue.h"
#include "mess_event.h"
#include "SRGen.h"
#include "SStd.h"

// *****************************************************//
// data structure to model the structure and behavior   //
// of routers.                                          //
// *****************************************************//
ostream & operator<<(ostream& os, const sim_router_template & sr) 
{
	long k = sr.address_.size();
	for(long i = 0; i < k; i++) {
		os<<sr.address_[i]<<" ";
	}
	os<<endl;
	return os;
}

//***************************************************************************//
sim_router_template::sim_router_template():
	address_(),
	input_module_(),
	output_module_(),
	power_module_(),
	init_data_(),
	ary_size_(),
	flit_size_(),
	physic_ports_(),
	vc_number_(),
	buffer_size_(),
	outbuffer_size_(),
	total_delay_(),
	routing_alg_(),
	curr_algorithm(),
	local_input_time_(),
	packet_counter_(),
	localinFile_()
	
{
}

//***************************************************************************//
//a : physical ports, b: vc number, c: buffer size,  d: output
//buffer size, e: address  f: ary_size_ g: flit_size_
sim_router_template::sim_router_template(long a, long b, long c,
	long d, const add_type & e, long f, long g):
	address_(e),
	input_module_(a, b),
	output_module_(a, b, c, d),
	power_module_(a, b, g),
	init_data_(),
	ary_size_(f),
	flit_size_(g),
	physic_ports_(a),
	vc_number_(b),
	buffer_size_(c),
	outbuffer_size_(d),
	total_delay_(0),
	routing_alg_(0),
	curr_algorithm(0),
	local_input_time_(),
	packet_counter_(0),
	localinFile_()
{
	init_data_.resize(g);
	for(long i = 0; i < g; i++) {
		init_data_[i] = SRGen::wrg().flat_ull(0, MAX_64_);
	}
	//get the trace file name
	init_local_file();
    localinFile() >> local_input_time_;

	routing_alg_ = configuration::ap().routing_alg();
	switch (routing_alg_) {

		case XY_ :
		curr_algorithm =  & sim_router_template::XY_algorithm;
		break;

		case TXY_ :
		curr_algorithm = & sim_router_template::TXY_algorithm;
		break;

		default :
		Sassert(0);
		break;
	}
}

power_template::power_template():
	flit_size_(),
	router_info_(),
	router_power_(),
	arbiter_vc_power_(),
	link_power_(),
	buffer_write_(),
	buffer_read_(),
	crossbar_read_(),
	crossbar_write_(),
	link_traversal_(),
	crossbar_input_(),
	arbiter_vc_req_(),
	arbiter_vc_grant_()
{
}
//a: physical ports, b: virtual ports c: flit size
power_template::power_template(long a, long b, long c):
	flit_size_(c),
	router_info_(),
	router_power_(),
	arbiter_vc_power_(),
	link_power_(),
	buffer_write_(),
	buffer_read_(),
	crossbar_read_(),
	crossbar_write_(),
	link_traversal_(),
	crossbar_input_(),
	arbiter_vc_req_(),
	arbiter_vc_grant_()

{
	FUNC(SIM_router_power_init, &router_info_, & router_power_);
	buffer_write_.resize(a);
	buffer_read_.resize(a);
	crossbar_read_.resize(a);
	crossbar_write_.resize(a);
	link_traversal_.resize(a);
	crossbar_input_.resize(a,0);
	for(long i = 0; i < a; i ++) {
		buffer_write_[i].resize(flit_size_, 0);
		buffer_read_[i].resize(flit_size_, 0);
		crossbar_read_[i].resize(flit_size_, 0);
		crossbar_write_[i].resize(flit_size_, 0);
		link_traversal_[i].resize(flit_size_, 0);
	}
	SIM_arbiter_init(& arbiter_vc_power_, 1, 1, a * b, 0, NULL);
	arbiter_vc_req_.resize(a);
	arbiter_vc_grant_.resize(a);
	for(long i = 0; i < a; i ++) {
		arbiter_vc_req_[i].resize(b, 1);
		arbiter_vc_grant_[i].resize(b, 1);
	}
	SIM_bus_init(& link_power_, GENERIC_BUS, IDENT_ENC, ATOM_WIDTH_, 
			0, 1, 1, configuration::wap().link_length(), 0);
}

void power_template::power_buffer_read(long in_port, Data_type & read_d)
{
	for(long i = 0; i < flit_size_; i++) {
		FUNC(SIM_buf_power_data_read, &(router_info_.in_buf_info), 
			&(router_power_.in_buf), read_d[i]);
		buffer_read_[in_port][i] = read_d[i];
	}
}
void power_template::power_link_traversal(long in_port, Data_type & read_d)
{
	for(long i = 0; i < flit_size_; i++) {
		Atom_type old_d = link_traversal_[in_port][i];
		Atom_type new_d = read_d[i];
		SIM_bus_record(&link_power_, old_d, new_d);
		link_traversal_[in_port][i] = read_d[i];
	}
}
int i = 0;
void power_template::power_buffer_write(long in_port, Data_type & write_d)
{
  
 
	for( i = 0; i < flit_size_; i ++) {
		Atom_type old_d = buffer_write_[in_port][i];
		Atom_type new_d = write_d[i];
		Atom_type old_d2 = buffer_write_[in_port][i];
		Atom_type new_d2 = write_d[i];
      
        FUNC(SIM_buf_power_data_write, &(router_info_.in_buf_info), 
			&(router_power_.in_buf), (char *) (&old_d), 
			(char *) (&old_d),
             (char *) (&new_d));
        
		buffer_write_[in_port][i] = write_d[i];

	}
}

void power_template::power_crossbar_trav(long in_port, long out_port, 
		Data_type & trav_d)
{
	for(long i = 0; i < flit_size_; i++) {
		SIM_crossbar_record(&(router_power_.crossbar), 1, trav_d[i], 
				crossbar_read_[in_port][i], 1, 1);
		SIM_crossbar_record(&(router_power_.crossbar), 0, trav_d[i], 
				crossbar_write_[out_port][i], crossbar_input_[out_port], 
				in_port);

		crossbar_read_[in_port][i] = trav_d[i];
		crossbar_write_[out_port][i] = trav_d[i];
		crossbar_input_[out_port] = in_port;
	}
}
void power_template::power_vc_arbit(long pc, long vc, Atom_type req, 
		unsigned long gra){
	SIM_arbiter_record(& arbiter_vc_power_, req, arbiter_vc_req_[pc][vc],
			gra, arbiter_vc_grant_[pc][vc]);
	arbiter_vc_req_[pc][vc] = req;
	arbiter_vc_grant_[pc][vc] = gra;
}

double power_template::power_arbiter_report()
{
	return SIM_arbiter_report(& arbiter_vc_power_);
}

double power_template::power_buffer_report()
{
	return SIM_array_power_report(&(router_info_.in_buf_info), 
			&(router_power_.in_buf));
}

double power_template::power_crossbar_report()
{
	return SIM_crossbar_report(&(router_power_.crossbar));
}

double power_template::power_link_report()
{
	return SIM_bus_report(& link_power_);
}

//***************************************************************************//
void sim_router_template::init_local_file(){
    string name_t = configuration::wap().trace_fname();
    for(long i = 0; i < address_.size(); i++) {
       ostringstream n_t;
       n_t << address_[i];
       name_t =name_t +  "." + n_t.str();
    }
	localinFile_ = new ifstream;
    localinFile_->open(name_t.c_str());
    if(!localinFile_) {
       cerr<<"can not open trace file :"<<name_t<<endl;
       assert(0);
   }
}

//***************************************************************************//
ostream & operator<<(ostream& os, const input_template& Ri)
{
	for(long i=0; i < (Ri.input_).size(); i++) {
		for(long j=0; j < ((Ri.input_)[i]).size(); j++) {
			for(long k=0; k < ((Ri.input_)[i][j]).size(); k++) {
				os<<Ri.input_[i][j][k]<<" ";
			} os<<"|";
		} os<<endl;
	}
	return os;
}

//***************************************************************************//

input_template::input_template():
	input_(),
	states_(),
	routing_(),
	crouting_(),
	ibuff_full_()
{
}

//***************************************************************************//
//a: phy ports. b: virtual number
input_template::input_template(long a, long b):
	input_(),
	states_(),
	routing_(),
	crouting_(),
	ibuff_full_(false)
{
	input_.resize(a);
	for(long i = 0; i < a; i++) {
		input_[i].resize(b);
	}
	states_.resize(a);
	for(long i = 0; i < a; i++) {
		states_[i].resize(b, INIT_);
	}
	routing_.resize(a);
	for(long i = 0; i < a; i++) {
		routing_[i].resize(b);
	}
	crouting_.resize(a);
	for(long i = 0; i < a; i++) {
		crouting_[i].resize(b, VC_NULL);
	}
}

//***************************************************************************//
//output buffer counter module
output_template::output_template():
	buffer_size_(),
  	counter_(),
	flit_state_(),
	assign_(),
	usage_(),
	outbuffers_(),
	outadd_(),
	localcounter_()
{
}
//a: phy size. b: vir number. c: input buffer size. d: output buffer size
output_template::output_template(long a, long b, long c, long d):
	buffer_size_(c),
	counter_(),
	flit_state_(),
	assign_(),
	usage_(),
	outbuffers_(),
	outadd_(),
	localcounter_()
{
	counter_.resize(a);
	for(long i = 0; i < a; i++) {
		counter_[i].resize(b, c);
	}
	localcounter_.resize(a, d);
	assign_.resize(a);
	for(long i = 0; i < a; i++) {
		assign_[i].resize(b, VC_NULL);
	}
	usage_.resize(a);
	for(long i = 0; i < a; i++) {
		usage_[i].resize(b, FREE_);
	}
	outbuffers_.resize(a);
	flit_state_.resize(a);
	outadd_.resize(a);
}

//***************************************************************************//
void sim_router_template::receive_credit(long a, long b)
{
	output_module_.counter_inc(a, b);
}

//***************************************************************************//
void sim_router_template::receive_packet()
{
	time_type event_time = mess_queue::m_pointer().current_time();
	long cube_s = sim_foundation::wsf().cube_size();
	add_type sor_addr_t;
	add_type des_addr_t;
	long pack_size_t;
	long pack_c;
	while((input_module_.ibuff_full() == false) && (local_input_time_ <=
				event_time + S_ELPS_)) {
		sor_addr_t.clear();
		des_addr_t.clear();
	    for(long i = 0; i < cube_s; i++) {
	        long t; localinFile() >> t;
	        if(localinFile().eof()) {
				return;
			}
	        sor_addr_t.push_back(t);
	    }
	    //read destination address
    	for(long i = 0; i < cube_s; i++) {
  	        long t; localinFile() >> t;
	        if(localinFile().eof()) {
				return;
			}
   	        des_addr_t.push_back(t);
  	    }
    	//read packet size
    	localinFile() >> pack_size_t;
	    inject_packet(packet_counter_, sor_addr_t,
	            des_addr_t, local_input_time_, pack_size_t);
	    packet_counter_++;

   	 	//second, create next EVG_ event
	    if(!localinFile().eof()) {
       	 	localinFile() >> local_input_time_;
       		if(localinFile().eof()) {
				return;
			}
    	}
	}
}
//***************************************************************************//
//a : flit id. b: sor add. c: des add. d: start time. e: size
void sim_router_template::inject_packet(long a, add_type & b, add_type & c,
			time_type d, long e)
{
	// if it is the HEADER_ flit choose the shortest waiting vc queue
	// next state, it should choose routing
	VC_type vc_t; 
	for(long l = 0; l < e; l++) {
		Data_type flit_data;
		for(long i = 0; i < flit_size_; i++) {
			init_data_[i] = static_cast<Atom_type>(init_data_[i] * CORR_EFF_ + SRGen::wrg().
				flat_ull(0, MAX_64_));
			flit_data.push_back(init_data_[i]);
		}
		if(l == 0) {
			vc_t = pair<long, long> (0, input_module_.input(0,0).size());
			for(long i = 0; i < vc_number_; i++) {
				long t = input_module_.input(0,i).size();
				if(vc_t.second > t){
				vc_t = pair<long, long>(i, t);
				}
			}
			//if the input buffer is empty, set it to be ROUTING_
			if(input_module_.input(0, vc_t.first).size() == 0) {
				input_module_.state_update(0, vc_t.first, ROUTING_);
			}
			//if the input buffer has more than predefined flits, then
			// add the flits and sign a flag
			if(input_module_.input(0, vc_t.first).size() > 100) {
				input_module_.ibuff_is_full();
			}
			input_module_.add_flit(0, (vc_t.first),  
								flit_template(a, HEADER_, b, c, d, flit_data));
		}else if(l == (e - 1)){
			input_module_.add_flit(0, (vc_t.first),  
								flit_template(a, TAIL_, b, c, d, flit_data));
		}else {
			input_module_.add_flit(0, (vc_t.first),  
								flit_template(a, BODY_, b, c, d, flit_data));
		}
		power_module_.power_buffer_write(0, flit_data);
	}

}

//***************************************************************************//
//receive a flit from other router
void sim_router_template::receive_flit(long a, long b, flit_template & c)
{
	input_module_.add_flit(a, b, c);
	power_module_.power_buffer_write(a, c.data());
	if(c.type() == HEADER_) {
		if(input_module_.input(a,b).size() == 1) {
			input_module_.state_update(a, b, ROUTING_);
		}else {
		}
	}else {
		if(input_module_.state(a,b) == INIT_) {
			input_module_.state_update(a, b, SW_AB_);
		}
	}
}

//***************************************************************************//
//switch arbitration pipeline stage
void sim_router_template::sw_arbitration() {
	map<long, vector<VC_type> > vc_o_map;
	for(long i = 0; i < physic_ports_; i++) {
		vector<long> vc_i_t;
		for(long j = 0; j < vc_number_; j++) {
			if(input_module_.state(i, j) == SW_AB_) {
				VC_type out_t = input_module_.crouting(i, j);
				if((output_module_.counter(out_t.first, out_t.second) > 0) &&
				   (output_module_.localcounter(out_t.first) > 0)) {
					vc_i_t.push_back(j);
				}
			}
		}
		long vc_size_t = vc_i_t.size();
		if(vc_size_t > 1) {
			long win_t = SRGen::wrg().flat_l(0, vc_size_t);
			VC_type r_t = input_module_.crouting(i, vc_i_t[win_t]);
			vc_o_map[r_t.first].push_back(VC_type(i, vc_i_t[win_t]));
		}else if(vc_size_t == 1) {
			VC_type r_t = input_module_.crouting(i, vc_i_t[0]);
			vc_o_map[r_t.first].push_back(VC_type(i, vc_i_t[0]));
		}
	}

	if(vc_o_map.size() == 0) {
		return;
	}

	for(long i = 0; i < physic_ports_; i++) {
		long vc_size_t = vc_o_map[i].size();
		if(vc_size_t > 0) {
			VC_type vc_win = vc_o_map[i][0];
			if(vc_size_t > 1) {
				vc_win = vc_o_map[i][SRGen::wrg().flat_l(0,
						vc_size_t)];
			}
			input_module_.state_update(vc_win.first, vc_win.second,
					SW_TR_);
			flit_template & flit_t = input_module_.get_flit(
					vc_win.first, vc_win.second);
		}
	}
}

//***************************************************************************//
//flit out buffer to the output buffer
void sim_router_template::flit_outbuffer()
{
	for(long i = 0; i < physic_ports_; i++) {
		for(long j = 0; j < vc_number_; j++) {
			if(input_module_.state(i, j) == SW_TR_) {
				VC_type out_t = input_module_.crouting(i, j);
				output_module_.counter_dec(out_t.first, out_t.second);

				time_type event_time = mess_queue::m_pointer().current_time();
				if(i != 0) {
					add_type cre_add_t = address_;
					long cre_pc_t = i;
					if((i % 2) == 0) {
						cre_pc_t = i - 1;
						cre_add_t[(i-1)/2] ++;
						if(cre_add_t[(i-1)/2] == ary_size_) {
							cre_add_t[(i-1)/2] = 0;
						}
					}else {
						cre_pc_t = i + 1;
						cre_add_t[(i-1)/2] --;
						if(cre_add_t[(i-1)/2] == -1) {
							cre_add_t[(i-1)/2] = ary_size_ - 1;
						}
					}
					mess_queue::wm_pointer().add_message(
						mess_event(event_time + CREDIT_DELAY_, 
						CREDIT_, address_, cre_add_t, cre_pc_t, j));  
				}

				long in_size_t = input_module_.input(i,j).size();
				Sassert(in_size_t >= 1);
				flit_template flit_t(input_module_.get_flit(i,j));
				input_module_.remove_flit(i, j);
				power_module_.power_buffer_read(i, flit_t.data());
				power_module_.power_crossbar_trav(i, out_t.first, flit_t.data());
				output_module_.add_flit(out_t.first, flit_t);
				if(i == 0) {
					if(input_module_.ibuff_full() == true) {
						if(input_module_.input(0,j).size() < BUFF_BOUND_) {
							input_module_.ibuff_not_full();
							receive_packet();
						}
					}
				}
				output_module_.add_add(out_t.first, out_t);
				if(flit_t.type() == TAIL_) {
					output_module_.release(out_t.first, out_t.second);
				}
				if(in_size_t > 1) {
					if(flit_t.type() == TAIL_) {
						if(configuration::ap().vc_share() == MONO_) {
							if(i != 0){
							if(in_size_t != 1) {
								cout<<i<<":"<<in_size_t<<endl;
							}
							Sassert(in_size_t == 1);}
						}
						input_module_.state_update(i, j, ROUTING_);
					}else {
						input_module_.state_update(i, j, SW_AB_);
					}
				}else {
					input_module_.state_update(i, j, INIT_);
				}
			}
		}
	}

}

//***************************************************************************//
//flit traversal through the link stage
void sim_router_template::flit_traversal(long i)
{
	time_type event_time = mess_queue::m_pointer().current_time();
	if(output_module_.outbuffers(i).size() > 0) {
		time_type flit_delay_t = WIRE_DELAY_ + event_time;
		add_type wire_add_t = address_;
		long wire_pc_t ;
		if((i % 2) == 0) {
			wire_pc_t = i - 1;
			wire_add_t[(i - 1) / 2] ++;
			if(wire_add_t[(i-1) / 2] == ary_size_) {
				wire_add_t[(i-1) / 2] = 0;
			}
		}else {
			wire_pc_t = i + 1;
			wire_add_t[(i - 1) / 2] --;
			if(wire_add_t[(i-1) / 2] == -1) {
				wire_add_t[(i-1) / 2] = ary_size_ - 1;
			}
		}
		flit_template flit_t(output_module_.get_flit(i));
		VC_type outadd_t = output_module_.get_add(i);
		power_module_.power_link_traversal(i, flit_t.data());

		output_module_.remove_flit(i);
		output_module_.remove_add(i);
		mess_queue::wm_pointer().add_message(mess_event(flit_delay_t,
			WIRE_, address_, wire_add_t, wire_pc_t, 
			outadd_t.second, flit_t));
	}
}

//***************************************************************************//
//receive the flit at the destination router
void sim_router_template::accept_flit(time_type a, const flit_template & b)
{
    if(b.type() == TAIL_) {
		mess_queue::wm_pointer().TotFin_inc();
		time_type t = a - b.start_time();
		delay_update(t);
	}
}

//***************************************************************************//
//flit traversal through link 
void sim_router_template::flit_traversal()
{
	for(long i = 1; i < physic_ports_; i++) {
		flit_traversal(i);
	}
}
//***************************************************************************//
//routing pipeline stages 
void sim_router_template::router_sim_pwr()
{
	//stage 5 flit traversal
	flit_traversal();
	//stage 4 flit output buffer
	flit_outbuffer();
	//stage 3 switch arbitration
	sw_arbitration();
	//stage 2, vc arbitration
	vc_arbitration();
	//stage 1, routing decision
	routing_decision();
}


//***************************************************************************//

void sim_router_template::empty_check() const
{
	for(long i = 0; i < physic_ports_; i ++) {
		for(long j = 0; j < vc_number_; j ++) {
			if(input_module_.input(i, j).size() > 0) {
				cout<<"Input is not empty"<<endl;
				//Sassert(0);
			}
			if(input_module_.state(i,j) != INIT_) {
				cout<<"Input state is wrong"<<endl;
				//Sassert(0);
			}cout <<output_module_.counter(i,j)<<":";
			if(output_module_.counter(i,j) != buffer_size_) {
				cout<<"Output vc counter is wrong"<<endl;
				//Sassert(0);
			}
			if(output_module_.usage(i,j) != FREE_) {
				cout<<"Output is not free"<<endl;
				//Sassert(0);
			}
			if(output_module_.assign(i,j) != VC_NULL) {
				cout<<"Output is not free"<<endl;
				//Sassert(0);
			}
		}
		if(output_module_.outbuffers(i).size() > 0) {
			cout<<"Output temp buffer is not empty"<<endl;
			//Sassert(0);
		}
		if(output_module_.outadd(i).size() > 0) {
			cout<<"Output temp buffer is not empty"<<endl;
			//Sassert(0);
		}
		if(output_module_.localcounter(i) != outbuffer_size_) {
			cout<<"Output local counter is not reset"<<endl;
			//Sassert(0);
		}
	}
}
