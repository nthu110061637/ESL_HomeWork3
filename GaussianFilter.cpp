#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "GaussianFilter.h"
#include "directives.h"

GaussianFilter::GaussianFilter( sc_module_name n ): sc_module( n )
{

	SC_THREAD( do_filter );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
        
#ifndef NATIVE_SYSTEMC
	for(unsigned int k = 0; k<9; ++k)
	{
		i_r[k].clk_rst(i_clk, i_rst);
    i_g[k].clk_rst(i_clk, i_rst);
    i_b[k].clk_rst(i_clk, i_rst);
	}
	
  o_result.clk_rst(i_clk, i_rst);
#endif
}

GaussianFilter::~GaussianFilter() {}

// Gaussian mask
const short mask[MASK_X][MASK_Y] = {{1,2,1}, {2,4,2}, {1,2,1}};
//const double mask[MASK_N][MASK_X][MASK_Y] = {{{0.077847, 0.123317, 0.077847}, {0.123317, 0.195346, 0.123317}, {0.077847, 0.123317, 0.077847}}};
//const int mask[MASK_X][MASK_Y] = {{77847, 123317, 77847}, {123317, 195346, 123317}, {77847, 123317, 77847}};

/*void GaussianFilter::do_filter() {
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_rgb.reset();
		o_result.reset();
#endif
		wait();
	}
	while (true) {
		PIPELINING;
		val = 0;
		for (unsigned int v = 0; v<MASK_Y; ++v) {
			CONV_LOOP;
			for (unsigned int u = 0; u<MASK_X; ++u) {
				sc_dt::sc_uint<24> rgb;
#ifndef NATIVE_SYSTEMC
				{
					HLS_DEFINE_PROTOCOL("input");
					rgb = i_rgb.get();
					wait();
				}
#else
				rgb = i_rgb.read();
#endif
				unsigned char grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16))/3;
        val += grey * mask[u][v]/1000000;
        //printf("%d",val);
			}
		}
		int total = 0;
		total = val * val;
		
#ifndef NATIVE_SYSTEMC
		{
			HLS_DEFINE_PROTOCOL("output");
			o_result.put(total);
			wait();
		}
#else
		  o_result.write(total);
#endif
    //printf("\n");
	}
}*/
/*void GaussianFilter::do_filter() {
	//{
	#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_rgb.reset();
		o_result.reset();
	#endif
		wait();
	//}
	while (true) {
		PIPELINING;
		val = 0;
    	unsigned char grey = 0;
		sc_dt::sc_uint<24> rgb;
		{
			HLS_CONSTRAIN_LATENCY(0, 2, "lat00");
			#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("input");
				rgb = i_rgb.get();
				wait();
			}
			#else
					rgb = i_rgb.read();
			#endif
			grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16))/3;
			//val += (grey * mask[0][0])/1000000;
			val += (grey * mask[0][0])/16;
		}
    
		{
			HLS_CONSTRAIN_LATENCY(0, 2, "lat01");
			#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("input");
				rgb = i_rgb.get();
				wait();
			}
			#else
					rgb = i_rgb.read();
			#endif
			grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16))/3;
			//val += (grey * mask[0][1])/1000000 ;
			val += (grey * mask[0][1])/16 ;
		}
		
		{
			HLS_CONSTRAIN_LATENCY(0, 2, "lat02");
			#ifndef NATIVE_SYSTEMC
				{
					HLS_DEFINE_PROTOCOL("input");
					rgb = i_rgb.get();
					wait();
				}
			#else
				rgb = i_rgb.read();
			#endif

			grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16))/3;
			//val += (grey * mask[0][2])/1000000 ;
			val += (grey * mask[0][2])/16 ;
		}
		
		{
			HLS_CONSTRAIN_LATENCY(0, 2, "lat10");
			#ifndef NATIVE_SYSTEMC
				{
					HLS_DEFINE_PROTOCOL("input");
					rgb = i_rgb.get();
					wait();
				}
			#else
				rgb = i_rgb.read();
			#endif
			grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16))/3;
			//val += (grey * mask[1][0])/1000000;
			val += (grey * mask[1][0])/16;
		}
		
		{
			HLS_CONSTRAIN_LATENCY(0, 2, "lat11");
			#ifndef NATIVE_SYSTEMC
				{
					HLS_DEFINE_PROTOCOL("input");
					rgb = i_rgb.get();
					wait();
				}
			#else
				rgb = i_rgb.read();
			#endif

			grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16))/3;
			//val += (grey * mask[1][1])/1000000 ;
			val += (grey * mask[1][1])/16 ;
		}
		
		{
			HLS_CONSTRAIN_LATENCY(0, 2, "lat12");
			#ifndef NATIVE_SYSTEMC
				{
					HLS_DEFINE_PROTOCOL("input");
					rgb = i_rgb.get();
					wait();
				}
			#else
				rgb = i_rgb.read();
			#endif
			grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16))/3;
			//val += (grey * mask[1][2])/1000000 ;
			val += (grey * mask[1][2])/16 ;
		}
		
		{
			HLS_CONSTRAIN_LATENCY(0, 2, "lat20");
			#ifndef NATIVE_SYSTEMC
				{
					HLS_DEFINE_PROTOCOL("input");
					rgb = i_rgb.get();
					wait();
				}
			#else
				rgb = i_rgb.read();
			#endif
			grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16))/3;
			//val += (grey * mask[2][0])/1000000 ;
			val += (grey * mask[2][0])/16 ;
		}
		
		{
			HLS_CONSTRAIN_LATENCY(0, 2, "lat21");
			#ifndef NATIVE_SYSTEMC
				{
					HLS_DEFINE_PROTOCOL("input");
					rgb = i_rgb.get();
					wait();
				}
			#else
				rgb = i_rgb.read();
			#endif
			grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16))/3;
			//val += (grey * mask[2][1])/1000000 ;
			val += (grey * mask[2][1])/16 ;
		}
		
		{
			HLS_CONSTRAIN_LATENCY(0, 2, "lat22");
			#ifndef NATIVE_SYSTEMC
				{
					HLS_DEFINE_PROTOCOL("input");
					rgb = i_rgb.get();
					wait();
				}
			#else
				rgb = i_rgb.read();
			#endif
			grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16))/3;
			//val += (grey * mask[2][2])/1000000 ;
			val += (grey * mask[2][2])/16 ;
		}
		
			int total = 0;
			total = val * val;
		#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("output");
				o_result.put(total);
				wait();
			}
		#else
				o_result.write(total);
		#endif
	}
}*/

