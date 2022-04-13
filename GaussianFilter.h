#ifndef Gaussian_FILTER_H_
#define Gaussian_FILTER_H_
#include <systemc>
using namespace sc_core;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

#include "filter_def.h"

class GaussianFilter: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<8> >::in i_r[9];
	cynw_p2p< sc_dt::sc_uint<8> >::in i_g[9];
	cynw_p2p< sc_dt::sc_uint<8> >::in i_b[9];
	cynw_p2p< sc_dt::sc_uint<32> >::out o_result;
#else
	sc_fifo_in< sc_dt::sc_uint<24> > i_rgb[9];
	sc_fifo_out< sc_dt::sc_uint<32> > o_result;
#endif

	SC_HAS_PROCESS( GaussianFilter );
	GaussianFilter( sc_module_name n );
	~GaussianFilter();
private:
	void do_filter();
	int val;
	unsigned char grey[9];
};
#endif
