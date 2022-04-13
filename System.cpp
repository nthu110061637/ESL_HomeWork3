#include "System.h"
System::System( sc_module_name n, string input_bmp, string output_bmp ): sc_module( n ), 
	tb("tb"), gaussian_filter("gaussian_filter"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst"), _output_bmp(output_bmp)
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	gaussian_filter.i_clk(clk);
	gaussian_filter.i_rst(rst);
	for(unsigned int k=0;k<9;k++)
	{
		tb.o_r[k](r[k]);
		tb.o_g[k](g[k]);
		tb.o_b[k](b[k]);
	}
	tb.i_result(result);
	for(unsigned int l=0;l<9;l++)
	{
		gaussian_filter.i_r[l](r[l]);
		gaussian_filter.i_g[l](g[l]);
		gaussian_filter.i_b[l](b[l]);
	}
	
	gaussian_filter.o_result(result);

  tb.read_bmp(input_bmp);
}

System::~System() {
  tb.write_bmp(_output_bmp);
}
