# Homework 3

### Information

---

**110061637  林佳瑩**

**Github link** **:** [https://github.com/nthu110061637/ESL_HomeWork3.git](https://github.com/nthu110061637/ESL_HomeWork3.git)

### Introduction

---

**High level synthesis of Gaussian Blur**

Design a synthesizable version of the Gaussian blur module.

- Use Stratus directive of loop pipelining, unrolling, etc. to improve the throughput and latency of your design.
- Show the areas of different versions of synthesized circuits.

### Implementation

---

GaussianFilter.cpp

我將RGB分成3個physical channel一次送9筆資料過來，算完灰階值之後利用adder tree的概念將運算partition為4層，以此做pipeline.

```cpp
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
```

Testbench.cpp

```cpp
void Testbench::feed_rgb() {
  unsigned int x, y, i, v, u; // for loop counter
  unsigned char R, G, B;      // color of R, G, B
  int adjustX, adjustY, xBound, yBound;

  sc_dt::sc_uint<8> r[9];
  sc_dt::sc_uint<8> g[9];
  sc_dt::sc_uint<8> b[9];

	n_txn = 0;
	max_txn_time = SC_ZERO_TIME;
	min_txn_time = SC_ZERO_TIME;
	total_txn_time = SC_ZERO_TIME;

#ifndef NATIVE_SYSTEMC
  for(unsigned int k = 0; k<9; ++k)
  {
	  o_r[k].reset();
    o_g[k].reset();
    o_b[k].reset();
  }
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);
	total_start_time = sc_time_stamp();
  for (y = 0; y != height; ++y) {
    for (x = 0; x != width; ++x) {
      adjustX = (MASK_X % 2) ? 1 : 0; // 1
      adjustY = (MASK_Y % 2) ? 1 : 0; // 1
      xBound = MASK_X / 2;            // 1
      yBound = MASK_Y / 2;            // 1

      for (v = -yBound; v != yBound + adjustY; ++v) {   //-1, 0, 1
        for (u = -xBound; u != xBound + adjustX; ++u) { //-1, 0, 1
          if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
            R = *(source_bitmap +
                  bytes_per_pixel * (width * (y + v) + (x + u)) + 2);
            G = *(source_bitmap +
                  bytes_per_pixel * (width * (y + v) + (x + u)) + 1);
            B = *(source_bitmap +
                  bytes_per_pixel * (width * (y + v) + (x + u)) + 0);
          }
					else {
						R = 0;
						G = 0;
						B = 0;
					}
					//sc_dt::sc_uint<24> rgb[9];
          unsigned short id = ((u+1)*3)+(v+1) ;
					r[id] = R;
					g[id] = G;
					b[id] = B;
          
        }
      }
      for(unsigned int l=0;l<9;++l)
      {
        #ifndef NATIVE_SYSTEMC
          o_r[l].put(r[l]);
          o_g[l].put(g[l]);
          o_b[l].put(b[l]);
        #else
          o_r[l].write(r[l]);
          o_g[l].write(g[l]);
          o_b[l].write(b[l]);
        #endif
      }
    }
  }
  
}
```

其餘關於physical channel瑣碎的宣告就不一一貼到這邊，多採用lab5+6下去更改。

### Result

---

**Latency**

- BASIC - 26869750 ns
- UNROLL - 24903670 ns
- PIPELINE - 22937620 ns

**Area**

- BASIC - 3737.0
- UNROLL - 2827.9
- PIPELINE - 8576.8

**TLM Latency**

我用HLS的結果去推算在TLM(HomeWork2)在initiator端大約需wait多少個cycle

- 26476544 ns