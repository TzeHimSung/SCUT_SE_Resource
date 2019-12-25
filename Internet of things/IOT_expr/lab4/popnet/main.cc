#include <exception>
#include <iostream>
#include "index.h"
#include "SStd.h"
#include "SRGen.h"
#include "configuration.h"
#include "sim_foundation.h"
#include "mess_queue.h"
extern "C" {
#include "SIM_power.h"
#include "SIM_router_power.h"
#include "SIM_power_router.h"
}

int main(int argc, char *argv[])
{
	try {
		SRGen random_gen;
		configuration c_par(argc, argv);
		cout<<c_par;
		mess_queue network_mess_queue(0.0);
		sim_foundation sim_net;
		network_mess_queue.simulator();
		sim_net.simulation_results();
	} catch (exception & e) {
		cerr << e.what();
	}
}
