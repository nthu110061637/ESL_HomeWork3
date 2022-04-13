#ifndef _DIRECTIVES_H_
#define _DIRECTIVES_H_


// Define a group of macros for each configuration

#ifdef UNROLLING
#   define CATCH_LOOP  HLS_UNROLL_LOOP( ON, "catch loop")
#   define CONV_LOOP   HLS_UNROLL_LOOP( ALL, "conv loop")
#   define PIPELINING
#endif

/*#ifdef UNROLLANDPIPELINE
#   define CONV_LOOP   HLS_UNROLL_LOOP( ALL, "conv loop")
#if defined (II)
    #   define PIPELINING HLS_PIPELINE_LOOP(SOFT_STALL, II, "Loop" )
#endif
#endif*/


#ifdef PIPELINING
/*#   define CATCH_LOOP  HLS_UNROLL_LOOP( ON, "catch loop")
#   define CONV_LOOP   HLS_UNROLL_LOOP( ALL, "conv loop")*/
#if defined (II)
    #   define PIPELINING HLS_PIPELINE_LOOP(SOFT_STALL, II, "Loop" )
#endif
#endif

// Define each macro as an empty string if it is not defined above
#ifndef CATCH_LOOP
#   define CATCH_LOOP
#endif

#ifndef CONV_LOOP
#   define CONV_LOOP
#endif

#ifndef PIPELINING
#   define PIPELINING
#endif
#endif // _DIRECTIVES_H_