void GaussianFilter::do_filter() {
	{
	#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		for(unsigned int k = 0; k<9; ++k)
		{
			i_r[k].reset();
      i_g[k].reset();
      i_b[k].reset();
		}
		o_result.reset();
	#endif
		wait();
	}
	while (true) {
		HLS_PIPELINE_LOOP(SOFT_STALL,6,"MAIN");
		val = 0;
    int total = 0;
		for (unsigned int v = 0; v<9; ++v) {
      HLS_UNROLL_LOOP( ON, "get loop");
      //HLS_CONSTRAIN_LATENCY(0,4,"LOOP1");
     	HLS_CONSTRAIN_LATENCY(0,4,"LOOP1");
			sc_dt::sc_uint<8> r[9];
			sc_dt::sc_uint<8> g[9];
			sc_dt::sc_uint<8> b[9];
			
			#ifndef NATIVE_SYSTEMC
				{
					HLS_DEFINE_PROTOCOL("input");
					r[v] = i_r[v].get();
					g[v] = i_g[v].get();
					b[v] = i_b[v].get();
					wait();
				}
			#else
				rgb[v] = i_rgb[v].read();
			#endif
			grey[v] = (r[v] + g[v] + b[v])/3;
		}
		{
      		HLS_CONSTRAIN_LATENCY(0,4,"CONV");
			val += ((grey[0] * mask[0][0]>>4)+(grey[1] * mask[0][1]>>4))+
				   ((grey[2] * mask[0][2]>>4)+(grey[3] * mask[1][0]>>4))+
				   ((grey[4] * mask[1][1]>>4)+(grey[5] * mask[1][2]>>4))+
				   ((grey[6] * mask[2][0]>>4)+(grey[7] * mask[2][1]>>4))+
				    (grey[8] * mask[2][2]>>4);	
		}
    {
      	  HLS_CONSTRAIN_LATENCY(0,1,"MUL");
		      total = val * val;
	  }
	{	
		#ifndef NATIVE_SYSTEMC
		{
			HLS_DEFINE_PROTOCOL("output");
			o_result.put(total);
			wait();
		}
		#else
		  	o_result.write(total);
		#endif
    //printf("\n");
    }
	}
}