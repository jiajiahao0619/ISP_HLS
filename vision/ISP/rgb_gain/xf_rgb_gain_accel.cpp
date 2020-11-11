#include "xf_rgb_gain_config.h"

extern "C"{
    
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

void mat_combin_rgb( float r_gain, float g_gain, float b_gain,
                     xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> &_r,  
                     xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> &_g,  
                     xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> &_b,
                     xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> &_dest)  
{
#pragma HLS DATAFLOW
    float rg,gg,bg;
    ap_uint<24 * NPC> dout;
    ap_uint<8 * NPC> r, g, b;
    float rf,gf,bf;
    rg = r_gain; gg = g_gain; bg = b_gain;
    for(int x=0; x< HEIGHT; x++){
        
        for(int y = 0; y<(WIDTH >> XF_BITSHIFT(NPC)); y++){
            b = _b.read(x * (WIDTH >> XF_BITSHIFT(NPC)) +y);
            g = _g.read(x * (WIDTH >> XF_BITSHIFT(NPC)) +y);
            r = _r.read(x * (WIDTH >> XF_BITSHIFT(NPC)) +y);
            
            for(int i = 0; i< (1 << XF_BITSHIFT(NPC));i++){
                bf = (float) b(i * 8 + 7, i * 8);
                gf = (float) g(i * 8 + 7, i * 8);
                rf = (float) r(i * 8 + 7, i * 8);
                //printf("<Executable Name> <input image path> y=%0d \n",y);
                dout(23 + i*24, i * 24)=
                    (ap_uint<8>(bf + bg),ap_uint<8>(gf + gg),ap_uint<8>(rf + rg));
            }
            _dest.write((x * (WIDTH >> XF_BITSHIFT(NPC)) + y), dout);
        }    
    }    
}

void rgb_gain_accel(xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> &rgb_img,
                    xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> &rgb_img_gain,
                    float r_gain,
                    float g_gain,
                    float b_gain)
{
#pragma HLS INTERFACE axis register both port=rgb_img_gain
#pragma HLS INTERFACE axis register both port=rgb_img

//#pragma HLS INTERFACE m_axi depth=256 port=rgb_img
//#pragma HLS INTERFACE m_axi depth=256 port=rgb_img_gain
//#pragma HLS INTERFACE m_axi depth=2 port=rgb_img
    //
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH,NPC> out_r(HEIGHT, WIDTH);
#pragma HLS STREAM variable=out_r depth=2
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH,NPC> out_g(HEIGHT, WIDTH);
#pragma HLS STREAM variable=out_g depth=2
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH,NPC> out_b(HEIGHT, WIDTH);
#pragma HLS STREAM variable=out_b depth=2
#pragma HLS DATAFLOW
    //
    mat_split_rgb(rgb_img,out_r,out_g,out_b);
 	//fprintf(stderr,"<Executable Name> <input image path>6 \n");
    mat_combin_rgb(r_gain,g_gain,b_gain,out_r,out_g,out_b,rgb_img_gain);
        
}
    
}
