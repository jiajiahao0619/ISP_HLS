

#include "xf_rgb_sat_config.h"

void mat_split_rgb(xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> &_src,
                   xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> &_r,
                   xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> &_g,
                   xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> &_b)
{
#pragma HLS DATAFLOW
    ap_uint<24 * NPC> din;
    ap_uint<8*NPC> r, g, b;
    #pragma HLS INLINE
    mat_split_x:
    for(int x=0; x< HEIGHT; x++){
        
        mat_split_y:
        for(int y=0; y< (WIDTH >>XF_BITSHIFT(NPC)); y++ ){
#pragma HLS PIPELINE
            din = _src.read(x*(WIDTH >> XF_BITSHIFT(NPC))+y);
            mat_split_i:
            for(int i=0; i < (1 <<XF_BITSHIFT(NPC));i++ ){
#pragma HLS UNROLL

                b(i * 8 + 7, i * 8) = din.range(23 + i * 24, 16 + i * 24);
                g(i * 8 + 7, i * 8) = din.range(15 + i * 24, 8 + i * 24);
                r(i * 8 + 7, i * 8) = din.range(7 + i * 24, 0+ i*24);    
            }
            _b.write((x * (WIDTH >> XF_BITSHIFT(NPC)) + y),b);
            _g.write((x * (WIDTH >> XF_BITSHIFT(NPC)) + y),g);
            _r.write((x * (WIDTH >> XF_BITSHIFT(NPC)) + y),r);
        }
        
    }        
} 

void mat_combin_rgb( xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> &_r,  
                     xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> &_g,  
                     xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> &_b,
                     xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> &_dest)  
{
#pragma HLS INLINE

    ap_uint<24 * NPC> dout;
    ap_uint<8 * NPC> r, g, b;
    ap_uint<8 * NPC> bf,gf,rf;
    mat_split_x:
    for(int x=0; x< HEIGHT; x++){
       map_split_y:
        for(int y = 0; y<(WIDTH >> XF_BITSHIFT(NPC)); y++){
#pragma HLS PIPELINE
            b = _b.read(x * (WIDTH >> XF_BITSHIFT(NPC)) +y);
            g = _g.read(x * (WIDTH >> XF_BITSHIFT(NPC)) +y);
            r = _r.read(x * (WIDTH >> XF_BITSHIFT(NPC)) +y);
            mat_split_i:
            for(int i = 0; i< (1 << XF_BITSHIFT(NPC));i++){
#pragma HLS UNROLL
                bf =  b(i * 8 + 7, i * 8);
                gf =  g(i * 8 + 7, i * 8);
                rf =  r(i * 8 + 7, i * 8);
                //printf("<Executable Name> <input image path> y=%0d \n",y);
                dout(23 + i*24, i * 24)=(bf,gf,rf);
            }
            _dest.write((x * (WIDTH >> XF_BITSHIFT(NPC)) + y), dout);
        }    
    }    
}




void rgb_sat_accel(xf::cv::Mat<INTYPE, HEIGHT, WIDTH, NPC>& rgb_in,
                        xf::cv::Mat<INTYPE, HEIGHT, WIDTH, NPC>& rgb_out,
                        float r_sat,
                        float g_sat,
                        float b_sat
                        ) {
#pragma HLS STREAM variable=rgb_out.data depth=2
#pragma HLS STREAM variable=rgb_in.data depth=2
    //xf::cv::addWeighted<INTYPE, OUT_TYPE, HEIGHT, WIDTH, NPC>(imgInput1, alpha, imgInput2, beta, gama, imgOutput);
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH,NPC> r(HEIGHT, WIDTH);
#pragma HLS STREAM variable=r.data depth=2
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH,NPC> g(HEIGHT, WIDTH);
#pragma HLS STREAM variable=g.data depth=2
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH,NPC> b(HEIGHT, WIDTH);
#pragma HLS STREAM variable=b.data depth=2
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH,NPC> r_o(HEIGHT, WIDTH);
#pragma HLS STREAM variable=r_o.data depth=2
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH,NPC> g_o(HEIGHT, WIDTH);
#pragma HLS STREAM variable=g_o.data depth=2
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH,NPC> b_o(HEIGHT, WIDTH);
#pragma HLS STREAM variable=b_o.data depth=2
#pragma HLS DATAFLOW

    mat_split_rgb(rgb_in,r,g,b);
    float alpha =1.0;
    float beta  =0.0;
    float rgama, ggama,bgama;
    rgama = (r_sat - 50)*1.25;
    ggama = (g_sat - 50)*1.75;
    bgama = (b_sat - 50)*0.76;
    xf::cv::addWeighted<XF_8UC1, XF_8UC1, HEIGHT, WIDTH, NPC>(r, alpha, r, beta, rgama, r_o);
    xf::cv::addWeighted<XF_8UC1, XF_8UC1, HEIGHT, WIDTH, NPC>(g, alpha, g, beta, ggama, g_o);
    xf::cv::addWeighted<XF_8UC1, XF_8UC1, HEIGHT, WIDTH, NPC>(b, alpha, b, beta, bgama, b_o);
    mat_combin_rgb(r_o,g_o,b_o,rgb_out);
    
}
